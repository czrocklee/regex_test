#include <stdio.h>
#include <time.h>

#include "pcre/config.h"
#include "pcre/pcre.h"
#include "main.h"

void pcre_find_all(char* pattern, char* subject, int subject_len, int repeat, int mode)
{
	pcre *re;
	const char *error;
	int err_val, match[64];
	pcre_extra *extra;
	pcre_jit_stack *stack = NULL;
	char *ptr;
	int len;
	clock_t best_time = 0, time = 0;
	int found;

	static int work_space[4096];
	re = pcre_compile(
		pattern,	/* the pattern */
		PCRE_MULTILINE | PCRE_NEWLINE_ANYCRLF,	/* options */
		&error,		/* for error message */
		&err_val,	/* for error offset */
		NULL);		/* use default character tables */

	if (!re) {
		printf("PCRE compilation failed at offset %d: %s\n", err_val, error);
		return;
	}

	error = NULL;
	extra = pcre_study(re, mode == 2 ? PCRE_STUDY_JIT_COMPILE : 0, &error);
	if (error) {
		printf("PCRE study failed: %s\n", error);
		return;
	}
	if (mode == 2) {
		found = 0;
		pcre_fullinfo(re, extra, PCRE_INFO_JIT, &found);
		if (!found) {
			printf("PCRE JIT compilation failed: %s\n", error);
			return;
		}
		stack = pcre_jit_stack_alloc(65536, 65536);
	}

	do {
		found = 0;
		ptr = subject;
		len = subject_len;
		switch (mode) {
		case 0:
			time = clock();
			while (1) {
				err_val = pcre_exec(
					re,		/* the compiled pattern */
					extra,		/* extra data */
					ptr,		/* the subject string */
					len,		/* the length of the subject */
					0,		/* start at offset 0 in the subject */
					0,		/* default options */
					match,		/* output vector for substring information */
					64);		/* number of elements in the output vector */

				if (err_val <= 0) {
					if (err_val == PCRE_ERROR_NOMATCH)
						break;
					printf("PCRE pcre_exec failed with: %d\n", err_val);
					break;
				}

				// printf("match: %d %d\n", (ptr - subject) + match[0], (ptr - subject) + match[1]);
				ptr += match[1];
				len -= match[1];
				found++;
			}
			time = clock() - time;
			break;

		case 1:
			time = clock();
			while (1) {
				err_val = pcre_dfa_exec(
					re,		/* the compiled pattern */
					extra,		/* extra data */
					ptr,	/* the subject string */
					len,	/* the length of the subject */
					0,		/* start at offset 0 in the subject */
					0,		/* default options */
					match,		/* output vector for substring information */
					2,		/* number of elements in the output vector */
					work_space,	/* number of elements (NOT size in bytes) */
					4096);

				if (err_val < 0) {
					if (err_val == PCRE_ERROR_NOMATCH)
						break;
					printf("PCRE pcre_exec failed\n");
					break;
				}

				// printf("match: %d %d\n", (ptr - subject) + match[0], (ptr - subject) + match[1]);
				ptr += match[1];
				len -= match[1];
				found++;
			}
			time = clock() - time;
			break;

		case 2:
			time = clock();
			while (1) {
				err_val = pcre_jit_exec(
					re,		/* the compiled pattern */
					extra,		/* extra data */
					ptr,		/* the subject string */
					len,		/* the length of the subject */
					0,		/* start at offset 0 in the subject */
					0,		/* default options */
					match,		/* output vector for substring information */
					64,		/* number of elements in the output vector */
					stack);		/* jit stack */

				if (err_val <= 0) {
					if (err_val == PCRE_ERROR_NOMATCH)
						break;
					printf("PCRE pcre_exec failed with: %d\n", err_val);
					break;
				}

				// printf("match: %d %d\n", (ptr - subject) + match[0], (ptr - subject) + match[1]);
				ptr += match[1];
				len -= match[1];
				found++;
			}
			time = clock() - time;
			break;
		}
		if (!best_time || time < best_time)
			best_time = time;
	} while (--repeat > 0);
	printResult(mode == 0 ? "pcre" : (mode == 1 ? "pcre-dfa" : "pcre-jit"), best_time * 1000 / CLOCKS_PER_SEC, found);

	if (extra)
		pcre_free_study(extra);
	if (stack)
		pcre_jit_stack_free(stack);
	pcre_free(re);
}
