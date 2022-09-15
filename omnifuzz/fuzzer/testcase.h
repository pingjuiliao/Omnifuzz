#ifndef TESTCASE_H
#define TESTCASE_H

struct Testcase {
  std::string fname;
  size_t len;
  Testcase(std::string file_name) {
    fname = file_name;
    len = 0;
  }
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

