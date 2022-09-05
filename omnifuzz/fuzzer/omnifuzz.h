#include "fuzzer.h"

namespace omnifuzz {


class Omnifuzz : public Fuzzer {
  void run(void) override;  
};

} // namespace omnifuzz
