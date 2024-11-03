#ifndef PATH_H
#define PATH_H
#include "shared.h"
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>

typedef enum { Read = 0, Write = 1, } PipeMode;

const char *getPathExtention(const char *path);
const char *getHomePath();
void expandPath(char *path, const int maxLen);
void compressPath(char *path);
bool isExtentionEqual(const char *path, const char *extention);

int dirTraverse(const char *dir, bool (*action)(const char *));
int traverseFile(const char *fileName, const int bufSize, bool (*action)(char[bufSize]));

bool isDir(const char *path);
bool isRegularFile(const char *fileName);
bool isPathExists(const char *path);

int echoFileWrite(const char *fileName, const char *fmt, ...);
int echoFileAppend(const char *fileName, const char *fmt, ...);
int readFile(const char *fileName, const char *fmt, ...);
int redirectFd(int srcFd, const char *destFileName);
int popen2(char *path, char *argv[], FILE *ppipe[2]);

void getFullFileName(const char *dirName, const char *fileName, char *dest, int destLen);

int nextInDir(DIR *dir, const char *dirName, char *destFileName, const int destLen);

int getFmtSize(const char *fmt, ...);
int getFmtSizeVa(const char *fmt, va_list ap);
size_t getFileSize(FILE *fp);

#endif
