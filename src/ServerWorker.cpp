//
// Created by cristobal, 2019
//

#include <cmath>
#include <functional>

#include "ServerWorker.h"

ServerWorker::ServerWorker(ServerSubject *serverSubject) :
serverSubject(serverSubject),
max_seq_digits(serverSubject->getMaxSeqNum()),
max_seq_num((int) (std::pow(10, max_seq_digits) - 1))
{

}

void ServerWorker::start() {
  thread = std::make_unique<std::thread>(&ServerWorker::threadFun, this);
}

ServerWorker::~ServerWorker() = default;

void ServerWorker::join() {
  if(thread && thread->joinable()){
    thread->join();
  }
}
