#ifndef STRING_H
#define STRING_H

#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
	char *data;
	int len;
} string;

typedef string strView;

string newStr(const char *fmt, ...);
strView newStrView(const char *s);
string newStrVa(const char *fmt, va_list ap);
string newStrFromExisting(string s);

int getFmtSize(const char *fmt, ...);
int getFmtSizeVa(const char *fmt, va_list ap);

void strPushc(string *s, char c);
char strTopc(string *s);
char strPopc(string *s);
void strPushs(string *s, const char *fmt, ...);
void strPush(string *s, string src);

bool strIsEmpty(string s);
int strCmp(string s1, string s2);
int strnCmp(string s1, string s2, int n);
bool strIsEqual(string s1, string s2);
bool strnIsEqual(string s1, string s2, int n);

strView strTokStart(string s, const char *delim);
strView strTok(string s, strView previuosView, const char *delim);
void strForEachTok(string s, const char *delim, void (*action)(strView));
int strCountc(string haystack, char needle);
int strCounts(string haystack, string needle);
int strStr(string haystack, string needle, int startOffset);
void strReplace(string *s, string sub, string by, int maxOccurrences);
void strReplaceC(string *s, const char *sub, const char *by, int maxOccurrences);
void strReplaceAll(string *s, string sub, string by);
void strReplaceAllC(string *s, const char *sub, const char *by);
void strReverse(string *s);
void strToLower(string *s);
void strToUpper(string *s);
int strToNumeric(string s);
bool strIsNumeric(string s);
void strTrim(string *s);
int strDisplayedLength(const string s);

void strFree(string *s);
void strPrint(string s);
void strPrintln(string s);

#ifdef STRING_IMPL

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

string newStr(const char *fmt, ...)
{
	// this is a comment
	va_list ap;
	va_start(ap, fmt);
	string s = newStrVa(fmt, ap);
	va_end(ap);
	return s;
}

string newStrVa(const char *fmt, va_list ap)
{
	string s;
	va_list ap1;
	va_copy(ap1, ap);

	s.len = getFmtSizeVa(fmt, ap1);
	va_end(ap1);

	s.data = malloc(sizeof(char) * s.len + 1);
	va_copy(ap1, ap);
	vsnprintf(s.data, s.len + 1, fmt, ap1);
	va_end(ap1);
    return s;
}

string newStrFromExisting(string s)
{
	return newStr("%s", s.data);
}

strView newStrView(const char *s)
{
	strView view;
	view.data = (char *)s;
	view.len = strlen(s);
	return view;
}

strView newStrViewRange(string s, int start, int end)
{
	strView view;
	view.data = s.data + start;
	view.len = end - start;
	return view;
}

int getFmtSize(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	int size = vsnprintf(NULL, 0, fmt, ap);
	va_end(ap);
	return size;
}

int getFmtSizeVa(const char *fmt, va_list ap)
{
	return vsnprintf(NULL, 0, fmt, ap);
}

void strPushc(string *s, char c)
{
	s->data = realloc(s->data, sizeof(char) * (++s->len + 1));
	s->data[s->len - 1] = c;
	s->data[s->len] = '\0';
}

char strTopc(string *s)
{
	return s->data[s->len - 1];
}

char strPopc(string *s)
{
	char c = strTopc(s);
	s->data = realloc(s->data, sizeof(char) * (--s->len + 1));
	s->data[s->len - 1] = '\0';
	return c;
}

void strPushs(string *s, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	string tmp = newStrVa(fmt, ap);
	va_end(ap);

	for (int i = 0; i < tmp.len; i++)
		strPushc(s, tmp.data[i]);

	strFree(&tmp);
}

void strPush(string *s, string src)
{
	strPushs(s, "%s", src.data);
}

bool strIsEmpty(string s)
{
	return s.len == 0;
}

int strCmp(string s1, string s2)
{
	return strncmp(s1.data, s2.data, s1.len);
}

int strnCmp(string s1, string s2, int n)
{
	if (s1.len < n)
		return -1;

	if (s2.len < n)
		return 1;

	return strncmp(s1.data, s2.data, n);
}

bool strIsEqual(string s1, string s2)
{
	if (s1.len != s2.len)
		return false;
	return strCmp(s1, s2) == 0;
}

bool strIsEqualC(string s1, const char *s2)
{
	return strCmp(s1, newStrView(s2)) == 0;
}

bool strnIsEqual(string s1, string s2, int n)
{
	return strnCmp(s1, s2, n) == 0;
}

bool isStrViewOverflow(string s, strView view)
{
	return view.data + view.len > s.data + s.len;
}

strView strTokStart(string s, const char *delim)
{
	strView view;
	view.data = s.data;
	view.len = 0;
	return strTok(s, view, delim);
}

strView strTok(string s, strView previuosView, const char *delim)
{
	strView view;
	view.data = previuosView.data + previuosView.len;
	view.len = 0;

	while (!isStrViewOverflow(s, view) &&
			strchr(delim, view.data[view.len]))
	{
		view.data++;
	}

	while (!isStrViewOverflow(s, view) &&
			!strchr(delim, view.data[view.len]))
	{
		view.len++;
	}

	return view;
}

void strForEachTok(string s, const char *delim, void (*action)(strView))
{
	strView view = strTokStart(s, delim);
	while (!strIsEmpty(view)) {
		action(view);
		view = strTok(s, view, delim);
	}
}

void strForEach(string s, void (*action)(char))
{
    for (int i = 0; i < s.len; i++)
        action(s.data[i]);
}

int strCountc(string haystack, char needle)
{
	int count = 0;
	for (int i = 0; i < haystack.len; ++i)
		if (haystack.data[i] == needle)
			count++;
	return count;
}

int strCounts(string haystack, string needle)
{
	int count = 0;
	int i = -1;

	while ((i = strStr(haystack, needle, i + 1)) != -1)
		count++;

	return count;
}

int strStr(string haystack, string needle, int startOffset)
{
	for (int i = startOffset; i < haystack.len - needle.len + 1; ++i) {

		strView view;
		view.data = haystack.data + i;
		view.len = needle.len;

		if (strIsEqual(view, needle))
			return i;
	}

	return -1;
}

void strReplaceC(string *s, const char *sub, const char *by, int maxOccurrences)
{
	strReplace(s, newStrView(sub), newStrView(by), maxOccurrences);
}

void strReplace(string *s, string sub, string by, int maxOccurrences)
{
	string result = newStr("");
	int i = 0;

	while (i < s->len - sub.len + 1 && maxOccurrences > 0) {
		strView tmp = newStrViewRange(*s, i, i + sub.len);
		if (strIsEqual(tmp, sub)) {
			strPush(&result, by);
			i += sub.len;
			maxOccurrences--;
		} else {
			strPushc(&result, s->data[i++]);
		}
	}

	// push the rest of the string
	strPushs(&result, "%.*s", s->len - i, s->data + i);

	strFree(&*s);
	*s = result;
}

void strReplaceAllC(string *s, const char *sub, const char *by)
{
	strReplaceC(s, sub, by, s->len);
}

void strReplaceAll(string *s, string sub, string by)
{
	strReplace(s, sub, by, s->len);
}

void strReverse(string *s)
{
	string tmp = newStr("");

	for (int i = s->len - 1; i >= 0; --i)
		strPushc(&tmp, s->data[i]);

	strFree(&*s);
	*s = tmp;
}

void strToLower(string *s)
{
	for (int i = 0; i < s->len; ++i)
		s->data[i] = tolower(s->data[i]);
}

void strToUpper(string *s)
{
	for (int i = 0; i < s->len; ++i)
		s->data[i] = toupper(s->data[i]);
}

int strToNumeric(string s)
{
	return atoi(s.data);
}

bool strIsNumeric(string s)
{
	for (int i = 0; i < s.len; ++i)
		if (!isdigit(s.data[i]))
			return false;
	return true;
}

void strTrim(string *s)
{
	int start = 0;
	int end = s->len - 1;

	while (start < s->len && isspace(s->data[start]))
		start++;

	while (end > 0 && isspace(s->data[end]))
		end--;

	strView view = newStrViewRange(*s, start, end + 1);
	string tmp = newStrFromExisting(view);
	strFree(&*s);
	*s = tmp;
}

void strFree(string *s)
{
    if (s->len > 0) {
	    free(s->data);
        s->len = 0;
    }
}

void strPrint(string s)
{
    printf("%.*s", s.len, s.data);
}

void strPrintln(string s)
{
    printf("%.*s\n", s.len, s.data);
}

#endif

#endif
