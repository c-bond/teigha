CXX = g++
WINCXX = x86_64-w64-mingw32-g++
CXXFLAGS += -std=c++11 -m64 #-O2 

TEIGHA_DIRS =$(HOME)/cpp/teigha/

INC_DIRS = -I$(TEIGHA_DIRS)Dgn/include -I$(TEIGHA_DIRS)Kernel/Include -I$(TEIGHA_DIRS)Drawing/Include -I$(TEIGHA_DIRS)Dgn/Extensions/ExServices -I$(TEIGHA_DIRS)Kernel/Extensions/ExServices -I$(TEIGHA_DIRS)ThirdParty/activation

LDFLAGS += -L$(TEIGHA_DIRS)lib/lnxX64_5.3 -pthread -lTG_Dgn7IO -lTG_ExamplesCommon -lTD_ExamplesCommon -lPSToolkit -lTG_ModelerGeometry -lTG_Db -ltinyxml -lTD_Zlib -loless -lTD_Db -lTD_Gs -lTD_Gi -lTD_DbRoot -lTD_Ge -lTD_SpatialIndex -lTD_Root -lFreeImage -lFreeType -lsisl -lTD_Alloc -llibcrypto -ldl 

WINLDFLAGS += -L$(TEIGHA_DIRS)/winlibs/lib/vc15_amd64dll -pthread -lTG_Dgn7IO -lTG_ExamplesCommon -lTD_ExamplesCommon -lPSToolkit -lTG_ModelerGeometry -lTG_Db -ltinyxml -lTD_Zlib -loless -lTD_Db -lTD_Gs -lTD_Gi -lTD_DbRoot -lTD_Ge -lTD_SpatialIndex -lTD_Root -lFreeImage -lFreeType -lsisl -lTD_Alloc -llibcrypto

OP_DIR = $(HOME)/cpp/teigha/bin/

default: linux

linux: lnx_bproc_qa lnx_bproc_worker lnx_bproc_main

windows: win_bproc_worker win_bproc_main

lnx_bproc_qa: bproc_qa.cc
	$(CXX) $(CXXFLAGS) -c $(INC_DIRS) $^ $(LDFLAGS)

lnx_bproc_worker: bproc_worker.cc
	$(CXX) $(CXXFLAGS) -c $(INC_DIRS) $^ $(LDFLAGS) 

lnx_bproc_main: bproc_main.cc bproc_worker.o bproc_qa.o
	$(CXX) $(CXXFLAGS) $(INC_DIRS) $^ $(LDFLAGS) -o $(OP_DIR)$@

win_bproc_worker: bproc_worker.cc
	$(WINCXX) $(CXXFLAGS) -c $(INC_DIRS) $^ $(WINLDFLAGS) -o $@
	
win_bproc_main: bproc_main.cc
	$(WINCXX) $(CXXFLAGS) $(INC_DIRS) $^ $(WINLDFLAGS) -o $(OP_DIR)$@

clean:
	rm -f *.o $(OP_DIR)*.o $(OP_DIR)lnx_bproc_main $(OP_DIR)win_bproc_main

#clean:
#vmlinuz.old	rm -f *.o *.pb.cc *.pb.h ../bin/bproc_grpc_sync_server ../bin/bproc_grpc_server



