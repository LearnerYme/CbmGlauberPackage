all: FitTool

FitTool: FitTool.cxx NbdTool/NbdTool.cxx FitManager/FitManager.cxx GlauberNbdSampler/GlauberNbdSampler.cxx
	g++ $^ -o $@ `root-config --libs --cflags` -std=c++14
