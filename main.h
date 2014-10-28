
// Common result print
void printResult(char* name, int time, int found);

// 0 - interpreter
// 1 - dfa
// 2 - jit
void pcre_find_all(char* pattern, char* subject, int subject_len, int repeat, int mode);
void tre_find_all(char* pattern, char* subject, int subject_len, int repeat);
void re2_find_all(char* pattern, char* subject, int subject_len, int repeat);
void onig_find_all(char* pattern, char* subject, int subject_len, int repeat);


