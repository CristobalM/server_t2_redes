//
// Created by cristobal, 2019
//

#ifndef PROJECT_SERVERDATAGLUER_H
#define PROJECT_SERVERDATAGLUER_H

#include <mutex>
#include <condition_variable>

#include <deque>

#include "ServerWorker.h"


class ServerDataGluer : public ServerWorker {

  std::mutex mutex_queue;
  std::condition_variable cv_queue;

  std::deque<std::string> incoming_data;

  std::mutex mutex_stop;
  volatile bool stopped;

public:
  void threadFun() override;

  explicit ServerDataGluer(ServerSubject *serverSubject);

  ~ServerDataGluer() override;

  void deliverData(const std::string &deliveredData);

  void stopGluer();
};


#endif //PROJECT_SERVERDATAGLUER_H
