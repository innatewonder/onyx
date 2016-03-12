#include "CommonPrecompiled.h"
#include "Message.h"

Message::Message(Message::MessageType type)
  : m_type(type), m_data(nullptr), m_dataSize(0), m_dataPtr(nullptr)
{}

Message::~Message()
{
  delete[] m_data;
}

void Message::AddData(void *data, s32 dataSize)
{
  char* newDat = new char[m_dataSize + dataSize];
  memcpy(newDat, m_data, m_dataSize);
  memcpy(newDat + m_dataSize, data, dataSize);
  m_dataSize += dataSize;

  delete[] m_data;
  m_data = newDat;
  m_dataPtr = m_data;
}

Message::MessageType Message::GetType()
{
  return m_type;
}

void Message::Reset()
{
  m_dataPtr = m_data;
}
