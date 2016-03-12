#pragma once
#include "Protocol.h"
#include "Address.h"
#include "Socket.h"

namespace Networking
{
  class ProtocolBase : public IProtocol
  {
  public:
    ProtocolBase(const String& name, u32 version, 
                SocketType type, u16 localPort, 
                const Address& dest = Address());
    ~ProtocolBase();

    virtual const String& Name() const {return m_name;}
    virtual u16 GetPort() const {return m_localPort;}
    virtual u32 Version() const {return m_version;}

    virtual void Update(f32 dt) = 0;
    virtual void HandleMessage(Message* m) = 0;

  protected:
    String m_name;
    u32 m_version;
    u16 m_localPort;
    
    Socket* m_socket;
  };
}