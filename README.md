# Glauber + NBD fit for CBM

Author: Yige Huang

Date: 15.03.2024

Version: 1.1

## About TGlauberMC package

One can download `TGlauberMC-3.2` from [TGlauberMC](https://tglaubermc.hepforge.org).

Version 3.2 referenced: [arxiv](https://arxiv.org/abs/1710.07098).

`TGlauberMC` package is under the terms of the GNU General Public License.

For more information about this license, see: [GNU](http://www.gnu.org/licenses).

## Quick start

### Step 1: fast MC Glauber simulation

1. Modify parameters in `FastGlauberParams.h`

2. Modify parameters in `./SubmitFastGlauberGeneration.sh` and run it
    > If you want to REgenerate some MCG events, do `./SubmitFastGlauberGeneration.sh cla` to clean it

### Step 2: Npart Ncoll 2D distribution

1. Change the parameters in `AddNpartNcollHist.sh` accordingly with those in step 1

2. Run `AddNpartNcollHist.sh`, actually, if you did not do additional modifications in previous part, only `nJobs` and `targetDir` is necessary to be changed here

### Step 3: Scan parameters and find the best fit pair

1. Compile `FitTool` with `make`, the only change you may need is `mmg->SetEfficiencyMode(1, -1);` in `FitTool.cxx`
    > First argument can be 0, 1 or 2, corresponds to non-effiency, constant efficiency and multiplicity-dependent efficiency and the second one (1) is recommanded
    > If mode is 2, one should also set second argument to be the largest number of multiplicity

2. Set all kinds of variables in `SubmitFitBacth.sh`, and directly run it to submit jobs
    > You can run `./SubmitFitBacth.sh cla` to clean the output directory as well

3. After all jobs are finished, change target directory path in `FindBestPars.sh` and run it
    > You can find details in `BestFit.root` and `BestFit.txt`

### Step 4: do centrality determination and make some plots

1. With `BestFit.root` and `BestFit.txt`, do the following:
    1. Change parameters in `AnalysiCentrality.cxx`, like `npp`, `x`, etc.
    2. And `nsignn` is the cross section you use in step 1
    3. Set `xAxisMax` which is the upper limit of x-axis, should be a bit larger than the maximum multiplicity
    4. Some descriptions should also be changed, e.g. `CBM Simulation` and `Au+Au @ 4.9 GeV`

2. Run it with root `root -l -b -q AnalysiCentrality.cxx`, and you can find `CentralityDefinition.pdf` and `CentralityDefinition.txt`

## Patch note

14.03.2024 by yghuang (v1.1):

> New Feature: reweight parameters are avaliable in `AnalysisCentrality.cxx`
>
> Chage: Centrality splitting is determined by data distribution for top 20%, and Glauber + data for higher centrality

02.03.2024 by yghuang (v1.0):

> Fisrt completed edition
