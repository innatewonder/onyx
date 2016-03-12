#include "CommonPrecompiled.h"
#include "NetworkEngine.h"

#include "OnyxTransfer.h"
#include "HTTP.h"
#include "UDPBase.h"
#include "TCPBase.h"

#include "ByteArray.h"

//handle closing connections
//handle timeouts
//handle sending queue
//handle arbitrary connect requests

int main(int argc, char** argv)
{
  ArgParser args(argc, argv);
  args.AddCommand("-p", "-port", true);
  args.AddCommand("-s", "-serv");
  args.AddCommand("-sa", "-serverAddr", false, true);
  args.AddCommand("-sp", "-serverPort", false, true);
  args.AddCommand("-f", "-folder", false, true);
  args.AddCommand("-tcp", "-tcp");
  args.AddCommand("-w", "-http");
  args.Parse();

  auto array = Networking::ByteWriter();
  array.WriteS16(-5);
  array.WriteU32(7052);
  array.WriteU16(89);
  array.WriteS32(-8787);
  array.WriteF32(56.329864f);

  auto buff = array.GetData();
  auto buffSize = array.GetSize();
  LOG("buffsize " << buffSize)
  auto readArray = Networking::ByteReader(buff, buffSize);
  
  s16 v1 = readArray.ReadS16();
  u32 v2 = readArray.ReadU32();
  u16 v3 = readArray.ReadU16();
  s32 v4 = readArray.ReadS32();
  f32 v5 = readArray.ReadF32();
  
  LOG(v1 << " " << v2 << " " << v3 << " " << v4 << " " << v5);
  
  return 0;

  int port = 0;
  if(args.Has("-p"))
  {
    port = args.Get("-port")->val.iVal;
  }

  auto engine = new Networking::NetworkEngine();
  engine->Initialize(args);

  auto serverAddr = Networking::Address("localhost", "10294");
  if(args.Has("-sa") && args.Has("-sp"))
  {
    LOG("Got server from command line");
    serverAddr.SetAddress(args.Get("-sa")->val.sVal, args.Get("-sp")->val.sVal);
  }

  if(args.Has("-folder"))
  {
    LOG("Adding onyx reader with given folder");
    String folder = args.Get("-folder")->val.sVal;
    auto reader = new Filesystem::FilesystemReader();
    reader->OpenFolder(folder, true);
    reader->OpenFile("10294.onyx");

    if(args.Has("-serv"))
    {
      engine->AddProtocol(
        new Networking::OnyxServer()
      );

      while(1)
      {
        engine->Update(0.016f);
        SLEEP_MILLI(5);
      }
    }
    else
    {
      engine->AddProtocol(
        new Networking::OnyxClient(serverAddr, reader->GetFolder(folder))
      );
      engine->Update(0.f);
    }
  }
  else if(args.Has("-w"))
  {
    LOG("serving web on " << port);
    engine->AddProtocol(
      new Networking::HTTP(port)
    );
  }
  else if(args.Has("-serv"))
  {
    LOG("serving on port " << port);
    if(args.Has("-tcp"))
    {
      LOG("TCP");
      engine->AddProtocol(
        new Networking::TCPBase(port)
      ); 
    }
    else
    {
      engine->AddProtocol(
        new Networking::UDPBase(port)
      );
    }
  }
  else
  {
    LOG("Connecting to server at: " << serverAddr)
    if(args.Has("-tcp"))
    {
      LOG("TCP");
      engine->AddProtocol(
        new Networking::TCPBase(serverAddr, port)
      );
    }
    else
    {
      engine->AddProtocol(
        new Networking::UDPBase(serverAddr, port)
      );
    }
  }

  delete engine;

  return 0;
}

