#include "teamutil.h"
#include "team.h"

#include <string.h>


static void team_swap(team_t* a, team_t* b);


void team_bubble_sort(team_t* arr, size_t arr_size, team_bubble_sort_cb_t func) { /* TODO: Optimize etmemiz gerek - SS */
	size_t i, j;
	team_t a;
	team_t b;
	for (i = 0; i < arr_size; i++) {
		for (j = 0; j < arr_size; j++) {
			//a = arr[i];
			//b = arr[j];
			if (func(arr[i], arr[j]) < 0)
				team_swap(&arr[i], &arr[j]);
		}
	}
}


static void team_swap(team_t* a, team_t* b) {
	team_t temp;
	temp = *a;
	*a = *b;
	*b = temp;
}
