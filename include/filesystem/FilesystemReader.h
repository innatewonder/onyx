#ifndef Filesystem_READER_H
#define Filesystem_READER_H

#include <string>
#include <istream>
#include <list>

#include <unordered_map>

#include "HBThread.h"

#include "FilesystemDefines.h"
#include "FileHandle.h"

/* notes
Get istream
thread safe

ostream?? write to temp then recompile?
need to be able to shutdown pool
*/

namespace Filesystem
{

typedef void (*StreamFunc)(FileHandle*);

class FolderHandle;
class FilesystemReader
{
  public:
      //Readmode options defined in FilesystemDefines.h
    FilesystemReader();
    ~FilesystemReader();

    //returns error code
    s32 OpenFile(std::string filename, bool thread = false);
    s32 OpenFolder(std::string folderName, bool recurse = false, bool thread = false);
    void WaitForThreads(); //this is mostly useful for tests
    //if you're using threading
    //make sure to call this each frame to make sure stream functions are called
    void Update();

    //retuns true if successful
    bool CloseFile(std::string filename);
    bool CloseFolder(std::string foldername);

    FileHandle* GetFile(std::string filename);
    FolderHandle* GetFolder(std::string foldername);

    void SetStreamFunction(std::string extension, StreamFunc f);
    void SetDefaultStreamFunction(StreamFunc f);

  private:
    s32 OpenFileMethod(const std::string& filename);

    typedef std::unordered_map<std::string, FileHandle*> FileCont;
    typedef FileCont::iterator FileContIt;
    FileCont files_;

    typedef std::unordered_map<std::string, FolderHandle*> FolderCont;
    typedef FolderCont::iterator FolderIt;
    FolderCont folders_;

    typedef std::unordered_map<std::string, StreamFunc> StreamFuncs;
    StreamFuncs streamers_;
    StreamFunc  defaultStreamer_;

    ThreadPool m_threadPool;

    friend class OpenFolderJob;
};

extern FilesystemReader* ReaderInstance;

}//Filesystem

#endif