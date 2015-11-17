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

class WordFreqMap;

class StrComp {
public:
	struct Result {
		StrComp* owner = nullptr;
		float relativeLetterResemblance = 0;	// how many identical letters relative to the total number of letters
		float totalWordsAvg = 0;				// average of total words from s1 and s2
		float relativeWordResemblance = 0;		// percentage that words in the two texts match 1:1
		float identicalWords = 0;				// how many identical words (can have fractions, if words are nearly-identical)
		float identicalWordsPercentage = 0;		// percentage of identical words between s1 and s2
		float identicalWordsNormalized = 0;		// number of identical words normalized by their length relative to the text's average length

		Result(StrComp *owner) : owner(owner) {}
	};

	StrComp(std::string const& s1, std::string const& s2);
	Result getStats(WordFreqMap* pFreqMap = nullptr);	// provide a WordFreqMap in order to get more relevant results (common words have a smaller weight in comparison than rarer words)
	std::string const& getS1() { return s1; }
	std::string const& getS2() { return s2; }

private:
	void preprocess();
	int getAbsLetterDiff(std::string const& s1, std::string const& s2);
	std::string s1, s2;
	std::vector<std::string> s1w, s2w;	// words
};


#endif /* STRCOMPARE_H_ */
