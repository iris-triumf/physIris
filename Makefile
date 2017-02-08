# Makefile
BASEDIR	      = $(shell pwd)/..
INSTALLDIR    = $(BASEDIR)/physIris
INCLUDEDIR    = $(INSTALLDIR)/include
LIBDIR        = $(INSTALLDIR)/lib
BINARYDIR     = $(INSTALLDIR)/bin
SOURCEDIR     = $(INSTALLDIR)/src
OBJECTDIR     = $(INSTALLDIR)/obj
TREEIRIS = INSTALLDIR

HEADER = -I$(INCLUDEDIR) 

CXX = g++
LD = g++
# ROOT library
ROOTLIBS  = $(shell $(ROOTSYS)/bin/root-config --libs)  -lXMLParser -lThread -Wl,-rpath,$(ROOTSYS)/lib
ROOTGLIBS = $(shell $(ROOTSYS)/bin/root-config --glibs) -lXMLParser -lThread -Wl,-rpath,$(ROOTSYS)/lib

# ifdef ROOTSYS
# CXXFLAGS += -DHAVE_LIBNETDIRECTORY
# OBJS     += $(ROOTANA)/libNetDirectory/netDirectoryServer.o
# 
# NETDIRLIB = $(ROOTANA)/libNetDirectory/libNetDirectory.a
# endif

# ROOT analyzer
#ROOTANA = $(HOME)/packages/rootana

CXXFLAGS += -g -O -Wall -Wuninitialized -I./  -I$(INCLUDEDIR) -I$(ROOTSYS)/include # -I$(ROOTANA) -I$(ROOTANA)/include

ROOTCFLAGS    = $(shell root-config --cflags)
CXXFLAGS      += -g -Wall -ansi -Df2cFortran -fPIC $(ROOTCFLAGS) 

ANAOBJECTS  =  $(OBJECTDIR)/HandlePHYSICS.o $(OBJECTDIR)/eloss.o $(LIBDIR)/libTEvent.so $(OBJECTDIR)/TEventDict.o $(OBJECTDIR)/nucleus.o $(OBJECTDIR)/runDepPar.o $(OBJECTDIR)/CalibPHYSICS.o $(OBJECTDIR)/Graphsdedx.o $(OBJECTDIR)/geometry.o 

ifdef MIDASSYS
CXXFLAGS += -DHAVE_MIDAS -DOS_LINUX -Dextname -I$(MIDASSYS)/include
MIDASLIBS = $(MIDASSYS)/linux/lib/libmidas.a
endif

SOFLAGS       = -g -shared
LDFLAGS	      = -O2	

all: $(BINARYDIR)/physIris $(LIBDIR)/libTEvent.so

$(BINARYDIR)/physIris: $(ANAOBJECTS) $(OBJECTDIR)/physIris.o $(MIDASLIBS) $(ROOTANA)/lib/librootana.a 
	$(CXX) -o $@ $(CXXFLAGS) $^ $(MIDASLIBS) $(NETDIRLIB) $(ROOTGLIBS) -lm -lz -lutil -lnsl -lpthread -lrt

$(LIBDIR)/libTEvent.so:	$(OBJECTDIR)/TEvent.o $(OBJECTDIR)/IDet.o $(OBJECTDIR)/ITdc.o $(OBJECTDIR)/IScaler.o $(OBJECTDIR)/TEventDict.o
	$(LD) $(SOFLAGS) $(LDFLAGS) $^ -o $@
	@echo "$@ done"

$(OBJECTDIR)/physIris.o: $(SOURCEDIR)/main.cxx 
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

$(OBJECTDIR)/HandlePHYSICS.o: $(SOURCEDIR)/HandlePHYSICS.cxx 
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/eloss.o: $(SOURCEDIR)/eloss.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/nucleus.o: $(SOURCEDIR)/nucleus.cxx 
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/runDepPar.o: $(SOURCEDIR)/runDepPar.cxx 
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/geometry.o: $(SOURCEDIR)/geometry.cxx 
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/Graphsdedx.o: $(SOURCEDIR)/Graphsdedx.cxx 
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/CalibPHYSICS.o: $(SOURCEDIR)/CalibPHYSICS.cxx 
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/IDet.o: $(SOURCEDIR)/IDet.cxx 
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/ITdc.o: $(SOURCEDIR)/ITdc.cxx 
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/IScaler.o: $(SOURCEDIR)/IScaler.cxx 
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/TEvent.o: $(SOURCEDIR)/TEvent.cxx 
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTDIR)/TEventDict.o: $(LIBDIR)/TEventDict.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(LIBDIR)/TEventDict.cxx:  $(INCLUDEDIR)/TEvent.h $(INCLUDEDIR)/IDet.h $(INCLUDEDIR)/ITdc.h $(INCLUDEDIR)/IScaler.h $(INCLUDEDIR)/TEventLinkDef.h
	@echo "Generating dictionary $@..."
	@rootcint -f $@ -c $(HEADER) $^

clean::
	rm -f $(OBJECTDIR)/*.o 
	rm -f $(LIBDIR)/*Dict.cxx 
	rm -f $(LIBDIR)/*Dict.cpp 
	rm -f $(LIBDIR)/*Dict.h 
	rm -f $(LIBDIR)/*.pcm 
	rm -f core 
	rm -f $(SOURCEDIR)/*~ $(INCLUDEDIR)/*~ 
	rm -f $(LIBDIR)/libTEvent.so
	rm -f $(INSTALLDIR)/bin/physIris
# end
