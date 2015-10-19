/*
 * log.cpp
 *
 *  Created on: Nov 28, 2014
 *      Author: bogdan
 */

#include "log.h"

std::deque<std::string> logger::prefix_;
logger logger::instance_;

void logger::writeprefix(std::ostream &stream) {
	if (prefix_.size())
		stream << "[";
	for (unsigned i=0, n=prefix_.size(); i<n; i++)
		stream << (i==0 ? "" : "::") << prefix_[i];
	if (prefix_.size())
		stream << "] ";
}
