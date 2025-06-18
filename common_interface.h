/*
 * common_interface.h
 *
 *  Created on: 25-Sep-2019
 *      Author: Avinash
 */

#ifndef COMMON_INTERFACE_H_
#define COMMON_INTERFACE_H_

#include "crick_project.h"
#include "batsman.h"
#include "matchball.h"

/* defines for "last 4" problem */
list <batsman>     NotOutBatsmen;
list <batsman>     outBatsmen;
vector <matchBall> inning;

/* defines for "the tie breaker" problem */
list <batsman>     Team1NotOutBatsmen;
list <batsman>     Team2NotOutBatsmen;
list <batsman>     Team1OutBatsmen;
list <batsman>     Team2OutBatsmen;
vector <matchBall> Team1Inning;
vector <matchBall> Team2Inning;


#endif /* COMMON_INTERFACE_H_ */
