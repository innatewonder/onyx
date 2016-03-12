#pragma once

#include "ProtocolBase.h"
#include "Address.h"

namespace Networking
{
  class TCPBase : public ProtocolBase
  {
  public:
    TCPBase(u16 localPort = 8000, const String& name = "TCPBase");
    TCPBase(const Address& serverAddr, u16 localPort = 0, const String& name = "TCPBase");
    ~TCPBase();

    virtual void Update(f32 dt);
    virtual void HandleMessage(Message* m) {}

  protected:
    virtual void HandlePacket(Packet& p);
    bool m_isServer;

    struct ConnectionData
    {
      ConnectionData(Socket* s, u32 time) 
        : sock(s), lastRecvTime(time), packNum(0) 
      {}
      u32 lastRecvTime;
      u08 packNum;
      Socket* sock;
    };

    typedef Containers::UnorderedMapUserHash<Address, ConnectionData, AddressHash> ConnectionCont;
    ConnectionCont m_connections;
    ThreadSafeQueue<Packet> m_recvData;

  private:
    bool RecvFromSocket(Socket* s);

    std::thread m_recvThread;
    bool m_end;
    void ThreadRecv();
  };
}
