#pragma once
#include <queuedConnectionManager.h>
#include <queuedConnectionReader.h>
#include <connectionWriter.h>

class Network {
public:
  Network();
  ~Network();
  
  uint16_t port = 9099;
  int timeout = 3000;
  QueuedConnectionManager* c_manager = NULL;
  QueuedConnectionReader* c_reader = NULL;
  ConnectionWriter* c_writer = NULL;
  Connection* server_conn = NULL;
  
  void connect(const std::string& addr, uint16_t port);
  void shutdown();
};
