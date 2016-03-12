#pragma once
#include "TCPBase.h"

#include "OnyxFile.h"

namespace Networking
{
  extern const u16 gOnyxVersion;
  class OnyxServer : public TCPBase
  {
  public:
    OnyxServer(u16 localPort = 10294);
    ~OnyxServer();

    u32 Version() const { return gOnyxVersion; }

    virtual void Update(f32 dt);

  private:
    virtual void HandlePacket(Packet& p);

    std::ofstream m_test;
  };

  class OnyxClient : public TCPBase
  {
  public:
    OnyxClient(Address& server, Filesystem::FolderHandle* reader);
    ~OnyxClient();

    u32 Version() const { return gOnyxVersion; }

    virtual void Update(f32 dt);

  private:
    virtual void HandlePacket(Packet& p);

    OnyxFile* m_local;
    Filesystem::FileHandle* m_testFile;
  };
}
