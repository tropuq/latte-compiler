#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void error() {
	puts("runtime error");
	exit(1);
}

void printInt(int a) {
	printf("%d\n", a);
}

void printString(const char *a) {
	printf("%s\n", a);
}

int readInt() {
	int a;
	if (scanf("%d", &a) != 1)
		return 0;
	return a;
}

char *readString() {
	char *s;
	scanf("%*[\n]");
	if (scanf("%m[^\n]", &s) != 1)
		return "";
	return s;
}

char *_add_strings(const char *a, const char *b) {
	size_t l1 = strlen(a);
	size_t l2 = strlen(b);
	char *s = malloc(l1 + l2 + 1);
	if (s == NULL)
		return "";
	memcpy(s, a, l1);
	memcpy(s + l1, b, l2 + 1);
	return s;
}

int _strings_equal(const char *a, const char *b) {
	return strcmp(a, b) == 0;
}

void* _alloc(unsigned long long size) {
    return malloc((size + 1) * 8);
}
