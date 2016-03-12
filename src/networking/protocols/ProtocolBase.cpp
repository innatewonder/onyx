#include "CommonPrecompiled.h"
#include "ProtocolBase.h"

namespace Networking
{
  ProtocolBase::ProtocolBase(const String& name, u32 version,
                             SocketType type, u16 localPort, 
                             const Address& dest)
    : m_name(name), m_version(version), m_localPort(localPort)
  {
    m_socket = new Socket(type, dest);
    m_socket->Open(m_localPort);
  }

  ProtocolBase::~ProtocolBase()
  {
    m_socket->Close();
    delete m_socket;
  }
}