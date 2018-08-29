#include <pandaFramework.h>

#include <queuedConnectionListener.h>
#include <queuedConnectionManager.h>
#include <queuedConnectionReader.h>
#include <connectionWriter.h>
#include <netDatagram.h>

#include <asyncTaskManager.h>
#include <genericAsyncTask.h>

#include <thread>
#include <chrono>
#include <vector>

uint16_t port = 9099;
int backlog = 1000;

std::vector<Connection*> active_connections;
QueuedConnectionManager* c_manager = new QueuedConnectionManager();
QueuedConnectionListener* c_listener = new QueuedConnectionListener(c_manager, 0);
QueuedConnectionReader* c_reader = new QueuedConnectionReader(c_manager, 0);
ConnectionWriter* c_writer = new ConnectionWriter(c_manager, 0);

PT(Connection) tcp_socket;

AsyncTask::DoneStatus listener_poll(GenericAsyncTask* task, void* data) {
  if (c_listener->new_connection_available()) {
	PT(Connection) rendezvous = tcp_socket;
	NetAddress net_addr = rendezvous->get_address();
	PT(Connection) new_conn;

	if (c_listener->get_new_connection(rendezvous, net_addr, new_conn)) {
	  c_listener->get_new_connection(new_conn);
	  active_connections.push_back(new_conn);
	  c_reader->add_connection(new_conn);
	}
  }
  return(AsyncTask::DS_cont);
}

AsyncTask::DoneStatus reader_poll(GenericAsyncTask* task, void* data) {
  NetDatagram result;
  if (c_reader->data_available()) {
	if (c_reader->get_data(result)) {
	  //process_data_fun(datagram)
	}
  }
  return(AsyncTask::DS_cont);
}

int main(int argc, char** argv) {
  PandaFramework framework;
  framework.open_framework(argc, argv);
  
  active_connections.resize(1, NULL);
  tcp_socket = c_manager->open_TCP_server_rendezvous(port, backlog);
  c_listener->add_connection(tcp_socket);

  AsyncTaskManager::get_global_ptr()->add(
	new GenericAsyncTask("Poll connection listener", &listener_poll, (void*)NULL));
  
  AsyncTaskManager::get_global_ptr()->add(
	new GenericAsyncTask("Poll the connection reader", &reader_poll, (void*)NULL));

  //framework.main_loop();
  //AsyncTaskManager::get_global_ptr()->step();
  while(true) {
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  
  return(0);
}
