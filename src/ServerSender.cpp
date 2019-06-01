//
// Created by cristobal, 2019
//

#include "ServerSender.h"


ServerSender::ServerSender(ServerSubject *serverSubject) :
ServerWorker(serverSubject),
socket(io_service, udp::endpoint(udp::v4(), 0)),
resolver(io_service),
response_port(std::to_string(serverSubject->getClientPort()))
{

}

void ServerSender::threadFun() {

  for(;;){
    std::unique_lock<std::mutex> ul_sender(sender_mutex);

    //std::cout << "Sender on Wait..." << std::endl;
    cv_sender.wait(ul_sender, [cs=this]{
      return cs->stopped || !cs->seq_queue.empty();
    });

    {
      std::lock_guard<std::mutex> lg_stop(mutex_stop);
      if(stopped){
        break;
      }
    }
    //std::cout << "Sender ready to process queue" << std::endl;



    while(!seq_queue.empty()){
      auto current_seqn = seq_queue.front();

      sendAck(current_seqn);


      seq_queue.pop_front();
    }

    //std::cout << "Queue processed" << std::endl;


  }

  disconnect();

}


ServerSender::~ServerSender() {
  join();
  std::cout << "ServerSender finished from joining" << std::endl;
}

void ServerSender::ackSeqnum(int seqnum, udp::endpoint& endpoint) {
  auto addr = endpoint.address();
  auto ip_addr_str = addr.to_string();
  if(ip_addr_str != current_ip_addr){
    current_ip_addr = ip_addr_str;
    std::cout << "current ip addr: " << current_ip_addr << std::endl;
    std::cout << "response port: " << response_port << std::endl;
    udp::resolver::query query(udp::v4(), current_ip_addr, response_port);
    udp::resolver::iterator qiterator(resolver.resolve(query));
    endpoint = udp::endpoint(*qiterator);
    //endpoint = endpoint;
  }

  {
    std::lock_guard<std::mutex> lg_sender(sender_mutex);
    seq_queue.push_back(seqnum);
  }

  cv_sender.notify_all();
}

void ServerSender::stopSender() {
  {
    std::lock_guard<std::mutex> lg_stop(mutex_stop);
    stopped = true;
  }
  cv_sender.notify_all();
}

void ServerSender::sendAck(int seqn) {
  if(current_packet.getSeqNum() != seqn){
    current_packet = Packet(max_seq_digits, seqn, "");
  }
  //else
  auto send_data = current_packet.generatePacketString(true);
  udp::resolver::query query(udp::v4(), current_ip_addr, response_port);
  udp::resolver::iterator qiterator(resolver.resolve(query));
  udp::endpoint d_ep(*qiterator);
  socket.send_to(asio::buffer(send_data), d_ep);
}

void ServerSender::disconnect() {
  std::string send_data;
  udp::resolver::query query(udp::v4(), current_ip_addr, response_port);
  udp::resolver::iterator qiterator(resolver.resolve(query));
  udp::endpoint d_ep(*qiterator);
  socket.send_to(asio::buffer(send_data), d_ep);
}
