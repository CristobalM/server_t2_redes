//
// Created by cristobal, 2019
//

#ifndef PROJECT_SERVERSENDER_H
#define PROJECT_SERVERSENDER_H

#include <mutex>
#include <condition_variable>
#include <deque>
#include <string>

#include "asio.hpp"

#include "ServerWorker.h"
#include "ServerSubject.h"
#include "Packet.h"

using asio::ip::udp;


class ServerSender : public ServerWorker {
  std::mutex sender_mutex;
  std::condition_variable cv_sender;

  std::mutex mutex_stop;
  bool stopped;

  std::deque<int> seq_queue;


  // NETWORK

  std::string response_port;

  asio::io_service io_service;
  udp::socket socket;

  udp::resolver resolver;
  udp::endpoint endpoint;

  std::string current_ip_addr;

  Packet current_packet;


public:
  explicit ServerSender(ServerSubject *serverSubject);

  ~ServerSender();

  void threadFun() override;

  void ackSeqnum(int seqnum, udp::endpoint &endpoint);

  void stopSender();

  void sendAck(int seqn);

  void disconnect();
};


#endif //PROJECT_SERVERSENDER_H
