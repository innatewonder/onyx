#include "CommonPrecompiled.h"
#include "NetworkingIncludes.h"
#include "NetworkEngine.h"

#include "Socket.h"

namespace Networking
{
  NetworkEngine::NetworkEngine()
  {}

  NetworkEngine::~NetworkEngine()
  {}

  bool NetworkEngine::Initialize(ArgParser& args)
  {
#if PLATFORM == PLAT_WINDOWS
    WSADATA WsaData;
    return WSAStartup(MAKEWORD(2,2), &WsaData) == NO_ERROR;
#else
    return true;
#endif
  }

  void NetworkEngine::Destroy()
  {
    for(auto p : m_protos)
      delete p;

#if PLATFORM == PLAT_WINDOWS
    WSACleanup();
#endif
  }

  void NetworkEngine::AddProtocol(IProtocol* proto)
  {
    m_protos.push_back(proto);
  }

  void NetworkEngine::Update(f32 dt)
  {
    for(auto p : m_protos)
    {
      p->Update(dt);
    }
  }

  void NetworkEngine::HandleMessage(Message* msg)
  {
    for(auto p : m_protos)
    {
      p->HandleMessage(msg);
    }
  }
}
