#include <stdio.h>
#include <string.h>
#include <time.h>

#include "onig/oniguruma.h"
#include "main.h"

void onig_find_all(char* pattern, char* subject, int subject_len, int repeat)
{
	regex_t* reg;
	OnigRegion *region;
	clock_t best_time = 0, time;
	unsigned char *ptr;
	int res, len, found;

	res = onig_new(&reg, (unsigned char *)pattern, (unsigned char *)pattern + strlen((char* )pattern),
		ONIG_OPTION_DEFAULT, ONIG_ENCODING_ASCII, ONIG_SYNTAX_DEFAULT, NULL);
	if (res != ONIG_NORMAL) {
		printf("Onig compilation failed\n");
		return;
	}
	region = onig_region_new();
	if (!region) {
		printf("Cannot allocate region\n");
		return;
	}

	do {
		found = 0;
		ptr = (unsigned char *)subject;
		len = subject_len;

		time = clock();
		while (1) {
			res = onig_search(reg, ptr, ptr + len, ptr, ptr + len, region, ONIG_OPTION_NONE);
			if (res < 0)
				break;
			// printf("match: %d %d\n", (ptr - (unsigned char *)subject) + region->beg[0], (ptr - (unsigned char *)subject) + region->end[0]);
			ptr += region->end[0];
			len -= region->end[0];
			found++;
		}
		time = clock() - time;
		if (!best_time || time < best_time)
			best_time = time;
	} while (--repeat > 0);
	printResult("onig", best_time * 1000 / CLOCKS_PER_SEC, found);

	onig_region_free(region, 1);
	onig_free(reg);
}
