/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "utils/FileUtils.h"
#include "dirent.h"
#include <iterator>


#if defined(_MSC_VER) || defined(__MINGW64_VERSION_MAJOR)
  #include <windows.h>
  #define MAXPATH MAX_PATH
  #define GETCWD _getcwd
  #define MKDIR(x) mkdir(x)
#else
  #include <unistd.h>
  #if defined(__APPLE__)
    #if defined(__clang__)
      #include <sys/syslimits.h>
    #endif
  #endif
  #if defined __GNUC__
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

  // Recursively create parents
  auto const sep = path.find_last_of("\\/");
  if (sep != std::string::npos && sep > 0)
  {
    if (!CreateFilePath(path.substr(0, sep)))
    {
      return false;
    }
  }

  MKDIR(path.c_str());
  return true;
}

bool WriteFile(const std::string& content, const std::string& filename)
{
  if (!CreateFilePath(filename))
    return false;
  std::ofstream ascii_ostream(filename, std::ios::out | std::ios::trunc);
  ascii_ostream << content;
  ascii_ostream.flush();
  ascii_ostream.close();
  return true;
}

bool ReadFile(const std::string& filename, std::string& content)
{
  if (!FileExists(filename))
    return false;

  std::ifstream input(filename);
  if (!input.is_open())
    return false;
  std::string buff((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
  input.close();
  content = buff;
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

bool FileExists(const std::string& filename)
{
  struct stat buffer;
  return (stat(filename.c_str(), &buffer) == 0);
}

bool IsJSONFile(const std::string& filename)
{
  size_t ext = filename.find_last_of(".");
  return filename.substr(ext) == ".json";
}
