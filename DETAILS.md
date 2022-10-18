
## Analyzer: 
Make static analysis

## Scheduler:
Handles Seed Selection Problem, i.e. prioritize among a set of seeds.


## Mutator
Make decision in generating new inputs. Note that this completes the rest 
Fuzz Configuration Selection (FCS). 

## Executor
Execute the program based on different execution context.
Hardly interacts with other parts.

This also handles the dynamic analysis. However, the feedback data should be interpreted by feedback mechanism.

## Feedback Mechanism

This should 
1. request program analysis (by talking to LLVM)
2. define instrumentation (by talking to LLVM)
3. interpret the result to other parts of the fuzzing.
4. Determine generic algorithm
  Generic algorithm consider whether a given executed testcase is interesting.
  Note that this add the testcase to the search space. However, it does not necessirity means prioritize the testcase. (Then again, some fuzzers combined them together.)
