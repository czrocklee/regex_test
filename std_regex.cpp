#include "std_regex.h"
#include <regex>
#include <time.h>
extern "C" {
#include "main.h"
}

void std_regex_find_all(char* pattern, char* subject, int subject_len, int repeat)
{
	std::regex re(pattern, std::regex::optimize);
	clock_t best_time = 0, time;
	int found;

	do {
		typedef std::regex_iterator<const char*> iter; 
		time = clock();
		found = std::distance(iter(subject, subject + subject_len, re), iter());
		time = clock() - time;
		if (!best_time || time < best_time)
			best_time = time;
	} while (--repeat > 0);
        
	printResult("std::regex", best_time * 1000 / CLOCKS_PER_SEC, found);
}
