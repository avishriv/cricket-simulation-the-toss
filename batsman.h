/*
 * batsman.h
 *
 *  Created on: 25-Sep-2019
 *      Author: Avinash Shrivastava
 */

#ifndef BATSMAN_H_
#define BATSMAN_H_

#include "common.h"

class batsman {
  private:
    unsigned batsmanID;
    string name;
    unsigned int currentBatsmanScore;
    unsigned int ballsFacedByBatsman;
    bool isOnStrike;
    bool isOut;
    unsigned shotOutcomeProbability[MAX_SHOT_OUTCOMES];
    unsigned possibleShots[MAX_POSSIBLE_SHOTS];
  public:
    batsman () { batsmanID = currentBatsmanScore = ballsFacedByBatsman = 0; isOnStrike = isOut = false;}
    batsman(unsigned int id, string name, bool onStrike, unsigned int prob[MAX_SHOT_OUTCOMES]);
    ~batsman () {}
    unsigned getBatsmanID () { return batsmanID;}
    string getName () { return name;}
    unsigned int getScore () { return currentBatsmanScore;}
    void increaseScore (unsigned runs_scored_this_ball) { this->currentBatsmanScore += runs_scored_this_ball; return; }
    unsigned int getBallsFaced () { return ballsFacedByBatsman;}
    void noteThisBallFaced () { this->ballsFacedByBatsman++; return; }
    bool checkIfOnStrike () { return isOnStrike;}
    void markStrike (bool strike_on_off) { this->isOnStrike = strike_on_off; return;}
    bool checkIfOut () { return isOut;}
    void markOut () { this->isOut = true; return;}
    unsigned getShotNowForThisBatsman () { return this->possibleShots[rand()%MAX_POSSIBLE_SHOTS];}
};


batsman::batsman(unsigned int id, string name, bool onStrike, unsigned int prob[MAX_SHOT_OUTCOMES]) {

  unsigned total_probability          = 0;
  unsigned idx                        = 0;
  this->batsmanID                     = id;
  this->name                          = name;
  this->currentBatsmanScore           = 0;
  this->ballsFacedByBatsman           = 0;
  this->isOnStrike                    = onStrike;
  this->isOut                         = false;

  /* Create expanded array with all possible shot outcomes based on their probability for this batsman */
  for (unsigned i = 0; i < MAX_SHOT_OUTCOMES; i++) {
    this->shotOutcomeProbability[i]   = prob[i];
    total_probability                += prob[i];
    for (unsigned j = 0; j < prob[i]; j++, idx++) {
      this->possibleShots[idx] = i;
    }
  }

  /* for the above obj creation to be meaningful the total should be adding up to 100% */
  assert (total_probability == MAX_POSSIBLE_SHOTS && idx == MAX_POSSIBLE_SHOTS);
}

#endif /* BATSMAN_H_ */
