/*
  Defines the interface for the Address Class
*/

#pragma once
struct sockaddr;
struct sockaddr_storage;
struct addrinfo;

namespace Networking
{
  class Address
  {
  public:
    Address();
    // supports IPv4 and IPv6 "192.168.1.1" or "[::1]" or "google.com"
    Address(const String& addr, const String& port);
    Address(const Address& rhs);
    Address(const sockaddr* addr);
    Address(const sockaddr_storage* addr, u32 addrlen);
    ~Address();

    Address& operator=(const Address& rhs);

    bool operator==(const Address& rhs) const;
    friend std::ostream& operator<<(std::ostream& str, const Address& addr);

    void SetAddress(const sockaddr* addr, u32 len);
    void SetAddress(const String& addr, const String& port);

    //u32 GetAddress() const;
    u16 GetPort() const;
    
    String ToString() const;
    const sockaddr& GetSendToAddress() const;
    u32 GetSendAddrLen() const;
    s32 GetProto() const;

  private:
    addrinfo* m_addr;
    mutable String m_strAddr;
  };

  struct AddressHash
  {
    std::size_t operator()(const Address& addr) const;
  };
}
