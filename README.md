# Omnifuzz
Design fuzzers in object-oriented fashion

## Ideas and Goals

- Fuzzer extension should be easier
```
// Not real code, 
class CollAfl : public Afl {
 public: 
  CollAfl(program_under_test) {
    fuzzer_ = Afl(program_under_test);
    fuzzer_.feedback_mechanism_.kBitMapSize = TotalBasicBlocks(program_under_test);
  }
};

class AflFast : public Afl {
 public:
  AflFast(program_under_test) {
    fuzzer_.scheduler_ = new MarkovChainScheduler();
  }
};
```

- Practice C++ & its code style
- Practice OOP design pattern
- Practice CMake
- Hopefully, serves as a standard?



## Build with cmake

The root CMakeLists.txt already tunes all the llvm options, a simple cmake command can do the trick.
```
## This will build with in a directory named "build"
cmake -Bbuild -H.
```


