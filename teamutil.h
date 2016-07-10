#ifndef __TEAMUTIL_H__
#define __TEAMUTIL_H__

#include <stdlib.h>

#include "team.h"

typedef int (*team_bubble_sort_cb_t) (team_t a, team_t b);

void team_bubble_sort(team_t* arr, size_t arr_size, team_bubble_sort_cb_t func);

#endif