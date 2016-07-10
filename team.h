#ifndef __TEAM_H__
#define __TEAM_H__

#include <stdlib.h>
#include <stdint.h>

typedef struct team *team_t;
typedef struct team *team_block_t;

typedef enum {
	TEAM_OM = 0x0,
	TEAM_G = 0x2,
	TEAM_B = 0x4,
	TEAM_M = 0x6,
	TEAM_AG = 0x8,
	TEAM_YG = 0xA,
	TEAM_A = 0xC,
	TEAM_P = 0xE,
	TEAM_DG = 0x10,
	TEAM_INDEX = 0x12
} team_offset_t;

team_t team_create(team_block_t block);
void team_destroy(team_t t);
void team_reset(team_t t);
void team_set_name(team_t t, const char* name);
const char* team_get_name(team_t t);
int16_t team_get_int16(team_t t, team_offset_t loc);
void team_set_int16(team_t t, team_offset_t loc, int16_t data);
void team_set_coefficient(team_t t, double coeff);
double team_get_coefficient(team_t t);
int team_comp_cb(const team_t t1, const team_t t2);
int team_coefficient_comp_cb(const team_t t1, const team_t t2);
int team_tiebreaker_third_placed_cb(const team_t t1, const team_t t2);
int team_tiebreaker_third_placed_no_dg_cb(const team_t t1, const team_t t2);
int team_tiebreaker_qualification_cb(const team_t t1, const team_t t2);
void team_match_up(team_t t1, team_t t2, int t1_score, int t2_score);
void team_match_reverse(team_t t1, team_t t2, int t1_score, int t2_score);
team_block_t team_allocate_block(size_t size);
void team_deallocate_block(team_block_t block);
team_t team_get_block_team(team_block_t block, int index);

#endif