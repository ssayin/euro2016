#ifndef __SCOREBOARD_H__
#define __SCOREBOARD_H__

#include "team.h"
#include "group.h"

#define PREFIX_QUALIFYING_GROUP ("Group")
#define PREFIX_GROUP ("Group")
#define PREFIX_SEEDING_POT ("Pot")

void scoreboard_print_group(const char* prefix, group_t grp);
void scoreboard_print_group_scores(group_t grp);
void scoreboard_print_group_scores_teams(team_t arr[], int count);

#endif
