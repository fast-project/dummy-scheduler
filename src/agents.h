/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   agents.h
 * Author: gad
 *
 * Created on December 3, 2015, 10:15 AM
 */

#ifndef AGENTS_H
#define AGENTS_H
#include "message.h"
#include <stdlib.h> 
#include <map>
#include <string>

namespace fast {
struct agentProperties{
    
    fast::parameter agentParameter;
    fast::name repeat;
    bool initialized;
    //agentProperties& operator=(agentProperties other){
    //    std::swap()
    //}
};

fast::agentProperties creatRandomAgentProperties();
/* bool initializeAgent(void);
 * return true if an initialization to any agent is done
 */ 
bool initializeAgents(std::shared_ptr<fast::MQTT_communicator> comm);
void printInitializedAgents(void);
void sendRandomStopMonitor(std::shared_ptr<fast::MQTT_communicator> comm);
}
extern std::map<fast::hostname, fast::agentProperties> agentMap;
 
#endif /* AGENTS_H */

