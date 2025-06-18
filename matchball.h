/*
 * matchball.h
 *
 *  Created on: 25-Sep-2019
 *      Author: Avinash
 */

#ifndef MATCHBALL_H_
#define MATCHBALL_H_

#include "common.h"

class matchBall {
  public:
    unsigned ballNum;
    unsigned strikerBatsmanID;
    unsigned nonStrikerBatsmanID;
    unsigned currentShotOutcome;
    unsigned currentTeamScore;
    bool isBatsmanOut;
    bool isLastBallOfAnOver;
    bool needStrikeRotation;

    matchBall() {
       /* init to impossible values */
      ballNum = strikerBatsmanID = nonStrikerBatsmanID = currentShotOutcome = currentTeamScore = 0xBAAD;
      isBatsmanOut = isLastBallOfAnOver = needStrikeRotation = false;
    }
    ~matchBall() {}
};

#endif /* MATCHBALL_H_ */
