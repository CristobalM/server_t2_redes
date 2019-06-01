//
// Created by cristobal, 2019
//

#ifndef PROJECT_SERVERSUBJECT_H
#define PROJECT_SERVERSUBJECT_H

#include <string>
#include "asio.hpp"

using asio::ip::udp;

class ServerSubject{
public:
  virtual int getMaxSeqNum() = 0;
  virtual int getClientPort() = 0;
  virtual int getServerPort() = 0;
  virtual std::string getServerHost() = 0;

  virtual void deliverData(const std::string &deliveredData) = 0;

  virtual void ackSeqNum(int seqnum, udp::endpoint& endpoint) = 0;

  virtual void stopServer() = 0;
};

#endif //PROJECT_SERVERSUBJECT_H
