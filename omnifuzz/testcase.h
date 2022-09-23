#ifndef TESTCASE_H
#define TESTCASE_H

#include <cstdint>
#include <feedback

struct Testcase {

  // Testcase Information regardless what the fuzzer is.
  char* file_name;      // file name
  uint32_t size;        // Input Length
  uint8_t trimmed;      // Was trimmed
  uint8_t fuzzed;       // Was Fuzzed
  uint8_t dummy_a;      // Still thinking 
  uint8_t dummy_b;      // Still thinking
  uint64_t generation;  // Depth in generic algorithm
  uint64_t exec_us;     // Execution time (us)
                        
  // Post-execution information is here.
  Feedback *feedback;
};

/*
 * struct queue_entry {
 *   u8* fname;                    // File name for the test case     
 *   u32 len;                      // Input length                    
 *   u8 cal_failed,                // Calibration failed?             
 *      trim_done,                 // Trimmed?                        
 *      was_fuzzed,                // Had any fuzzing done yet?       
 *      passed_det,                // Deterministic stages passed?    
 *      has_new_cov,               // Triggers new coverage?          
 *      var_behavior,              // Variable behavior? 
 *      favored,                   // Currently favored?  
 *      fs_redundant;              // Marked as redundant in the fs?  
 *   u32 bitmap_size,              // Number of bits set in bitmap 
 *       exec_cksum;               // Checksum of the execution trace 
 *   u64 exec_us,                  // Execution time (us)   
 *       handicap,                 // Number of queue cycles behind 
 *       depth;                    // Path depth 
 *   u8* trace_mini;               // Trace bytes, if kept 
 *   u32 tc_ref;                   // Trace bytes, ref count 
 *
 *   struct queue_entry *next,     // Next element, if any
 *                      *next_100; // 100 elements ahead
 * }; */ 



#endif  // TESTCASE_H

