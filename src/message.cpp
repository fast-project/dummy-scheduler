/* 
 * File:   message.cpp
 * Author: gad
 * 
 * Created on June 9, 2015, 10:26 AM
 */

#include "message.h"
#include "fast-lib/communication/mqtt_communicator.hpp"

using namespace fast;

void message::send() {
    this->comm->send_message(this->to_string(), this->topic, this->Qos);
}

YAML::Node startvm::emit() const {
    YAML::Node node, node1;
    node["host"] = hostname;
    node["task"] = "start vm";
    for (auto &item : this->vm_configurations) {
        YAML::Node node2;
        for (auto &mp : item) {
            node2[mp.first] = mp.second;
        }
        node1.push_back(node2);
    }
    node["vm-configurations"] = node1;
    return node;
}

void startvm::load(const YAML::Node &node) {
    throw std::runtime_error("startvm::load "+ node.as<name>()+ "  not supported\n");
}

YAML::Node fast::stopvm::emit() const {
    YAML::Node node, node1;
    node["host"] = hostname;
    node["task"] = "stop vm";
    for (auto &item : this->vmMachines) {
        YAML::Node node2;
        node2["vm-name"] = item;
        node1.push_back(node2);
    }
    node["list"] = node1;
    //node["vm-configurations"] = node1;
    return node;
}

void stopvm::load(const YAML::Node &node) {
    throw std::runtime_error("stopvm::load "+ node.as<name>()+ " not supported\n");
}


YAML::Node fast::migratevm::emit() const {
    YAML::Node node, node1;
    node["host"] = hostname;
    node["task"] = "migrate vm";
    node["vm-name"] = this->vm_name;
    node["destination"] = this->destination;
    for(auto &item : this->par)
    {
        node1[item.first]=item.second;
    }
    node["parameter"] = node1;
    return node;
}

void migratevm::load(const YAML::Node &node){
    throw std::runtime_error("migratevm::load "+ node.as<name>()+ " not supported\n");
}

YAML::Node fast::initAgent::emit() const {
    YAML::Node node,  categ;
    node["task"] = "init agent";
    for(auto &item :categories)
    {
        categ[item.first]= item.second;
    }
    node["KPI"]["categories"] = categ;
    node["KPI"]["repeat"] = repeat;
    return node;
}
void fast::initAgent::load(const YAML::Node &node) {
    throw std::runtime_error("initAgent::load "+ node.as<name>()+ " not supported\n");
}

YAML::Node fast::stopMonitor::emit() const {
    YAML::Node node;
    node["task"] = "stop monitor";
    node["job-description"]["job-id"] = job_id;
    node["job-description"]["process-id"] = process_id;
    return node;
}

void fast::stopMonitor::load(const YAML::Node& node) {
    throw std::runtime_error("stopMonitor::load "+ node.as<name>()+ " not supported\n");
}



