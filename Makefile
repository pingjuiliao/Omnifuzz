LLVM_ROOT=./omnifuzz/llvm-project-llvmorg-14.0.6/
LLVM_BUILD_ROOT=./omnifuzz/build/
LLVM_BIN_PATH=${LLVM_BUILD_ROOT}/llvm-project-llvmorg-14.0.6/llvm/bin

CC=${LLVM_BIN_PATH}/clang
CXX=${LLVM_BIN_PATH}/clang++
OPT=$(LLVM_BIN_PATH)/opt
LLC=${LLVM_BIN_PATH}/llc

PASSES=omnifuzz-debug

all: a
debug: a
	$(OPT) -passes=${PASSES} -disable-output a.ll -debug-pass-manager
a: a.c
	$(CC) -o a.ll -O1 -m64 -emit-llvm -S $< 
	$(OPT) -o a.bc -passes=${PASSES} a.ll
	$(CC) -o a.exe a.bc
clean:
	rm $(wildcard *.exe) $(wildcard *.bc) $(wildcard *.ll)
