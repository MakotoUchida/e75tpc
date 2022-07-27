CXX       := g++
ROOTCLING := rootcling

CXXFLAGS := -pipe -m64 -Wshadow -Wall -W  -fPIC  -std=c++17 -O2 

LIBNAME := tpc
LIB     := libtpc.so
RMF     := $(basename $(LIB))_rdict.rootmap
PCM     := $(basename $(LIB))_rdict.pcm

ROOT_INCLUDE := $(ROOTSYS)/include
ROOT_LIB     := $(ROOTSYS)/lib


HEADERS        := TPCRawHit.h TPCUnpackerModule.h linkdef.h
SRCFILE1        := TPCRawHit.cc 
SRCFILE_OBJECT1 := $(patsubst %.cc,%.o,$(SRCFILE1))
SRCFILE2        := TPCUnpackerModule.cc
SRCFILE_OBJECT2 := $(patsubst %.cc,%.o,$(SRCFILE2))

RFLX        := $(basename $(LIB))_rdict.cc
RFLX_OBJECT := $(patsubst %.cc,%.o,$(RFLX))

all: $(LIB)

$(RFLX) $(RMF) $(PCM): $(HEADERS)
	echo "Generating reflex source, dictionary and rootmap files"
	$(ROOTCLING) -f $(RFLX) -rml $(notdir $(LIB)) -rmf $(RMF) -I$(ROOT_INCLUDE) $^
$(SRCFILE_OBJECT1):$(SRCFILE1)
	echo "Building source object 1"
	$(CXX) -I$(ROOT_INCLUDE) $(CXXFLAGS) -o $@ -c $<
$(SRCFILE_OBJECT2):$(SRCFILE2)
	echo "Building source object 2"
	$(CXX) -I$(ROOT_INCLUDE) $(CXXFLAGS) -o $@ -c $<

$(RFLX_OBJECT): $(RFLX)
	echo "Building reflex object"
	$(CXX) -I$(ROOT_INCLUDE) $(CXXFLAGS) -o $@ -c $<

$(LIB): $(RFLX_OBJECT) $(SRCFILE_OBJECT1) $(SRCFILE_OBJECT2)
	echo "Building the library"
	$(CXX) $(CXXFLAGS)  -shared -o $@ $^  -L$(ROOT_LIB) -lCore -lImt -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lROOTVecOps -lTree -lTreePlayer -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc -lROOTDataFrame -pthread -lm -ldl -rdynamic

#$(shell root-config --libs)

.PHONY: clean

clean:
	echo "Removing files"
	$(RM) -f $(SRCFILE_OBJECT) $(RFLX_OBJECT) $(RFLX) $(PCM) $(RMF) $(LIB) 
