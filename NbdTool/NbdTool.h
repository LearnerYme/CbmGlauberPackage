#ifndef __NDBTOOL_HDEAER__
#define __NDBTOOL_HDEAER__

#include <memory>

class TH1D;

class NbdTool {
    
    private:
        TH1D* mHistNbd;
        double npp;
        double k;
        static const int nBins = 100; // usually, it will be less than 100, change if only when you really need to do so

    public:
        NbdTool();
        ~NbdTool(){}

        void SetNbdParameters(double, double);
        void CreateHistogram();
        double Nbd(double);
        int Sample();
        void Reset();
        TH1D* GetHistogram();
};

#endif