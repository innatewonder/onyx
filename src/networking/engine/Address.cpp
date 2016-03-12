#include "CommonPrecompiled.h"
#include "NetworkingIncludes.h"
#include "Address.h"

#include <stdio.h>

namespace Networking
{
  std::ostream& operator<<(std::ostream& os, const Networking::Address& addr)
  {
    os << addr.ToString();
    return os;
  }

  std::size_t AddressHash::operator()(const Address& addr) const
  {
    return std::hash<std::string>()(addr.ToString());
  }

  Address::Address()
  {
    SetAddress("127.0.0.1", "0");
  }

  Address::Address(const String& addr, const String& port)
  {
    if(addr == "loopback" || addr == "localhost")
      SetAddress("127.0.0.1", port);
    else
      SetAddress(addr, port);
  }

  Address::Address(const Address& rhs)
  {
    SetAddress(rhs.m_addr->ai_addr, rhs.m_addr->ai_addrlen);
  }

  Address::Address(const sockaddr* addr)
  {
    SetAddress(addr, sizeof(addr));
  }

  Address::Address(const sockaddr_storage* addr, u32 addrLen)
  {
    SetAddress((sockaddr*)addr, addrLen);
  }

  Address::~Address()
  {
    freeaddrinfo(m_addr);
  }

  Address& Address::operator=(const Address& rhs)
  {
    SetAddress(rhs.m_addr->ai_addr, rhs.m_addr->ai_addrlen);
    return *this;
  }

  bool Address::operator==(const Address& rhs) const
  {
    if(rhs.m_addr->ai_addrlen != m_addr->ai_addrlen)
      return false;

    return 0 == memcmp(rhs.m_addr->ai_addr, m_addr->ai_addr, m_addr->ai_addrlen);
  }

  void Address::SetAddress(const sockaddr* addr, u32 len)
  {
    u32 addrlen = NI_MAXHOST;
    u32 portlen = 6;
    char* addrStr = new char[addrlen];
    char* portStr = new char[portlen];
    getnameinfo(addr, len,
      addrStr, addrlen,
      portStr, portlen, NI_NUMERICHOST);

    SetAddress(addrStr, portStr);
    m_strAddr.assign(addrStr);
    m_strAddr.append(":");
    m_strAddr.append(portStr);

    delete[] addrStr;
    delete[] portStr;
  }

  void Address::SetAddress(const String& addr, const String& port)
  {
    getaddrinfo(addr.c_str(), port.c_str(), NULL, &m_addr);
    m_strAddr = "";
  }

  String Address::ToString() const
  {
    if(!m_strAddr.size())
    {
      u32 addrlen = NI_MAXHOST;
      u32 portlen = 6;
      char* addrStr = new char[addrlen];
      char* portStr = new char[portlen];
      getnameinfo(m_addr->ai_addr, m_addr->ai_addrlen,
        addrStr, addrlen,
        portStr, portlen, NI_NUMERICHOST);

      m_strAddr.assign(addrStr);
      m_strAddr.append(":");
      m_strAddr.append(portStr);

      delete[] addrStr;
      delete[] portStr;
    }
    return m_strAddr;
  }

  const sockaddr& Address::GetSendToAddress() const
  {
    return *m_addr->ai_addr;
  }

  u32 Address::GetSendAddrLen() const
  {
    return m_addr->ai_addrlen;
  }

  s32 Address::GetProto() const
  {
    return m_addr->ai_family;
  }

  u16 Address::GetPort() const
  {
    if(m_addr->ai_addr->sa_family == AF_INET) {
      return ntohs(((struct sockaddr_in*)m_addr->ai_addr)->sin_port);
    }

    return ntohs(((struct sockaddr_in6*)m_addr->ai_addr)->sin6_port);
  }
}
