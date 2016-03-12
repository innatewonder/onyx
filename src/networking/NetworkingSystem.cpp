#include "CommonPrecompiled.h"
#include "NetworkingIncludes.h"
#include "NetworkingSystem.h"
#include "NetworkEngine.h"

#include "HTTP.h"
#define SERVER_PORT 40000

namespace Networking
{
  NetworkingSystem::NetworkingSystem()
    : m_engine(Memory::New<NetworkEngine>())
  {}

  NetworkingSystem::~NetworkingSystem()
  {
    Memory::Delete(m_engine);
  }

  bool NetworkingSystem::Initialize(ArgParser& args)
  {
    bool ret = m_engine->Initialize(args);
    if(ret)
    {
      m_engine->AddProtocol(new HTTP(1024));
    }
    return ret;
  }

  void NetworkingSystem::Destroy()
  {
    m_engine->Destroy();
  }

  void NetworkingSystem::Test()
  {}

  void NetworkingSystem::Tick(f32 dt)
  {
    m_engine->Update(dt);
  }

  void NetworkingSystem::HandleMessage(Message* msg)
  {
    m_engine->HandleMessage(msg);
  }
}
