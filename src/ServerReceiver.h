//
// Created by cristobal, 2019
//

#ifndef PROJECT_SERVERRECEIVER_H
#define PROJECT_SERVERRECEIVER_H

#include "asio.hpp"

#include "ServerWorker.h"
#include "ServerSubject.h"

using asio::ip::udp;

class ServerReceiver : public ServerWorker {
  asio::io_service io_service;
  udp::socket socket;

  int expected_seq_num;

public:
  explicit ServerReceiver(ServerSubject *serverSubject);
  ~ServerReceiver() override;

  void threadFun() override;

  void deliverData(const std::string &data);
  void ackSeqNum(int seqnum, udp::endpoint& endpoint);

};


#endif //PROJECT_SERVERRECEIVER_H
