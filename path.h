#ifndef PATH_H
#define PATH_H

#include "string.h"

strView getPathTail(const char *path);

strView getEnvView(const char *name);

strView getHomePath();

void expandPath(string *path);

void compressPath(string *path);

int getFileSize(FILE *fp);

string readWholeFile(const char *fileName);

string readWholeFileB(const char *fileName, int maxBytes);

bool scanfFileByName(const char *fileName, const char *fmt, ...);

#ifdef PATH_IMPL

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

strView getPathTail(const char *path)
{
	(void)path;
	return EMPTY_STR;
	// strView s;
	// int pathLen = strlen(path);
	// int i = pathLen - 1;

	// if (path[i] == '/') {
	// 	s.len = 0;
	// 	return s;
	// }

	// while (i > 0 && path[i - 1] != '/')
	// 	i--;

	// return path + i;
}

strView getEnvView(const char *name)
{
	const char *env = getenv(name);
	if (env == NULL)
		return EMPTY_STR;
	return newStrView(env);
}

strView getHomePath()
{
	strView s = getEnvView("HOME");
	if (strIsEmpty(s))
		return newStrView(".");
	return s;
}

void expandPath(string *path)
{
	if (path->data[0] == '~')
		strReplace(path, newStrView("~"), getHomePath(), 1);
}

void compressPath(string *path)
{
	strView home = getHomePath();
	if (strnIsEqual(*path, home, home.len))
		strReplace(path, home, newStrView("~"), 1);
}

int getFileSize(FILE *fp)
{
	int curr = ftell(fp);
	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	fseek(fp, curr, SEEK_SET);
	return size;
}

string readWholeFile(const char *fileName)
{
	return readWholeFileB(fileName, -1);	
}

string readWholeFileB(const char *fileName, int maxBytes)
{
	string expandedPath = newStr(fileName);
	expandPath(&expandedPath);

	FILE *fp = fopen(expandedPath.data, "r");
	strFree(expandedPath);

	string s = newStr("");

	if (fp == NULL)
		return s;

	char c;

	while ((c = fgetc(fp)) != EOF && s.len != maxBytes)
		strPushc(&s, c);

	fclose(fp);
	return s;
}

bool scanfFileByName(const char *fileName, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	FILE *fp = fopen(fileName, "r");

	if (fp == NULL)
		return false;

	if (vfscanf(fp, fmt, ap) == EOF)
		return false;

	va_end(ap);

	return true;
}

#endif

#endif
