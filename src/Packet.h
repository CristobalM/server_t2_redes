#ifndef PROJECT_PACKET_H
#define PROJECT_PACKET_H


#include <memory>
#include <sstream>
#include <algorithm>
#include <utility>

#include <string>
#include <iostream>
#include <chrono>

#include <mutex>

#include "digestpp-master/digestpp.hpp"


class Packet {
  int seqnum_digits;
  int seqnum;
  std::string packet_data;

  std::string packetString;
  bool cached;

  bool retransmitted;// = false;
  std::mutex retransmitted_mutex;

  std::chrono::milliseconds starting_time;

  bool packet_sent = false;


public:

  Packet() = default;


  Packet(int seqnum_digits, int seqnum, const std::string &packet_data) :
          seqnum_digits(seqnum_digits),
          seqnum(seqnum),
          packet_data(packet_data),
          cached(false),
          retransmitted(false),
          starting_time(0) {
  }

  Packet(const Packet &other) {
    seqnum_digits = other.seqnum_digits;
    seqnum = other.seqnum;
    packet_data = other.packet_data;
    packetString = other.packetString;
    cached = other.cached;
    retransmitted = other.retransmitted;
    starting_time = other.starting_time;
  }

  Packet &operator=(Packet other) {
    std::swap(seqnum_digits, other.seqnum_digits);
    std::swap(seqnum, other.seqnum);
    std::swap(packet_data, other.packet_data);
    std::swap(packetString, other.packetString);
    std::swap(cached, other.cached);
    std::swap(retransmitted, other.retransmitted);
    std::swap(starting_time, other.starting_time);
    return *this;
  }

  static std::string getHashFromString(const std::string &inputString) {
    return digestpp::md5().absorb(inputString).hexdigest();
  }

  static int getNumberOfDigits(int number) {
    if (number == 0) {
      return 1;
    }
    int result = 0;
    while (number > 0) {
      number /= 10;
      result++;
    }
    return result;
  }

  static bool checkPacketIntegrity(const std::string &raw_full_packet, int seqnum_digits, bool checkACK = true) {
    auto endOfMD5Str = seqnum_digits + 32;
    auto md5String = raw_full_packet.substr(seqnum_digits, 32);

    std::string dataString, dataStringHashed;

    if (checkACK) {
      dataString = raw_full_packet.substr(0, seqnum_digits);
    } else {
      dataString = raw_full_packet.substr(endOfMD5Str, raw_full_packet.length());
    }
    dataStringHashed = getHashFromString(dataString);

    return md5String == dataStringHashed;
  }

  static int extractSeqNum(const std::string &raw_full_packet, int seqnum_digits) {
    auto str_val = raw_full_packet.substr(0, seqnum_digits);
    std::string real_str_val;
    std::istringstream iss(str_val);
    int number;
    iss >> number;
    return number;
  }

  static std::string extractData(const std::string &raw_full_packet, int seqnum_digits) {
    return raw_full_packet.substr(seqnum_digits + 32);
  }


  int getSeqNum() {
    return seqnum;
  }

  std::string generatePacketString(bool ack_mode = false) {
    if (cached) {
      return packetString;
    }
    std::stringstream ss;
    char md5buf[32];

    int extra_zeros = seqnum_digits - getNumberOfDigits(seqnum);
    for (int i = 0; i < extra_zeros; i++) {
      ss << "0";
    }
    ss << std::to_string(seqnum);
    auto seqnum_str = ss.str();
    if (ack_mode) {
      ss << getHashFromString(seqnum_str);
    } else {
      ss << getHashFromString(packet_data);
      ss << packet_data;
    }


    packetString = ss.str();
    cached = true;

    return packetString;
  }

  void setRetransmitted() {
    std::lock_guard<std::mutex> lg(retransmitted_mutex);
    retransmitted = true;
  }

  bool getRetransmitted() {
    std::lock_guard<std::mutex> lg(retransmitted_mutex);
    return retransmitted;
  }

  void startTimeoutCount() {
    starting_time = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
    );
  }

  std::chrono::milliseconds getStartingTime() {
    return starting_time;
  }

  void setSent() {
    packet_sent = true;
  }

};


#endif //PROJECT_PACKET_H
