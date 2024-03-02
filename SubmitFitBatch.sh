#!/bin/bash

#### Set parameters here
export FitBatch_Nev=1000000
export FitBatch_FileNameDist="/u/yige/DataAnalysis/cbm/4p9/xGeneration/GlauberNbdFit/output.hadd.root"
export FitBatch_HistNameDist="hRefMult3"
export FitBatch_FileName2D="/u/yige/DataAnalysis/cbm/4p9/xGeneration/GlauberNbdFit/npart_ncoll_hist.root"
export FitBatch_HistName2D="hNcoll_Npart"
export FitBatch_OutPutName="multOut.root"

export FitBatch_Nscan_npp=14
export FitBatch_Min_npp=0.3
export FitBatch_Max_npp=1.0

export FitBatch_Nscan_x=1
export FitBatch_Min_x=0.12
export FitBatch_Max_x=0.12

export FitBatch_Nscan_k=10
export FitBatch_Min_k=0.5
export FitBatch_Max_k=1.5

export FitBatch_Nscan_eff=1
export FitBatch_Min_eff=1.00
export FitBatch_Max_eff=1.00

export FitBatch_Njobs=`echo $FitBatch_Nscan_npp*$FitBatch_Nscan_x*$FitBatch_Nscan_k*$FitBatch_Nscan_eff | bc`

export FitBatch_Step_npp=`echo "scale=4;($FitBatch_Max_npp-$FitBatch_Min_npp)/$FitBatch_Nscan_npp" | bc`
export FitBatch_Step_x=`echo "scale=4;($FitBatch_Max_x-$FitBatch_Min_x)/$FitBatch_Nscan_x" | bc`
export FitBatch_Step_k=`echo "scale=4;($FitBatch_Max_k-$FitBatch_Min_k)/$FitBatch_Nscan_k" | bc`
export FitBatch_Step_eff=`echo "scale=4;($FitBatch_Max_eff-$FitBatch_Min_eff)/$FitBatch_Nscan_eff" | bc`

# to include the maximum into scan range
export FitBatch_Max_npp=`echo "scale=4;$FitBatch_Max_npp+$FitBatch_Step_npp" | bc`
export FitBatch_Max_x=`echo "scale=4;$FitBatch_Max_x+$FitBatch_Step_x" | bc`
export FitBatch_Max_k=`echo "scale=4;$FitBatch_Max_k+$FitBatch_Step_k" | bc`
export FitBatch_Max_eff=`echo "scale=4;$FitBatch_Max_eff+$FitBatch_Step_eff" | bc`
export FitBatch_Step_npp=`echo "scale=4;($FitBatch_Max_npp-$FitBatch_Min_npp)/$FitBatch_Nscan_npp" | bc`
export FitBatch_Step_x=`echo "scale=4;($FitBatch_Max_x-$FitBatch_Min_x)/$FitBatch_Nscan_x" | bc`
export FitBatch_Step_k=`echo "scale=4;($FitBatch_Max_k-$FitBatch_Min_k)/$FitBatch_Nscan_k" | bc`
export FitBatch_Step_eff=`echo "scale=4;($FitBatch_Max_eff-$FitBatch_Min_eff)/$FitBatch_Nscan_eff" | bc`

export FitBatch_IterN_npp=1
export FitBatch_IterN_x=$(( $FitBatch_IterN_npp * $FitBatch_Nscan_npp ))
export FitBatch_IterN_k=$(( $FitBatch_IterN_x * $FitBatch_Nscan_x ))
export FitBatch_IterN_eff=$(( $FitBatch_IterN_k * $FitBatch_Nscan_k ))

export targetDir="/lustre/hades/user/yige/4p9/x/glauber/fitbatch/refmult3"
jobName="GlauberFit"
partition="main"
time="1:00:00"

if [ $# -eq 1 ] && [ $1 = "cla" ]; then
    echo "[LOG] Now clean target directory..."
    rm -rf ${targetDir}
else

    if [ ! -d $targetDir ]; then
        mkdir -p $targetDir
    fi

    if [ ! -d $targetDir/LOGS ]; then
        mkdir $targetDir/LOGS
    fi

    if [ ! -d $targetDir/src ]; then
        mkdir $targetDir/src
    fi

    cp ./FitTool ${targetDir}/src/
    cp ${FitBatch_FileNameDist} ${targetDir}/src/
    cp ${FitBatch_FileName2D} ${targetDir}/src/

    sbatch --job-name=${jobname} --partition=${partition} --time=${time} --array=1-${FitBatch_Njobs} -D $targetDir/LOGS -o FitBatch_%j.out -e FitBatch_%j.err --export=ALL -- FitWithParameters.sh
fi