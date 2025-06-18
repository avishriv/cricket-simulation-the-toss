//============================================================================
// Name        : crick_project.cpp
// Author      : Avinash Shrivastava
// Version     : V2
// Copyright   : Why copy, if you can write one !
// Description : This is the main file for solution of:
//					https://www.geektrust.in/coding-problem/backend/cricket
//============================================================================

#include "common_interface.h"

void problem_1_the_toss () {

  // collect inputs
  cout << "The toss... !\n";
  string daynight, weather;
  getline( cin, weather, ' ' );
  getline( cin, daynight, ' ' );
  int nWeather = (weather == "Clear")? 0:1;
  int nDaynight = (daynight == "Day")? 0:1;

  // verify inputs
  if ((weather != "Clear" && weather != "Cloudy") || (daynight != "Day" && daynight != "Night")) {
    cout << "Invalid input.\n";
    return;
  }

  // Core logic:
  //  1. Identify who won the toss
  //  2. Check the action by Weather
  //  3. Check the action by Day/Night
  //  4. Consolidate conflicting action
  //  5. Output as desired
  //
  srand(time(0));
  int who_won_the_toss = rand()%2;
  int action_by_weather  = weatherMatrix[who_won_the_toss][nWeather];
  int action_by_daynight = daynightMatrix[who_won_the_toss][nDaynight];
  cout << "TOSS: <" << who_won_the_toss << ", " <<action_by_weather << ", " << action_by_daynight << ">" << endl;
  int consolidated_action = (action_by_weather == action_by_daynight) ? action_by_daynight : BATS;
  string teamName = (who_won_the_toss == 0) ? "Lengaburu" : "Enchai";
  string actionName = (consolidated_action == BATS) ? "bats" : "bowls";
  cout << teamName << " wins the toss and " << actionName << endl;
  cout << "The toss...done ! ...for: <" << weather << ", " << daynight << ", " << nWeather << ", " << nDaynight << ">" <<endl;
}

bool getBatsmanFromFactory (unsigned id, batsman & newBatsman, bool isTieBreaker = false) {

  bool rc = true; /* result code: 0 (FAIL) 1 (SUCCESS) */

  switch (id) {
    case 0:
      {
        unsigned tempProb[MAX_SHOT_OUTCOMES] = {5,30,25,10,15,1,9,5};
        if (isTieBreaker) {
          const static unsigned tieBreakProb[] = {5,10,25,10,25,1,14,10};
          copy(tieBreakProb, (tieBreakProb + MAX_SHOT_OUTCOMES), tempProb);
        }
        batsman tempBatsman (0, "Kirat Boli", true, tempProb);
        newBatsman = tempBatsman;
      }
      break;

    case 1:
      {
        unsigned tempProb[MAX_SHOT_OUTCOMES] = {10,40,20,5,10,1,4,10};
        if (isTieBreaker) {
          const static unsigned tieBreakProb[] = {5,15,15,10,20,1,19,15};
          copy(tieBreakProb, (tieBreakProb + MAX_SHOT_OUTCOMES), tempProb);
        }
        batsman tempBatsman (1, "N.S Nodhi", false, tempProb);
        newBatsman = tempBatsman;
      }
      break;

    case 2:
      {
        unsigned tempProb[MAX_SHOT_OUTCOMES] = {20,30,15,5,5,1,4,20};
        batsman tempBatsman (2, "R Rumrah", false, tempProb);
        newBatsman = tempBatsman;
      }
      break;

    case 3:
      {
        unsigned tempProb[MAX_SHOT_OUTCOMES] = {30,25,5,0,5,1,4,30};
        batsman tempBatsman (3, "Shashi Henra", false, tempProb);
        newBatsman = tempBatsman;
      }
      break;

    case 4: /* Batsman ID 4 is only known to the factory */
      {
        unsigned tempProb[MAX_SHOT_OUTCOMES] = {5,10,25,10,25,1,14,10};
        batsman tempBatsman (0, "DB Vellyers", true, tempProb);
        newBatsman = tempBatsman;
      }
      break;

    case 5:  /* Batsman ID 5 is only known to the factory */
      {
        unsigned tempProb[MAX_SHOT_OUTCOMES] = {10,15,15,10,20,1,19,10};
        batsman tempBatsman (1, "H Mamla", false, tempProb);
        newBatsman = tempBatsman;
      }
      break;

    default:
      assert(0); /* this must not happen */
      break;
  }

  return rc;
}

static inline void addToBatsmanScore (list <batsman> & batsmen, matchBall & thisBall) {

  bool score_updated_successfully = false;

  for (auto iter = batsmen.begin(); iter != batsmen.end(); iter++) {
    if (iter->getBatsmanID() == thisBall.strikerBatsmanID) {
      iter->increaseScore( thisBall.currentShotOutcome );
      score_updated_successfully = true;
      break;
    }
  }

  /* Invalid operation ! can't update score of a batsman who is already out !! */
  assert(score_updated_successfully); /**/
  return;
}

static inline void noteThisBallFacedByBatsman (list <batsman> & batsmen, matchBall & thisBall) {

  bool ball_faced_updated_successfully = false;

  for (auto iter = batsmen.begin(); iter != batsmen.end(); iter++) {
    if (iter->getBatsmanID() == thisBall.strikerBatsmanID) {
      iter->noteThisBallFaced();
      ball_faced_updated_successfully = true;
      break;
    }
  }

  /* Invalid operation ! can't update balls faced by a batsman who is already out !! */
  assert(ball_faced_updated_successfully); /**/
  return;
}

static inline string getBatsmanByID (unsigned batsmanID, list <batsman> outList, list <batsman>notOutList) {
  list <batsman>::iterator oIter = outList.begin();
  list <batsman>::iterator nIter = notOutList.begin();

  /* check batsman if he is out */
  while ( oIter != outList.end() ) {
    if (oIter->getBatsmanID() == batsmanID) {
      return oIter->getName();
    }
    oIter++;
  }
  /* check batsman if he is not-out */
  while ( nIter != notOutList.end() ) {
    if (nIter->getBatsmanID() == batsmanID) {
      return nIter->getName();
    }
    nIter++;
  }

  return "BATSMAN not found !! ";
}

static inline unsigned getShotoutcomeForBatsman (list <batsman> batsmen, unsigned batsmanID) {

  list <batsman>::iterator nIter = batsmen.begin();
  unsigned thisBallShot = 0xBAAD;
    while ( nIter != batsmen.end() ) {
      if (nIter->getBatsmanID() == batsmanID) {
        thisBallShot = nIter->getShotNowForThisBatsman();
      }
      nIter++;
    }

  return thisBallShot;
}

static inline void rotateStrike (list <batsman> & batsmen, matchBall & aBall) {

  bool strike1_updated_successfully = false;
  bool strike2_updated_successfully = false;

  for (auto iter = batsmen.begin(); iter != batsmen.end(); iter++) {
    if (iter->getBatsmanID() == aBall.strikerBatsmanID) {
      iter->markStrike( false ); /* move the striker to non-striker end */
      strike1_updated_successfully = true;
    } else if (iter->getBatsmanID() == aBall.nonStrikerBatsmanID) {
      iter->markStrike( true ); /* move the non-striker to striker end */
      strike2_updated_successfully = true;
    }
  }

  unsigned temp = aBall.strikerBatsmanID;
  aBall.strikerBatsmanID = aBall.nonStrikerBatsmanID;
  aBall.nonStrikerBatsmanID = temp;
  // cout << "(Ball # " << aBall.ballNum << ") STRIKE after rotation: < "
  //      << aBall.strikerBatsmanID << ", " << aBall.nonStrikerBatsmanID << " >" << endl;

  /* Invalid operation ! can't update strike of a batsman who is already out !! */
  assert(strike1_updated_successfully);
  assert(strike2_updated_successfully);
  return;
}

static inline bool removeBatsmanFromNotOutList (list <batsman> & notOutList, list <batsman> & outList, unsigned id) {

  bool removed_batsman = false;

  for (auto itr = notOutList.begin(); itr != notOutList.end(); itr++) {
    if (itr->getBatsmanID() == id) {
      /* first note that the batsman is out and then he can never be on strike anyway !! */
      itr->markOut(); itr->markStrike(false);
      /* then add the batsman to outBatsmen list */
      outList.push_back(*itr);
      /* now remove the batsman frm notOutBatsmen list */
      notOutList.erase(itr);

      removed_batsman = true;
      break;
    }
  }
  return removed_batsman;
}

void handleBatsmanOut (list <batsman> & NotOutList, list <batsman> & OutList, matchBall & currentBall, bool isTieBreak = false) {

  // cout << "handleBatsmanOut for ID: " << currentBall.strikerBatsmanID << ", wickets remaining: "<< NotOutList.size() << endl;
  list <batsman>::iterator iter = NotOutList.begin();

  /* It's not possible that the list was empty and a batsman got out !! */
  assert(!NotOutList.empty());
  /* Match would have been already over by this time, if one of the batsman in last pair got out !! */
  assert(isTieBreak || NotOutList.size() > NUM_BATSMAN_FINAL_PAIR);

  if (removeBatsmanFromNotOutList(NotOutList, OutList, currentBall.strikerBatsmanID)) {
    // cout << "(Ball # " << currentBall.ballNum << ") ERASED: batsman ID: " << currentBall.strikerBatsmanID << endl;
  } else {
    // cout << "(Ball # " << currentBall.ballNum << ") Unable to ERASE: batsman ID: " << currentBall.strikerBatsmanID << endl;
    assert(0);
  }

  /* for Tie break, there is only one wicket at hand */
  if (isTieBreak) {
    return;
  }

  iter = NotOutList.begin();
  if (iter->getBatsmanID() == currentBall.nonStrikerBatsmanID) {
    iter++;
    currentBall.strikerBatsmanID = iter->getBatsmanID();
    // cout << "(Ball # " << currentBall.ballNum << ") NEW: strike batsman ID: " << currentBall.strikerBatsmanID << endl;
    iter->markStrike (false);
    for (auto iter2 = NotOutList.begin(); iter2 != NotOutList.end(); iter2++) {
      if (iter2->getBatsmanID() == currentBall.strikerBatsmanID) {
        iter2->markStrike (true);
      }
    }
  } else if (iter->getBatsmanID() == currentBall.strikerBatsmanID) {
    iter++;
    currentBall.strikerBatsmanID = iter->getBatsmanID();
    // cout << "(Ball # " << currentBall.ballNum << ") NEW: strike batsman ID: " << currentBall.strikerBatsmanID << endl;
    iter->markStrike (false);
    for (auto iter2 = NotOutList.begin(); iter2 != NotOutList.end(); iter2++) {
      if (iter2->getBatsmanID() == currentBall.strikerBatsmanID) {
        iter2->markStrike (true);
      }
    }
  } else {
    assert(0); /* this should not happen */
  }

  return;
}

void print_batsman_list (list <batsman> batsmen, bool isInternalFormat) {

  if (isInternalFormat)
    cout << "< BATSMAN ID / name / currentScore / ballsFaced / isOnStrike >" << endl;

  for (auto it = batsmen.begin(); it != batsmen.end(); it++) {
    batsman temp = *it;
    if (isInternalFormat) {
      cout << "< "<< temp.getBatsmanID() << " / " << temp.getName() << " / " << temp.getScore() << " / "
           << temp.getBallsFaced() << " / " << temp.checkIfOnStrike() << " >" << endl;
    } else {
      if (temp.getBallsFaced() != 0)  {
          cout << temp.getName() << " - " << temp.getScore() << ((!temp.checkIfOut()) ?"* (":" (")
               << temp.getBallsFaced() << " balls)" << endl;
      } else {
        cout << temp.getName() << " - Did not bat." << endl;
      }
    }
  }

  return;
}

void create_batsmen_list (list <batsman> & batsmen, bool isTieBreaker, bool isTeam2, unsigned num_batsmen) {
  // cout << "create_batsmen_list... !\n";
  batsman newBatsman;
  for (unsigned i=0; i < num_batsmen; i++) {
    unsigned team2_player_start_offset = (isTieBreaker && isTeam2) ? 4:0;
    if (getBatsmanFromFactory((i + team2_player_start_offset), newBatsman, isTieBreaker)) {
      batsmen.push_back(newBatsman);
    }
  }

  // cout << "create_batsmen_list...done !\n";
  // print_batsman_list(batsmen, true);
  return;
}

void identify_striker_and_non_striker (list <batsman> batsmen, matchBall & thisBall) {

  list <batsman>::iterator iter = batsmen.begin();
  thisBall.strikerBatsmanID    = iter->getBatsmanID();
  iter++;
  thisBall.nonStrikerBatsmanID = iter->getBatsmanID();
  return;
}

static inline void let_the_ball_be_bowled (unsigned i, list <batsman> batsmen, matchBall & thisBall) {

    thisBall.ballNum             = i;
    thisBall.isBatsmanOut        = false;
    thisBall.isLastBallOfAnOver  = (thisBall.ballNum % BALLS_PER_OVER == LAST_BALL_OF_OVER);
    thisBall.needStrikeRotation  = false;
    thisBall.currentShotOutcome  = getShotoutcomeForBatsman (batsmen, thisBall.strikerBatsmanID);

    switch (thisBall.currentShotOutcome) {
      case 0: /* DOT_BALL */
          if (thisBall.isLastBallOfAnOver) {
            /* strike changes if there is a change of over */
            thisBall.needStrikeRotation = true;
          }
        break;

      case 1: /* 1 run scored */
      case 3: /* 3 run scored */
      case 5: /* 5 run scored */
          if (!thisBall.isLastBallOfAnOver) {
            /* strike changes unless there is a change of over */
            thisBall.needStrikeRotation = true;
          }
        break;

      case 2: /* 2 run scored */
      case 4: /* 4 run scored */
      case 6: /* 6 run scored */
          if (thisBall.isLastBallOfAnOver) {
            /* strike changes if there is a change of over */
            thisBall.needStrikeRotation = true;
          }
        break;

      case 7:
          thisBall.isBatsmanOut = true;
          if (thisBall.isLastBallOfAnOver) {
            /* strike changes  if there is a change of over */
            thisBall.needStrikeRotation = true;
          }
        break;

      default:
        cout << "Ball # " << thisBall.ballNum << ", Current shot: " << thisBall.currentShotOutcome << endl;
        assert (0); /* this must not happen */
    }

  return;
}

static inline void update_scores (list <batsman> & batsmen, matchBall & thisBall) {

  if ( (thisBall.currentShotOutcome != OUT_IDENTIFIER_IDX) && (thisBall.currentShotOutcome != DOT_BALL_IDENTIFIER_IDX) ) {
    addToBatsmanScore (batsmen, thisBall);
    thisBall.currentTeamScore += thisBall.currentShotOutcome;
  }
  noteThisBallFacedByBatsman (batsmen, thisBall);

  return;
}

void handle_out_and_rotate_strike (matchBall & thisBall, unsigned superoverTeam, bool isTieBreaker = false) {

  if ( thisBall.currentShotOutcome == OUT_IDENTIFIER_IDX ) {
    // cout << "(Ball # " << thisBall.ballNum << ") Batsman OUT with ID: " << thisBall.strikerBatsmanID << endl;
    if (!isTieBreaker) {
      handleBatsmanOut (NotOutBatsmen, outBatsmen, thisBall);
    } else if (isTieBreaker && (superoverTeam == TEAM_1)) {
      handleBatsmanOut (Team1NotOutBatsmen, Team1OutBatsmen, thisBall, true);
    } else if (isTieBreaker && (superoverTeam == TEAM_2)) {
      handleBatsmanOut (Team2NotOutBatsmen, Team2OutBatsmen, thisBall, true);
    } else {
      cout << "Impossible OUT scenerio.." << endl;
      assert(0);
    }

    // cout << "(Ball # " << thisBall.ballNum << ") Handled OUT. New striker ID: " << thisBall.strikerBatsmanID
    //      << ", New non-striker ID: " << thisBall.nonStrikerBatsmanID << endl;
  }
  if (thisBall.needStrikeRotation && !isTieBreaker) {
    rotateStrike (NotOutBatsmen, thisBall);
  } else if (thisBall.needStrikeRotation && isTieBreaker && (superoverTeam == TEAM_1)) {
    rotateStrike (Team1NotOutBatsmen, thisBall);
  } else if (thisBall.needStrikeRotation && isTieBreaker && (superoverTeam == TEAM_2)) {
    rotateStrike (Team2NotOutBatsmen, thisBall);
  }

  return;
}

void create_inning () {
  // cout << "create_inning... !\n";
  matchBall thisBall;
  thisBall.currentTeamScore    = 0;

  /* first decide the striker and non-striker batsman */
  identify_striker_and_non_striker (NotOutBatsmen, thisBall);

  for (auto i=0; i < BALLS_REMAINING; i++) {
    /* update matchBall state based on the outcome of the ball, so let the bowl be bowled */
    let_the_ball_be_bowled (i, NotOutBatsmen, thisBall);

    /* accomdate case of Batsman scored runs & Balls faced */
    update_scores (NotOutBatsmen, thisBall);

    /* Account for the outcome of this ball in the inning */
    inning.push_back(thisBall);
    balls_remaining--;
    final_team_score = thisBall.currentTeamScore;

    /* check the validity conditions for a clear match finish: runs target achieved or wickets exhausted */
    runs_target_achieved = (thisBall.currentTeamScore >= RUNS_NEEDED);
    wickets_exhausted    = ( (NotOutBatsmen.size() == NUM_BATSMAN_FINAL_PAIR) &&
                             (thisBall.currentShotOutcome == OUT_IDENTIFIER_IDX) );
    if (runs_target_achieved)
    {
      didBattingTeamWin = true;
      break;
    } else if (wickets_exhausted) {
      didBattingTeamWin = false;
      if (removeBatsmanFromNotOutList(NotOutBatsmen, outBatsmen, thisBall.strikerBatsmanID)) {
        // cout << "(Ball # " << thisBall.ballNum << ") ERASED: batsman ID: " << thisBall.strikerBatsmanID << endl;
      } else {
        // cout << "(Ball # " << thisBall.ballNum << ") Unable to ERASE: batsman ID: " << thisBall.strikerBatsmanID << endl;
        assert(0);
      }
      break;
    } /* default is false, so if the balls exhausted the balling team wins anyway */

    /* check the condition for handling a Tie -> Super Over for T20 */
    /* 1. Was it a Last ball                 */
    /* 2. Are the scores level now           */
    /* 3. Is still their is a wicket in hand */
    is_it_a_tie = ( (thisBall.ballNum == BALLS_REMAINING) &&
                    (thisBall.currentTeamScore == (RUNS_NEEDED - 1)) &&
                    (NotOutBatsmen.size() >= NUM_BATSMAN_FINAL_PAIR) );
    if (is_it_a_tie) {
      cout << " Super over is handled seperately to resolve the Tie !! Team score: " << thisBall.currentTeamScore << endl;
      assert (0);
    }

    /* if match is not already over, handle batsman out case and then rotate strike if needed */
    handle_out_and_rotate_strike(thisBall, 0, false);
  }
  // cout << "create_inning...done !\n";
}

void print_inning (vector <matchBall> oneInning) {

  cout << "< Ball # / strikerBatsmanID / nonStrikerBatsmanID / currentShot / currentTeamScore/ isBatsmanOut >" << endl;
  for (matchBall aBall : oneInning) {
    cout << "< " <<aBall.ballNum << ", " << aBall.strikerBatsmanID << ", " << aBall.nonStrikerBatsmanID << ", " <<
                aBall.currentShotOutcome << ", " << aBall.currentTeamScore << ", " << aBall.isBatsmanOut << " >" << endl;
  }
  return;
}

void print_final_result (list <batsman> notOutBatsmen) {
  cout << (didBattingTeamWin? "Lengaburu":"Enchai") << " won by "
       << (didBattingTeamWin? (notOutBatsmen.size() -1) : (RUNS_NEEDED - final_team_score) )
       << (didBattingTeamWin? " wicket(s) " : " run(s) ")
       << "and with " << balls_remaining << " ball(s) remaining" << endl << endl;
  return;
}

void print_result () {
  print_final_result (NotOutBatsmen);
  print_batsman_list(outBatsmen, false);
  print_batsman_list(NotOutBatsmen, false);
  return;
}

void print_commentry () {
  cout << "Live commentary" << endl;

  unsigned overs_left   = NUM_OVERS;
  unsigned balls_left   = BALLS_REMAINING;
  unsigned current_ball = 0;
  unsigned current_over = 0;
  unsigned runs_to_win  = RUNS_NEEDED;

  for (matchBall aBall : inning) {
    bool isNewOver = (current_ball % BALLS_PER_OVER == 0);
    if (isNewOver) {
      cout << "\n" << overs_left << " overs left. " << runs_to_win << " runs to win" << "\n" << endl;
      overs_left--;
      current_over++;
    }
    runs_to_win = (RUNS_NEEDED - aBall.currentTeamScore);
    bool isOut  = (aBall.currentShotOutcome == OUT_IDENTIFIER_IDX);
    string printString = (aBall.currentShotOutcome > 1) ? " runs" : " run";
    string batsmanName = getBatsmanByID (aBall.strikerBatsmanID, outBatsmen, NotOutBatsmen);
    if (!isOut) {
      cout << "( " << (current_over - 1) << "." << (current_ball % BALLS_PER_OVER) << " ) "
           << batsmanName << " scores " << aBall.currentShotOutcome << printString << endl;
    } else {
      cout << "( " << (current_over - 1) << "." << (current_ball % BALLS_PER_OVER)
           << " ) " << batsmanName << " is OUT." << endl;
    }
    balls_left--; current_ball++;
  }
  return;
}

void problem_2_the_last_4 () {
  // cout << "The last 4... !\n";
  create_batsmen_list (NotOutBatsmen, false, false, NUM_BATSMAN);
  create_inning();
  // print_inning(inning);
  cout << "======================================================================" << endl;
  print_result();
  cout << "======================================================================" << endl;
  print_commentry();
  cout << "======================================================================" << endl;
  // cout << "The last 4...done !\n";
}

void create_tie_break_inning(unsigned which_team) {

  matchBall thisBall;
  thisBall.currentTeamScore    = 0;
  balls_remaining = NUM_BALLS_SUPEROVER;

  /* first decide the striker and non-striker batsman */
  identify_striker_and_non_striker ( (which_team == TEAM_1) ? Team1NotOutBatsmen : Team2NotOutBatsmen, thisBall);

  for (auto i=0; i < NUM_BALLS_SUPEROVER; i++) {
    /* update matchBall state based on the outcome of the ball, so let the bowl be bowled */
    let_the_ball_be_bowled (i, (which_team == TEAM_1) ? Team1NotOutBatsmen : Team2NotOutBatsmen, thisBall);

    /* accomdate case of Batsman scored runs & Balls faced */
    update_scores ((which_team == TEAM_1) ? Team1NotOutBatsmen : Team2NotOutBatsmen, thisBall);

    /* Account for the outcome of this ball in the inning */
    (which_team == TEAM_1) ? Team1Inning.push_back(thisBall) : Team2Inning.push_back(thisBall);
    balls_remaining--;
    ((which_team == TEAM_1) ? final_team1_score : final_team2_score) = thisBall.currentTeamScore;

    /* check the validity conditions for a clear match finish: runs target achieved or wickets exhausted */
    runs_target_achieved = (thisBall.currentTeamScore > final_team1_score);
    wickets_exhausted    = ( (thisBall.currentShotOutcome == OUT_IDENTIFIER_IDX) );
    if ((which_team == TEAM_2) && runs_target_achieved)
    {
      winning_team_tie_break = TEAM_2;
      break;
    } else if (wickets_exhausted) {
      bool removeResult;
      if ((which_team == TEAM_1)) {
        removeResult = removeBatsmanFromNotOutList(Team1NotOutBatsmen, Team1OutBatsmen, thisBall.strikerBatsmanID);
      } else {
        removeResult = removeBatsmanFromNotOutList(Team2NotOutBatsmen, Team2OutBatsmen, thisBall.strikerBatsmanID);
        winning_team_tie_break = TEAM_1;
      }

      if ( removeResult ) {
        // cout << "(Ball # " << thisBall.ballNum << ") ERASED: batsman ID: " << thisBall.strikerBatsmanID << endl;
      } else {
        // cout << "(Ball # " << thisBall.ballNum << ") Unable to ERASE: batsman ID: " << thisBall.strikerBatsmanID << endl;
        assert(0);
      }
      break;
    } /* default is false, so if the balls exhausted the balling team wins anyway */

    /* if match is not already over, handle batsman out case and then rotate strike if needed */
    handle_out_and_rotate_strike(thisBall, which_team, true);
  }

  return;
}

void print_tie_break_final_result (list <batsman> notOutBatsmen) {

  cout << ( (winning_team_tie_break == TEAM_1) ? "Lengaburu" : "Enchai" ) << " won with "
       << balls_remaining << " ball(s) remaining" << endl << endl;
  return;
}

void print_tie_break_result () {
  print_tie_break_final_result (NotOutBatsmen);
  cout << "Lengaburu: " << endl;
  print_batsman_list(Team1OutBatsmen, false); print_batsman_list(Team1NotOutBatsmen, false);
  cout << endl;
  cout << "Enchai: " << endl;
  print_batsman_list(Team2OutBatsmen, false); print_batsman_list(Team2NotOutBatsmen, false);
  return;
}

void print_commentry_tie_break (unsigned which_team, vector <matchBall> inning) {

  unsigned balls_left   = BALLS_REMAINING;
  unsigned current_ball = 0;
  unsigned current_over = 0;

  cout << ((which_team == TEAM_1) ? "Lengaburu innings: " : "Enchai innings: ") << endl;
  for (matchBall aBall : inning) {
    bool isOut  = (aBall.currentShotOutcome == OUT_IDENTIFIER_IDX);
    string printString = (aBall.currentShotOutcome > 1) ? " runs" : " run";
    string batsmanName;
    if (which_team == TEAM_1) {
      batsmanName = getBatsmanByID(aBall.strikerBatsmanID, Team1OutBatsmen, Team1NotOutBatsmen);
    } else {
      batsmanName = getBatsmanByID(aBall.strikerBatsmanID, Team2OutBatsmen, Team2NotOutBatsmen);
    }

    if (!isOut) {
      cout << "( " << (current_over ) << "." << (current_ball % BALLS_PER_OVER) << " ) "
           << batsmanName << " scores " << aBall.currentShotOutcome << printString << endl;
    } else {
      cout << "( " << (current_over) << "." << (current_ball % BALLS_PER_OVER)
           << " ) " << batsmanName << " gets OUT! "
           << ( (which_team == TEAM_1) ? "Lengaburu all out. " : "Enchai all out. ") << endl;
    }
    balls_left--; current_ball++;
  }
  return;
}

void problem_3_the_tie_breaker () {
  // cout << "The tie breaker... !\n";
  create_batsmen_list (Team1NotOutBatsmen, true, false, NUM_BATSMAN_TIE_BREAK);
  create_batsmen_list (Team2NotOutBatsmen, true, true, NUM_BATSMAN_TIE_BREAK);
  create_tie_break_inning(TEAM_1);
  // print_inning(Team1Inning);
  create_tie_break_inning(TEAM_2);
  // print_inning(Team2Inning);
  cout << "======================================================================" << endl;
  print_tie_break_result();
  cout << "======================================================================" << endl;
  cout << "Live commentary" << endl;
  print_commentry_tie_break(TEAM_1, Team1Inning);
  print_commentry_tie_break(TEAM_2, Team2Inning);
  cout << "======================================================================" << endl;
  // cout << "The tie breker...done !\n";
}

int main() {
    /* Challenge description : https://www.geektrust.in/coding-problem/backend/cricket */
    cout << "Welcome to view the solution of cricket challenge problem! .....\n";
    string cli_input; getline( cin, cli_input, ' ' );
    int problem_to_test = atoi(cli_input.c_str());
    srand(time(NULL));

    switch (problem_to_test) {
      case 1:
        problem_1_the_toss();
        break;
      case 2:
        problem_2_the_last_4();
        break;
      case 3:
        problem_3_the_tie_breaker();
        break;
      default:
        break;
    }
  cout << "Finished cricket challenge problem! \n";
}
