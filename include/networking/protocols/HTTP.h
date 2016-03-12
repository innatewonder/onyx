#pragma once

#include "TCPBase.h"
namespace Networking
{
  class HTTP : public TCPBase
  {
    const String m_httpVersion = "HTTP/1.1";
  public:
    HTTP(u16 localPort = 80);
    ~HTTP();

  private:
    virtual void HandlePacket(Packet& p);
    
    enum Status
    {
      OK = 200,
      NoContent = 204,
      NotFound = 404,
      Conflict = 409,
    };

    enum DataType
    {
      TextHTML,
    };

    String HandleRequest(const String& request);
    String MakeResponse(Status err, DataType type, const String& data, bool sendData = true);

    Containers::UnorderedMap<u16, String> m_dataTypes;
  };
}
