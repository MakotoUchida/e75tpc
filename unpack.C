void unpack(){
  gSystem->Load("libtpc.so");
  auto c = new E75::TPCUnpackerModule();
  c->initialize("output.dat","output.root");
  c->process();
  c->terminate();
}
