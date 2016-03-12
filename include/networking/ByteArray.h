#pragma once

namespace Networking
{
  class ByteWriter
  {
  public:
    ByteWriter(u32 size = 2048);
    ~ByteWriter();

    const c08* GetData() const;
    u32  GetSize() const;
    void Reset();

    // WRITE
    void WriteS08(s08 data, u16 bits = 8);
    void WriteU08(u08 data, u16 bits = 8);

    void WriteS16(s16 data, u16 bits = 16);
    void WriteU16(u16 data, u16 bits = 16);

    void WriteS32(s32 data, u16 bits = 32);
    void WriteU32(u32 data, u16 bits = 32);

    void WriteS64(s64 data, u16 bits = 64);
    void WriteU64(u64 data, u16 bits = 64);

    void WriteF32(f32 data, u16 bits = 32);
    void WriteF64(f64 data, u16 bits = 64);

    void WriteBytes(const c08* data, u32 dataSize);

  private:
    c08* m_data;
    c08* m_ptr;

    u32 m_maxSize;
    u32 m_realSize;
  };


  class ByteReader
  {
  public:
    ByteReader(const c08* data, u32 dataSize);

    //READ
    s08 ReadS08(u16 bits = 8);
    u08 ReadU08(u16 bits = 8);

    s16 ReadS16(u16 bits = 16);
    u16 ReadU16(u16 bits = 16);

    s32 ReadS32(u16 bits = 32);
    u32 ReadU32(u16 bits = 32);

    s64 ReadS64(u16 bits = 64);
    u64 ReadU64(u16 bits = 64);

    f32 ReadF32(u16 bits = 32);
    f64 ReadF64(u16 bits = 64);

    c08* ReadBytes(u32 numBytes);

  private:
    const c08* m_data;
    const c08* m_ptr;

    u32 m_size;
  };
}
