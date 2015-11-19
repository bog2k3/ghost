/*
 * wordFreq.h
 *
 *  Created on: Nov 17, 2015
 *      Author: bog
 */

#ifndef WORDFREQ_H_
#define WORDFREQ_H_

#include <map>
#include <string>
#include <vector>

class WordFreqMap {
public:
	void addWordList(std::vector<std::string> words);
	double getWordFreq(std::string const& word) const;
	int getSampleSize() const { return nTotalWords; }

	void debugPrint(std::ostream &stream);
private:
	std::map<std::string, int> occurences;
	int nTotalWords = 0;
};



#endif /* WORDFREQ_H_ */
