/*
  Defines the interface for the Socket class
*/

#pragma once
#include "Packet.h"
#include "Address.h"

namespace Networking
{
#if PLATFORM == PLAT_WINDOWS
  // we have to manually define this to what SOCKET is defined as
  // so that we don't shit everywhere with windows.h
  typedef unsigned long long SockHandle;
#else
  typedef int SockHandle;
#endif

  enum SocketType : short
  {
    SOCK_UDP,
    SOCK_TCP_SERVER, //will accept new connections
    SOCK_TCP_CLIENT, //will connect to server
    SOCK_MULTICAST_SERVER, //will send data out over socket
    SOCK_MULTICAST_CLIENT, //will only receive data
    SOCK_BROADCAST,
  };

  class Socket
  {
  public:
    Socket(SocketType type, const Address& dest);
    Socket(SockHandle handle, const Address& dest, SocketType type);
    ~Socket();

    bool Open(u16 localPort = 0);
    void Close();
    const Address& GetDest() const;

    void SetNoBlock(bool on = true);

    bool IsOpen();
    SocketType Type();

    /* TCP Only */
    Socket* Accept();

    bool Send(Packet& data);
    bool Send(const c08* data, s32 size);
    bool SendTo(const c08* data, s32 size, const Address& dest);
    
    Packet Receive(const u32 size);
    /*returns num bytes received*/
    s32 Receive(c08* buffer, s32 bufferSize);

  private:
    bool InitTCPServer();
    bool InitTCPClient();
    bool InitUDP();
    bool InitMulticast(bool listen);
    bool InitBroadcast();
    bool Bind();

    SocketType m_type;
    SockHandle m_sock;
    Address    m_dest;

    u16        m_localPort;

    bool       m_open;
  };
}
