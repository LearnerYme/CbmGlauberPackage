#!/bin/bash

INDEX=$(printf "%05d" "${SLURM_ARRAY_TASK_ID}")
echo "SLURM_ARRAY_TASK_ID = ${SLURM_ARRAY_TASK_ID}"
echo "OUTDIR = ${targetDir}"
cd ${targetDir}
mkdir -p ${INDEX}
cd ${INDEX}
srcDir=${targetDir}/src

ln -s ${srcDir}/TGlauberMC-3.2 ./
ln -s ${srcDir}/rootlogon.C ./
ln -s ${srcDir}/FastGlauberGeneration.cxx ./
ln -s ${srcDir}/FastGlauberParams.h ./

if [ ! -e TGlauberMC-3.2/runglauber_v3.2_C.d ]; then
    echo "[LOG] It's your first time doing MC Glauber simulation or some files are already removed, now initializing..."
    root -l -b -q
fi

root -l -b -q FastGlauberGeneration.cxx\(\"FastGlauber.root\"\)