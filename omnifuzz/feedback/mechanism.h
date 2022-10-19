#ifndef OMNIFUZZ_FEEDBACK_MECHANISM_H
#define OMNIFUZZ_FEEDBACK_MECHANISM_H

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "omnifuzz/feedback/fuzz_score.h"

namespace omnifuzz {

class ExecutionVariable final {
 public:
  enum Type {
    Int8, 
    Int16, 
    Int32, 
    Int64, 
    Pointer
  };
  // ISSUE: default ctor just for using unorder_map
  ExecutionVariable() {}
  ExecutionVariable(std::string name, ExecutionVariable::Type type) : name_(name), type_(type) {}
  ExecutionVariable(std::string name, 
                    ExecutionVariable::Type type,
                    uint32_t offset, 
                    size_t size) : name_(name), type_(type), 
                    offset_(offset), size_(size) {}
  ~ExecutionVariable() {}
  ExecutionVariable::Type GetType(void) const { return type_; }
  size_t GetSize(void) const { return size_; }
  uint32_t GetOffset(void) const { return offset_; }
  std::string GetName(void) const { return name_; }
 private:
  std::string name_;
  ExecutionVariable::Type type_;
  uint32_t offset_;
  size_t size_;
};

/* The key of the feedback fuzzer. */
class FeedbackMechanism {
 public:
  
  virtual ~FeedbackMechanism() = default;

  virtual void AddMechanism(FeedbackMechanism*) {};

  // Pre-execution functions: talks to the Instrumentator
  virtual void RegisterExecutionVariable(void) = 0;
  virtual void WriteOnBasicBlock(std::string&) = 0;
  
  // In-execution functions: talks to the Excutor
  virtual size_t RegisterFeedbackData(void) = 0;
  virtual void ResetFeedbackDataState(void*) = 0;
  std::unordered_map<std::string, ExecutionVariable> exec_var_map_;
  std::unordered_map<std::string, ExecutionVariable> fdbk_data_map_;

  // Post-execution functions: talks to the Fuzzer 
  virtual FuzzScore DeemInteresting(void*) = 0;
  virtual bool DeemUniqueCrash(void*) = 0;
  virtual void InterpretFeedback(void*,
      std::unordered_map<std::string, std::pair<void*, size_t>>*) = 0;
};

class FeedbackMechanismComposite : public FeedbackMechanism {
 public:
  virtual ~FeedbackMechanismComposite();
  virtual void AddMechanism(FeedbackMechanism*) override;
  virtual void RegisterExecutionVariable(void) override;
  virtual size_t RegisterFeedbackData(void) override;
  virtual void ResetFeedbackDataState(void*) override;
  virtual void WriteOnBasicBlock(std::string&) override;
 private:
  std::vector<FeedbackMechanism*> mechanisms_;
};

}

#endif  // OMNIFUZZ_FEEDBACK_MECHANISM_H
