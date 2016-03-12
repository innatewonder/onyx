#pragma once

#include "ProtocolBase.h"
#include "Address.h"

namespace Networking
{
  extern const u16 gUdpVersion;
  class UDPBase : public ProtocolBase
  {
  public:
    UDPBase(u16 localPort = 26845, const String& name = "UDPBase");
    UDPBase(const Address& serverAddr, u16 localPort = 26845, const String& name = "UDPBase");
    ~UDPBase();

    virtual void Update(f32 dt);
    virtual void HandleMessage(Message* m) {}

  protected:
    virtual void HandlePacket(Packet& p);

    struct ConnectionData
    {
      ConnectionData() : lastRecvTime(0), packNum(0) {}
      ConnectionData(u32 time) : lastRecvTime(time), packNum(0) {}
      u32 lastRecvTime;
      u08 packNum;
    };

    typedef Containers::UnorderedMapUserHash<Address, ConnectionData, AddressHash> ConnectionCont; 
    ConnectionCont m_connections;
    ThreadSafeQueue<Packet> m_recvData;

  private:

    std::thread m_recvThread;
    bool m_end;
    void ThreadRecv();

    bool test_serv;
    f32 test_time;
  };
}
