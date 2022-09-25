#ifndef OMNIFUZZ_FEEDBACK_FUZZ_SCORE_H
#define OMNIFUZZ_FEEDBACK_FUZZ_SCORE_H

namespace omnifuzz {

enum FuzzScore {
  kNotInteresting=0, 
  kRevisitCoverage,
  kNewVisitCoverage, 
  kMaxScore,
};

} // namespace omnifuzz


#endif  // OMNIFUZZ_FEEDBACK_FUZZ_SCORE_H
