#include <cstdlib>
#include <iostream>

#include "../forkserver.h"
int main(int argc, char ** argv) {
  omnifuzz::Forkserver::SendInitResponse(/*response_fd =*/22);
  std::cout << "[SERVER] Init response has sent" << std::endl;
  std::cout << "[SERVER] Start Serving until (the pipe close?)" << std::endl;

  while (1) {
    bool r = omnifuzz::Forkserver::ServeRequest(/*request_fd =*/  21, 
                                    /*response_fd =*/ 22);
    if (!r)
      break;
  }
  return 0;
}
