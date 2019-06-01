//
// Created by cristobal, 2019
//

#ifndef PROJECT_SERVER_H
#define PROJECT_SERVER_H

#include <string>

#include "asio.hpp"

#include "ServerReceiver.h"
#include "ServerSender.h"
#include "ServerSubject.h"
#include "ServerDataGluer.h"

using asio::ip::udp;

class Server : public ServerSubject {
  int max_seq_num;
  int server_port;
  int client_port;

  std::string server_host;

  ServerReceiver serverReceiver;
  ServerSender serverSender;
  ServerDataGluer serverDataGluer;

public:
  Server(int max_seq_num, int server_port, int client_port, const std::string &server_host);

  int getMaxSeqNum() override;

  int getServerPort() override;

  int getClientPort() override;

  std::string getServerHost() override;

  int start();

  void deliverData(const std::string &deliveredData) override;

  void ackSeqNum(int seqnum, udp::endpoint &endpoint) override;

  void stopServer() override;

};


#endif //PROJECT_SERVER_H
