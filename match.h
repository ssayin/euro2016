#ifndef __MATCH_H__
#define __MATCH_H__

#include "team.h"

typedef struct match_info *match_info_t;
typedef struct match_date *match_date_t;
typedef struct match_result* match_result_t;

struct match_date {
	int d;
	int m;
	int y;
};

struct match_info {
	team_t home;
	team_t away;
	match_date_t d;
};

struct match_result {
	team_t home;
	team_t away;
	int score1;
	int score2;
};

void match_evaluate(match_info_t info, match_result_t res, int alter_stats);
team_t match_penalty_shoot_out(match_info_t info, match_result_t res);
void match_reverse(match_result_t res);
int match_date_compare(match_info_t i1, match_info_t i2);

#endif