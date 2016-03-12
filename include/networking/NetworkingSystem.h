#pragma once

namespace Networking
{
  class Socket;
  class NetworkEngine;
  class NetworkingSystem : public System
  {
  public:
    NetworkingSystem();
    ~NetworkingSystem();

    virtual bool Initialize(ArgParser& args);
    virtual void Destroy();
    virtual void Test();

    virtual void Tick(f32 dt);

    virtual void HandleMessage(Message* msg);

  private:
    NetworkEngine* m_engine;

    bool m_server;
  };
}
