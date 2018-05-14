/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "utils/FileUtils.h"
#include "dirent.h"


#if defined(_MSC_VER) || defined(__MINGW64_VERSION_MAJOR)
  #include <windows.h>
  #define MAXPATH MAX_PATH
  #define GETCWD _getcwd
  #define MKDIR(x) mkdir(x)
#else
  #include <unistd.h>
    #if defined(__APPLE__) && defined(__clang__)
    #include <sys/syslimits.h>
  #elif defined __GNUC__
    #include <sys/types.h>
    #include <sys/stat.h>
  #endif

  #define MAXPATH PATH_MAX
  #define GETCWD getcwd
  #define MKDIR(x) mkdir(x, 0755)

  #if defined(__gnu_linux__)
    #include <cstring>
  #endif
#endif

std::string Replace(const std::string& original, const std::string& replace, const std::string& withThis)
{
  size_t idx = 0;
  std::string s = original;
  idx = s.find(replace);
  if (idx != std::string::npos)
  {
    s.erase(idx, replace.length());
    s.insert(idx, withThis);
  }
  return s;
}

bool CreateFilePath(const std::string& path)
{
  if (path.empty())
  {
    return false;
  }

  std::string buffer = path;
  std::vector<std::string> folderLevels;
  char* c_str = (char*)buffer.c_str();

  // Point to end of the string
  char* strPtr = &c_str[strlen(c_str) - 1];

  // Break out each directory into our vector
  do
  {
    do
    {
      strPtr--;
    } while ((*strPtr != '\\') && (*strPtr != '/') && (strPtr >= c_str));
    folderLevels.push_back(std::string(strPtr + 1));
    strPtr[1] = 0;
  } while (strPtr >= c_str);

  std::string destDir = "";

  std::string dir;

  // Create the folders iteratively, backwards
  for (size_t i = folderLevels.size() - 1; i >= 1; i--)
  {
    dir = folderLevels.at(i);
    if (dir == "/" || dir == "\\")
      continue;
    destDir += dir;
    MKDIR(destDir.c_str());
  }
  return true;
}

void ListFiles(const std::string& dir, std::vector<std::string>& files, const std::string& mask)
{
  DIR *d;
  struct dirent *ent;
  std::string filename;
  if ((d = opendir(dir.c_str())) != nullptr)
  {
    while ((ent = readdir(d)) != nullptr)
    {
      size_t nameLength = strlen(ent->d_name);

      if (ent->d_name[0] == '.' &&
        ((nameLength == 1) || (nameLength == 2 && ent->d_name[1] == '.')))
        continue;
      filename = dir;
      filename += "/";
      filename += ent->d_name;

      if (!IsDirectory(ent))
      {
        if (filename.find(mask) != std::string::npos)
          files.push_back(filename);
      }
      else
      {
        ListFiles(filename, files, mask);
      }
    }
  }
}

void MakeDirectory(std::string const& dir)
{
  MKDIR(dir.c_str());
}

void DeleteDirectory(const std::string &dir, bool bDeleteSubdirectories)
{
  DIR *d;
  struct dirent *ent;
  std::string filename;
  if ((d = opendir(dir.c_str())) != nullptr)
  {
    while ((ent = readdir(d)) != nullptr)
    {
      size_t nameLength = strlen(ent->d_name);

      if (ent->d_name[0] == '.' &&
        ((nameLength == 1) || (nameLength == 2 && ent->d_name[1] == '.')))
        continue;
      filename = dir;
      filename += "/";
      filename += ent->d_name;

      if (!IsDirectory(ent))
      {
        std::remove(filename.c_str());
      }
      else
      {
        DeleteDirectory(filename, bDeleteSubdirectories);
      }
    }
  }
  rmdir(dir.c_str());
}

std::string GetCurrentWorkingDirectory()
{
  char path[MAXPATH];
  GETCWD(path, MAXPATH);
  return std::string(path);
}

bool IsDirectory(struct dirent* ent)
{
  return ent->d_type == DT_DIR;
}
