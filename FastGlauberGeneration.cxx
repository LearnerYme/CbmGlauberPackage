#include "FastGlauberParams.h"

void FastGlauberGeneration(const char* outName) {
    gSystem->Load("libMathMore");
    cout << "er"<< endl;
    cout << "[LOG] - Read parameters from header:" << endl;
    cout << "\t- Number of events: " << params::n << endl;
    cout << "\t- System A: " << params::sysA << endl;
    cout << "\t- System B: " << params::sysB << endl;
    cout << "\t- Inelastic cross section: " << params::signn << " mb " << endl;
    cout << "\t- Signal width: " << params::sigwidth << endl;
    cout << "\t- Minimum distance: " << params::mind << endl;
    cout << "\t- Omega: " << params::omega << endl;
    cout << "\t- Node distance: " << params::noded << endl;
    cout << "\t- Root file will be saved as: " << outName << endl;
    cout << "[LOG] - Simulation starts..." << endl;
    runAndSaveNtuple(
        params::n,
        params::sysA,
        params::sysB,
        params::signn,
        params::sigwidth,
        params::mind,
        params::omega,
        params::noded,
        outName
    );
    cout << "[LOG] - Task end!" << endl;
}