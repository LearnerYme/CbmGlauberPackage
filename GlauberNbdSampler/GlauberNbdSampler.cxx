#include "TFile.h"
#include "TH2D.h"
#include "TRandom3.h"

#include "GlauberNbdSampler.h"
#include "../NbdTool/NbdTool.h"

GlauberNbdSampler::GlauberNbdSampler() {
    x = 0.12;
    eff = 0.98;
    fNpartNcoll = 0;
    mEnableEfficiency = false;
    mEnableMultiplicityDependence = false;
    mMaximumMult = 
    mSampleNpart = 0;
    mSampleNcoll = 0;
    mSampleMult = 0;

    mNbdTool = new NbdTool();
    rd = new TRandom3();
}

void GlauberNbdSampler::Read2DHistogram(const char* fileName="npart_ncoll_hist.root", const char* histName="hNcoll_Npart") {
    fNpartNcoll = new TFile(fileName);
    hNpartNcoll = (TH2D*)(fNpartNcoll->Get(histName))->Clone();
}

void GlauberNbdSampler::SetParameters(double npp, double x, double k, double eff) {
    /*
        Args: 4
        :npp and k: for Negative binomial
        :x: for two component multiplicity
        :eff: factor from ideal to measured multiplicity
        Returns: None
    */
    this->x = x;
    this->eff = eff;
    mNbdTool->SetNbdParameters(npp, k);
    mNbdTool->CreateHistogram();
}

void GlauberNbdSampler::SetEfficiencyMode(int mode, int maxMult=100) {
    /*
        Args: 1 + 1
        :mode: integer, 0, 1 or 2
        :-> 0, disable efficiency sampling
        :-> 1, enable a constant efficiency sampling
        :-> 2, enable a multiplicity dependent efficiency sampling
        :maxMult: only when mode is 2 (mult-dependent-efficiency), efficiency will be as a linear function of multiplicity
        :-> eff(0) = 1, and eff(maxMult) = eff0
        :-> where eff0 is hyperparameter (see SetParameters)
    */
    assert(mode >= 0 && mode <= 2);
    switch (mode) {
    case 0:
        mEnableEfficiency = false;
        mEnableMultiplicityDependence = false;
        break;
    case 1:
        mEnableEfficiency = true;
        mEnableMultiplicityDependence = false;
        break;
    case 2:
        mEnableEfficiency = true;
        mEnableMultiplicityDependence = true;
        mMaximumMult = maxMult;
        break;
    }
}

double GlauberNbdSampler::GetEfficiency(int Nch) {
    double efficiency = 1.0;
    
    if (mEnableEfficiency) {
        if (mEnableMultiplicityDependence) {
            efficiency = 1 - (1 - eff) * Nch / mMaximumMult;
            efficiency *= effciencyFactor;
        } else {
            efficiency = eff;
        }
    }
    return efficiency;
}

void GlauberNbdSampler::Sample() {
    // Before get sample results, call Sample() is a must
    // step 1, sample Npart and Ncoll
    double npart = 0.0;
    double ncoll = 0.0;
    while (npart < 2 || ncoll < 1) {
        hNpartNcoll->GetRandom2(npart, ncoll);
    }
    mSampleNpart = static_cast<int>(npart);
    mSampleNcoll = static_cast<int>(ncoll);
    // step 2, calculate sampled mult
    double multSample = x == 0 ? npart : 0.5*(1.0-x)*npart + x*ncoll;
    // step 3, from sampled mult to ideal Nch
    int idealNch = 0;
    int multSampleInt = static_cast<int>(multSample);
    for (int i=0; i<multSampleInt; i++) {
        idealNch += mNbdTool->Sample();
    }
    // step 4, (if ON), from ideal Nch to measured Nch
    if (!mEnableEfficiency) {
        mSampleMult = idealNch;
    } else if (!mEnableMultiplicityDependence) {
        // Note: add this rd->Uniform() is to avoid some jumping points
        // for mult-dependent-efficiency case, the reason is the same
        mSampleMult = static_cast<int>(rd->Binomial(idealNch, eff) + rd->Uniform());
    } else {
        mSampleMult = 0;
        for (int i=0; i<idealNch; i++) {
            if (rd->Uniform() < GetEfficiency(idealNch)) { mSampleMult += 1; }
        }
        mSampleMult = static_cast<int>(mSampleMult + rd->Uniform());
    }
}

void GlauberNbdSampler::GetSampleResult(int* arr) {
    arr[0] = mSampleNpart;
    arr[1] = mSampleNcoll;
    arr[2] = mSampleMult;
}
