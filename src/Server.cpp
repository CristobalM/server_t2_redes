//
// Created by cristobal, 2019
//

#include "Server.h"

Server::Server(int max_seq_num, int server_port, int client_port, const std::string &server_host) :
max_seq_num(max_seq_num),
server_port(server_port),
client_port(client_port),
server_host(server_host),
serverReceiver(this),
serverSender(this),
serverDataGluer(this)
{
}

void Server::stopServer() {
  serverSender.stopSender();
  serverDataGluer.stopGluer();
}

int Server::getMaxSeqNum() {
  return max_seq_num;
}

int Server::getServerPort() {
  return server_port;
}

int Server::getClientPort() {
  return client_port;
}

std::string Server::getServerHost() {
  return server_host;
}

int Server::start() {
  serverReceiver.start();
  serverSender.start();
  serverDataGluer.start();


  return 0;
}

void Server::deliverData(const std::string &deliveredData) {
  serverDataGluer.deliverData(deliveredData);

}

void Server::ackSeqNum(int seqnum, udp::endpoint& endpoint) {
  serverSender.ackSeqnum(seqnum, endpoint);
}
