args=("$@")

build(){
    rivet-build -r ${args[0]}.cpp -std=c++17 -Wno-switch -Wno-unused-function -Wno-deprecated-declarations    #-Wno-switch because that warning is just stupid, -Wno-unused-function to be able to reuse headers in different analyses, and -Wno-deprecated-declarations to avoid warnings from Root/Rivet internal files
}

run(){
    if [[ $(file "${args[1]}") == *ROOT\ file* && ("${args[1]}" == */EVNT.* || "${args[1]}" == EVNT.*) ]]
    then
        echo "theApp.EvtMax = 1830

import AthenaPoolCnvSvc.ReadAthenaPool
svcMgr.EventSelector.InputCollections = ['${args[1]}']

from AthenaServices.AthenaServicesConf import AthenaEventLoopMgr
ServiceMgr += AthenaEventLoopMgr()
ServiceMgr.AthenaEventLoopMgr.EventPrintoutInterval = 2500
ServiceMgr.AthenaEventLoopMgr.OutputLevel = INFO

from AthenaCommon.AlgSequence import AlgSequence
job = AlgSequence()

from Rivet_i.Rivet_iConf import Rivet_i
rivet = Rivet_i()
import os
rivet.AnalysisPath = os.environ['PWD']

rivet.Analyses += ['${args[0]}']
rivet.RunName = ''
rivet.HistoFile = '/dev/null'
rivet.CrossSection = 1.0
rivet.OutputLevel = 0
rivet.SkipWeights=True
job += rivet" > .rivet_JO.py
        athena .rivet_JO.py
        rm .rivet_JO.py
    else
        rivet --analysis=${args[0]} ${args[1]} --pwd
    fi
    rm -f neg_weights.dat pos_weights.dat Rivet.yoda weights.dat eventLoopHeartBeat.txt PoolFileCatalog.xml PoolFileCatalog.xml.BAK
}

if [[ "$1.cpp" -nt "RivetAnalysis.so" || $(rivet --list --pwd) != *${args[0]}* ]]
then
    build && run
else
    run
fi