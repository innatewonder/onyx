#pragma once

#include "Protocol.h"
namespace Networking
{
  class NetworkEngine
  {
  public:
    NetworkEngine();
    ~NetworkEngine();

    bool Initialize(ArgParser& args);
    void Destroy();

    void AddProtocol(IProtocol* proto);
    void Update(f32 dt);

    void HandleMessage(Message* msg);
  private:
    Containers::List<IProtocol*> m_protos;
  };
}
