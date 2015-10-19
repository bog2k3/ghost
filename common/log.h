/*
 * log.h
 *
 *  Created on: Nov 14, 2014
 *      Author: bogdan
 */

#ifndef LOG_H_
#define LOG_H_

#define _ENABLE_LOGGING_

#ifdef _ENABLE_LOGGING_

#include <iostream>
#include <string>

#define LOGPREFIX(PREF) logger_prefix logger_prefix_token(PREF);

#define LOG(X) { logger::writeprefix(std::cout); std::cout << X; }
#define LOGNP(X) std::cout << X
#define LOGLN(X) LOG(X << "\n")
#define ERROR(X) { std::cerr << "[ERROR]"; logger::writeprefix(std::cerr); std::cerr << X << "\n"; }

#else
#define LOG(X)
#define LOGNP(X)
#define LOGLN(X)
#define ERROR(X)
#endif

#include <string>
#include <deque>
#include <stack>

class logger {
public:
	static void writeprefix(std::ostream &stream);

private:
	static std::deque<std::string> prefix_;
	static logger instance_;

	static void push_prefix(std::string prefix) { prefix_.push_back(prefix); }
	static void pop_prefix() { prefix_.pop_back(); }

	friend class logger_prefix;
};

class logger_prefix {
public:
	logger_prefix(std::string s) {
		logger::push_prefix(s);
	}
	~logger_prefix() {
		logger::pop_prefix();
	}
};

#endif /* LOG_H_ */
