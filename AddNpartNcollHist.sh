#!/bin/bash

nJobs=50
targetDir="/lustre/hades/user/yige/4p9/x/glauber/fastglauber"
rootFileName="FastGlauber.root"
treeName="nt_Au_Au"
outputName="npart_ncoll_hist.root"

echo "[LOG] Now adding Npart and Ncoll 2D histogram..."

echo "[LOG] Number of root files: ${nJobs}"
echo "[LOG] Target path of fast glauber tasks: ${targetDir}"
echo "[LOG] Root file name of fast glauber tasks: ${rootFileName}"
echo "[LOG] TNtuple name: ${treeName}"
echo "[LOG] 2D distribution will be saved as: ${outputName}"

root -l -b -q AddNpartNcollHist.cxx\(${nJobs},\"${targetDir}\",\"${rootFileName}\",\"${treeName}\",\"${outputName}\"\)

echo "[LOG] Done, please chekc ${outputName}"