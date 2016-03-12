#pragma once

class OnyxFile
{
public:
  OnyxFile(u16 port, Filesystem::FolderHandle* sys);
  ~OnyxFile();

  void WriteLocal();

  void SetRemote(const String& text);
  String Differences();
  String DeletedFiles();

private:
  void ParseLocal();

  Filesystem::FolderHandle* m_sys;
  String m_remote;

  String m_localName;
  Filesystem::FileHandle* m_local;
};

