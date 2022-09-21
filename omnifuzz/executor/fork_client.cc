#include "fork_client.h"

namespace {

void MapFDs() {
  
}

} // anonymous namespace


namespace omnifuzz {

ForkClient::ForkClient() {

}

pid_t ForkClient::SendRequest(void) {
  // write(fsrv_ctl_fd, &prev_timed_out, 4)
  // read(fsrv_sd_fd, &child_pid, 4)
  // write(fsrv)
}

} // namespace omnifuzz
