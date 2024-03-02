#ifndef __FITMANAGER_HEADER__
#define __FITMANAGER_HEADER__

class TH1D;
class TH2D;
class TFile;
class GlauberNbdSampler;

class FitManager {

    private:
        GlauberNbdSampler* mSampler;
        TH1D* hReal;
        TH1D* hSample;
        TH1D* hChi2;
        TH2D* h2NpartVsMult;
        TH2D* h2NcollVsMult;
        TFile* tfout;

        double mChi2;
        int mNdf; // degree of freedom

    public:
        FitManager();
        ~FitManager(){}

        void ReadRealDistribution(const char*, const char*);
        void SetOutput(const char*);
        void Sample(long);
        void Save();

        // settings for GlauberNbdSampler
        void Read2DHistogram(const char*, const char*);
        void SetParameters(double, double, double, double);
        void SetEfficiencyMode(int, int);

        // chi2 and ndf
        void CalcChi2(int);
        double GetChi2() { return mChi2; }
        double GetNdf() { return mNdf; }
};

#endif