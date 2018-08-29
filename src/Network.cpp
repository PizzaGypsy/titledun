#include "Network.hpp"
#include <iostream>

Network::Network() {
  c_manager = new QueuedConnectionManager();
  c_reader = new QueuedConnectionReader(c_manager, 0);
  c_writer = new ConnectionWriter(c_manager, 0);

  connect("localhost", port);
}

Network::~Network() {}

void Network::connect(const std::string& addr, uint16_t port) {
  server_conn = c_manager->open_TCP_client_connection(addr, port, timeout);

  if (server_conn) {
	c_reader->add_connection(server_conn);
	std::cout<<"Connected to "<<addr<<std::endl;
  }
}

void Network::shutdown() {
  if (server_conn) {
	c_manager->close_connection(server_conn);
  }
}
