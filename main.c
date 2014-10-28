#include <stdio.h>
#include <time.h>
#include <string.h>
#include <malloc.h>
#include "main.h"
#include "std_regex.h"
#include "boost_regex.h"

//#define HTML

static char* data = NULL;
static int data_len = 0;

void load(char* file_name)
{
	int i;

	FILE* f;
	f = fopen(file_name, "rb");
	if (!f) {
		printf("Cannot open '%s'!\n", file_name);
		return;
	}

	fseek(f, 0, SEEK_END);
	data_len = ftell(f);
	fseek(f, 0, SEEK_SET);

	data = (char*)malloc(data_len + 1);
	if (!data) {
		printf("Cannot allocate memory!\n");
		fclose(f);
		return;
	}
	data[data_len] = '\0';
	fread(data, data_len, 1, f);
	fclose(f);

	for (i = 0; i < data_len; ++i)
		if (data[i] == '\r')
			data[i] = '\n';
	printf("'%s' loaded. (Length: %d bytes)\n", file_name, data_len);
}

void find_all(char* pattern, char* subject, int subject_len)
{
#ifdef HTML
	printf("<tr><td class=\"pattern\">%s</td>", pattern);
#else
	printf("-----------------\nRegex: '%s'\n", pattern);
#endif
	pcre_find_all(pattern, subject, subject_len, 5, 0);
	pcre_find_all(pattern, subject, subject_len, 5, 1);
	tre_find_all(pattern, subject, subject_len, 5);
	onig_find_all(pattern, subject, subject_len, 5);
	re2_find_all(pattern, subject, subject_len, 5);
	pcre_find_all(pattern, subject, subject_len, 5, 2);
	std_regex_find_all(pattern, subject, subject_len, 5);
	boost_regex_find_all(pattern, subject, subject_len, 5);
#ifdef HTML
	printf("</tr>\n");
#endif
}

void printResult(char* name, int time, int found)
{
#ifdef HTML
	printf("<td class=\"time\">%d ms</td>", (int)time);
#else
	printf("[%8s] time: %5d ms (%d matches)\n", name, (int)time, found);
#endif
	fflush(stdout);
}

int main()
{
	load("mark.txt");
	if (data_len == 0)
		return -1;
	//find_all("a(aa)+(y)", "xxaaaayyaaayssaayaaaay", 22); // Test function

	find_all("Twain", data, data_len);
	find_all("^Twain", data, data_len);
	find_all("Twain$", data, data_len);
	find_all("Huck[a-zA-Z]+|Finn[a-zA-Z]+", data, data_len);
	find_all("a[^x]{20}b", data, data_len);
	find_all("Tom|Sawyer|Huckleberry|Finn", data, data_len);
	find_all(".{0,3}(Tom|Sawyer|Huckleberry|Finn)", data, data_len);
	find_all("[a-zA-Z]+ing", data, data_len);
	find_all("^[a-zA-Z]{0,4}ing[^a-zA-Z]", data, data_len);
	find_all("[a-zA-Z]+ing$", data, data_len);
	find_all("^[a-zA-Z ]{5,}$", data, data_len);
	find_all("^.{16,20}$", data, data_len);
	find_all("([a-f](.[d-m].){0,2}[h-n]){2}", data, data_len);
	find_all("([A-Za-z]awyer|[A-Za-z]inn)[^a-zA-Z]", data, data_len);
	find_all("\"[^\"]{0,30}[?!\\.]\"", data, data_len);
	find_all("Tom.{10,25}river|river.{10,25}Tom", data, data_len);
	return 0;
}
