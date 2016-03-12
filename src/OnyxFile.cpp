#include "CommonPrecompiled.h"
#include <regex>
#include "OnyxFile.h"

#include "OnyxTransfer.h"

OnyxFile::OnyxFile(u16 port, Filesystem::FolderHandle* sys)
  : m_sys(sys)
{
  m_localName = std::to_string(port) + ".onyx";
  m_local = sys->GetFile(m_localName);
  ParseLocal();
}

OnyxFile::~OnyxFile()
{}

void OnyxFile::WriteLocal()
{
  String diffs = Differences();
  std::ofstream out(m_localName);
  out << std::to_string(Networking::gOnyxVersion) << std::endl;

  LOG("num files " << m_sys->GetNumFiles());
  auto filenames = m_sys->GetFileNames();
  for(auto it : filenames)
  {
    auto file = m_sys->GetFile(it);
    out << file->GetFileName() << "|" 
        << file->GetLastModified() << "|"
        << 0
        << std::endl;
  }

  out.close();
}

void OnyxFile::SetRemote(const String& text)
{
  m_remote = text;
}

String OnyxFile::Differences()
{
  return "";
}

String OnyxFile::DeletedFiles()
{
  return "";
}

void OnyxFile::ParseLocal()
{
  String line;
  m_local->ReadLine(line);
  if(atoi(line.c_str()) != Networking::gOnyxVersion)
  {
    LOG("Onyx Versions are different!");
    return;
  }

  m_local->ReadLine(line);
  while(!m_local->Eof())
  {
    s32 pos = line.find_first_of('|');
    String filename = (line.substr(0, pos));
    LOG(filename);
    
    pos += 1;
    s32 nextPos = line.find_first_of('|', pos);
    s32 modifyTime = atoi(line.substr(pos, nextPos - pos).c_str());
    LOG(" time " << modifyTime);

    s32 deletedTime = atoi(line.substr(nextPos + 1).c_str());
    LOG(" deleted " << deletedTime);
    m_local->ReadLine(line);

    auto file = m_sys->GetFile(filename);
    if(!file)
      LOG("File Deleted now " << std::time(0));
  }
}