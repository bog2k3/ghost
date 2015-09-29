/*
 * test.cpp
 *
 *  Created on: Jul 11, 2015
 *      Author: bog
 */

#include <curl/curl.h>
#include <string>
#include <iostream>

static char errorBuffer[CURL_ERROR_SIZE+1];
static std::string buffer;

static int writer(char *data, size_t size, size_t nmemb, std::string *writerData) {
  if (writerData == NULL)
    return 0;

  writerData->append(data, size*nmemb);
  return size * nmemb;
}

void testCurl() {
	CURL *curl;
	CURLcode res;

	curl_global_init(CURL_GLOBAL_ALL);

	/* get a curl handle */
	curl = curl_easy_init();
	if (curl) {
		/* First set the URL that is about to receive our POST. This URL can
	   	   just as well be a https:// URL if that is what should receive the
	   	   data. */
		curl_easy_setopt(curl, CURLOPT_URL, "https://en.wikipedia.org/wiki/Ghost");
		/* Now specify the POST data */
		// curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "name=daniel&project=curl");

		if (curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer) != CURLE_OK) {
			fprintf(stderr, "Failed to set error buffer\n");
		}

		if (curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer) != CURLE_OK)
		{
			fprintf(stderr, "Failed to set writer [%s]\n", errorBuffer);
		}

		if (curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer) != CURLE_OK)
		{
			fprintf(stderr, "Failed to set writer [%s]\n", errorBuffer);
		}

		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if(res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

		std::cout<<buffer;

		/* always cleanup */
		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();
}

void testDB() {

}
