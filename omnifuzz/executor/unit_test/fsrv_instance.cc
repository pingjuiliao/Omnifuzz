#include <cstdlib>
#include <iostream>
#include "omnifuzz/executor/forkserver.h"

int main(int argc, char ** argv) {

  omnifuzz::Forkserver::SendInitResponse(/*response_fd =*/22);
  std::cout << "[Server] Init response has sent" << std::endl;
  std::cout << "[Server] Start Serving " << std::endl;
  std::cout << "===================================================="
            << std::endl << std::endl << std::endl;

  while (omnifuzz::Forkserver::ReceiveClientRequest(21)) {
    bool r = omnifuzz::Forkserver::ServeRequest(/*request_fd =*/  21, 
                                    /*response_fd =*/ 22);
    if (!r)
      break;
  }

  std::cout << "[Server] Exit gracefully" << std::endl;
  return 0;
}
