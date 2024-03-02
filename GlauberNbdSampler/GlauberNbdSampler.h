#ifndef __MCGNBDSPLER_HEADER__
#define __MCGNBDSPLER_HEADER__

class NbdTool;
class TFile;
class TH2D;
class TRandom3;

class GlauberNbdSampler {

    private:
        NbdTool* mNbdTool;
        TFile* fNpartNcoll;
        TH2D* hNpartNcoll;
        double x;
        double eff;
        bool mEnableEfficiency;
        bool mEnableMultiplicityDependence;
        int mMaximumMult;
        static constexpr double effciencyFactor = 1.00;
        TRandom3* rd;
        // static const double effciencyFactor = 0.98; // for mult-dependent-efficiency

        // sampled values
        int mSampleNpart;
        int mSampleNcoll;
        int mSampleMult;

    public:
        GlauberNbdSampler();
        ~GlauberNbdSampler(){}

        void Read2DHistogram(const char*, const char*);
        void SetParameters(double, double, double, double);
        void SetEfficiencyMode(int, int);
        double GetEfficiency(int);
        void Sample();
        int GetSampleNpart() { return mSampleNpart; }
        int GetSampleNcoll() { return mSampleNpart; }
        int GetSampleMult() { return mSampleMult; }
        void GetSampleResult(int*); // quite dangerous XD
};

#endif