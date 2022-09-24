#ifndef FEEDBACK_H
#define FEEDBACK_H
#include <string>
#include <unordered_map>

namespace omnifuzz {

class FeedbackData final {
 public:
  enum Type {
    Int8, 
    Int16, 
    Int32, 
    Int64, 
    Pointer
  };
  // ISSUE: default ctor just for using unorder_map
  FeedbackData() {}
  FeedbackData(std::string name, FeedbackData::Type type) : name_(name), type_(type) {}
  ~FeedbackData() {}
  FeedbackData::Type GetType(void) const { return type_; }
  size_t GetSize(void) const { return size_; }
  std::string GetName(void) const { return name_; }
 private:
  std::string name_;
  size_t size_;
  FeedbackData::Type type_;
};


class Feedback {
 public:
  virtual ~Feedback() = default;
  virtual bool DeemInteresting(void) = 0;
  virtual void RegisterFeedbackData(void) = 0;
  virtual void WriteOnBasicBlock(std::string&) = 0;
  std::unordered_map<std::string, FeedbackData> feedback_data_map_;
};

}

#endif  // FEEDBACK_H
