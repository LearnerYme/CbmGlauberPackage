#include "TH1F.h"
#include "TMath.h"
#include "TString.h"

#include "NbdTool.h"

NbdTool::NbdTool() {
    npp = 2.0;
    k = 1.0;
    mHistNbd = new TH1D(
        Form("nbd%.3f_%.3f", npp, k),
        "",
        nBins, 0, nBins
    );
}

void NbdTool::SetNbdParameters(double npp, double k) {
    this->npp = npp;
    this->k = k;
}

void NbdTool::CreateHistogram() {
    // Note: before call this member function, make sure that you have already set npp and k 
    // via NbdTool::SetNbdParameters(double npp, double k)
    mHistNbd->SetName(Form("nbd%.3f_%.3f", npp, k));
    for (int i=0; i<nBins; i++) {
        double val = Nbd(i);
        val = val > 1e-10 ? val : 0.0;
        mHistNbd->SetBinContent(i+1, val);
    }
}

double NbdTool::Nbd(double n) {
    const double c = TMath::Exp(
        TMath::LnGamma(n+k) - TMath::LnGamma(n+1) - TMath::LnGamma(k)
    );
    const double term = n * TMath::Log(npp/k) - (n+k) * TMath::Log(npp/k+1.0);
    return c*TMath::Exp(term);
}

int NbdTool::Sample() {
    return static_cast<int>(mHistNbd->GetRandom());
}

void NbdTool::Reset() {
    npp = 2.0;
    k = 1.0;
    for (int i=0; i<nBins; i++) {
        mHistNbd->SetBinContent(i+1, 0.0);
    }
}

TH1D* NbdTool::GetHistogram() {
    return mHistNbd;
}