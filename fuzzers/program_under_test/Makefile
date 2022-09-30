CC=../../llvm-project/llvm/bin/clang
CXX=../../llvm-project/llvm/bin/clang++
OPT=../../llvm-project/llvm/bin/opt
PASS=omnifuzz-afl


all: coldbrew
coldbrew: coldbrew.c
	$(CC) -o $@.ll -O1 -m64 -emit-llvm -S $<
	$(OPT) -o $@.bc -passes=$(PASS) $@.ll
	$(CC) -o $@.exe $@.bc
clean:
	rm $(wildcard *.bc) $(wildcard *.exe) ${wildcard *.ll}
