/*
 * iChelner.h
 *
 *  Created on: Apr 8, 2015
 *      Author: bog
 */

#ifndef ICHELNER_H_
#define ICHELNER_H_

#include "cupidData.h"

/**
 * interfata asta primeste comenzile de la cupid si le trimite mai departe la site.
 * asta discuta efectiv cu serverul casei prin http.
 * Pentru fiecare site o sa fie o clasa diferita care implementeaza interfata asta.
 */

class iChelner {
public:
	virtual ~iChelner() {}

	virtual bool issueCommand(std::string gameId, pariu p, uint32_t suma, uint32_t timeoutMiliseconds) = 0;
};

#endif /* ICHELNER_H_ */
