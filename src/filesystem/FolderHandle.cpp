#include "CommonPrecompiled.h"
#include "FilesystemDefines.h"
#include "FilesystemReader.h"

#include "FileHandle.h"
#include "FolderHandle.h"

#if (_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#include <dirent.h>
#endif

namespace Filesystem
{
FolderHandle::FolderHandle(std::string folder, bool recurse) : folderName_(folder),
  totalSize_(0), folderState_(Filesystem_FILE_OK)
{
  CheckFolder(recurse);
}

FolderHandle::~FolderHandle()
{
  for(std::string f : files_)
  {
    if(!ReaderInstance->CloseFile(f))
      ReaderInstance->CloseFolder(f);
  }
}

s32 FolderHandle::CheckFolder(bool recurse)
{
  totalSize_ = 0;
  FileHandle* f = nullptr;

  std::string folder = folderName_;
  folder += "/";

#if (_WIN32)
  WIN32_FIND_DATAA FindFileData;
  HANDLE hFind;

  hFind = FindFirstFileA((folder + "*").c_str(), &FindFileData);
  if (hFind == INVALID_HANDLE_VALUE)
  {
    folderState_ = Filesystem_FOLDER_EMPTY;

    FindClose(hFind);
    return folderState_;
  }
  FindNextFileA(hFind, &FindFileData); //get rid of other hidden folder ".."

  while(FindNextFileA(hFind, &FindFileData))
  {
    std::string currentFileName = folder + FindFileData.cFileName;
    u32 currentFileSize = FindFileData.nFileSizeLow;

#else //linux loop

  struct dirent *directoryInfo;
  DIR* directory;

  directory = opendir(folder.c_str());
  if(directory == NULL)
  {
    return Filesystem_FILE_ERROR;
  }

  while((directoryInfo = readdir(directory)) != NULL)
  {
    std::string currentFileName = folder + directoryInfo->d_name;
    u32 currentFileSize = directoryInfo->d_off;
    if(currentFileName.find_last_of('.') == currentFileName.size() - 1)
      continue;

#endif //win32 / linux

    folderState_ = ReaderInstance->OpenFile(currentFileName);
    if(folderState_ == Filesystem_IS_FOLDER && recurse)
    {
      folderState_ = ReaderInstance->OpenFolder(currentFileName, recurse);
      auto files = ReaderInstance->GetFolder(currentFileName)->GetFileNames();
      for(auto it : files)
      {
        AddFileName(it, 0);
      }
      continue;
    }

    if(folderState_ != Filesystem_FILE_ERROR &&
       folderState_ != Filesystem_IS_FOLDER)
    {
      //make sure we dont already contain the file
      bool found = false;
      for(std::string f : files_)
      {
        if(f == currentFileName)
        {
          found = 1;
          break;
        }
      }
      if(!found)
      {
        AddFileName(currentFileName, currentFileSize);
      }
    }

    if(folderState_ == Filesystem_FILE_ERROR)
    {
      break;
    }
  }//~while more files

#ifdef _WIN32
  FindClose(hFind);
#else
  closedir (directory);
#endif //windows/linux close

  return folderState_;
}//~CheckFolder

void FolderHandle::AddFileName(std::string file, u32 size)
{
  files_.push_back(file);
  totalSize_ += size;
}

s32 FolderHandle::GetFolderState() const
{
  return folderState_;
}

u32 FolderHandle::GetTotalSize()
{
  if(totalSize_ != 0)
    return totalSize_;
  else
  {
    for(std::string name : files_)
    {
      totalSize_ += ReaderInstance->GetFile(name)->GetFileSize();
    }
  }
  return totalSize_;
}

u32 FolderHandle::GetNumFiles() const
{
  return files_.size();
}

const String FolderHandle::GetFolderName() const
{
  return folderName_;
}

StringVec& FolderHandle::GetFileNames()
{
  return files_;
}

FileHandle* FolderHandle::GetFile(const String& name) const
{
  return ReaderInstance->GetFile(name);
}

}//~Filesystem
