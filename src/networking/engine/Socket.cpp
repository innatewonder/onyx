#include "CommonPrecompiled.h"
#include "NetworkingIncludes.h"
#include "Socket.h"
#include "Address.h"

namespace Networking
{
#if PLATFORM == PLAT_WINDOWS
  typedef int socklen_t;
#endif

  s32 getSockErr()
  {
#if PLATFORM == PLAT_WINDOWS
    return WSAGetLastError();
#else
    return errno;
#endif
  }

#if PLATFORM == PLAT_WINDOWS
#define IS_ERR(err) \
    err < 0 || err == UINT_PTR(-1)

#define SOCK_CHECK(err, msg, ret) \
  if(IS_ERR(err)) \
    { \
    s32 err = getSockErr(); \
    if(err != WSAEWOULDBLOCK) \
        { \
      WARN(msg << ": [" << err << "] "); \
      return ret; \
        } \
    }
#else
#define IS_ERR(err) \
    err < 0

#define SOCK_CHECK(err, msg, ret) \
  if(IS_ERR(err)) \
    { \
    s32 err = getSockErr(); \
    if(err != EWOULDBLOCK && err != EAGAIN) \
        { \
      ERR(msg << ": [" << err << "] " << strerror(err)); \
      return ret; \
        } \
    }
#endif

  Socket::Socket(SocketType type, const Address& dest)
    : m_sock(0), m_dest(dest), m_type(type), m_open(false)
  {
  }

  Socket::Socket(SockHandle handle, const Address& dest, SocketType type)
    : m_sock(handle), m_dest(dest), m_type(type), m_open(true)
  {
    SetNoBlock();
  }

  Socket::~Socket()
  {
    if(m_open)
    {
      Close();
    }
  }

  bool Socket::Open(u16 localPort)
  {
    m_localPort = localPort;
    switch(m_type)
    {
    case SOCK_UDP:
      m_open = InitUDP();
      break;
    case SOCK_TCP_SERVER:
      m_open = InitTCPServer();
      break;
    case SOCK_TCP_CLIENT:
      m_open = InitTCPClient();
      break;
    case SOCK_MULTICAST_SERVER:
    case SOCK_MULTICAST_CLIENT:
      m_open = InitMulticast(m_type == SOCK_MULTICAST_SERVER);
      break;
    case SOCK_BROADCAST:
      m_open = InitBroadcast();
      break;
    }
    if(!m_open)
    {
      Close();
      return false;
    }

    SetNoBlock();
    return m_open;
  }

  void Socket::Close()
  {
#if PLATFORM == PLAT_MAC || PLATFORM == PLAT_UNIX
    close(m_sock);
#elif PLATFORM == PLAT_WINDOWS
    closesocket(m_sock);
#endif
    m_open = false;
  }

  const Address& Socket::GetDest() const
  {
    return m_dest;
  }

  void Socket::SetNoBlock(bool on /* = true */)
  {
    int err = 0;
#if PLATFORM == PLAT_WINDOWS
    u_long mode = on ? 1 : 0;
    err = ioctlsocket(m_sock, FIONBIO, &mode);
#else
    s32 flags = fcntl(m_sock, F_GETFL, 0);
    if (flags < 0) 
      return;
    flags = on ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK);
    err = fcntl(m_sock, F_SETFL, flags);
#endif

    SOCK_CHECK(err, "SetNoBlock", );
  }

  bool Socket::IsOpen()
  {
    return m_open;
  }

  SocketType Socket::Type()
  {
    return m_type;
  }

  /* TCP Only */
  Socket* Socket::Accept()
  {
    SOFTASSERT(m_type == SOCK_TCP_SERVER, "Socket is not a tcp accepter!", nullptr);

    sockaddr_storage sockAddr;
#if PLATFORM == PLAT_WINDOWS
    s32 addrSize = sizeof(sockAddr);
#else
    u32 addrSize = sizeof(sockAddr);
#endif
    SockHandle ret = accept(m_sock, (sockaddr*)&sockAddr, &addrSize);
    SOCK_CHECK(ret, "accept err", nullptr);
    if(IS_ERR(ret))
      return nullptr;

    Address fromAddr(&sockAddr, addrSize);
    return new Socket(ret, fromAddr, SOCK_TCP_CLIENT);
  }

  bool Socket::Send(Packet& data)
  {
    return SendTo(data.GetData(), data.GetSize(), m_dest);
  }

  bool Socket::Send(const c08* data, s32 size)
  {
    return SendTo(data, size, m_dest);
  }

  bool Socket::SendTo(const c08* data, s32 size, const Address& dest)
  {
    int sent = sendto(m_sock, data, size, 0,
      &dest.GetSendToAddress(),
      sizeof(sockaddr_storage));

    SOCK_CHECK(sent, "Send Error", false);
    return sent == size;
  }

  s32 Socket::Receive(c08* buffer, s32 bufferSize)
  {
    s32 bytes = recvfrom(m_sock, buffer, bufferSize, 0, NULL, 0);

    SOCK_CHECK(bytes, "recvfrom", 0);
    if(bytes <= 0)
      return 0;

    return bytes;
  }

  Networking::Packet Socket::Receive(const u32 size)
  {
    TODO("This should be a staic buffer");
    c08* buffer = new c08[size];

    sockaddr_storage from;
#if PLATFORM == PLAT_WINDOWS
    s32 fromLength = sizeof(from);
#else
    u32 fromLength = sizeof(from);
#endif

    s32 bytes = recvfrom(m_sock, buffer, size, 0,
      (sockaddr*)&from, &fromLength);

    SOCK_CHECK(bytes, "recvfrom", Packet());
    if(bytes <= 0)
      return Packet();

    Packet ret(buffer, bytes);

    if(fromLength > 0)
    {
      Address fromAddr(&from, fromLength);
      ret.SetAddress(fromAddr);
    }
    else if(m_type == SOCK_TCP_CLIENT)
    {
      ret.SetAddress(m_dest);
    }
    else
    {
      ERR("We have not fromAddr!");
    }
    delete[] buffer;
    return ret;
  }

  ///////////////
  // PRIVATE
  bool Socket::InitTCPServer()
  {
    m_sock = socket(AF_INET6, SOCK_STREAM, 0);
    SOCK_CHECK(m_sock, "failed to create socket", false);

    bool ret = Bind();
    if(!ret)
      return false;

    LOG("listening");
    s32 err = listen(m_sock, 5);
    SOCK_CHECK(err, "listen err", false)

    return true;
  }

  bool Socket::InitTCPClient()
  {
    m_sock = socket(m_dest.GetProto(), SOCK_STREAM, 0);
    SOCK_CHECK(m_sock, "failed to create socket", false);

    LOG("connecting");
    s32 err = connect(m_sock, 
      &m_dest.GetSendToAddress(), m_dest.GetSendAddrLen());
    SOCK_CHECK(err, "failed to connect", false);

    return true;
  }

  bool Socket::InitUDP()
  {
    m_sock = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    SOCK_CHECK(m_sock, "init udp", false);
    return Bind();
  }

  bool Socket::InitMulticast(bool listen)
  {
    m_sock = socket(AF_INET6, SOCK_DGRAM, 0);
    SOCK_CHECK(m_sock, "init multicast", false);
    int err = 0;

    if(listen)
    {
      const char on = 'a';
      err = setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
      SOCK_CHECK(err, "Reusing addr", false);

      if(!Bind())
        return false;

      TODO("Allow any multicast address");
      struct ip_mreq mreq;
      mreq.imr_multiaddr.s_addr = inet_addr("255.0.0.102");
      mreq.imr_interface.s_addr = htonl(INADDR_ANY);

      err = setsockopt(m_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq));
      SOCK_CHECK(err, "add membership", false);
    }
    else
    {
      char loopch = 0;

      err = setsockopt(m_sock, IPPROTO_IP, IP_MULTICAST_LOOP, (char*)&loopch, sizeof(loopch));
      SOCK_CHECK(err, "IP_MULTICAST_LOOP", false);

      struct in_addr localInterface;
      localInterface.s_addr = inet_addr("9.5.1.1");
      err = setsockopt(m_sock, IPPROTO_IP, IP_MULTICAST_IF, (char*)&localInterface, sizeof(localInterface));
      SOCK_CHECK(err, "multicast_if", false);
    }
    return true;
  }

  bool Socket::InitBroadcast()
  {
    m_sock = socket(AF_INET6, SOCK_DGRAM, 0);
    SOCK_CHECK(m_sock, "init broadcast", false);

    char broadcast = 'a';
    int err = setsockopt(m_sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));
    SOCK_CHECK(err, "SO_BROADCAST", false);

    return Bind();
  }

  bool Socket::Bind()
  {
    int no = 0;
    s32 err = setsockopt(m_sock, IPPROTO_IPV6, IPV6_V6ONLY, (char*)&no, sizeof(no));
    SOCK_CHECK(err, "ipv6 only err", false);

    sockaddr_in6 address;
    memset(&address, 0, sizeof(address));
    address.sin6_family = AF_INET6;
    address.sin6_addr = in6addr_any;
    address.sin6_port = htons(m_localPort);

    err = bind(m_sock, (const sockaddr*)&address, sizeof(address));
    SOCK_CHECK(err, "bind", false);

    return err >= 0;
  }
}
