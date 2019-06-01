//
// Created by cristobal, 2019
//

#ifndef PROJECT_SERVERWORKER_H
#define PROJECT_SERVERWORKER_H


#include <thread>
#include "ServerSubject.h"

class ServerWorker {
protected:
  ServerSubject *serverSubject;

  int max_seq_digits;
  int max_seq_num;


  std::unique_ptr<std::thread> thread;

  void join();

public:
  explicit ServerWorker(ServerSubject *serverSubject);

  virtual ~ServerWorker();

  virtual void threadFun() = 0;

  void start();
};


#endif //PROJECT_SERVERWORKER_H
