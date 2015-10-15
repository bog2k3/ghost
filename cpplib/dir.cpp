/*
 * dir.cpp
 *
 *  Created on: Oct 15, 2015
 *      Author: bog
 */

#include "../cpplib/log.h"

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#include <iostream>
#include <string>
#include <vector>
#include <functional>

bool isDir(std::string const& dir) {
    struct stat fileInfo;
    stat(dir.c_str(), &fileInfo);
    return S_ISDIR(fileInfo.st_mode);
}

bool pathExists(std::string const& path) {
	struct stat fileInfo;
	return stat(path.c_str(), &fileInfo) == 0;
}

void mkDir(std::string const& path) {
	mkdir(path.c_str(), S_IRWXU);
}

std::string getFileName(std::string const& path) {
	if (path.find('/') != path.npos)
		return path.c_str() + path.find_last_of('/') + 1;
	else
		return path;
}

std::string stripExt(std::string const& path) {
	return path.substr(0, path.find_last_of('.'));
}

unsigned long getFileTimestamp(std::string const& path) {
	struct stat fileInfo;
	stat(path.c_str(), &fileInfo);
	return (unsigned long)fileInfo.st_mtim.tv_sec;
}

std::vector<std::string> getFiles(std::string const& baseDir) {
	std::vector<std::string> files;
    DIR *dp;
    struct dirent *dirp;
    if ((dp = opendir(baseDir.c_str())) == NULL) {
    	ERROR(errno << ": Couldn't open " << baseDir << "\n");
    } else {
        while ((dirp = readdir(dp)) != NULL) {
            if (dirp->d_name != std::string(".") && dirp->d_name != std::string(".."))
				files.push_back(baseDir + dirp->d_name);
        }
        closedir(dp);
    }
    return files;
}

void applyRecursive(std::string const& baseDir, std::function<void(std::string const& filename)> func) {
	std::vector<std::string> files = getFiles(baseDir);
	for (auto f : files) {
		if (isDir(f))
			applyRecursive(f, func);
		else
			func(f);
	}
}
