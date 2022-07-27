void disp(){
  gSystem->Load("libtpc.so");
  TFile* f = new TFile("output.root");
  TTree* t = f->Get<TTree>("tree");
  auto tpc = new E75::TPCRawHit();
  auto br = t->GetBranch("TPCRawHit");
  br->SetAddress(&tpc);
  t->GetEntry(0);
  TCanvas *c = new TCanvas();
  std::vector<int> y = tpc->getChData(3);
  const int n  = y.size();
  std::vector<int> x(n);
  for(int j = 0;j<n;++j){
    x[j] = j;
  }
  TGraph *gr = new TGraph(n,&x[0], &y[0]);
  gr->Draw("AP");
  gr->SetMaximum(2000);
  //  c->SaveAs("test.png");
}
