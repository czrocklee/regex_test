#include <stdio.h>
#include <time.h>

#include "re2/util/c_helper.h"
#include "main.h"

void re2_find_all(char* pattern, char* subject, int subject_len, int repeat)
{
	void* obj = get_re2_object(pattern);
	clock_t best_time = 0, time;
	int found;

	if (!obj) {
		printf("RE2 compilation failed\n");
		return;
	}

	do {
		time = clock();
		found = search_all_re2(obj, subject, subject_len);
		time = clock() - time;
		if (!best_time || time < best_time)
			best_time = time;
	} while (--repeat > 0);
	printResult("re2", best_time * 1000 / CLOCKS_PER_SEC, found);

	free_re2_object(obj);
}
