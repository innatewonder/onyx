#pragma once

//Networking Includes
#if PLATFORM == PLAT_WINDOWS
///shitty windows has to be in commonPrecompiled
#elif PLATFORM == PLAT_MAC
#elif PLATFORM == PLAT_UNIX
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <fcntl.h>
  #include <netdb.h>
  #include <arpa/inet.h>
#endif
