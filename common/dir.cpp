/*
 * dir.cpp
 *
 *  Created on: Oct 15, 2015
 *      Author: bog
 */

#include "log.h"
#include "strManip.h"

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/sendfile.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

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

bool mkDir(std::string const& path) {
	LOGPREFIX("mkDir");
	if (mkdir(path.c_str(), S_IRWXU) < -1) {
		ERROR(errno << ": Nu am putut crea directorul \"" << path << "\"");
		return false;
	}
	return true;
}

bool mkDirRecursive(std::string const& path) {
	std::vector<std::string> dirs = strSplit(path, '/');
	std::string builtPath = "";
	for (auto d : dirs) {
		builtPath += "/" + d;
		if (!pathExists(builtPath))
			if (!mkDir(builtPath))
				return false;
	}
	return true;
}

std::string getFileName(std::string const& path) {
	if (path.find('/') != path.npos)
		return path.c_str() + path.find_last_of('/') + 1;
	else
		return path;
}

std::string stripExt(std::string const& path) {
	auto ppos = path.find_last_of('.');
	if (ppos != path.npos)
		return path.substr(0, ppos);
	else
		return path;
}

std::string getFileExt(std::string const& path) {
	auto ppos = path.find_last_of('.');
	if (ppos != path.npos)
		return path.substr(ppos);
	else
		return "";
}

unsigned long getFileTimestamp(std::string const& path) {
	LOGPREFIX("getFileTimestamp");
	struct stat fileInfo;
	if (stat(path.c_str(), &fileInfo) < 0) {
		ERROR("Nu am putut stat() fisierul \"" << path << "\"");
		return 0;
	}
	return (unsigned long)fileInfo.st_mtim.tv_sec;
}

bool copyFile(std::string const& source, std::string const& dest) {
	LOGPREFIX("copyFile");
	bool ret = true;
	int fs = open(source.c_str(), O_RDONLY, 0);
	if (fs < 0) {
		ERROR(errno << ": Nu a putut deschide fisierul sursa \"" << source <<"\"");
		ret = false;
	}
	int fd = open(dest.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd < 0) {
		ERROR(errno << ": Nu a putut deschide fisierul destinatie \"" << dest <<"\"");
		ret = false;
	}

	if (ret) {
		struct stat stat_source;
		if (fstat(fs, &stat_source) < 0) {
			ERROR(errno << ": Nu am putut stat() fisierul sursa \"" << source << "\"");
			ret = false;
		}
		if (ret && sendfile(fd, fs, 0, stat_source.st_size) < 0) {
			ERROR(errno << ": Nu am putut copia fisierul \n" <<
					"\"" << source <<"\" -> \"" << dest << "\"");
			ret = false;
		}
	}

	close(fs);
	close(fd);

	return ret;
}

bool deleteFile(std::string const& path) {
	LOGPREFIX("deleteFile");
	if (unlink(path.c_str()) < 0) {
		ERROR(errno << ": Nu am putut sterge fișierul \"" << path << "\"");
		return false;
	}
	return true;
}

std::vector<std::string> getFiles(std::string const& baseDir) {
	LOGPREFIX("getFiles");
	std::vector<std::string> files;
    DIR *dp;
    struct dirent *dirp;
    if ((dp = opendir(baseDir.c_str())) == NULL) {
    	ERROR(errno << ": Nu am putut deschide directorul \"" << baseDir << "\"\n");
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
