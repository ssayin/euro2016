#include "group.h"
#include "team.h"
#include "teamutil.h"

#include <memory.h>
#include <assert.h>
#include <string.h>


static void __Shift(group_t grp, int index);


struct group {
	size_t size;
	size_t size_max;
	team_t* teams;
	char name;
};


void group_create(size_t max_size, group_t grp, team_block_t block, int* end) {
	grp->size = 0;
	grp->size_max = max_size;
	grp->teams = block;
	*end += max_size;
}


//group_t group_create(size_t max_size, group_block_t block, int index) {
//	group_t grp;
//	grp = block;
//	grp->teams = calloc(max_size, sizeof(team_t));
//	grp->size_max = max_size;
//	grp->size = 0;
//	return grp;
//}


//group_t group_create(group_block_t block, size_t max_size, team_block_t tblock) {
//	group_t grp;
//	grp = block;
//	grp->teams = calloc(max_size, sizeof(team_t));
//	grp->size_max = max_size;
//	grp->size = 0;
//	return grp;
//}


void group_destroy(group_t grp) {
	free(grp->teams);
	free(grp);
}


void group_set_name(group_t grp, char l) {
	grp->name = l;
}


char group_get_name(group_t grp) {
	return grp->name;
}


void group_add(group_t grp, team_t t) {
	grp->teams[grp->size] = t;
	grp->size++;
}


team_t group_at(group_t grp, int index) {
	return grp->teams[index];
}


team_t group_get_last(group_t grp) {
	return grp->teams[grp->size - 1];
}


void group_remove_last(group_t grp) {
	grp->teams[grp->size - 1] = 0;
	grp->size--;
}


void group_remove_at(group_t grp, int index) {
	__Shift(grp, index);
	grp->size--;
}


void group_remove_by_ptr(group_t grp, team_t t) {
	int i;
	for (i = 0; i < group_size(grp); ++i) {
		if (grp->teams[i] == t) {
			group_remove_at(grp, i);
		}
	}
}


team_t group_find_by_name(group_t grp, const char* name) {
	int i;
	for (i = 0; i < group_size(grp); ++i) {
		if (strcmp(team_get_name(grp->teams[i]), name) == 0) {
			return grp->teams[i];
		}
	}

	return 0;
}


void group_sort(group_t grp) {
	team_bubble_sort(grp->teams, grp->size, team_comp_cb);
}


void group_sort_coefficient(group_t grp) {
	team_bubble_sort(grp->teams, grp->size, team_coefficient_comp_cb);
}


void group_sort_third_placed(group_t grp) {
	team_bubble_sort(grp->teams, grp->size, team_tiebreaker_third_placed_cb);
}


void group_sort_qualification(group_t grp) {
	team_bubble_sort(grp->teams, grp->size, team_tiebreaker_qualification_cb);
}


size_t group_size(group_t grp) {
	return grp->size;
}


/* Fisher–Yates shuffle implementasyonu */
void group_shuffle(group_t grp) {
	int i;
	int j;
	team_t t;

	for (i = 0; i < grp->size - 1; ++i) {
		j = i + rand() / (RAND_MAX / (grp->size - i) + 1);
		t = grp->teams[j];
		grp->teams[j] = grp->teams[i];
		grp->teams[i] = t;
	}
}


group_block_t group_allocate_block(size_t size) {
	group_block_t block;
	block = calloc(size, sizeof(struct group));
	return block;
}


void group_deallocate_block(group_block_t block) {
	free(block);
}


group_t group_get_block_group(group_block_t block, int index, size_t count, int* end) {
	*end += count;
	return (block + index);
}


static void __Shift(group_t grp, int index) {
	int i;
	for (i = index; i < grp->size - 1; ++i) {
		grp->teams[i] = grp->teams[i + 1];
	}
}