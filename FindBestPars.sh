#!/bin/bash

targetDir="/lustre/hades/user/yige/4p9/x/glauber/fitbatch/refmult3"

echo "[LOG] Target directory is: ${targetDir}"
echo "[LOG] Now scanning best chi2/ndf, please be patient..."

cat ${targetDir}/*/GlauberNbdFitLog.txt | grep 0 > FitResults.txt
awk -F"|" 'BEGIN {mMin=9999;mIdx=0;mNpp=0;mX=0;mK=0;mEff=0;mChi2=0;mNdf=0} {if ($9+0 < mMin+0) {mMin=$9;mIdx=$1;mNpp=$2;mX=$3;mK=$4;mEff=$5;mChi2=$7;mNdf=$8}} END {print "[LOG] Minimum chi2/ndf at #",mIdx,"#(npp,x,k,eff,chi2,NFD,chi2/NFD)=",mNpp,mX,mK,mEff,mChi2,mNdf,mMin}' FitResults.txt > BestFit.txt
bestIdx=`cut -d "#" -f2 BestFit.txt`
bestIdx=`echo $bestIdx | sed 's/ //g'`
cp ${targetDir}/${bestIdx}/multOut.root ./BestFit.root

echo "[LOG] Best fit results see BestFit.txt and root file see BestFit.root"
echo "[LOG] This is the end of best chi2/ndf finding script, good bye"