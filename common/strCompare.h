/*
 * strCompare.h
 *
 *  Created on: Oct 21, 2015
 *      Author: alexandra
 */

#ifndef STRCOMPARE_H_
#define STRCOMPARE_H_

#include <string>

struct stringCompareResult {
	float letterOverlap;		// [0..1] percentage of letters that overlap (not necessarily in the same positions)
	float wordCountSimilarity;	// [0..1] 1 means same number of words, 0.5 means one string has double the amount of words etc.
	float phraseSimilarity;		// [0..1] how many identical words * (sum of identical words length) / (character count) / (total word count)
	float weighedLetterOverlap; // [0..1] percentage of letters that overlap normalized by the length ratio of the strings (% * larger_len / smaller_len)
};

stringCompareResult compareStrings(std::string const& s1, std::string const& s2);



#endif /* STRCOMPARE_H_ */
