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
#ifdef DEBUG
		std::string s1, s2;
#endif
		double relativeLetterResemblance = 0;	// how many identical letters relative to the total number of letters
		double totalWordsAvg = 0;				// average of total words from s1 and s2
		double relativeWordResemblance = 0;		// percentage that words in the two texts match 1:1
		double identicalWords = 0;				// how many identical words (can have fractions, if words are nearly-identical)
		double identicalWordsPercentage = 0;		// percentage of identical words between s1 and s2
		double identicalWordsNormalized = 0;		// number of identical words normalized by their length relative to the text's average length
	};

	StrComp(std::string const& s1, std::string const& s2);
	Result getStats(const WordFreqMap* pFreqMap = nullptr);	// provide a WordFreqMap in order to get more relevant results (common words have a smaller weight in comparison than rarer words)
	std::string const& getS1() { return s1_; }
	std::string const& getS2() { return s2_; }

private:
	void preprocess();
	int getAbsLetterDiff(std::string const& s1, std::string const& s2);
	void getNGramResemblance(int n, std::string const& s1, std::string const& s2, double &out_similarity, int &out_longestStreak);
	std::vector<std::string> generateNGrams(int n, std::string const& s);
	std::string s1_, s2_;
	std::vector<std::string> s1w_, s2w_;	// words
};


#endif /* STRCOMPARE_H_ */
