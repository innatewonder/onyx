#pragma once

class Message
{
public:
  enum MessageType
  {
    MSG_KEY,
    MSG_WINDOW,
    MSG_RESIZE,
    MSG_MOUSE,
    MSG_MOUSE_BUTTON,
    MSG_SCROLL,
  };

  Message(MessageType type);
  ~Message();

  void AddData(void* data, s32 dataSize);

  MessageType GetType();
  void Reset();

  template <typename T>
  T* GetFormatedData()
  {
    T* ret = SCAST(T*, (void*)m_dataPtr);
    m_dataPtr += sizeof(T);
    return ret;
  }

private:
  MessageType m_type;

  char*       m_data;
  char*       m_dataPtr;
  s32         m_dataSize;
};

#define DAT(x) &x, sizeof(x)
