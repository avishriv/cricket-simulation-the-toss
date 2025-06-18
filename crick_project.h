/*
 * crick_project.h
 *
 *  Created on: 25-Sep-2019
 *      Author: Avinash Shrivastava
 */

#ifndef CRICK_PROJECT_H_
#define CRICK_PROJECT_H_

#include "common.h"

/* global data & enums for "the toss" problem */
enum weather_e  {CLEAR = 0,     CLOUDY = 1};
enum daynight_e {DAY = 0,       NIGHT = 1};
enum teams_e    {LENGABURU = 0, ENCHAI = 1};
enum action_e   {BATS = 0,      BOWLS = 1};
static const int weatherMatrix[2][2]  = {{BATS, BOWLS}, {BOWLS, BATS}};
static const int daynightMatrix[2][2] = {{BATS, BOWLS}, {BOWLS, BATS}};

/* global data & classes for "the last 4" problem */
#define DOT_BALL_IDENTIFIER_IDX 0
#define OUT_IDENTIFIER_IDX      7
#define OUT_IDENTIFIER          0xDEAD
#define BALLS_PER_OVER          6
#define LAST_BALL_OF_OVER       5
#define BALLS_REMAINING         (NUM_OVERS * BALLS_PER_OVER)
#define RUNS_NEEDED             40
#define MAX_SHOT_OUTCOMES       8
#define NUM_BATSMAN             4
#define NUM_BATSMAN_FINAL_PAIR  2
#define NUM_OVERS               4
#define MAX_POSSIBLE_SHOTS      100
bool runs_target_achieved = false;
bool wickets_exhausted    = false;
bool didBattingTeamWin    = false;
bool is_it_a_tie          = false;
unsigned balls_remaining  = (BALLS_REMAINING);
unsigned final_team_score = 0;
enum team_e  {TEAM_1 = 0,     TEAM_2 = 1};

/* global data & enums for "the tie breaker" problem */
#define NUM_BATSMAN_TIE_BREAK   2
#define NUM_BALLS_SUPEROVER     6
unsigned final_team1_score = 0;
unsigned final_team2_score = 0;
unsigned winning_team_tie_break = 0;

#endif /* CRICK_PROJECT_H_ */
