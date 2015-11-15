/*
 * strCompare.h
 *
 *  Created on: Oct 21, 2015
 *      Author: alexandra
 */

#ifndef STRCOMPARE_H_
#define STRCOMPARE_H_

#include <string>
#include <vector>

class StrComp {
public:

	struct Result {
		float relativeLetterResemblance;	// how many identical letters relative to the total number of letters
		float totalWordsAvg;				// average of total words from s1 and s2
		float relativeWordResemblance;		// percentage that words in the two texts match 1:1
		int identicalWords;					// how many identical words
		float identicalWordsPercentage;		// percentage of identical words between s1 and s2
		float identicalWordsNormalized;		// number of identical words normalized by their length relative to the text's average length
	};

	StrComp(std::string const& s1, std::string const& s2);
	Result getStats();

private:
	void preprocess();
	int getAbsLetterDiff(std::string const& s1, std::string const& s2);
	std::string s1, s2;
	std::vector<std::string> s1w, s2w;	// words
};


#endif /* STRCOMPARE_H_ */
