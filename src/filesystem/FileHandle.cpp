#include "CommonPrecompiled.h"
#include "FileHandle.h"

#ifdef _WIN32
#pragma warning( disable: 4996 ) //fopen/read warning
#endif

namespace Filesystem
{

  /*!*****************************
   *  @brief Constructor for FileHandle using external file
   *
   *  @param filename - Name of the file to open
  *******************************/
  FileHandle::FileHandle(std::string filename) 
    : fileName_(filename), timeModified_(0), 
    fileState_(Filesystem_FILE_ERROR), file_(nullptr)
  {
    s32 pos = fileName_.find_last_of('.');
    if((pos == std::string::npos)) // || (pos < fileName_.size() - 4))
    {
      fileState_ = Filesystem_IS_FOLDER;
      return;
    }
    else
    {
      ++pos;
      extension_ = fileName_.substr(pos, std::string::npos);
    }

    CheckFile();
  }

  /*!*****************************
   *  @brief Destructor
  *******************************/
  FileHandle::~FileHandle()
  {
    handle_.close();
  }

  /*!*****************************
   *  @brief Opens internal file handle
   *
   *  @return true if the file is now open
  *******************************/
  bool FileHandle::Open()
  {
    if(handle_.is_open())
    {
      handle_.seekg(0);
      if(!handle_.good())
      {
        handle_.close();
      }
      else
      {
        return true;
      }
    }
    handle_.open(fileName_, std::istream::binary | std::istream::in);
    return handle_.is_open();
  }

  /*!*****************************
   *  @brief Closes internal file handle
  *******************************/
  void FileHandle::Close()
  {
    handle_.close();
  }

  /*!*****************************
   *  @brief Gets a file struct, USER IS RESPONSABLE FOR CLOSING
   *  
   *  @return The FILE* of the file
  *******************************/
  FILE* FileHandle::GetFileStruct()
  {
    //todo THIS NEEDS TESTING
    return fopen(fileName_.c_str(), "r");
  }

  void FileHandle::MovePosition(s32 move)
  {
    if(!handle_.is_open())
    {
      return;
    }

    handle_.seekg(move, std::ios_base::cur);
  }

  /*!*****************************
   *  @brief Read a line into out ( assumes text file )
   *
   *  @param out - The variable to be filled with data
  *******************************/
  void FileHandle::ReadLine(std::string& out)
  {
    if(!handle_.is_open())
    {
      Open();
    }

    if(!handle_.good())
      return;

    std::getline(handle_, out);
  }

  /*!*****************************
   *  @brief Checks if the pointer is at the end of the file
   *
   *  @return True if at end of file
  *******************************/
  bool FileHandle::Eof()
  {
    if(!handle_.is_open())
      return false;
    return handle_.eof();
  }

  /*!*****************************
   *  @brief Gets the file name
   *
   *  @return The file name
  *******************************/
  const std::string& FileHandle::GetFileName() const
  {
    return fileName_;
  }

  /*!*****************************
   *  @brief Gets the file state: either Filesystem_FILE_OK, Filesystem_FILE_ERROR, or Filesystem_FILE_MODIFIED
   *
   *  @return The current file state
  *******************************/
  s32 FileHandle::GetFileState()
  {
    return fileState_;
  }

  /*!*****************************
   *  @brief Gets the file extension
   *
   *  @return extension as a string
  *******************************/
  std::string& FileHandle::GetFileExtension()
  {
    return extension_;
  }

  /*!*****************************
   *  @brief Gets the file's size in bytes
   *
   *  @return filesize
  *******************************/
  u32 FileHandle::GetFileSize() const
  {
    return size_;
  }

  s64 FileHandle::GetLastModified() const
  {
    return timeModified_;
  }

  /*!*****************************
   *  @brief Reads in the entire file
   *
   *  @return pointer to entire file data
  *******************************/
  const char* FileHandle::ReadFile()
  {
    if(file_ == nullptr)
    {
      file_ = new char[size_];
      ReadArray(file_, size_);
    }

    return file_;
  }

  /*!*****************************
   *  @brief Updates the internal file state
   *  @details Filesystem_FILE_OK, Filesystem_FILE_ERROR, or Filesystem_FILE_MODIFIED
   *
   *  @return the current filestate
  *******************************/
  s32 FileHandle::CheckFile()
  {
    struct stat attributes;

    if(stat(fileName_.c_str(), &attributes) == -1)
    {
      fileState_ = Filesystem_FILE_ERROR;
      return fileState_;
    }

    size_ = attributes.st_size;

    if(attributes.st_mtime > timeModified_)
    {
      timeModified_ = attributes.st_mtime;
      fileState_ = Filesystem_FILE_MODIFIED;
      return fileState_;
    }
    fileState_ = Filesystem_FILE_OK;
    return fileState_;
  }
}//Filesystem
