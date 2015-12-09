/*
 * sanitize.h
 *
 *  Created on: Nov 15, 2015
 *      Author: bog
 */

#ifndef SANITIZE_H_
#define SANITIZE_H_

#include <string>
#include <vector>

struct sanitizeResult {
	bool failedDiacritics = false;
	bool age_u19 = false;
	bool age_u20 = false;
	bool age_u21 = false;
	bool tineret = false;
	bool feminin = false;
};

sanitizeResult sanitize(std::string &s);
std::vector<sanitizeResult> sanitize(std::vector<std::string> &vs);


#endif /* SANITIZE_H_ */
