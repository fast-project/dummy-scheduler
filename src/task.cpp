/* 
 * File:   task.cpp
 * Author: gad
 * 
 * Created on June 26, 2015, 12:20 PM
 */

#include <iostream>
#include "message.h"

#include "task.h"
#include "pluginConfiguration.h"
#include "agents.h"
extern std::map<fast::hostname,fast::agentProperties>  agentMap;

task::task() {
}

void vmStarted::execute() {
    std::cout << "vm started received" << std::endl;
    for (auto &item : this->vm_configurations) {
        std::cout << "\t\t" << item["vm-name"] << " : " << item["status"] << " " << item["process-id"] << std::endl;
    }
}

YAML::Node vmStarted::emit() const {
    //return 0;
}

void vmStarted::load(const YAML::Node& node) {
    fast::load(this->hostname, node["scheduler"]);
    YAML::Node list = node["list"];
    for (int i = 0; i < (int) list.size(); i++) {
        fast::machineConf mc;
        mc.insert(std::pair<fast::name, fast::name>("vm-name", list[i]["vm-name"].as<std::string>()));
        mc.insert(std::pair<fast::name, fast::name>("status", list[i]["status"].as<std::string>()));
        mc.insert(std::pair<fast::name, fast::name>("process-id", list[i]["process-id"].as<std::string>()));
        this->vm_configurations.push_back(mc);
    }

}

void vmStoped::execute() {
    std::cout << "vm stopped received" << std::endl;
    for (auto &item : this->vm_configurations) {
        std::cout << "\t\t" << item["vm-name"] << " : " << item["status"] << std::endl;
    }
}

void vmStoped::load(const YAML::Node& node) {
    YAML::Node list = node["list"];
    for (int i = 0; i < (int) list.size(); i++) {
        fast::machineConf mc;
        mc.insert(std::pair<fast::name, fast::name>("vm-name", list[i]["vm-name"].as<std::string>()));
        mc.insert(std::pair<fast::name, fast::name>("status", list[i]["status"].as<std::string>()));
        this->vm_configurations.push_back(mc);
    }

}

YAML::Node vmStoped::emit() const {
    //return 0;
}

void vmMigrated::execute() {
    std::cout << "vm migrated received" << std::endl;
    std::cout << "\t\t" << this->vmName << "  " << this->status << "  " << this->processID << std::endl;
}

YAML::Node vmMigrated::emit() const {
    //return 0;
}

void vmMigrated::load(const YAML::Node& node) {
    this->vmName = node["vm-name"].as<std::string>();
    this->status = node["status"].as<std::string>();
    this->processID = node["process-id"].as<std::string>();
}

void initAgent::execute() {
    std::cout << "Agent init received" << " " << "hostname: " << this->hostname << std::endl;
    
    fast::agentProperties * agPro = new fast::agentProperties;
    agPro->initialized = false;
    fast::hostname agHost = this->hostname;
    agentMap.insert(std::pair<fast::hostname,fast::agentProperties>(agHost,*agPro));
    delete agPro;
         
}

YAML::Node initAgent::emit() const {

}

void initAgent::load(const YAML::Node& node) {
    this->hostname = node["source"].as<std::string>();
}