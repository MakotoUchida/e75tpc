import numpy as np
import matplotlib.pyplot as plt
import ROOT
ROOT.gSystem.Load('libtpc.so')

f = ROOT.TFile('output.root')
t = f.Get('tree')
t.GetEntry(0)
tpc = t.TPCRawHit
vec = []
for i in range(16):
    v = np.array(tpc.getChData(i))
    vec.append(v) 
fig = plt.figure()
ax = fig.add_subplot()
for i in range(16):
    ax.plot(vec[i],'-')
plt.ylim(0,2000)
fig.savefig('test.png')    
