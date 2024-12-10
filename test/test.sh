#! /usr/bin/env bash

if [ $# -ne 2 ]; then
    echo " usage: $0 [nruns] [nspill] "
    exit 1
fi

nruns=$1
nspill=$2

for run in $(seq 1 ${nruns}); do

    drich_run_start.sh all ${nspill}
    echo $(cat /tmp/drich_run_start.runname) ${deltathr} >>test.database.txt

    echo " ---------------------------------------------------"
    echo " ---------------------------------------------------"

    for K in 195; do
        for I in {16..23}; do
            nroll=$(od -An -w4 -tx4 -v /home/eic/DATA/2024-testbeam/actual/physics/latest/kc705-${K}/raw/alcdaq.fifo_$I.dat | grep 5c5c5c5c | wc -l)
            freq=$(python -c "print(${nroll} * 2**15 / (0.5 * ${nspill}))")
            chip=$(python -c "print(${I} // 4)")
            lane=$(python -c "print(${I} % 4)")
            echo "kc705-${K} ch-$chip ln-$lane: ${freq} MHz ($nroll rollovers)" | tee -a /home/eic/DATA/2024-testbeam/actual/physics/latest/test.txt

            #	    /au/readout/bin/alcor_register --connection /etc/drich/drich_ipbus_connections.xml --device kc705-${K} --chip ${chip} --eccr ${lane} | tee -a /home/eic/DATA/2023-testbeam/actual/physics/latest/test.txt
        done
    done

    #    rm -rf /home/eic/DATA/2023-testbeam/actual/physics/latest/kc705-*

done
