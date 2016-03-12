#include "CommonPrecompiled.h"
#include "OnyxTransfer.h"
#include "Socket.h"

#include <iostream>

/*
update file
  named the port of the server (10294.onyx)
  version #
  filename modifiedTime deletedTime
  ...
  */

namespace Networking
{
  const u16 gOnyxVersion = 000001;
  const u32 MAX_ONYX_PACKET = 40000;

  struct PacketHeader
  {
    PacketHeader(u16 seq)
      : m_seq(seq)
    {}

    u16 m_seq;
  };

  OnyxServer::OnyxServer(u16 localPort)
    : TCPBase(localPort, "OnyxServer"), m_test("recvFile.gif")
  {}

  OnyxServer::~OnyxServer()
  {}

  void OnyxServer::Update(f32 dt)
  {
    TCPBase::Update(dt);
  }

  void OnyxServer::HandlePacket(Packet& p)
  {
    m_test.write(p.GetData(), p.GetSize());
  }

  ////////////////////////////////////////
  // CLIENT
  OnyxClient::OnyxClient(Address& server, Filesystem::FolderHandle* folder)
    : TCPBase(server, 0, "OnyxClient")
  {
    LOG("server " << server);
    m_local = new OnyxFile(server.GetPort(), folder);
    //WriteOnyxFile(reader, baseFolder, server.GetPort());
    m_testFile = folder->GetFile("assets/test/hh.gif");
  }

  OnyxClient::~OnyxClient()
  {}

  void OnyxClient::Update(f32 dt)
  {
    TCPBase::Update(dt);

    u32 size = m_testFile->GetFileSize();
    LOG("file size " << size);
    c08 buffer[MAX_PACKET_SIZE];
    while(size)
    {
      u32 buffSize = MAX_PACKET_SIZE > size ? size : MAX_PACKET_SIZE;
      size -= buffSize;

      m_testFile->ReadBytes(buffer, buffSize);
      Send(Packet(buffer, buffSize));
      SLEEP_MILLI(6);
    }
    LOG("done sending file");
  }

  void OnyxClient::HandlePacket(Packet& p)
  {

  }

}


/*
Client:
  Check for local update file
  Connect and get updates that dont overwrite
  send updates
  loop check local + recv new updates

Server:
  Recv updates, update file
  send finished + streaming updates to clients
*/
