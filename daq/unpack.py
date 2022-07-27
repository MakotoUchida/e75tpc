import ROOT
ROOT.gSystem.Load('libtpc.so')
c = ROOT.E75.TPCUnpackerModule()
c.initialize('output.dat','output.root')
c.process()
c.terminate()




