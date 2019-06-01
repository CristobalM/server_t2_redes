#include <iostream>

#include <memory>

#include "Server.h"
#include "InputProccesor.h"

int main(int argc, char **argv) {
  std::unique_ptr<Server> server;
  try {
    server = initServerFromInput(argc, argv);
  }
  catch (const InvalidServerInput &e) {
    std::cout << e.what() << std::endl;
    return 1;
  }

  std::cout
          << "server_host: " << server->getServerHost() << "\n"
          << "server_port: " << server->getServerPort() << "\n"
          << "client_port: " << server->getClientPort() << "\n"
          << "max_seq_num: " << server->getMaxSeqNum() << std::endl;


  return server->start();
}