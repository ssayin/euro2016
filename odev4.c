/*
	29.06.2016 SS
	Avrupa 2016 grup kodu
	Eksiklikler: Bilinen yok.
*/

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define GROUPS_TEAM_COUNT 4

typedef struct { 
	int OM;
	int G;
	int B;
	int M;
	int AG;
	int YG;
	int A;
	int P;
} TeamStats;

typedef struct {
	TeamStats s;
	char name[21];
} Team;

typedef int (*bubble_sort_cb_t) (const Team* a, const Team* b);


int comp(const Team* a, const Team* b);
void swap(Team* a, Team* b);
void bubble_sort(Team* arr, size_t size, bubble_sort_cb_t func);
void match_up(Team* t1, Team *t2, int t1_goals, int t2_goals)


int comp(const Team* a, const Team* b) {
    if (a->s.P > b->s.P) return -1;
    if (a->s.P < b->s.P) return 1;
    else { /* Puanlari esit, averajlara bakalim - SS */
		if (a->s.A > b->s.A) return -1;
		if (a->s.A < b->s.A) return 1;
		else return 0;
	}
}

void swap(Team* a, Team* b) {
	/* TODO: Correct possible mistakes - SS */
	/* TODO: Undefined behaviorlar icin memory leak var mi bakalim - SS */

	if (a == b) return; /* Optimize ediyorum - SS */
	Team* temp;
	temp = malloc(sizeof(Team));
	memcpy(temp, a, sizeof(Team));
	memcpy(a, b, sizeof(Team));
	memcpy(b, temp, sizeof(Team));
	free(temp);
}

void bubble_sort(Team* arr, size_t size, bubble_sort_cb_t func) { /* TODO: Optimize etmemiz gerek - SS */
	size_t i, j;
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			if (func(arr + i, arr + j) < 0)
				swap(arr + i, arr + j);
		}
	}
}

void match_up(Team* t1, Team *t2, int t1_goals, int t2_goals) {
	/* T1 Kazanir */
	if (t1_goals > t2_goals) {
		t1->s.G++; /* Galibiyet */
		t2->s.M++; /* Maglubiyet */
		t1->s.P += 3; /* Puan */
	/* T2 Kazanir */
	} else if (t1_goals < t2_goals) {
		t2->s.G++; /* Galibiyet */
		t1->s.M++; /* Maglubiyet */
		t2->s.P += 3; /* Puan */
	/* Beraberlik */
	} else {
		t1->s.B++; /* Beraberlik */
		t2->s.B++; /* Beraberlik */
		t1->s.P++; /* Puan */
		t2->s.P++; /* Puan */
	}
	
	t1->s.OM++; /* Oynanan mac */
	t2->s.OM++; /* Oynanan mac */
	
	t1->s.AG += t1_goals; /* Atilan gol */
	t2->s.AG += t2_goals; /* Atilan gol */
	
	t1->s.YG += t2_goals; /* Yenilen gol */
	t2->s.YG += t1_goals; /* Yenilen gol */
	
	t1->s.A += (t1_goals - t2_goals); /* Averaj */
	t2->s.A += (t2_goals - t1_goals); /* Averaj */
}

void print_table_header() {
	printf("#                   Takim   OM   G   B   M   AG   YG   A   P\n");
}

void print_team(Team* t, int index) {
	const char* str_format = "%d%24s%5d%4d%4d%4d%5d%5d%4d%4d\n";
	
	printf(str_format, index + 1, t->name, t->s.OM, t->s.G, t->s.B, t->s.M, t->s.AG, t->s.YG, t->s.A, t->s.P);
}

int chk_format(int* s1, int *s2) {
	char buf[100];
	char* end;
	
	gets(buf);
	
	*s1 = strtol(buf, &end, 10);
	
	if (end[0] != '-') {
		printf("Duzgun yazin\n");
		return 0;
	}
	
	*s2 = strtol(end + 1, &end, 10);
	
	if (end[0] != '\0') {
		printf("Duzgun yazin\n");
		return 0;
	}
	
	return 1;
}

int main(int argc, char* argv[]) {
	Team teams[GROUPS_TEAM_COUNT] = {{0}, {0}, {0}, {0}};
	int i;
	int j;
	int score1;
	int score2;
	
	for (i = 0; i < GROUPS_TEAM_COUNT; ++i) {
		printf("%d. takimin ismini giriniz: ", i + 1);
		gets(teams[i].name);
	}
	
	for (j = 0; j < GROUPS_TEAM_COUNT - 1; ++j) {
		for (i = j; i < GROUPS_TEAM_COUNT - 1; ++i) {
			printf("%s ile %s arasindaki skor: ", teams[j].name, teams[i + 1].name);
			while (!chk_format(&score1, &score2));
			match_up(&teams[j], &teams[i + 1], score1, score2);
		}
	}
	
	bubble_sort(teams, GROUPS_TEAM_COUNT, comp);
	
	print_table_header();
	for (i = 0; i < GROUPS_TEAM_COUNT; i++) {
		print_team(&teams[i], i);
	}
	
	return 0;
}
