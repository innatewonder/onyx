#ifndef Filesystem_FOLDER_HANDLE_H
#define Filesystem_FOLDER_HANDLE_H

namespace Filesystem
{

class FileHandle;
class FolderHandle;
typedef std::list<FolderHandle*> FolderCont;
typedef FolderCont::iterator     FolderContIt;

class FolderHandle
{
  public:
    FolderHandle(std::string folder, bool recurse = false);
    ~FolderHandle();

    s32 GetFolderState() const;
    u32 GetTotalSize();
    u32 GetNumFiles() const;

    const String GetFolderName() const;
    StringVec& GetFileNames();

    FileHandle* GetFile(const String& name) const;

    s32 CheckFolder(bool recurse = false);

    void AddFileName(std::string filename, u32 filesize = 0);

  private:
    std::string folderName_;

    StringVec   files_;

    u32        totalSize_;
    s32        folderState_;
};

}

#endif