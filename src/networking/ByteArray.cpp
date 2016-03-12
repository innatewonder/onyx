#include "CommonPrecompiled.h"
#include "ByteArray.h"

namespace Networking
{
  ByteWriter::ByteWriter(u32 size)
    : m_maxSize(size), m_data(new c08[size]), m_ptr(m_data)
    , m_realSize(0)
  {
  }

  ByteWriter::~ByteWriter()
  {
    delete[] m_data;
  }

  const c08* ByteWriter::GetData() const
  {
    return m_data;
  }

  u32 ByteWriter::GetSize() const
  {
    return m_realSize;
  }

  void ByteWriter::Reset()
  {
    m_ptr = m_data;
  }

  //WRITE
  void ByteWriter::WriteS08(s08 data, u16 bits)
  {
    u32 dataSize = sizeof(data);
    assertion(dataSize + m_realSize <= m_maxSize);
    memcpy(m_ptr, &data, dataSize);
    m_ptr += dataSize;
    m_realSize += dataSize;
  }
  void ByteWriter::WriteU08(u08 data, u16 bits)
  {
    u32 dataSize = sizeof(data);
    assertion(dataSize + m_realSize <= m_maxSize);
    memcpy(m_ptr, &data, dataSize);
    m_ptr += dataSize;
    m_realSize += dataSize;
  }

  void ByteWriter::WriteS16(s16 data, u16 bits)
  {
    u32 dataSize = sizeof(data);
    assertion(dataSize + m_realSize <= m_maxSize);
    memcpy(m_ptr, &data, dataSize);
    m_ptr += dataSize;
    m_realSize += dataSize;
  }
  void ByteWriter::WriteU16(u16 data, u16 bits)
  {
    u32 dataSize = sizeof(data);
    assertion(dataSize + m_realSize <= m_maxSize);
    memcpy(m_ptr, &data, dataSize);
    m_ptr += dataSize;
    m_realSize += dataSize;
  }

  void ByteWriter::WriteS32(s32 data, u16 bits)
  {
    u32 dataSize = sizeof(data);
    assertion(dataSize + m_realSize <= m_maxSize);
    memcpy(m_ptr, &data, dataSize);
    m_ptr += dataSize;
    m_realSize += dataSize;
  }
  void ByteWriter::WriteU32(u32 data, u16 bits)
  {
    u32 dataSize = sizeof(data);
    assertion(dataSize + m_realSize <= m_maxSize);
    memcpy(m_ptr, &data, dataSize);
    m_ptr += dataSize;
    m_realSize += dataSize;
  }

  void ByteWriter::WriteS64(s64 data, u16 bits)
  {
    u32 dataSize = sizeof(data);
    assertion(dataSize + m_realSize <= m_maxSize);
    memcpy(m_ptr, &data, dataSize);
    m_ptr += dataSize;
    m_realSize += dataSize;
  }
  void ByteWriter::WriteU64(u64 data, u16 bits)
  {
    u32 dataSize = sizeof(data);
    assertion(dataSize + m_realSize <= m_maxSize);
    memcpy(m_ptr, &data, dataSize);
    m_ptr += dataSize;
    m_realSize += dataSize;
  }

  void ByteWriter::WriteF32(f32 data, u16 bits)
  {
    u32 dataSize = sizeof(data);
    assertion(dataSize + m_realSize <= m_maxSize);
    memcpy(m_ptr, &data, dataSize);
    m_ptr += dataSize;
    m_realSize += dataSize;
  }
  
  void ByteWriter::WriteF64(f64 data, u16 bits)
  {
    u32 dataSize = sizeof(data);
    assertion(dataSize + m_realSize <= m_maxSize);
    memcpy(m_ptr, &data, dataSize);
    m_ptr += dataSize;
    m_realSize += dataSize;
  }

  void ByteWriter::WriteBytes(const c08* data, u32 dataSize)
  {
    assertion(dataSize + m_realSize <= m_maxSize);
    memcpy(m_ptr, data, dataSize);
    m_ptr += dataSize;
    m_realSize += dataSize;
  }

  //READ
  ByteReader::ByteReader(const c08* data, u32 dataSize)
    : m_data(data), m_ptr(m_data), m_size(dataSize)
  {
  }

  s08 ByteReader::ReadS08(u16 bits)
  {
    s08 out = 0;
    u32 outSize = sizeof(out);
    assertion(m_ptr + outSize <= m_data + m_size);
    memcpy(&out, m_ptr, outSize);
    m_ptr += outSize;
    return out;
  }
  u08 ByteReader::ReadU08(u16 bits)
  {
    u08 out = 0;
    u32 outSize = sizeof(out);
    assertion(m_ptr + outSize <= m_data + m_size);
    memcpy(&out, m_ptr, outSize);
    m_ptr += outSize;
    return out;
  }

  s16 ByteReader::ReadS16(u16 bits)
  {
    s16 out = 0;
    u32 outSize = sizeof(out);
    assertion(m_ptr + outSize <= m_data + m_size);
    memcpy(&out, m_ptr, outSize);
    m_ptr += outSize;
    return out;
  }
  u16 ByteReader::ReadU16(u16 bits)
  {
    u16 out = 0;
    u32 outSize = sizeof(out);
    assertion(m_ptr + outSize <= m_data + m_size);
    memcpy(&out, m_ptr, outSize);
    m_ptr += outSize;
    return out;
  }

  s32 ByteReader::ReadS32(u16 bits)
  {
    s32 out = 0;
    u32 outSize = sizeof(out);
    assertion(m_ptr + outSize <= m_data + m_size);
    memcpy(&out, m_ptr, outSize);
    m_ptr += outSize;
    return out;
  }
  u32 ByteReader::ReadU32(u16 bits)
  {
    u32 out = 0;
    u32 outSize = sizeof(out);
    assertion(m_ptr + outSize <= m_data + m_size);
    memcpy(&out, m_ptr, outSize);
    m_ptr += outSize;
    return out;
  }

  s64 ByteReader::ReadS64(u16 bits)
  {
    s64 out = 0;
    u32 outSize = sizeof(out);
    assertion(m_ptr + outSize <= m_data + m_size);
    memcpy(&out, m_ptr, outSize);
    m_ptr += outSize;
    return out;
  }
  u64 ByteReader::ReadU64(u16 bits)
  {
    u64 out = 0;
    u32 outSize = sizeof(out);
    assertion(m_ptr + outSize <= m_data + m_size);
    memcpy(&out, m_ptr, outSize);
    m_ptr += outSize;
    return out;
  }

  f32 ByteReader::ReadF32(u16 bits)
  {
    f32 out = 0.f;
    u32 outSize = sizeof(out);
    assertion(m_ptr + outSize <= m_data + m_size);
    memcpy(&out, m_ptr, outSize);
    m_ptr += outSize;
    return out;
  }
  
  f64 ByteReader::ReadF64(u16 bits)
  {
    f64 out = 0.0;
    u32 outSize = sizeof(out);
    assertion(m_ptr + outSize <= m_data + m_size);
    memcpy(&out, m_ptr, outSize);
    m_ptr += outSize;
    return out;
  }

  c08* ByteReader::ReadBytes(u32 numBytes)
  {
    assertion(m_ptr + numBytes <= m_data + m_size);
    c08* out = new c08[numBytes];
    memcpy(out, m_ptr, numBytes);
    m_ptr += numBytes;
    return out;
  }
}
