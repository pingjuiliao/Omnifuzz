# executor
add_library(omnifuzz_executor ${OMNIFUZZ_LIB_TYPE}
  executor.cc
  executor.h
)
add_library(omnifuzz::executor ALIAS
  omnifuzz_executor
)
target_include_directories(omnifuzz_executor
  PUBLIC ${OMNIFUZZ_INCLUDE_DIR}
  )


# fork_client
add_library(omnifuzz_fork_client ${ONMIFUZZ_LIB_TYPE}
  fork_client.cc
  fork_client.h
)
add_library(omnifuzz::fork_client ALIAS
  omnifuzz_fork_client
)
target_include_directories(omnifuzz_fork_client
  PUBLIC ${OMNIFUZZ_INCLUDE_DIR}
)
# forkserver
add_library(omnifuzz_forkserver ${OMNIFUZZ_LIB_TYPE}
  forkserver.cc
  forkserver.h
)
add_library(omnifuzz::forkserver ALIAS
  omnifuzz_forkserver
)
target_include_directories(omnifuzz_forkserver
  PUBLIC ${OMNIFUZZ_INCLUDE_DIR}
)

# forksrv_executor
add_library(omnifuzz_forksrv_executor ${OMNIFUZZ_LIB_TYPE}
  forksrv_executor.cc
  forksrv_executor.h
)
add_library(omnifuzz::forksrv_executor ALIAS
  omnifuzz_forksrv_executor)

target_link_libraries(omnifuzz_forksrv_executor
  PUBLIC omnifuzz::forkserver
         omnifuzz::fork_client
         omnifuzz::executor)
target_include_directories(omnifuzz_forksrv_executor
  PUBLIC ${OMNIFUZZ_INCLUDE_DIR}
)
