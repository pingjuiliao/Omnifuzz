


int main(int argc, char** argv) {
  omnifuzz::omnifuzz fuzzer();
  
  fuzzer.config(omnifuzz::Config::AFL);
  fuzzer.run("./main.exe");
  return 0;
}
