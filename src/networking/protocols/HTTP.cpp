#include "CommonPrecompiled.h"
#include <sstream>
#include "Socket.h"
#include "HTTP.h"

namespace Networking
{
  const String NEWLINE = "\r\n";
  HTTP::HTTP(u16 localPort)
    : TCPBase(localPort, "HTTP")
  {
    m_dataTypes.emplace(TextHTML, "text/html");
  }

  HTTP::~HTTP()
  {}

  void HTTP::HandlePacket(Packet& p)
  {
    String got;
    got.assign(p.GetData(), p.GetSize());
    LOG(got);

    //handle request, close connection after handling
    String response = HandleRequest(got);
    auto foundIt = m_connections.find(p.GetAddress());
    if(foundIt != m_connections.end())
      foundIt->second.sock->Send(response.c_str(), response.size());
    else
      ERR("where did the connection go!?!?");
  }

  String NextSubstring(const String& str, u32& pos, const String& find = NEWLINE)
  {
    u32 end = str.find_first_of(find, pos);
    if(end == std::string::npos)
      end = str.size() - 1;
    String ret = str.substr(pos, end - pos);
    pos = end + 1;
    return ret;
  }

  String HTTP::HandleRequest(const String& request)
  {
    u32 line = 0;
    u32 pos = 0;
    String requestLine = NextSubstring(request, line);

    String verb = NextSubstring(requestLine, pos, " ");
    if(verb == "GET" || verb == "HEAD")
    {
      LOG("Verb: " << verb);

      String requestedResource = NextSubstring(requestLine, pos, " ");
      String requestVersion = NextSubstring(requestLine, pos, " ");
      LOG("resource: " << requestedResource);
      LOG("version: " << requestVersion);

      return MakeResponse(OK, TextHTML, "<html><body><p>hai</p></body><html>", verb == "GET");
    }
    //MakeResponse(NotFound, TextHTML, "<html><body><h1>NotFound!</h1></body></html>");
    return EMPTY_STRING;
  }

  String HTTP::MakeResponse(HTTP::Status stat, HTTP::DataType type, const String& data, bool sendData)
  {
    std::stringstream ret;
    ret << m_httpVersion << " ";
    ret << stat;
    ret << NEWLINE;
    if(data.size() > 0)
    {
      ret << "Content-Type: " << m_dataTypes.at(type);
      ret << NEWLINE;
      ret << "Content-Length: " << data.size();
      ret << NEWLINE << NEWLINE;
      if(sendData)
        ret << data;
    }
    return ret.str();
  }
}
