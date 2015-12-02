/*
 * dbLabels.h
 *
 *  Created on: Oct 19, 2015
 *      Author: bog
 */

#ifndef DBLABELS_H_
#define DBLABELS_H_

#include <string>

struct {
	const std::string sesiune {"sesiune"};
	const std::string echipa1 {"echipa1"};
	const std::string echipa2 {"echipa2"};
	const std::string cota1 {"cota_1"};
	const std::string cota2 {"cota_2"};
	const std::string cotax {"cota_x"};
	const std::string cota1x {"cota_1x"};
	const std::string cota2x {"cota_2x"};
	const std::string cota12 {"cota_12"};
	const std::string siteId {"site_id_1"};
	const std::string gameId {"game_id_1"};
	const std::string data {"data_joc"};
	const std::string hashComun {"hash_joc_comun"};
	const std::string hashUnic {"hash_joc"};
	const std::string statusTraduceri {"status_echipe"};
} dbLabels;

#endif /* DBLABELS_H_ */
