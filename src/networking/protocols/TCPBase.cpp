#include "CommonPrecompiled.h"
#include "TCPBase.h"

namespace Networking
{
  const u16 gTcpVersion = 000001;

  TCPBase::TCPBase(u16 localPort, const String& name)
    : ProtocolBase(name, gTcpVersion, SOCK_TCP_SERVER, localPort),
    m_recvThread(&TCPBase::ThreadRecv, this), m_end(false), m_isServer(true)
  {}

  TCPBase::TCPBase(const Address& serverAddr, u16 localPort, const String& name)
    : ProtocolBase(name, gTcpVersion, SOCK_TCP_CLIENT, localPort, serverAddr),
    m_recvThread(&TCPBase::ThreadRecv, this), m_end(false), m_isServer(false)
  {}

  TCPBase::~TCPBase()
  {
    m_end = true;
    m_recvThread.join();
  }

  void TCPBase::Update(f32 dt)
  {
    if(!m_socket->IsOpen())
      exit(1);
    u32 num = m_recvData.Size();
    while(num--)
    {
      Packet p = m_recvData.Pop();
      HandlePacket(p);
    }
  }

  ///// PRIVATE
  void TCPBase::HandlePacket(Packet& p)
  {
  }

  bool TCPBase::RecvFromSocket(Socket* s)
  {
    Packet received = s->Receive(MAX_PACKET_SIZE);
    if(received)
    {
      m_recvData.Push(received);
      return true;
    }
    return false;
  }

  void TCPBase::ThreadRecv()
  {
    SLEEP_MILLI(10);
    while(!m_end)
    {
      u32 now = time(NULL);
      if(m_isServer)
      {
        Socket* s = m_socket->Accept();
        if(s)
        {
          const Address& dest = s->GetDest();
          LOG("New TCP Connection from " << dest);
          m_connections.emplace(dest, ConnectionData(s, now));
        }

        auto it = m_connections.begin();
        for(; it != m_connections.end(); ++it)
        {
          ConnectionData& data = it->second;
          if(RecvFromSocket(data.sock))
            data.lastRecvTime = time(NULL);
        }
      }
      else
      {
        RecvFromSocket(m_socket);
      }

      SLEEP_MILLI(5);
    }
  }
}
