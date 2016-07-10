#include "team.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


struct team {
	int16_t OM;
	int16_t G;
	int16_t B;
	int16_t M;
	int16_t AG;
	int16_t YG;
	int16_t A;
	int16_t P;
	int16_t DG;
	int16_t index;
	double coefficient;
	char name[128];
};


team_t team_create(team_block_t block) {
	team_t team;
	team = block;
	team_reset(team);
	team->index = 0; /* Burayi boyle biraksam mi bilemedim - SS */

	return team;
}


void team_destroy(team_t t) {
	free(t->name);
	free(t);
}


void team_reset(team_t t) {
	t->A = 0;
	t->AG = 0;
	t->B = 0;
	t->G = 0;
	t->M = 0;
	t->OM = 0;
	t->P = 0;
	t->YG = 0;
	t->DG = 0;
}


void team_set_name(team_t t, const char* name) {
	strcpy(t->name, name);
}


const char* team_get_name(team_t t) {
	return t->name;
}


int16_t team_get_int16(team_t t, team_offset_t loc) {
	return *((int*)(((char*)t) + loc));
}


void team_set_int16(team_t t, team_offset_t loc, int16_t data) {
	*((int*)(((char*)t) + loc)) = data;
}


void team_set_coefficient(team_t t, double coeff) {
	t->coefficient = coeff;
}


double team_get_coefficient(team_t t) {
	return t->coefficient;
}


int team_comp_cb(const team_t a, const team_t b) {
	if (a->P > b->P) return -1;
	if (a->P < b->P) return 1;
	else { /* Puanlari esit, averajlara bakalim - SS */
		if (a->A > b->A) return -1;
		if (a->A < b->A) return 1;
		else return 0;
	}
}


int team_coefficient_comp_cb(const team_t a, const team_t b) {
	if (a->coefficient > b->coefficient) return -1;
	if (a->coefficient < b->coefficient) return 1;
	else return 0;
}


int team_tiebreaker_third_placed_cb(const team_t a, const team_t b) {
	if (a->P > b->P) return -1;
	if (a->P < b->P) return 1;
	else { /* Puanlari esit, averajlara bakalim - SS */
		if (a->A > b->A) return -1;
		if (a->A < b->A) return 1;
		else { /* En fazla gol atan kazanir - SS */
			if (a->AG > b->AG) return -1;
			if (a->AG < b->AG) return 1;
			else { /* Deplasman golleri onemli */
				if (a->DG > b->DG) return -1;
				if (a->DG < b->DG) return 1;
				else { /* Butun umutlar tukendi, katsayilara bakiyorum - SS */
					if (a->coefficient > b->coefficient) return -1;
					if (a->coefficient < b->coefficient) return 1;
					/* Bundan sonra esit olmasi imkansiz artik - SS */
				}
			}
		}
	}
}


int team_tiebreaker_third_placed_no_dg_cb(const team_t a, const team_t b) {
	if (a->P > b->P) return -1;
	if (a->P < b->P) return 1;
	else { /* Puanlari esit, averajlara bakalim - SS */
		if (a->A > b->A) return -1;
		if (a->A < b->A) return 1;
		else { /* En fazla gol atan kazanir - SS */
			if (a->AG > b->AG) return -1;
			if (a->AG < b->AG) return 1;
			else { /* Butun umutlar tukendi, katsayilara bakiyorum - SS */
				if (a->coefficient > b->coefficient) return -1;
				if (a->coefficient < b->coefficient) return 1;
				/* Bundan sonra esit olmasi imkansiz artik - SS */
				return 0;
			}
		}
	}
}


int team_tiebreaker_qualification_cb(const team_t a, const team_t b) {
	if (a->P > b->P) return -1;
	if (a->P < b->P) return 1;
	else { /* Puanlari esit, averajlara bakalim - SS */
		if (a->A > b->A) return -1;
		if (a->A < b->A) return 1;
		else { /* Averajlar esit, en fazla gol atan kazanir - SS */
			if (a->AG > b->AG) return -1;
			if (a->AG < b->AG) return 1;
			else {
				if (a->DG > b->DG) return -1;
				if (a->DG < b->DG) return 1;
				else return 0; /* Burada tekrardan iki takimin kendi aralarinda oynadigi 
							   maclar icin yukaridakilerini uygulamam gerekiyor 
							   ancak, cok uzun oldugun icin yapmaktan vazgectim */
			}
		}
	}
}


void team_match_up(team_t t1, team_t t2, int t1_score, int t2_score) {
	if (t1_score > t2_score) /* T1 Kazanir */ {
		t1->G++; /* Galibiyet */
		t2->M++; /* Maglubiyet */
		t1->P += 3; /* Puan */
	
	} else if (t1_score < t2_score) /* T2 Kazanir */ {
		t2->G++; /* Galibiyet */
		t1->M++; /* Maglubiyet */
		t2->P += 3; /* Puan */
					  
	} else {
		t1->B++; /* Beraberlik */
		t2->B++; /* Beraberlik */
		t1->P++; /* Puan */
		t2->P++; /* Puan */
	}

	t1->OM++; /* Oynanan mac */
	t2->OM++; /* Oynanan mac */

	t1->AG += t1_score; /* Atilan gol */
	t2->AG += t2_score; /* Atilan gol */

	t1->YG += t2_score; /* Yenilen gol */
	t2->YG += t1_score; /* Yenilen gol */

	t1->A += (t1_score - t2_score); /* Averaj */
	t2->A += (t2_score - t1_score); /* Averaj */

	t2->DG += t2_score; /* Deplasman golleri */
}


void team_match_reverse(team_t t1, team_t t2, int t1_score, int t2_score) {
	if (t1_score > t2_score) /* T1 Kazanir */ {
		t1->G--; /* Galibiyet */
		t2->M--; /* Maglubiyet */
		t1->P -= 3; /* Puan */

	}
	else if (t1_score < t2_score) /* T2 Kazanir */ {
		t2->G--; /* Galibiyet */
		t1->M--; /* Maglubiyet */
		t2->P -= 3; /* Puan */

	}
	else {
		t1->B--; /* Beraberlik */
		t2->B--; /* Beraberlik */
		t1->P--; /* Puan */
		t2->P--; /* Puan */
	}

	t1->OM--; /* Oynanan mac */
	t2->OM--; /* Oynanan mac */

	t1->AG -= t1_score; /* Atilan gol */
	t2->AG -= t2_score; /* Atilan gol */

	t1->YG -= t2_score; /* Yenilen gol */
	t2->YG -= t1_score; /* Yenilen gol */

	t1->A -= (t1_score - t2_score); /* Averaj */
	t2->A -= (t2_score - t1_score); /* Averaj */

	t2->DG -= t2_score; /* Deplasman golleri */
}


team_block_t team_allocate_block(size_t size) {
	team_block_t block;
	block = calloc(size, sizeof(struct team));
	return block;
}


void team_deallocate_block(team_block_t block) {
	free(block);
}


team_t team_get_block_team(team_block_t block, int index) {
	return block + index;
}