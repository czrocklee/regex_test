#include "boost_regex.h"
#include <boost/regex.hpp>
#include <time.h>
extern "C" {
#include "main.h"
}

void boost_regex_find_all(char* pattern, char* subject, int subject_len, int repeat)
{
	boost::regex re(pattern, boost::regex::optimize);
	clock_t best_time = 0, time;
	int found;

	do {
		typedef boost::regex_iterator<const char*> iter; 
		time = clock();
		found = std::distance(iter(subject, subject + subject_len, re), iter());
		time = clock() - time;
		if (!best_time || time < best_time)
			best_time = time;
	} while (--repeat > 0);
        
	printResult("boost::regex", best_time * 1000 / CLOCKS_PER_SEC, found);
}
