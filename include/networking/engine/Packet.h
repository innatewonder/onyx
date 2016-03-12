/*
  Defines the interface for the Packet class
*/

#pragma once
#include "Address.h"
namespace Networking
{
  const u32 MAX_PACKET_SIZE = 1024;
  
  class Packet
  {
  public:
    enum State
    {
      OK,
      EMPTY,
      ERR,
    };

    enum Priority
    {
      LOW,
      HIGH,
      MUST_SEND
    };

    Packet();
    Packet(Packet::State state);
    Packet(const String& string);
    Packet(const c08* data, s32 size);
    Packet(const Packet& rhs);
    ~Packet();

    Packet& operator=(const Packet& rhs);
    operator bool() const;

    void             SetPriority(Packet::Priority p);
    Packet::Priority GetPriority() const;

    Packet::State    GetState() const;

    s32        GetSize() const;
    const c08* GetData() const;

    void AppendData(const c08* data, s32 size);

    void SetAddress(const Address& addr);
    const Address& GetAddress() const;

  private:
    struct PacketData
    {
      PacketData(const c08* data, s32 dataSize);
      ~PacketData();
      void Attatch();
      s16  Detatch(); //returns new refCount

      c08* data;
      s32  dataSize;
      s16  refCount;
    };

    PacketData* m_data;
    State       m_state;
    Priority    m_priority;
    Address     m_address;
  };
}
