#define THREAD_COUNT 1000

#include <iostream>
#include <asio.hpp>
#include <thread>
#include <spdlog/spdlog.h>
#include "logconsole.h"

using asio::ip::tcp;

struct packet {
	short size;
	char buf[6];
};

#ifndef WIN32
#include <sys/time.h>
#include <chrono>

unsigned int GetTickCount()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::high_resolution_clock::now().time_since_epoch() ).count();
}
#endif

int main( int argc, char* argv[] )
{
  if (argc != 2)
  {
    std::cerr << "Usage: stress_test <host>" << std::endl;
    return 1;
  }

  static int stress_index = 0;
  static std::string host_ip = argv[1];
  spdlog::set_pattern( "[%H:%M:%S.%e %z] [%L] [thread %t] %v" );
  static auto log = spdlog::stdout_logger_mt("StressTest");

  log->notice( "Setting host to %s\n", host_ip.c_str() );

  std::thread io_thread_[THREAD_COUNT];
  for( int idx = 0; idx < THREAD_COUNT; idx++ )
  {
    io_thread_[idx] = std::thread([]() {
      unsigned int starttime = GetTickCount( );
      int thread_id = stress_index++;

      char buf[8];
      packet* pakin;
      packet pak;
      pak.size = 8;
      memcpy(pak.buf, "monkey", 6);

      asio::error_code ec;
      asio::io_service io_service;
      tcp::resolver resolver(io_service);
      auto endpoint_iterator = resolver.resolve(host_ip.c_str(), std::to_string(29000));

      tcp::socket socket(io_service);
      asio::connect(socket, endpoint_iterator, ec);
      if(ec) {
        log->critical("[%d] Failed to connect: [%d] %s\n", thread_id, ec.value(), ec.message().c_str());
        return 1;
      }

      asio::write(socket, asio::buffer((char*)&pak, 8), ec);
      if(ec) {
        log->critical("[%d] Failed to send data: [%d] %s\n", thread_id, ec.value(), ec.message().c_str());
        return 1;
      }

      asio::read(socket, asio::buffer(buf, 8), ec);
      if(ec) {
        log->critical("[%d] Failed to read data: [%d] %s\n", thread_id, ec.value(), ec.message().c_str());
        return 1;
      }
      pakin = (packet*)&buf;

      if (memcmp(&pak, pakin, 8) != 0) {
        log->critical("[%d] Server failed to echo properly\n", thread_id);
        return 1;
      }

      log->notice("[%d] Completed in %d ms\n", thread_id, GetTickCount()-starttime);
      std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
      socket.shutdown(asio::socket_base::shutdown_both, ec);
      return 0;
    });

    std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
  }
  std::this_thread::sleep_for( std::chrono::milliseconds( 400 ) );
  log->notice("Waiting for threads to finish\n");
  for( int idx = 0; idx < THREAD_COUNT; idx++ )
  {
    io_thread_[idx].join();
  }

  stress_index = 0;

  return 0;
}
