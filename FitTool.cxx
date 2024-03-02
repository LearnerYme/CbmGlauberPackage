#include <iostream>
#include <fstream>
// #include 

#include "GlauberNbdSampler/GlauberNbdSampler.h"
#include "NbdTool/NbdTool.h"
#include "FitManager/FitManager.h"

using std::cout;
using std::endl;
using std::ofstream;

int main(int argc, char** argv) {
    ofstream fout("GlauberNbdFitLog.txt");
    const char* fileNameReal = argv[1];
    const char* histNameReal = argv[2];
    const char* fileName2D = argv[3];
    const char* histName2D = argv[4];
    double npp = atof(argv[5]);
    double x = atof(argv[6]);
    double k = atof(argv[7]);
    double eff = atof(argv[8]);
    long nEv = atof(argv[9]);
    const char* outName = argv[10];
    const char* taskTag = argv[11];
    cout << "[LOG] Glauber + NBD fit ++++++++" << endl;
    cout << "[LOG] Read real distribution from: " << fileNameReal << endl;
    cout << "[LOG] Histogram name: " << histNameReal << endl;
    cout << "[LOG] Read Npart+Ncoll distribution from: " << fileName2D << endl;
    cout << "[LOG] Histogram name: " << histName2D << endl;
    cout << "[LOG] Parameters: npp, x, k, eff = " << npp << ", ";
    cout << x << ",";
    cout << k << ",";
    cout << eff << endl;
    cout << "[LOG] Number of simulated events: " << nEv << endl;
    cout << "[LOG] Output root file: " << outName << endl;

    auto mmg = new FitManager();
    mmg->ReadRealDistribution(fileNameReal, histNameReal);
    mmg->Read2DHistogram(fileName2D, histName2D);
    mmg->SetParameters(npp, x, k, eff);
    mmg->SetEfficiencyMode(1, -1);
    mmg->SetOutput(outName);
    mmg->Sample(nEv);
    mmg->CalcChi2(10); // about 10 is okay
    double chi2 = mmg->GetChi2();
    double ndf = mmg->GetNdf();
    double chi2OverNdf = ndf == 0 ? -999 : chi2 / ndf;
    cout << "[LOG] Task <" << taskTag << "> finished! [npp, x, k, eff, n, chi2, ndf, chi2/ndf] = ";
    cout << npp << ", ";
    cout << x << ", ";
    cout << k << ", ";
    cout << eff << ", ";
    cout << nEv << ", ";
    cout << chi2 << ", ";
    cout << ndf << ", ";
    cout << chi2OverNdf << endl;
    mmg->Save();
    
    fout << "Task|npp|x|k|eff|n|chi2|ndf|chi2/ndf" << endl; 
    fout << taskTag << "|";
    fout << npp << "|";
    fout << x << "|";
    fout << k << "|";
    fout << eff << "|";
    fout << nEv << "|";
    fout << chi2 << "|";
    fout << ndf << "|";
    fout << chi2OverNdf << endl;
    fout.close();
    
    cout << "[LOG] All done, please check " << outName << endl;
    return 0;
}