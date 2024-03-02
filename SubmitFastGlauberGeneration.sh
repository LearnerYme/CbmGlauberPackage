#!/bin/bash

# Set number of jobs here
# To set number of MC events per job -----> FastGlauberParams.h
# suggested: 50 jobs with 200000 events per job
nJobs=5
export targetDir="/lustre/hades/user/yige/4p9/x/glauber/fastglauber"
jobName="FastGlauberGeneration"
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

    cp -r ./TGlauberMC-3.2 ${targetDir}/src/
    cp rootlogon.C ${targetDir}/src/
    cp FastGlauberGeneration.cxx ${targetDir}/src/
    cp FastGlauberParams.h ${targetDir}/src/

    sbatch --job-name=${jobname} --partition=${partition} --time=${time} --array=1-${nJobs} -D $targetDir/LOGS -o FastGlauber_%j.out -e FastGlauber_%j.err --export=ALL -- FastGlauberGeneration.sh
fi