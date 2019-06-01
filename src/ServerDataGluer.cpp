//
// Created by cristobal, 2019
//

#include <fstream>
#include <iostream>

#include "ServerDataGluer.h"

const char *OUTPUT_FILENAME = "received_data.txt";

ServerDataGluer::ServerDataGluer(ServerSubject *serverSubject) :
        ServerWorker(serverSubject),
        stopped(false) {
}

void ServerDataGluer::threadFun() {
  std::ofstream file;
  file.open(OUTPUT_FILENAME);

  for (;;) {
    std::unique_lock<std::mutex> ul(mutex_queue);
    cv_queue.wait(ul, [cs = this] {
      std::lock_guard<std::mutex> lg_stop(cs->mutex_stop);
      return cs->stopped || !cs->incoming_data.empty();
    });

    {
      std::lock_guard<std::mutex> lg_stop(mutex_stop);
      if (stopped) {
        break;
      }
    }


    while (!incoming_data.empty()) {
      auto &next_data = incoming_data.front();

      file << next_data;

      incoming_data.pop_front();
    }

  }

  file.close();

}

void ServerDataGluer::deliverData(const std::string &deliveredData) {
  {
    std::lock_guard<std::mutex> lg_queue(mutex_queue);
    incoming_data.push_back(deliveredData);
  }

  cv_queue.notify_all();

}

ServerDataGluer::~ServerDataGluer() {
  join();
  std::cout << "ServerDataGluer finished from joining" << std::endl;

}

void ServerDataGluer::stopGluer() {
  {
    std::lock_guard<std::mutex> lg_stop(mutex_stop);
    stopped = true;
  }

  cv_queue.notify_all();
}
