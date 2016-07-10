#include "tournament.h"
#include "group.h"
#include "team.h"
#include "match.h"
#include "literals.h"
#include "scoreboard.h"
#include "teamutil.h"

#include <time.h>
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>


static void create_teams(team_block_t block);


static void step(match_info_t info, size_t team_count, group_t first, 
	group_t second, match_result_t ms1, match_result_t ms2);


static void get_group_array(group_t* grp, size_t count, group_block_t grp_block, int* last_node_grp);


void tournament() {
	team_block_t team_block;
	group_block_t grp_block;

	team_t third[QUALIFYING_STAGE_GROUP_COUNT];
	team_t third2[GROUP_COUNT];

	int last_node_grp = 0;
	int last_node_team = TEAM_COUNT + 1; /* Fransayi da katiyoruz */

	group_t seeding_pots[QUALIFYING_STAGE_GROUP_TEAM_COUNT];
	group_t qualifying[QUALIFYING_STAGE_GROUP_COUNT];
	group_t playoff_pots[PLAYOFF_POT_COUNT];
	group_t pots[GROUP_TEAM_COUNT];
	group_t groups[GROUP_COUNT];

	group_t qualified;
	group_t knockout;
	group_t quarter_final;
	group_t semi_final;
	group_t final_stage;

	match_info_t info;
	struct match_result* results;
	struct match_result ms1, ms2;

	team_t team;
	team_t france;
	team_t t1, t2;

	int rnd;
	int i, j, k;
	int count = 0;
	int diff = 0;

	int score1 = 0;
	int score2 = 0;

	srand(time(NULL));

	team_block = team_allocate_block(TEAM_COUNT + 1 + 218); /* Fransa icin */
	grp_block = group_allocate_block(QUALIFYING_STAGE_GROUP_TEAM_COUNT + QUALIFYING_STAGE_GROUP_COUNT +
		PLAYOFF_POT_COUNT + GROUP_TEAM_COUNT + GROUP_COUNT + 5); /* 5 tane arraysiz grup var */
	create_teams(team_block);

	get_group_array(seeding_pots, QUALIFYING_STAGE_GROUP_TEAM_COUNT, grp_block, &last_node_grp);
	get_group_array(qualifying, QUALIFYING_STAGE_GROUP_COUNT, grp_block, &last_node_grp);
	get_group_array(playoff_pots, PLAYOFF_POT_COUNT, grp_block, &last_node_grp);
	get_group_array(pots, GROUP_TEAM_COUNT, grp_block, &last_node_grp);
	get_group_array(groups, GROUP_COUNT, grp_block, &last_node_grp);

	for (i = 0; i < QUALIFYING_STAGE_GROUP_TEAM_COUNT; ++i) {
		/* Potlardan takimlar eslestirilip gruplar olusturulacak, 
		6 tane pot var yani 6 grup oluyor. 
		Potlardaki takim sayisi da maks. 9 - SS */
		group_create(QUALIFYING_STAGE_GROUP_COUNT, seeding_pots[i], team_get_block_team(team_block, last_node_team), &last_node_team);

		group_set_name(seeding_pots[i], '1' + i);

		for (j = 0; j < QUALIFYING_STAGE_GROUP_COUNT; ++j) {
			/* Her bir gruba 9 tane koyacak */
			if (i * QUALIFYING_STAGE_GROUP_COUNT + j == 53) break;
			group_add(seeding_pots[i], team_get_block_team(team_block, QUALIFYING_STAGE_GROUP_COUNT * i + j));
		}

		scoreboard_print_group(PREFIX_SEEDING_POT, seeding_pots[i]);
	}

	for (i = 0; i < QUALIFYING_STAGE_GROUP_COUNT; ++i) {
		group_create(QUALIFYING_STAGE_GROUP_TEAM_COUNT, qualifying[i], team_get_block_team(team_block, last_node_team), &last_node_team);
		group_set_name(qualifying[i], 'A' + i);

		/* Butun potlari tara (6 tane pot var) her pottan bir tane takim ekle gruba, 
		her potta 9 takim var dolayisi ile 9 tane grup olusacak - SS */
		for (j = 0; j < QUALIFYING_STAGE_GROUP_TEAM_COUNT; ++j) {
			if (group_size(seeding_pots[j]) == 0) break;
			rnd = rand() % group_size(seeding_pots[j]);
			team = group_at(seeding_pots[j], rnd);
			group_remove_at(seeding_pots[j], rnd);
			group_add(qualifying[i], team);
		}

		scoreboard_print_group(PREFIX_QUALIFYING_GROUP, qualifying[i]);
	}

	/* Artik bununla isimiz bitti - SS */
	//for (i = 0; i < QUALIFYING_STAGE_GROUP_TEAM_COUNT; ++i) { 
	//	group_destroy(seeding_pots[i]);
	//}

	info = malloc(sizeof(struct match_info));

	/* Hafiza fragmentasyonunu onlemek icin blok olarak alloc edecegim - SS */
	/* Bunu 260 buldum nasil buldugumu anlatmayacagim - SS */
	results = calloc(QUALIFICATION_MATCH_COUNT, sizeof(struct match_result));
	
	/* Artik qualification maclarini oynatabiliriz - SS */
	for (i = 0; i < QUALIFYING_STAGE_GROUP_COUNT; ++i) { /* Butun gruplar */
		for (j = 0; j < group_size(qualifying[i]); ++j) { /* Gruptaki butun takimlar */
			info->home = group_at(qualifying[i], j);
			for (k = 0; k < j; ++k) { /* Kendi haric herkesle oynar (rovanslari dahil ettik) */
				info->away = group_at(qualifying[i], k);
				match_evaluate(info, &results[count], 1);
				count++;
			}
			
			for (k = j + 1; k < group_size(qualifying[i]); ++k) {
				info->away = group_at(qualifying[i], k);
				match_evaluate(info, &results[count], 1);
				count++;
			}
		}
	}

	/* Grup sonuncularini gruptan atip bir de oyle hesaplayalim, 
	cunku son grupta 5 takim var adil olmuyor - SS */
	for (i = 0; i < QUALIFYING_STAGE_GROUP_COUNT - 1; ++i) {
		group_sort_qualification(qualifying[i]);
		team = group_get_last(qualifying[i]);

		/* Bu takimin oynadigi maclari saymayalim - SS */
		for (j = 0; j < 30; ++j) {
			if (results[j + i * 30].home == team || results[j + i * 30].away == team) {
				match_reverse(&results[j + i * 30]);
			}
		}

		group_remove_last(qualifying[i]);
	}

	qualified = group_get_block_group(grp_block, last_node_grp, 1, &last_node_grp);
	group_create(QUALIFIED_TEAM_COUNT, qualified, team_get_block_team(team_block, last_node_team), &last_node_team);

	/* Gruptaki ilk 2 takim bize lazim - SS */
	for (i = 0; i < QUALIFYING_STAGE_GROUP_COUNT; ++i) {
		group_sort_qualification(qualifying[i]);
		group_add(qualified, group_at(qualifying[i], 0));
		group_add(qualified, group_at(qualifying[i], 1));
	}

	/* Sonuclari hesaplayip gruplarin son halini basalim - SS */
	for (i = 0; i < QUALIFYING_STAGE_GROUP_COUNT; ++i) {
		scoreboard_print_group_scores(qualifying[i]);
	}

	/* Gruplarda en iyi 3. takim direkt olarak Fransa'ya gider */
	for (i = 0; i < QUALIFYING_STAGE_GROUP_COUNT; ++i) {
		third[i] = group_at(qualifying[i], 2); /* Grup 3.su */
	}

	/* Aslinda bunu define etsek daha dogru olur - SS */
	team_bubble_sort(third, QUALIFYING_STAGE_GROUP_COUNT, team_tiebreaker_third_placed_cb); 

	group_add(qualified, third[0]);

	printf("---------- Grup 3.leri ----------\n");

	scoreboard_print_group_scores_teams(third, QUALIFYING_STAGE_GROUP_COUNT);

	/* Artik bununla ugrasmak istemiyorum - SS */
	/* Yine seeding potlari olusturuyoruz, en yuksek kat sayililar ile en azlar ayrilacak - SS */
	team_bubble_sort(third + 1, QUALIFYING_STAGE_GROUP_COUNT - 1, team_coefficient_comp_cb);

	/* Play-offlar */
	/* Potlari yapalim */
	for (i = 0; i < PLAYOFF_POT_COUNT; ++i) {
		group_create(PLAYOFF_TEAM_COUNT / PLAYOFF_POT_COUNT, playoff_pots[i], 
			team_get_block_team(team_block, last_node_team), &last_node_team);
		group_set_name(playoff_pots[i], '1' + i);
	}

	/* Burada potlardan ilki seeded, digeri unseeded */
	for (k = 0; k < PLAYOFF_POT_COUNT; ++k) {
		for (i = 1; i < PLAYOFF_TEAM_COUNT / PLAYOFF_POT_COUNT + 1; ++i) {
			group_add(playoff_pots[k], third[k * PLAYOFF_TEAM_COUNT / PLAYOFF_POT_COUNT + i]);
		}

		scoreboard_print_group(PREFIX_SEEDING_POT, playoff_pots[k]);
	}

	printf("----------------\n");
	printf("Play-offlar\n");
	printf("----------------\n");

	/* Iki pottan rastgele iki takip eslesip 2 mac oynarlar */
	for (i = 0; i < PLAYOFF_TEAM_COUNT / PLAYOFF_POT_COUNT; ++i) {
		rnd = rand() % group_size(playoff_pots[0]);
		t1 = group_at(playoff_pots[0], rnd);
		group_remove_at(playoff_pots[0], rnd);

		rnd = rand() % group_size(playoff_pots[1]);
		t2 = group_at(playoff_pots[1], rnd);
		group_remove_at(playoff_pots[1], rnd);

		info->home = t1;
		info->away = t2;
		match_evaluate(info, &ms1, 0);
		printf("%s %d - %d %s\n", team_get_name(ms1.home), ms1.score1, ms1.score2, team_get_name(ms1.away));
		diff += ms1.score1;
		diff -= ms1.score2;


		info->home = t2;
		info->away = t1;
		match_evaluate(info, &ms2, 0);
		printf("%s %d - %d %s\n", team_get_name(ms2.home), ms2.score1, ms2.score2, team_get_name(ms2.away));
		diff += ms2.score1;
		diff -= ms2.score2;

		if (diff > 0) {
			group_add(qualified, t1);
		}

		else if (diff < 0) {
			group_add(qualified, t2);
		}

		else {
			/* Tie-breaker */
			group_add(qualified, t1);
		}

		diff = 0;
	}

	putchar('\n');

	
	for (i = 0; i < GROUP_TEAM_COUNT; ++i) { /* Pot sayisi */
		group_create(GROUP_COUNT, pots[i], team_get_block_team(team_block, last_node_team), &last_node_team);
		group_set_name(pots[i], '1' + i);
	}

	/* Son olarak Fransa'yi ekliyorum Fransa elemelere katilmadan direkt gecti, 
		ev sahibi oldugu icin */
	france = team_get_block_team(team_block, 53);
	team_set_int16(france, TEAM_INDEX, 53);
	team_set_name(france, "Fransa");
	team_set_coefficient(france, 30.992);
	/* group_add(qualified, france); */
	group_add(pots[0], france); /* Fransa ev sahibi, otomatikmen ilk potta - SS */
 
	t1 = group_find_by_name(qualified, "Ispanya");
	group_remove_by_ptr(qualified, t1); /* Gruptan atalim */
	group_sort_coefficient(qualified);
	

	if (t1) { /* Ispanya qualified olmayabilir o yuzden boyle */
		team_reset(t1); /* Once bi skorlari resetleyelim - SS */
		group_add(pots[0], t1); /* Ispanya bir onceki sampiyon, otomatikmen ilk potta - SS */
	}
	
	count = 0;

	for (i = 0; i < GROUP_TEAM_COUNT; ++i) { /* Pot sayisi */
		while (group_size(pots[i]) != GROUP_COUNT) { /* Doldu */
			team_reset(group_at(qualified, count)); /* On eleme skorlarini resetleyelim */
			group_add(pots[i], group_at(qualified, count));
			count++;
		}

		scoreboard_print_group(PREFIX_SEEDING_POT, pots[i]);
	}

	/* Iste basliyoruz, son kez gruplar olusturulacak - SS */
	for (i = 0; i < GROUP_COUNT; ++i) {
		group_create(GROUP_TEAM_COUNT, groups[i], team_get_block_team(team_block, last_node_team), &last_node_team);
		group_set_name(groups[i], 'A' + i);

		for (j = 0; j < GROUP_TEAM_COUNT; ++j) {
			rnd = rand() % group_size(pots[j]);
			team = group_at(pots[j], rnd);
			group_remove_at(pots[j], rnd);
			group_add(groups[i], team);
		}

		scoreboard_print_group(PREFIX_GROUP, groups[i]);
	}

	for (k = 0; k < GROUP_COUNT; ++k) {
		for (j = 0; j < group_size(groups[k]); ++j) {
			for (i = j; i < group_size(groups[k]) - 1; ++i) {
				info->home = group_at(groups[k], j);
				info->away = group_at(groups[k], i + 1);
				match_evaluate(info, &ms1, 1);
				printf("%s %d - %d %s\n", team_get_name(info->home), ms1.score1, ms1.score2, team_get_name(info->away));
			}
		}

		putchar('\n');
	}

	for (i = 0; i < GROUP_COUNT; ++i) {
		group_sort_qualification(groups[i]); /* Aslinda bu gruplar icin ayri bir 
											 tie-break sistemi var, ama bunu kullanicam - SS */
		scoreboard_print_group_scores(groups[i]);
	}

	/* Ucuncu takimlarin rankingleri */
	for (i = 0; i < GROUP_COUNT; ++i) {
		third2[i] = group_at(groups[i], 2); /* Ucunculeri bir alalim - SS */
	}

	team_bubble_sort(third2, GROUP_COUNT, team_tiebreaker_third_placed_no_dg_cb);

	printf("---------- Grup 3.leri ----------\n");

	scoreboard_print_group_scores_teams(third2, GROUP_COUNT);

	knockout = group_get_block_group(grp_block, last_node_grp, 1, &last_node_grp);
	quarter_final = group_get_block_group(grp_block, last_node_grp, 1, &last_node_grp);
	semi_final = group_get_block_group(grp_block, last_node_grp, 1, &last_node_grp);
	final_stage = group_get_block_group(grp_block, last_node_grp, 1, &last_node_grp);

	group_create(KNOCKOUT_PHASE_TEAM_COUNT, knockout, team_get_block_team(team_block, last_node_team), &last_node_team);
	group_create(QUARTER_FINAL_TEAM_COUNT, quarter_final, team_get_block_team(team_block, last_node_team), &last_node_team);
	group_create(SEMI_FINAL_TEAM_COUNT, semi_final, team_get_block_team(team_block, last_node_team), &last_node_team);
	group_create(FINAL_TEAM_COUNT, final_stage, team_get_block_team(team_block, last_node_team), &last_node_team);

	/* Knock-out phase */
	for (int i = 0; i < 4; ++i) {
		group_add(knockout, third[i]); /* Ucunculerin ilk 4'u lazim */
	}

	for (i = 0; i < GROUP_COUNT; ++i) { /* Birinci ve ikinciler de lazim */
		group_add(knockout, group_at(groups[i], 0)); 
		group_add(knockout, group_at(groups[i], 1));
	}

	group_shuffle(knockout); /* Karistiralim takimlari */

	/* Knock-out phase */

	printf("Knock-out phase\n");

	step(info, KNOCKOUT_PHASE_TEAM_COUNT, knockout, quarter_final, &ms1, &ms2);

	/* Ceyrek final */

	printf("\nCeyrek final\n");

	step(info, QUARTER_FINAL_TEAM_COUNT, quarter_final, semi_final, &ms1, &ms2);

	/* Yari final */

	printf("\nYari final\n");

	step(info, SEMI_FINAL_TEAM_COUNT, semi_final, final_stage, &ms1, &ms2);

	/* Final */

	printf("\nFinal\n");

	step(info, FINAL_TEAM_COUNT, final_stage, 0, &ms1, &ms2);

	/* Temizlik */
	free(results);
	free(info);

	team_deallocate_block(team_block);
	group_deallocate_block(grp_block);

	return;
}


static void create_teams(team_block_t block) {
	team_t team;
	int i;

	/* Burada aslinda cok fazla memory fragmentasyonu oluyor,
	cunku her biri icin ayri ayri malloc ediyoruz, block olarak alloc etmek sart - SS */
	for (i = 0; i < TEAM_COUNT; ++i) { /* Ilk once butun takimlari yaratalim - SS */
		team = team_get_block_team(block, i);
		team_create(team);
		team_set_int16(team, TEAM_INDEX, i);
		team_set_name(team, TEAM_NAMES[i]);
		team_set_coefficient(team, COEFFICIENTS[i]);
	}
}


static void step(match_info_t info, size_t team_count, group_t first, group_t second, match_result_t ms1, match_result_t ms2) {
	int i;
	int score1;
	int score2;
	team_t team;

	for (i = 0; i < team_count; i += 2) {
		info->home = group_at(first, i);

		info->away = group_at(first, i + 1);
		match_evaluate(info, ms1, 0);

		score1 = ms1->score1;
		score2 = ms1->score2;

		if (score1 > score2) {
			if (second != 0) {
				group_add(second, info->home);
			}
		}

		else if (score1 < score2) {
			if (second != 0) {
				group_add(second, info->away);
			}
		}

		else {
			team = match_penalty_shoot_out(info, ms2); /* Kazanani donduruyor - SS */
			score1 = score1 + ms2->score1;
			score2 = score2 + ms2->score2;
			printf("(penalti) ");
			if (second != 0) {
				group_add(second, team);
			}
		}

		printf("%s %d - %d %s\n", team_get_name(info->home), score1, score2, team_get_name(info->away));
	}
}


static void get_group_array(group_t* grp, size_t count, group_block_t grp_block, int* last_node_grp) {
	int i;
	for (i = 0; i < count; ++i) {
		grp[i] = group_get_block_group(grp_block, *last_node_grp, 1, last_node_grp);
	}
}