#!/bin/bash
# 
# File:   runTest.bash
# Author: gad
#
# Created on Jun 29, 2015, 12:39:40 PM
#

while true
do
    
    #echo $vm_migrate
    mosquitto_pub -t "fast/migfra/bebo/status" -f vm_migrate.yml 
    sleep 1
    mosquitto_pub -t "fast/migfra/bebo/status" -f vm_started.yml
    sleep 1
    mosquitto_pub -t "fast/migfra/bebo/status" -f vm_stopped.yml 
    sleep 1
    #cat vm_stopped.yml | mosquitto_pub -l -t "fast/migfra/bebo/status"
    #sleep 20
    mosquitto_pub  -t "fast/agent/bebo/status" -f init_agent.yml
    #sleep 20
done