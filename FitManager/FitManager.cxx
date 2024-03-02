#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TString.h"
#include "TMath.h"

#include "FitManager.h"
#include "../GlauberNbdSampler/GlauberNbdSampler.h"

FitManager::FitManager() {
    mSampler = new GlauberNbdSampler();
    mChi2 = 0.0;
    mNdf = 0;
    hChi2 = new TH1D("hChi2", "#chi2,NDF;contents;", 2, -0.5, 2.5);
}

void FitManager::ReadRealDistribution(const char* fileName, const char* histName) {
    TFile* tf = new TFile(fileName);
    TH1D* h1 = (TH1D*)tf->Get(histName);
    hReal = (TH1D*)h1->Clone();
    hReal->SetName("hReal");
    hSample = new TH1D(
        "hSample", Form(";%s;", h1->GetXaxis()->GetTitle()),
        h1->GetNbinsX(), h1->GetXaxis()->GetXmin(), h1->GetXaxis()->GetXmax()
    );
    h2NpartVsMult = new TH2D(
        "h2NpartVsMult", Form(";N_{part};%s", h1->GetXaxis()->GetTitle()),
        400, -0.5, 399.5,
        h1->GetNbinsX(), h1->GetXaxis()->GetXmin(), h1->GetXaxis()->GetXmax()
    );
    h2NcollVsMult = new TH2D(
        "h2NcollVsMult", Form(";N_{coll};%s", h1->GetXaxis()->GetTitle()),
        1600, -0.5, 1599.5,
        h1->GetNbinsX(), h1->GetXaxis()->GetXmin(), h1->GetXaxis()->GetXmax()
    );
}

void FitManager::SetOutput(const char* outName) {
    tfout = new TFile(outName, "recreate");
    hChi2->SetDirectory(tfout->CurrentDirectory());
    hReal->SetDirectory(tfout->CurrentDirectory());
    hSample->SetDirectory(tfout->CurrentDirectory());
    h2NpartVsMult->SetDirectory(tfout->CurrentDirectory());
    h2NcollVsMult->SetDirectory(tfout->CurrentDirectory());
}

void FitManager::Save() {
    tfout->cd();
    tfout->Write();
    tfout->Close();
}

void FitManager::Sample(long n) {
    int res[3];
    for (int i=0; i<n; i++) {
        mSampler->Sample();
        mSampler->GetSampleResult(res);
        hSample->Fill(res[2]);
        h2NpartVsMult->Fill(res[0], res[2]);
        h2NcollVsMult->Fill(res[1], res[2]);
    }
}

void FitManager::CalcChi2(int minMult=10) {
    /*
        Args: 1
        :minMult: minimum of multiplicity to count chi2
        Return: None
        :Note: please setup sampler, read in real distribution, and call Sample before calculate chi2
    */
    auto nEvReal = hReal->Integral();
    auto nEvSim = hSample->Integral();
    hSample->Scale(nEvReal*1.0 / nEvSim);
    mNdf = 0;
    mChi2 = 0.0;
    for (int i=1; i<hReal->GetNbinsX()+1; i++) {
        if (i < minMult) { continue; }
        const double dataErr = hReal->GetBinError(i);
        if (dataErr == 0) { continue; }
        const double simErr = hSample->GetBinError(i);
        if (simErr == 0) { continue; }
        const double totErr = TMath::Power((dataErr*dataErr + simErr*simErr),0.5);
        const double data = hReal->GetBinContent(i);
        const double sim = hSample->GetBinContent(i);
        const double delta = (data - sim) / totErr;
        mChi2 += delta * delta;
        mNdf += 1;
    }
    mNdf -= 2;
    hChi2->SetBinContent(1, mChi2);
    hChi2->SetBinContent(2, mNdf);
}

void FitManager::Read2DHistogram(const char* fileName="npart_ncoll_hist.root", const char* histName="hNcoll_Npart") {
    mSampler->Read2DHistogram(fileName, histName);
}

void FitManager::SetParameters(double npp, double x, double k, double eff) {
    mSampler->SetParameters(npp, x, k, eff);
}

void FitManager::SetEfficiencyMode(int mode, int maxMult=100) {
    mSampler->SetEfficiencyMode(mode, maxMult);
}
