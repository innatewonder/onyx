#include "CommonPrecompiled.h"
#include "FilesystemReader.h"
#include "FolderHandle.h"

namespace Filesystem
{
FilesystemReader* ReaderInstance = nullptr;

class OpenFolderJob : public ThreadJob
{
public:
  OpenFolderJob(std::string folderName, bool recurse)
    : ThreadJob(true), 
    m_name(folderName), m_recurse(recurse), retValue(0),
    m_postFolderSet(nullptr) {}

  virtual void Execute()
  {
    m_postFolderSet = new FolderHandle(m_name, m_recurse);
  }

  virtual void PostExecute()
  {
    ReaderInstance->folders_[m_name] = m_postFolderSet;
    retValue = m_postFolderSet->GetFolderState();
  }

  s32 retValue;

private:
  FolderHandle* m_postFolderSet;

  std::string m_name;
  bool m_recurse;
};

void Streamer(Filesystem::FileHandle* f)
{
  //dbg
  //std::cout << "internal streamer " << f->GetFileName() << std::endl;
}

/*!*****************************
 *  @brief Constructor for Reader, creates global Filesystem::reader
 *
 *  @param readMode - Read either Filesystem package files or raw files in folders READ_PACKAGE / READ_RAW
*******************************/
FilesystemReader::FilesystemReader()
  : defaultStreamer_(Streamer)
{
  if(ReaderInstance == nullptr)
    ReaderInstance = this;
}

/*!*****************************
 *  @brief Destructor for Reader
*******************************/
FilesystemReader::~FilesystemReader()
{
  FileContIt it;
  while((it = files_.begin()) != files_.end())
  {
    delete it->second;
    files_.erase(it);
  }
  FolderIt fit;
  while((fit = folders_.begin()) != folders_.end())
  {
    delete fit->second;
    folders_.erase(fit);
  }
  ReaderInstance = nullptr;
}

/*!*****************************
 *  @brief Opens a file, either raw or Filesystem pacakge file
 *  @return returns error code or Filesystem_FILE_OK
 *
 *  @param filename - Name of the file to open
 *  @param thread - True if you want to thread file loading (use WaitForThreads to make sure everything is done)
*******************************/
s32 FilesystemReader::OpenFile(std::string filename, bool thread)
{
  return OpenFileMethod(filename);
}

/*!*****************************
 *  @brief Open a folder, either from a package file or actual folder
 *  @brief If it's a raw folder, opens all files contained within
 *
 *  @param folderName - the path of the folder
 *  @param recurse - true to recurse through subfolders, opening all files within
 *  @param thread - true if you want to thread the loading of files
*******************************/
s32 FilesystemReader::OpenFolder(std::string folderName, bool recurse, bool thread)
{
  OpenFolderJob* job = new OpenFolderJob(folderName, recurse);
  if(thread)
  {
    m_threadPool.AddJob(job);
    return Filesystem_FOLDER_OK;
  }
  //else

  job->Execute();
  job->PostExecute();
  s32 ret = job->retValue;
  delete job;

  return ret;
}

/*!*****************************
 *  @brief Wait for all threads to return from loading of files - will also call main thread functions
*******************************/
void FilesystemReader::WaitForThreads()
{
  while(m_threadPool.HasJobs())
  {
    m_threadPool.Update();
  }
}

/*!*****************************
*  @brief Calls main thread functions for file thread pool
*******************************/
void FilesystemReader::Update()
{
  m_threadPool.Update();
}


/*!*****************************
 *  @brief Close a file by its name, deletes any memory and removes file from list
 *
 *  @return false if file wasn't found
 *
 *  @param filename -  the name of the file to close
*******************************/
bool FilesystemReader::CloseFile(std::string filename)
{
  if(files_.find(filename) == files_.end())
    return false;

  delete files_.find(filename)->second;
  files_.erase(filename);
  return true;
}

/*!*****************************
 *  @brief Closes a folder and all its contents by foldername
 *  @return false if folder not found
 *
 *  @param foldername - the name of the folder to close
*******************************/
bool FilesystemReader::CloseFolder(std::string foldername)
{
  if(folders_.find(foldername) == folders_.end())
    return false;

  delete folders_.find(foldername)->second;
  folders_.erase(foldername);
  return true;
}

/*!*****************************
 *  @brief Get a FileHanlde by file name
 *  @return desired FileHandle*, if not found then nullptr
 *
 *  @param filename - the name of the file to get, including path
*******************************/
FileHandle* FilesystemReader::GetFile(std::string filename)
{
  if(files_.find(filename) != files_.end())
    return files_.find(filename)->second;
  return nullptr;
}

/*!*****************************
 *  @brief Get a FolderHanlde by folder name
 *  @return desired FolderHandle*, if not found then nullptr
 *
 *  @param foldername - the name of the folder to get, including path
*******************************/
FolderHandle* FilesystemReader::GetFolder(std::string foldername)
{
  if(folders_.find(foldername) != folders_.end())
    return folders_.find(foldername)->second;
  return nullptr;
}

/*!*****************************
 *  @brief Set the stream (loading) function for a file type
 *
 *  @param extension - the file extension for this function
 *  @param f - the function to run on this extension
*******************************/
void FilesystemReader::SetStreamFunction(std::string extension, StreamFunc f)
{
  if(streamers_.find(extension) != streamers_.end())
    streamers_.find(extension)->second = f;
  else
    streamers_.emplace(extension, f);
}

/*!*****************************
 *  @brief Sets the default loading function for all file types
 *
 *  @param f - The desired default function pointer
*******************************/
void FilesystemReader::SetDefaultStreamFunction(StreamFunc f)
{
  defaultStreamer_ = f;
}

///////////
//private
///////////
s32 FilesystemReader::OpenFileMethod(const std::string& filename)
{
  //load in the single specified file
  Filesystem::FileHandle* f = new Filesystem::FileHandle(filename);
    
  if(f->GetFileState() == Filesystem_IS_FOLDER)
  {
    return Filesystem_IS_FOLDER;
  }
  else
  {
    if(streamers_.find(f->GetFileExtension()) != streamers_.end())
      streamers_.find(f->GetFileExtension())->second(f);
    else
      defaultStreamer_(f);

    files_[filename] = f;
  }
  return f->GetFileState();
}

}//Filesystem
