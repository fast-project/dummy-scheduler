#!/bin/bash
# 
# File:   runTest.bash
# Author: gad
#
# Created on Jun 29, 2015, 12:39:40 PM
#
while true
do
    cat vm_migrate.yml | mosquitto_pub -l -t "fast/migfra/bebo/status"
    sleep 5
    cat vm_started.yml | mosquitto_pub -l -t "fast/migfra/bebo/status"
    sleep 5
    cat vm_stopped.yml | mosquitto_pub -l -t "fast/migfra/bebo/status"
    sleep 5
    cat init_agent.yml | mosquitto_pub -l -t "fast/agent/bebo/status"
    sleep 5
done