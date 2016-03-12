#include "CommonPrecompiled.h"
#include "Packet.h"

namespace Networking
{
  Packet::Packet()
    : m_data(nullptr), m_state(EMPTY), m_priority(LOW)
  {}

  Packet::Packet(Packet::State state)
    : m_data(nullptr), m_state(state), m_priority(LOW)
  {}

  Packet::Packet(const String& string)
    : m_priority(LOW), m_state(OK)
  {
    m_data = new PacketData(string.c_str(), string.size());
    m_data->Attatch();
  }

  Packet::Packet(const c08* data, s32 size)
    : m_priority(LOW), m_state(OK)
  {
    m_data = new PacketData(data, size);
    m_data->Attatch();
  }

  Packet::Packet(const Packet& rhs)
    : m_address(rhs.m_address)
  {
    m_priority = rhs.m_priority;
    m_state = rhs.m_state;
    m_data = rhs.m_data;
    rhs.m_data->Attatch();
  }

  Packet::~Packet()
  {
    s16 refNum = 0;

    if(m_data != nullptr)
      refNum = m_data->Detatch();

    if(refNum == 0)
      delete m_data;
  }

  Packet& Packet::operator=(const Packet& rhs)
  {
    m_address = rhs.m_address;
    m_priority = rhs.m_priority;
    m_state = rhs.m_state;
    m_data = rhs.m_data;
    if(m_data)
      rhs.m_data->Attatch();

    return *this;
  }

  Packet::operator bool() const
  {
    return m_state == OK;
  }

  void Packet::SetPriority(Packet::Priority p)
  {
    m_priority = p;
  }

  Packet::Priority Packet::GetPriority() const
  {
    return m_priority;
  }

  Packet::State Packet::GetState() const
  {
    return m_state;
  }

  s32 Packet::GetSize() const
  {
    if(m_data != nullptr)
      return m_data->dataSize;
    return 0;
  }

  const c08* Packet::GetData() const
  {
    if(m_data != nullptr)
      return m_data->data;
    return nullptr;
  }

  void Packet::AppendData(const c08 *inData, s32 inSize)
  {
    if(m_data != nullptr)
    {
      s32 dataSize = inSize + m_data->dataSize;
      c08* data = new c08[dataSize];

      memcpy(data, m_data->data, m_data->dataSize);
      memcpy(data + m_data->dataSize, inData, inSize);

      delete[] m_data->data;
      m_data->data = data;
      m_data->dataSize = dataSize;
    }
    else
    {
      m_data = new PacketData(inData, inSize);

      if(m_state == EMPTY)
        m_state = OK;
    }
  }

  void Packet::SetAddress(const Address& addr)
  {
    m_address = addr;
  }

  const Address& Packet::GetAddress() const
  {
    return m_address;
  }

  ///////////////////
  // PRIVATE FUNCTIONS
  Packet::PacketData::PacketData(const c08* data_, s32 dataSize_)
    : data(new c08[dataSize_]), dataSize(dataSize_), refCount(0)
  {
    memcpy(data, data_, dataSize_);
  }

  Packet::PacketData::~PacketData()
  {
    delete[] data;
  }

  void Packet::PacketData::Attatch()
  {
    ++refCount;
  }

  s16 Packet::PacketData::Detatch()
  {
    return --refCount;
  } //returns new refCount
}
