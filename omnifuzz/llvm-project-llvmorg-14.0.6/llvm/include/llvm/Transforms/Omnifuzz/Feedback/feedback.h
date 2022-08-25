#ifndef FEEDBACK_H
#define FEEDBACK_H
#include <string>
#include <unordered_map>

namespace omnifuzz {

class FeedbackData final {
 public:
  // ISSUE: default ctor just for using unorder_map
  FeedbackData() {}
  FeedbackData(std::string name, size_t size) : name_(name), size_(size) {}
  ~FeedbackData() {}
  size_t GetSize(void) const { return size_; }
  std::string GetName(void) const { return name_; }
 private:
  std::string name_;
  size_t size_;
};


class Feedback {
 public:
  virtual ~Feedback() = default;
  virtual void RegisterFeedbackData(void) = 0;
  virtual void WriteOnBasicBlock(std::string&) const = 0;
  std::unordered_map<std::string, FeedbackData> feedback_data_map_;
};

}

#endif  // FEEDBACK_H
