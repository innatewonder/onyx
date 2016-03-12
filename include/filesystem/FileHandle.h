#ifndef FILE_HANDLE_H
#define FILE_HANDLE_H

namespace Filesystem
{class FileHandle;}
#include "FilesystemReader.h"
#include "FilesystemDefines.h"

/* @brief returned on file deletion as well as if it should be excluded */
#define Filesystem_FILE_ERROR   -1
#define Filesystem_FILE_OK       0
#define Filesystem_FILE_MODIFIED 1

#define Filesystem_IS_FOLDER     3
#define Filesystem_FOLDER_OK     4
#define Filesystem_FOLDER_EMPTY  5
#define Filesystem_FILE_IGNORED  6

#ifdef _WIN32
#define NEWLINE_CHAR '\n'
#elif __linux__
#define NEWLINE_CHAR '\n'
#else //__APPLE__ 
#define NEWLINE_CHAR '\r'
#endif

namespace Filesystem
{

class FileHandle
{
  public:
    FileHandle(std::string filename);
    ~FileHandle();

    /* open and close internal file handle */
    bool Open();
    void Close();
    
    /* read data */
    void ReadLine(std::string& out);
    FILE* GetFileStruct();

    /* move read pointer in direction */
    void MovePosition(s32 move);

    /*!*****************************
      *  @brief Reads one of any type from file in byte form
      *
      *  @param out - The variable to fill with read data
    *******************************/
    template<typename T>
    void ReadBinary(T& out)
    {
      if(!handle_.is_open())
        Open();
      if(handle_.good())
        handle_.read((c08*)(&out), sizeof(T));
    }

    /*!*****************************
      *  @brief Reads one integral type from file in char form
      *
      *  @param out - The variable to fill with read data
    *******************************/
    template<typename T>
    void ReadIntegral(T& out)
    {
      if(!handle_.is_open())
      {
        Open();
      }

      handle_ >> out;
    }

    /*!*****************************
      *  @brief Reads an array of any type from file
      *
      *  @param out - The array to fill, must be continuous memory
      *  @param arraySize - The number of elements in the array
    *******************************/
    template<typename T>
    void ReadArray(T* out, u32 arraySize)
    {
      if(!handle_.is_open())
        Open();
      handle_.read((c08*)(out), sizeof(T) * arraySize);
    }

    /*!*****************************
      *  @brief Reads specified number of bytes from file
      *
      *  @param out - The variable to fill with read data
      *  @param numBytes - The number of bytes to be read
    *******************************/
    template<typename T>
    void ReadBytes(T* out, u32 numBytes)
    {
      if(!handle_.is_open())
        Open();
      handle_.read((c08*)(out), numBytes);
    }

    bool Eof();

    /* Get File information */
    const std::string&  GetFileName() const;
    std::string&        GetFileExtension();
    u32                 GetFileSize() const;
    s64                 GetLastModified() const;

    const char* ReadFile();

    //returns Filesystem_FILE_OK, Filesystem_FILE_ERROR, Filesystem_FILE_MODIFIED defined in precompiled
    s32 GetFileState(void);
    
    /* Update file state */
    s32 CheckFile(void);

  private:
    std::string fileName_;
    std::string extension_;

    u32         size_;
    s64         timeModified_;

    std::ifstream handle_;
    char*         file_;

    char* readPointer_;

    s32 fileState_;
};

typedef std::list<FileHandle*> FileCont;
typedef FileCont::iterator     FileContIt;

}//Filesystem
#endif
