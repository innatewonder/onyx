/*
  Defines interface for Connection Class
*/
#pragma once

namespace Networking
{
  class Packet;
  class Socket;
  class Address;

  enum SocketType : short;

  class IProtocol
  {
  public:
    virtual const String& Name() const = 0; // name for debugging
    virtual u16 GetPort() const = 0;
    virtual u32 Version() const = 0; // versioning major, minor, rev 000000

    virtual void Update(f32 dt) = 0;
    virtual void HandleMessage(Message* m) = 0;
  };
}
