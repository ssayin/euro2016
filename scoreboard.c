#include "scoreboard.h"
#include "team.h"

#include <stdio.h>


const char* str_format = "%d%24s%5d%4d%4d%4d%5d%5d%4d%4d\n";
const char* header = "#                   Takim   OM   G   B   M   AG   YG   A   P\n";


void scoreboard_print_group(const char* prefix, group_t grp) {
	int i;
	printf("---------------\n");
	printf("%s %c\n", prefix, group_get_name(grp));
	printf("---------------\n");

	for (i = 0; i < group_size(grp); ++i) {
		printf("%s\n", team_get_name(group_at(grp, i)));
	}

	putchar('\n');
}


void scoreboard_print_group_scores(group_t grp) {
	team_t t;
	int i;
	printf("---------- Group %c ----------\n", group_get_name(grp));
	printf(header);
	
	for (i = 0; i < group_size(grp); ++i) {
		t = group_at(grp, i);

		printf(str_format, i + 1, team_get_name(t), team_get_int16(t, TEAM_OM),
			team_get_int16(t, TEAM_G), team_get_int16(t, TEAM_B), 
			team_get_int16(t, TEAM_M), team_get_int16(t, TEAM_AG), 
			team_get_int16(t, TEAM_YG), team_get_int16(t, TEAM_A), team_get_int16(t, TEAM_P));
	}

	putchar('\n');
}


void scoreboard_print_group_scores_teams(team_t arr[], int count) {
	team_t t;
	int i;
	printf(header);
	for (i = 0; i < count; ++i) {
		t = arr[i];

		printf(str_format, i + 1, team_get_name(t), team_get_int16(t, TEAM_OM),
			team_get_int16(t, TEAM_G), team_get_int16(t, TEAM_B),
			team_get_int16(t, TEAM_M), team_get_int16(t, TEAM_AG),
			team_get_int16(t, TEAM_YG), team_get_int16(t, TEAM_A), team_get_int16(t, TEAM_P));
	}

	putchar('\n');
}
