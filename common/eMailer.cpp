/*
 * eMailer.cpp
 *
 *  Created on: Nov 30, 2015
 *      Author: bog
 */

#include "eMailer.h"
#include "strManip.h"
#include "log.h"
#include <curl/curl.h>
#include <cstring>
#include <ctime>
#include <iomanip>

EMailer::EMailer(std::string const& smtpServer, std::string const& smtpUser, std::string const& smtpPassw, std::string const& smtpSenderAddr,
		std::string const& senderScreenName, std::string const& footer)
	: server_(strLower(smtpServer)), user_(smtpUser), passw_(smtpPassw),
	  senderAddr_(strLower(smtpSenderAddr)),
	  senderName_(senderScreenName), footer_(footer) {
	if (senderName_.empty())
		senderName_ = smtpUser;
}

struct upload_context {
	std::stringstream stream;
};

size_t EMailer::payload_source(void *ptr, size_t size, size_t nmemb, void *userp) {
	struct upload_context* upload_ctx = (struct upload_context*) userp;
	std::string data;

	if ((size == 0) || (nmemb == 0) || ((size * nmemb) < 1)) {
		return 0;
	}

	if (!std::getline(upload_ctx->stream, data))
		return 0;
	data.append("\r\n");	// trebuie pus inapoi, pt ca std::getline consuma CR\LF-u

	size_t len = data.length();
	memcpy(ptr, data.c_str(), len);
	return len;
}

std::string EMailer::formatCrtDateTime() {
	time_t t = time(0);   // get time now
	struct tm * now = localtime( & t );
	const char* wdays[] {
		"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
	};
	const char* months[] {
		"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
	};
	// output date & time in this format: "Mon, 29 Nov 2010 21:54:29 +7200"
	std::stringstream s;
	s << wdays[now->tm_wday] << ", "
		<<  now->tm_mday << " "
		<< months[now->tm_mon] << " "
		<< (now->tm_year + 1900) << " "
		<< std::setw(2) << std::setfill('0') << now->tm_hour << ":"
		<< std::setw(2) << std::setfill('0') << now->tm_min << ":"
		<< std::setw(2) << std::setfill('0') << now->tm_sec << " "
		<< (now->tm_gmtoff > 0 ? "+" : "-") << std::setw(2) << std::setfill('0') << abs(now->tm_gmtoff)/3600 << "00";
	return s.str();
}

//static const char *payload_text[] =
//		{ "Date: Mon, 29 Nov 2010 21:54:29 +1100\r\n",
//				"To: \"The Bosses\":" TO1 "," TO2 ";\r\n",
//				"From: \"Maimutza\" " FROM "\r\n",
//#ifdef CC
//				"Cc: " CC "(poison)\r\n",
//#endif
////				"Message-ID: <dcd7cb36-11db-487a-9f3a-e652a9458efd@rfcpedant.example.org>\r\n",
//				"Subject: test Maimuță\r\n",
//				"\r\n", /* empty line to divide headers from body, see RFC5322 */
//				"Salutari de la maimuță\r\n",
//				NULL };

void EMailer::buildMsgBody(std::stringstream &ostream, std::vector<std::string> const& destAddr, std::string const& subject, std::string const& body) {
	ostream << "Date: " << formatCrtDateTime() << "\r\n";
	ostream << "To: ";
		for (unsigned i=0; i<destAddr.size(); i++) {
			if (i > 0)
				ostream << ",";
			ostream << "<" << destAddr[i] << ">";
		}
		ostream << "\r\n";
	ostream << "From: " << senderName_ << " <" << senderAddr_ << ">" << "\r\n";
//	ostream << "Message-ID: <dcd7cb36-11db-487a-9f3a-e652a9458efd@rfcpedant.example.org>\r\n";
	ostream << "Subject: " << subject << "\r\n";
	ostream << "\r\n"; // empty line to divide headers from body, see RFC5322
	ostream << body;
	if (!footer_.empty())
		ostream << "\r\n\r\n--------\r\n" << footer_ << "\r\n";

	ostream << "\r\n.\r\n";
}

bool EMailer::send(std::vector<std::string> const& destAddr, std::string const& subject, std::string const& body) {
	CURL *curl;
	CURLcode res = CURLE_OK;
	struct curl_slist *recipients = NULL;
	struct upload_context upload_ctx;
	buildMsgBody(upload_ctx.stream, destAddr, subject, body);

	curl = curl_easy_init();
	if (curl) {
		/* Set username and password */
		curl_easy_setopt(curl, CURLOPT_USERNAME, user_.c_str());
		curl_easy_setopt(curl, CURLOPT_PASSWORD, passw_.c_str());

		/* This is the URL for your mailserver. Note the use of smtps:// rather
		 * than smtp:// to request a SSL based connection. */
		curl_easy_setopt(curl, CURLOPT_URL, server_.c_str());
		if (server_.find("smtps://") == 0)
			curl_easy_setopt(curl, CURLOPT_USE_SSL, (long )CURLUSESSL_ALL);
		else
			curl_easy_setopt(curl, CURLOPT_USE_SSL, (long )CURLUSESSL_TRY);

		/* If you want to connect to a site who isn't using a certificate that is
		 * signed by one of the certs in the CA bundle you have, you can skip the
		 * verification of the server's certificate. This makes the connection
		 * A LOT LESS SECURE.
		 *
		 * If you have a CA cert for the server stored someplace else than in the
		 * default bundle, then the CURLOPT_CAPATH option might come handy for
		 * you. */
#ifdef SKIP_PEER_VERIFICATION
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

		/* If the site you're connecting to uses a different host name that what
		 * they have mentioned in their server certificate's commonName (or
		 * subjectAltName) fields, libcurl will refuse to connect. You can skip
		 * this check, but this will make the connection less secure. */
#ifdef SKIP_HOSTNAME_VERIFICATION
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif

		/* Note that this option isn't strictly required, omitting it will result in
		 * libcurl sending the MAIL FROM command with empty sender data. All
		 * autoresponses should have an empty reverse-path, and should be directed
		 * to the address in the reverse-path which triggered them. Otherwise, they
		 * could cause an endless loop. See RFC 5321 Section 4.5.5 for more details.
		 */
		curl_easy_setopt(curl, CURLOPT_MAIL_FROM, senderAddr_.c_str());

		/* Add two recipients, in this particular case they correspond to the
		 * To: and Cc: addressees in the header, but they could be any kind of
		 * recipient. */
		for (auto &s : destAddr)
			recipients = curl_slist_append(recipients, s.c_str());
#ifdef CC
		recipients = curl_slist_append(recipients, CC);
#endif
		curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

		/* We're using a callback function to specify the payload (the headers and
		 * body of the message). You could just use the CURLOPT_READDATA option to
		 * specify a FILE pointer to read from. */
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
		curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

		/* Since the traffic will be encrypted, it is very useful to turn on debug
		 * information within libcurl to see what is happening during the
		 * transfer */
		curl_easy_setopt(curl, CURLOPT_VERBOSE, verbose_ ? 1L : 0L);

		/* Send the message */
		res = curl_easy_perform(curl);

		/* Check for errors */
		if (res != CURLE_OK)
			ERROR("EMailer::send()::curl_easy_perform() failed: " << curl_easy_strerror(res));

		/* Free the list of recipients */
		curl_slist_free_all(recipients);

		/* Always cleanup */
		curl_easy_cleanup(curl);
	}

	return res == CURLE_OK;
}

