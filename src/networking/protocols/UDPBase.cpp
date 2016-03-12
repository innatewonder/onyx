#include "CommonPrecompiled.h"
#include "UDPBase.h"

namespace Networking
{
  const u16 gUdpVersion = 000001;

  UDPBase::UDPBase(u16 localPort, const String& name)
    : ProtocolBase(name, gUdpVersion, SOCK_UDP, localPort),
    m_recvThread(&UDPBase::ThreadRecv, this), m_end(false)
  {
    test_serv = true;
  }

  UDPBase::UDPBase(const Address& serverAddr, u16 localPort, const String& name)
    : ProtocolBase(name, gUdpVersion, SOCK_UDP, localPort, serverAddr),
    m_recvThread(&UDPBase::ThreadRecv, this), m_end(false)
  {
    test_serv = false;
    test_time = 0;
  }

  UDPBase::~UDPBase()
  {
    m_end = true;
    m_recvThread.join();
  }

  void UDPBase::Update(f32 dt)
  {
    u32 num = m_recvData.Size();
    while(num--)
    {
      Packet p = m_recvData.Pop();
      HandlePacket(p);
    }

    test_time += dt;
    if(test_time >= 2.f && !test_serv)
    {
      m_socket->Send("test", 4);
      test_time = 0;
    }
  }

  ///// PRIVATE
  void UDPBase::HandlePacket(Packet& p)
  {
    String data;
    data.assign(p.GetData(), p.GetSize());
    LOG(data);
    if(test_serv)
      m_socket->SendTo("ret", 3, p.GetAddress());
  }

  void UDPBase::ThreadRecv()
  {
    Packet received;
    while(!m_end)
    {
      received = m_socket->Receive(MAX_PACKET_SIZE);
      if(received)
      {
        const Address& from = received.GetAddress();
        u32 now = time(NULL);
        auto foundIt = m_connections.find(from);
        if(foundIt == m_connections.end())
        {
          LOG("New connection from " << from);
          m_connections.emplace(from, ConnectionData(now));
        }
        else
        {
          foundIt->second.lastRecvTime = now;
        }
        m_recvData.Push(received);
      }
      else
      {
        // so we dont spin doing nothing
        SLEEP_MILLI(5);
      }
    }
  }
}
