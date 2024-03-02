#!/bin/bash

INDEX=$(printf "%06d" "${SLURM_ARRAY_TASK_ID}")
IDX=$((${SLURM_ARRAY_TASK_ID}-1))
echo "task tag = <${SLURM_ARRAY_TASK_ID}>"
echo "OUTDIR = ${targetDir}"
cd ${targetDir}
mkdir -p ${INDEX}
cd ${INDEX}
srcDir=${targetDir}/src

This_FileNameDist=`basename ${FitBatch_FileNameDist}`
This_FileName2D=`basename ${FitBatch_FileName2D}`

ln -s ${srcDir}/FitTool ./
ln -s ${srcDir}/${This_FileNameDist} ./
ln -s ${srcDir}/${This_FileName2D} ./

# compute npp, x, k and eff for this job
Idx_npp=$(( (${IDX} / ${FitBatch_IterN_npp}) % ${FitBatch_Nscan_npp} ))
Idx_x=$(( (${IDX} / ${FitBatch_IterN_x}) % ${FitBatch_Nscan_x} ))
Idx_k=$(( (${IDX} / ${FitBatch_IterN_k}) % ${FitBatch_Nscan_k} ))
Idx_eff=$(( (${IDX} / ${FitBatch_IterN_eff}) % ${FitBatch_Nscan_eff} ))

This_npp=`echo "scale=3;${Idx_npp}*${FitBatch_Step_npp}+${FitBatch_Min_npp}" | bc`
This_x=`echo "scale=3;${Idx_x}*${FitBatch_Step_x}+${FitBatch_Min_x}" | bc`
This_k=`echo "scale=3;${Idx_k}*${FitBatch_Step_k}+${FitBatch_Min_k}" | bc`
This_eff=`echo "scale=3;${Idx_eff}*${FitBatch_Step_eff}+${FitBatch_Min_eff}" | bc`

echo "./FitTool ${This_FileNameDist} ${FitBatch_HistNameDist} ${This_FileName2D} ${FitBatch_HistName2D} ${This_npp} ${This_x} ${This_k} ${This_eff} ${FitBatch_Nev} ${FitBatch_OutPutName} ${INDEX}"
./FitTool ${This_FileNameDist} ${FitBatch_HistNameDist} ${This_FileName2D} ${FitBatch_HistName2D} ${This_npp} ${This_x} ${This_k} ${This_eff} ${FitBatch_Nev} ${FitBatch_OutPutName} ${INDEX}