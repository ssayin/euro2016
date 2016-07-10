#ifndef __GROUP_H__
#define __GROUP_H__

#include "team.h"

#include <stdlib.h>

typedef struct group *group_t;
typedef struct group *group_block_t;

void group_create(size_t max_size, group_t grp, team_block_t block, int* end);
//group_t group_create(group_block_t block, size_t max_size, team_block_t tblock);
void group_destroy(group_t grp);
void group_set_name(group_t grp, char l);
char group_get_name(group_t grp);
void group_add(group_t grp, team_t t);
team_t group_at(group_t grp, int index);
team_t group_get_last(group_t grp);
void group_remove_last(group_t grp);
void group_remove_at(group_t grp, int index);
void group_remove_by_ptr(group_t grp, team_t t);
team_t group_find_by_name(group_t grp, const char* name);
void group_sort(group_t grp);
void group_sort_coefficient(group_t grp);
void group_sort_third_placed(group_t grp);
void group_sort_qualification(group_t grp);
size_t group_size(group_t grp);
void group_shuffle(group_t grp);
group_block_t group_allocate_block(size_t size);
void group_deallocate_block(group_block_t block);
group_t group_get_block_group(group_block_t block, int index, size_t count, int* end);

#endif
