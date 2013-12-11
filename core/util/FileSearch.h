/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file FileSearch.h
* @brief 
* @date 1/2/2011
*/

#pragma once

#include <iostream>
#include "dirent.h"
#include <vector>
#include <string>

namespace Etoile
{
	struct FileInfo
	{
		std::string _filename;
		std::string _path;
		std::string _dir;
	};

	class FileSearch
	{
	public:
		std::vector<FileInfo> search(std::string curr_directory, std::string extension)
		{
			std::vector<FileInfo> filenames;
			DIR* dir_point = opendir(curr_directory.c_str());
			dirent* entry = readdir(dir_point);
			while (entry)
			{         // if !entry then end of directory
				if (entry->d_type == DT_DIR)
				{        // if entry is a directory
					std::string fname = entry->d_name;
					if (fname.compare(".") != 0 && fname .compare("..") != 0)
					{
						std::string dir = curr_directory+ ( "/" )+( fname);
						std::vector<FileInfo> f_recu = search(dir, extension);        // search through it
						filenames.insert(filenames.end(), f_recu.begin(), f_recu.end());
					}
				}
				else if (entry->d_type == DT_REG)
				{                // if entry is a regular file
					std::string fname = entry->d_name;        // filename
					// if filename's last characters are extension
					if (fname.find(extension, (fname.length() - extension.length())) != std::string::npos)
					{
						FileInfo info;
						info._filename = fname;
						info._dir = curr_directory;
						info._path = curr_directory+("/")+(fname);
						filenames.push_back(info);                // add filename to results vector
					}
				}
				entry = readdir(dir_point);
			}
			return filenames;
		}
	};
}