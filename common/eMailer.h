/*
 * eMailer.h
 *
 *  Created on: Nov 24, 2015
 *      Author: bog
 */

#ifndef EMAILER_H_
#define EMAILER_H_

#include <string>
#include <vector>
#include <sstream>

class EMailer {
public:
	EMailer(std::string const& smtpServer,		// smtp server address in the form smtp://my.server.org:52 or smtps://my.secure.server.org:465
			std::string const& smtpUser,		// smtp user name for authentication
			std::string const& smtpPassw,		// smtp user password for authentication
			std::string const& smtpSenderAddr,	// email address to use as <sender> (from: field) - MUST belong to the user, otherwise the SMTP server may refuse it
			std::string const& senderScreenName,	// screen name of sender (From: filed will contain "Screen Name <email@address>")
			std::string const& footerSignature);	// signature to append to all message footers

	bool send(std::vector<std::string> const& destAddr,
			std::string const& subject,
			std::string const& body);

	void setVerbose(bool verbose) { verbose_ = verbose; }

private:
	static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp);
	void buildMsgBody(std::stringstream &ostream, std::vector<std::string> const& destAddr, std::string const& subject, std::string const& body);
	std::string formatCrtDateTime();

	std::string server_, user_, passw_, senderAddr_, senderName_, footer_;
	bool verbose_ = false;
};



#endif /* EMAILER_H_ */
