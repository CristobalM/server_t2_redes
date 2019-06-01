//
// Created by cristobal, 2019
//

#include <string>
#include <iostream>

#include "ServerReceiver.h"
#include "Packet.h"

ServerReceiver::ServerReceiver(ServerSubject *serverSubject) :
        ServerWorker(serverSubject),
        socket(io_service, udp::endpoint(udp::v4(), (unsigned short) serverSubject->getServerPort())),
        expected_seq_num(0) {
}

const int BUFFER_SZ = 65500;

void ServerReceiver::threadFun() {

  asio::streambuf packet_buffer;
  size_t buffer_sz = BUFFER_SZ;
  auto tmp_buffer = std::make_unique<char[]>(buffer_sz);
  for (;;) {
    udp::endpoint endpoint;
    std::fill(tmp_buffer.get(), tmp_buffer.get() + buffer_sz, 0);
    size_t recv_length = socket.receive_from(asio::buffer(tmp_buffer.get(), buffer_sz), endpoint);
    if (recv_length == 0) {
      break;
    }

    std::string input_raw_packet(tmp_buffer.get());
    if (Packet::checkPacketIntegrity(input_raw_packet, max_seq_digits, false)) {
      auto packet_seq_num = Packet::extractSeqNum(input_raw_packet, max_seq_digits);

      if (packet_seq_num != expected_seq_num) {
        //discard
        std::cout << "DISCARDED PACKET, expected: " << expected_seq_num << ". Given: " << packet_seq_num << std::endl;
        continue;
      }
      //else
      auto extractedData = Packet::extractData(input_raw_packet, max_seq_digits);
      std::cout << "Sending ACK #" << packet_seq_num << std::endl;
      deliverData(extractedData);
      ackSeqNum(packet_seq_num, endpoint);
      expected_seq_num = (expected_seq_num + 1) % (max_seq_num + 1);


    } else {
      std::cout << "BAD CHECKSUM!" << std::endl;
    }
  }

  std::cout << "Connection Terminated by empty packet" << std::endl;
  serverSubject->stopServer();
}

void ServerReceiver::deliverData(const std::string &data) {
  serverSubject->deliverData(data);
}

void ServerReceiver::ackSeqNum(int seqnum, udp::endpoint &endpoint) {
  serverSubject->ackSeqNum(seqnum, endpoint);
}


ServerReceiver::~ServerReceiver() {
  join();
}
