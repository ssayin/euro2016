#include "match.h"
#include "team.h"

#include <assert.h>
#include <math.h>
#include <time.h>


void match_evaluate(match_info_t info, match_result_t res, int alter_stats) {
	int score1 = 0;
	int score2 = 0;

	/* Burada oyle bir algoritma lazim ki adil olsun skorlar */
	score1 = rand() % 8;
	score2 = rand() % 8;

	if (res != 0) { /* Ikinci parametre opsiyonel olsun - SS */
		res->home = info->home;
		res->away = info->away;
		res->score1 = score1;
		res->score2 = score2;
	}

	if (alter_stats) {
		team_match_up(info->home, info->away, score1, score2);
	}
}


team_t match_penalty_shoot_out(match_info_t info, match_result_t res) {
	int score1 = 0;
	int score2 = 0;
	int i;

	/* Buraya takim gucu etken olarak eklenebilir */

	for (i = 0; i < 5; ++i) {
		score1 += rand() % 2;
		score2 += rand() % 2;
		/* Yetismek imkansizsa devam ettirmenin bir anlami yok - SS */
		if (abs(score1 - score2) > 5 - i) break;
	}

	while (score1 == score2) {
		score1 += rand() % 2;
		score2 += rand() % 2;
	}

	res->home = info->home;
	res->away = info->away;
	res->score1 = score1;
	res->score2 = score2;

	if (score1 > score2) return info->home;
	if (score1 < score2) return info->away;
}


void match_reverse(match_result_t res) {
	team_match_reverse(res->home, res->away, res->score1, res->score2);
}


int match_date_compare(match_info_t i1, match_info_t i2) {
	if (i1->d->d == i2->d->d &&
		i1->d->m == i2->d->m &&
		i1->d->y == i2->d->y) return 0;
	else return -1;
}