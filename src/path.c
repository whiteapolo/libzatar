#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "path.h"

const char *getPathExtention(const char *path)
{
	const char *lastDot = strrchr(path, '.');
	if (lastDot == NULL)
		return path;
	return lastDot + 1;
}

const char *getHomePath()
{
	const char *home = getenv("HOME");
	if (home == NULL)
		return ".";
	return home;
}

void expandPath(char *path, const int maxLen)
{
	if (path[0] == '~') {
		char buf[maxLen];
		snprintf(buf, maxLen, "%s%s", getHomePath(), path + 1);
		strncpy(path, buf, maxLen);
	}
}

void compressPath(char *path)
{
	const char *home = getHomePath();
	const int homeLen = strlen(home);
	if (strncmp(home, path, homeLen) == 0) {
		const int bufLen = strlen(path) + homeLen;
		char buf[bufLen];
		snprintf(buf, bufLen, "~%s", path + homeLen);
		strncpy(path, buf, bufLen);
	}
}

Result nextInDir(DIR *dir, const char *dirName, char *destFileName, const int destLen)
{
	struct dirent *de;
	de = readdir(dir);
	if (de == NULL)
		return Err;
	snprintf(destFileName, destLen, "%s/%s", dirName, de->d_name);
	return Ok;
}

Result dirTraverse(const char *dir, bool (*action)(const char *))
{
	struct dirent *de;
	DIR *dr = opendir(dir);

	if (dr == NULL)
		return Err;

	while ((de = readdir(dr)) != NULL) {
		const char *file = de->d_name;
		char fullPath[PATH_MAX];
		const int len = snprintf(fullPath, PATH_MAX, "%s/%s", dir, file);
		fullPath[len] = '\0';
		if (action(fullPath) == false)
			break;
	}

	closedir(dr);
	return Ok;
}

bool isExtentionEqual(const char *path, const char *extention)
{
	return strcmp(getPathExtention(path), extention) == 0;
}

bool isDir(const char *path)
{
	struct stat sb;
	stat(path, &sb);
	return S_ISDIR(sb.st_mode);
}

bool isRegularFile(const char *path)
{
	struct stat sb;
	stat(path, &sb);
	return S_ISREG(sb.st_mode);
}

bool isPathExists(const char *path)
{
	return !access(path, F_OK);
}

Result echoFileWrite(const char *fileName, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	FILE *fp = fopen(fileName, "w");

	if (fp == NULL)
		return Err;

	vfprintf(fp, fmt, ap);
	va_end(ap);
	return Ok;
}

Result echoFileAppend(const char *fileName, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	FILE *fp = fopen(fileName, "a");

	if (fp == NULL)
		return Err;

	vfprintf(fp, fmt, ap);
	va_end(ap);
	return Ok;
}

Result readFile(const char *fileName, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	FILE *fp = fopen(fileName, "r");

	if (fp == NULL)
		return Err;

	if (vfscanf(fp, fmt, ap) == EOF) {
        fclose(fp);
		return Err;
    }

	va_end(ap);
    fclose(fp);
	return Ok;
}

Result redirectFd(int srcFd, const char *destFileName)
{
	int destFd = open(destFileName, O_WRONLY);
	if (destFd == -1)
		return Err;

	if (dup2(destFd, srcFd) == -1) {
		close(destFd);
		return Err;
	}

	close(destFd);
	return Ok;
}

Result traverseFile(const char *fileName, const int bufSize, bool (*action)(char[bufSize]))
{
	FILE *fp = fopen(fileName, "r");
	if (fp == NULL)
		return Err;

	char buf[bufSize];
	while (fgets(buf, bufSize, fp) && action(buf));

	return Ok;
}

void getFullFileName(const char *dirName, const char *fileName, char *dest, int destLen)
{
	snprintf(dest, destLen, "%s/%s", dirName, fileName);
}

Result popen2(char *path, char *argv[], FILE *ppipe[2])
{
	int output[2];
	int input[2];

	if (pipe(output) == -1 || pipe(input) == -1)
		return Err;

	int pid = fork();

	if (pid == -1)
		return Err;

	if (pid) {
		// parent
		close(output[Write]);
		ppipe[Write] = fdopen(input[Write], "w");
		ppipe[Read] = fdopen(output[Read], "r");
	} else {
		// child
		dup2(input[Read], STDIN_FILENO);
		dup2(output[Write], STDOUT_FILENO);
		close(input[Write]);
		close(input[Read]);
		close(output[Write]);
		close(output[Read]);
		execvp(path, argv);
		exit(EXIT_FAILURE);
	}

	return Ok;
}
