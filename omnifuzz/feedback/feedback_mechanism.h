#ifndef FEEDBACK_H
#define FEEDBACK_H

#include <string>
#include <unordered_map>

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
                    size_t size) : name_(name), type_(type), size_(size) {}
  ~ExecutionVariable() {}
  ExecutionVariable::Type GetType(void) const { return type_; }
  size_t GetSize(void) const { return size_; }
  std::string GetName(void) const { return name_; }
 private:
  std::string name_;
  ExecutionVariable::Type type_;
  size_t size_;
};

/* The key of the feedback fuzzer. */
class FeedbackMechanism {
 public:
  
  virtual ~FeedbackMechanism() = default;

  // Pre-execution functions: talks to the Instrumentator
  virtual void RegisterExecutionVariable(void) = 0;
  virtual void WriteOnBasicBlock(std::string&) = 0;
  
  // In-execution functions: talks to the Excutor
  virtual size_t RegisterFeedbackData(void) = 0;
  std::unordered_map<std::string, ExecutionVariable> exec_var_map_;
  std::unordered_map<std::string, ExecutionVariable> fdbk_data_map_;

  // Post-execution functions: talks to the 
  virtual FuzzScore DeemInteresting(void* data) = 0;
  // virtual bool InterpretFeedback(void) = 0;
};


}

#endif  // FEEDBACK_H
