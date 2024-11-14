#ifndef STR_H
#define STR_H
#include "shared.h"
#include <ctype.h>

typedef struct {
	char *data;
	size_t len;
	size_t capacity;
} string;

typedef string strSlice;

typedef struct {
	FILE *fp;
	string line;
} Scanner;

#define MIN_STR_CAPACITY (16)
#define EMPTY_STR ((strSlice){0})

string newStr(const char *fmt, ...);
string newStrWithCapacity(const size_t initialCapacity, const char *fmt, ...);
string newStrVa(const size_t minSize, const char *fmt, va_list ap);
string strClone(const strSlice s);
string newStrFromArray(char *s[], const size_t len, const char *delim);
string newStrSlice(const strSlice s, ssize_t start, ssize_t end, const ssize_t step);

void strShrinkToFit(string *s);
strSlice sliceStr(const char *s, const size_t len);
strSlice sliceStrRange(const strSlice s, const size_t start, const size_t end);
strSlice sliceStrC(const char *s);

void strPushc(string *s, const char c);
char strTopc(const strSlice s);
char strPopc(string *s);
void strPushs(string *s, const char *fmt, ...);
void strPush(string *s, const strSlice src);
void strPushcAt(string *s, const size_t n, const char c);
void strPushsAt(string *s, const size_t n, const char *fmt, ...);

bool strIsEmpty(const strSlice s);
int strCmp(const strSlice s1, const strSlice s2);
int strCmpPtr(const strSlice *s1, const strSlice *s2);
int strnCmp(const strSlice s1, const strSlice s2, size_t n);
int strnCmpC(const strSlice s1, const char *s2, const size_t n);
int strCmpC(const strSlice s1, const char *s2);
bool strIsEqual(const strSlice s1, const strSlice s2);
bool strIsEqualPtr(const strSlice *s1, const strSlice *s2);
bool strIsEqualC(const strSlice s1, const char *s2);
bool strnIsEqual(const strSlice s1, const strSlice s2, const size_t n);
bool strnIsEqualC(const strSlice s1, const char *s2, const size_t n);

strSlice strTokStart(const strSlice s, const char *delim);
strSlice strTok(const strSlice s, const strSlice prevSlice, const char *delim);
void strForEachTok(const strSlice s, const char *delim, void (*action)(const strSlice));

size_t strCountc(const strSlice haystack, const char needle);
size_t strCounts(const strSlice haystack, const strSlice needle);
size_t strCountsC(const strSlice haystack, const char *needle);

ssize_t strChr(const strSlice haystack, const char needle, const size_t startOffset, size_t occurance);
ssize_t strrChr(const strSlice haystack, const char needle, const size_t startOffset, size_t occurance);
ssize_t strStr(const strSlice haystack, const strSlice needle, const size_t startOffset);
ssize_t strStrC(const strSlice haystack, const char *needle, const size_t startOffset);

strSlice strExtractBetween(const strSlice s, const strSlice left, const strSlice right);
strSlice strExtractBetweenC(const strSlice s, const char *left, const char *right);

void strReplace(string *s, const char *sub, const char *by, size_t maxOccurrences);
void strReplaceAll(string *s, const char *sub, const char *by);

void strReverse(string *s);
void strToLower(string *s);
void strToUpper(string *s);
void strTrimCset(string *s, const char *cset);
void strTrim(string *s);

int charToInt(const char c);
long long strToNumeric(const strSlice s);
bool strIsNumeric(const strSlice s);
int strScanf(const strSlice s, const char *fmt, ...);

void strClear(string *s);
void strFree(string *s);
void strPrint(const strSlice s);
void strPrintln(const strSlice s);
void strDebugPrint(const strSlice s);

string strGetLine(FILE *fp);
string strInput(const char *prompt);
string readWholeFile(const char *fileName);

Scanner newScanner(FILE *fp);
const string *scannerNextLine(Scanner *scanner);
void scannerFree(Scanner *scanner);

size_t strDisplayedLength(const strSlice s);

int getFmtSize(const char *fmt, ...);
int getFmtSizeVa(const char *fmt, va_list ap);
void swap(void *a, void *b, const size_t size);
size_t getFileSize(FILE *fp);

int charCmp(const char a, const char b);
int charCaseCmp(const char a, const char b);
unsigned int strCalculateEditDistanceC(const char *s1, const char *s2, int (*cmpChar)(const char a, const char b));
unsigned int strCalculateEditDistance(const strSlice s1, const strSlice s2, int (*cmpChar)(const char a, const char b));

#endif
