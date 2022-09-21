
namespace omnifuzz {
class Fuzzer {
 public:
  virtual ~Fuzzer() {}
  virtual void Run(void);
 protected:
  Instrumentator* instrumentator_;
  MutatorComposite* mutator_;
  Executor* executor_;
};

} // namespace omnifuzz
