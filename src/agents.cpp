/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "agents.h"
#include <string>
#include <iostream>
fast::agentProperties fast::creatRandomAgentProperties() {
    std::string levels[] = {"low", "medium", "high"};
    fast::agentProperties P;
    P.repeat = std::to_string(rand() % 100 + 1);
    std::cout << P.repeat << std::endl;
    P.agentParameter.insert(std::pair<fast::name, fast::name>("energy consumption", std::to_string(rand() % 100 + 1)));
    P.agentParameter.insert(std::pair<fast::name, fast::name>("compute intensity", levels[rand() % 3 ]));
    P.agentParameter.insert(std::pair<fast::name, fast::name>("IO intensity", levels[rand() % 3 ]));
    P.agentParameter.insert(std::pair<fast::name, fast::name>("communication intensity (network)", levels[rand() % 3 ]));
    P.agentParameter.insert(std::pair<fast::name, fast::name>("expected runtime", levels[rand() % 3 ]));
    return P;
}

bool fast::initializeAgents(std::shared_ptr<fast::MQTT_communicator> comm) {
    bool init = false;
    for (auto &item : agentMap) {
        if (item.second.initialized == false) {
            fast::agentProperties P = fast::creatRandomAgentProperties();
            item.second = P;
            item.second.initialized = true;
            std::cout << "Sending Initialization to agent" << item.first << std::endl;
            fast::initAgent(item.first, item.second.agentParameter, item.second.repeat, comm, 2);
            init = true;
        }
    }
    return init;
}

void fast::printInitializedAgents(void) {
    if(agentMap.size() == 0) return;
    std::cout << "Initialized Agent List: ";
    for (auto &item : agentMap) {
        if (item.second.initialized == true) {
            std::cout << item.first << " ";
        }
    }
    std::cout << std::endl;
    //std::cout << "debug1"<<std::endl;
}

void fast::sendRandomStopMonitor(std::shared_ptr<fast::MQTT_communicator> comm)
{
    //random send time
    int t = rand() % 100;
    if (t > 25) return; //send nothing
    // pick a host
    if (agentMap.size() == 0) return;
    auto item = agentMap.begin();
    std::advance( item, rand() % agentMap.size());
    if(item->second.initialized)
    {
        fast::stopMonitor(item->first,std::to_string(rand() % 100000),std::to_string(rand() % 100000),comm,2 );
    }
}
