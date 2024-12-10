#! /usr/bin/env bash

if [ "$#" -ne 4 ]; then
    echo " usage: $0 [device] [sleep] [outfile] [ntimes]"
    exit 1
fi
device=$1
sleep=$2
outfile=$3
ntimes=$4

reg="/au/readout/bin/register --connection /etc/drich/drich_ipbus_connections.xml --device ${device}"
blo="/au/readout/bin/block --connection /etc/drich/drich_ipbus_connections.xml --device ${device}"

run() {

    ### run sequence
    mode 0x0  # run OFF
    filter on # filter
    mode 0x1  # run ON
    reset     # reset FIFO
    mode 0x3  # spill ON
    #echo " --- sleep $sleep seconds "
    sleep $sleep # sleep
    mode 0x1     # spill OFF
    mode 0x0     # run OFF

    for chip in {0..5}; do
        for lane in {0..3}; do
            ### read ALCOR ECCR
            #echo " --- $(/au/readout/bin/alcor_register --connection /etc/drich/drich_ipbus_connections.xml --device $device --chip $chip --eccr $lane)"

            ### read FIFO time
            timer=$(${reg} --node alcor_readout_id${chip}_lane${lane}.fifo_timer | awk {'print $3'})
            hex=${timer:2}
            dec=$((16#$hex))
            nsec=$(echo "$dec / 31250000." | bc -l)
            #echo " --- the timer is $timer: $nsec seconds  "

            ### download data
            download /tmp/data.chip$chip.lane$lane.dat $chip $lane
        done
    done

    # Fill the array with numbers (you can modify this as needed)
    for i in {1..5}; do
        numbers+=("$i") # Append numbers to the array
    done

    echo -n "$(date +"%Y%m%d-%H%M%S.$(date +%3N)") "
    same_count=true # Assume all counts are the same
    first_count=""  # Placeholder for the first count to compare
    for chip in {0..5}; do
        rollovers=0

        for lane in {0..3}; do
            count=$(grep 5c5c5c5c /tmp/data.chip$chip.lane$lane.dat | wc -l)
            echo -n "$count "

            # Check if the count matches the first count
            if [ -z "$first_count" ]; then
                first_count="$count" # Set the first count for comparison
            elif [ "$count" -ne "$first_count" ]; then
                same_count=false
            fi
        done
    done
    # Report if all counts are the same for this chip
    if $same_count; then
        echo -n "0"
    else
        echo -n "1"
    fi
}

download() {
    rm -f $1
    chip=$2
    lane=$3
    while true; do
        ### read FIFO occupancy
        nwords=$(${reg} --node alcor_readout_id${chip}_lane${lane}.fifo_occupancy | awk {'print $3'})
        nwords=$(($nwords & 0xFFFF))
        echo " --- there are $nwords words in the FIFO "
        [[ $nwords == "0" ]] && break
        ${blo} --node alcor_readout_id${chip}_lane${lane}.fifo_data --size ${nwords} >>$1
    done
    echo " --- downloaded data: $1 "
}

mode() {
    ${reg} --node regfile.mode --write $1
}

filter() {
    for chip in {0..5}; do
        if [ "$1" == "on" ]; then
            ${reg} --node alcor_controller_id${chip} --write 0x0330000f
            ${reg} --node alcor_controller_id${chip}
        elif [ "$1" == "off" ]; then
            ${reg} --node alcor_controller_id${chip} --write 0x03300000
            ${reg} --node alcor_controller_id${chip}
        fi
    done
}

reset() {
    for chip in {0..5}; do
        for lane in {0..3}; do
            ${reg} --node alcor_readout_id${chip}_lane${lane}.fifo_reset --write 1
        done
    done
}

main() {
    echo "#time fifo0 fifo1 fifo2 fifo3 fifo4 fifo5 fifo6 fifo7 fifo8 fifo9 fifo10 fifo11 fifo12 fifo13 fifo14 fifo15 fifo16 fifo17 fifo18 fifo19 fifo20 fifo21 fifo22 fifo23 check" >>$outfile
    for iter in $(seq 0 "$ntimes"); do
        last_line=$(run | tail -n 1)
        echo "$last_line" >>$outfile
    done
}

main
