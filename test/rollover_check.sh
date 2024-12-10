#! /usr/bin/env bash

# ---   Script call and arguments
help() {
    echo "Usage: $0 [target_device] [target_chip]"
    echo
    echo "  target_device   The name or ID of the target device. The allowed values are kc705-196 through kc705-211."
    echo "  target_chip     The ID of the target chip. The allowed values are 0 through 5 and all"
    echo
    echo "Examples:"
    echo "  $0 kc705-196 3"
}

target_device=$1
target_chip="all"

if [ -z $1 ]; then
    echo " usage: $0 [target_device] [target_chip = all]"
    exit 1
fi

if [! -z $2 ]; then
    target_chip=$2
fi

# ---   Global variables
READOUT_CONF="/au/pdu/conf/readout.${target_device}.conf"
CONNECTION="/etc/drich/drich_ipbus_connections.xml"
SWITCH="-s -i -m 0xffffffff -p 1"

# ---   Reading the configuration file
while read -r chip lane eccr bcr pcr; do
    [[ $chip =~ ^#.* ]] && continue
    [[ $_chip != "all" ]] && [[ $_chip != $chip ]] && continue
    ldec=$(printf "%d" $lane)
    [ $ldec -eq 0 ] && continue
    # ---   Initialise the chips one by one
    /home/eic/bin/alcor-chip-init.sh $device $chip $lane $eccr $bcr $pcr
done <$READOUT_CONF
wait
sleep 0.1

# ---   Frequency test
while read -r chip lane eccr bcr pcr; do
    [[ $chip =~ ^#.* ]] && continue
    [[ $_chip != "all" ]] && [[ $_chip != $chip ]] && continue
    ldec=$(printf "%d" $lane)
    [ $ldec -eq 0 ] && continue
    for thelane in {0..3}; do
        /au/pdu/measure/alcor-frequency-test.sh $device $chip $thelane 0.1
    done
done <$READOUT_CONF
