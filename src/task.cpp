/* 
 * File:   task.cpp
 * Author: gad
 * 
 * Created on June 26, 2015, 12:20 PM
 */

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp>
#include "message.h"
#include <unistd.h>
#include <fcntl.h>
#include "task.h"
#include "pluginConfiguration.h"
#include "agents.h"
extern std::map<fast::hostname, fast::agentProperties> agentMap;
extern YAML::Node configPublic;

task::task() {
}

void vmStarted::execute() {
    std::cout << "vm started received" << std::endl;
    for (auto &item : this->vm_configurations) {
        std::cout << "\t\t" << item["vm-name"] << " : " << item["status"] << " " << item["process-id"] << std::endl;
    }
    for (auto &item : this->vm_configurations) {
        std::ofstream outputfile;
        std::string directory = configPublic["receive"]["path"].as<std::string>()
                + "/status/" + item["vm-name"];
        boost::filesystem::create_directories(boost::filesystem::path{directory});
        
        /*
        std::string fifoName = directory + "/vmStarted";


        if (access(fifoName.c_str(), F_OK) != -1) {
            // file exists
        } else {
            // file doesn't exist
            mkfifo(fifoName.c_str(), 0666);
        }
        int fd = open(fifoName.c_str(), O_WRONLY);
        std::string data = item["status"] + "\n";
        write(fd, data.c_str(), data.size());
        close(fd);
        */
    
        outputfile.open(directory + "/vmStarted", std::ios::trunc);
        if (outputfile.is_open()) {
            outputfile << item["status"] << std::endl;
        } else {
            std::cout << "Error opening file: " << directory + "/vmStarted" << std::endl;
        }
        outputfile.close();
    }
}

YAML::Node vmStarted::emit() const {
    YAML::Node n;
    return n;
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

void vmStopped::execute() {
    std::cout << "vm stopped received" << std::endl;
    for (auto &item : this->vm_configurations) {
        std::cout << "\t\t" << item["vm-name"] << " : " << item["status"] << std::endl;
    }
    std::cout << "Receive Path " << configPublic["receive"]["path"] << std::endl;
    for (auto &item : this->vm_configurations) {
        std::ofstream outputfile;
        std::string directory = configPublic["receive"]["path"].as<std::string>()
                + "/status/" + item["vm-name"];
        boost::filesystem::create_directories(directory);
        /*
        std::string fifoName = directory + "/vmStopped";

        if (access(fifoName.c_str(), F_OK) != -1) {
            // file exists
        } else {
            // file doesn't exist
            mkfifo(fifoName.c_str(), 0666);
        }
        int fd = open(fifoName.c_str(), O_WRONLY);
        std::string data = item["status"] + "\n";
        write(fd, data.c_str(), data.size());
        close(fd);
        */
        
        outputfile.open(directory + "/vmStopped", std::ios::trunc);
        if (outputfile.is_open()) {
            outputfile << item["status"] << std::endl;
        } else {
            std::cout << "Error opening file" << directory + "/vmStopped" << std::endl;
        }
        outputfile.close();
    }
}

void vmStopped::load(const YAML::Node& node) {
    YAML::Node list = node["list"];
    for (int i = 0; i < (int) list.size(); i++) {
        fast::machineConf mc;
        mc.insert(std::pair<fast::name, fast::name>("vm-name", list[i]["vm-name"].as<std::string>()));
        mc.insert(std::pair<fast::name, fast::name>("status", list[i]["status"].as<std::string>()));
        this->vm_configurations.push_back(mc);
    }
}

YAML::Node vmStopped::emit() const {
    YAML::Node n;
    return n;
}

void vmMigrated::execute() {
    std::cout << "vm migrated received" << std::endl;
    std::cout << "\t\t" << this->vmName << "  " << this->status << "  " << this->processID << std::endl;
    std::ofstream outputfile;
    std::string directory = configPublic["receive"]["path"].as<std::string>()
            + "/status/" + this->vmName;
    boost::filesystem::create_directories(directory);

    /*std::string fifoName = directory + "/vmMigrated";

    if (access(fifoName.c_str(), F_OK) != -1) {
        // file exists
    } else {
        // file doesn't exist
        mkfifo(fifoName.c_str(), 0666);
    }
    int fd = open(fifoName.c_str(), O_WRONLY);
    std::string data = this->status + " " + this->processID + "\n";
    write(fd, data.c_str(), data.size());
    close(fd);
     */
    
    
    outputfile.open(directory + "/vmMigrated", std::ios::trunc);
    if (outputfile.is_open()) {
    outputfile << this->status << " " << this->processID << std::endl;
    } else {
    std::cout << "Error opening file" << directory + "/vmMigrated" << std::endl;
    }
    outputfile.close();
}

YAML::Node vmMigrated::emit() const {
    YAML::Node n;
    return n;
}

void vmMigrated::load(const YAML::Node& node) {
    this->vmName = node["name"].as<std::string>();
    this->status = node["status"]["result"].as<std::string>();
    this->processID = node["status"]["process-id"].as<std::string>();
}

void initAgent::execute() {
    std::cout << "Agent init received" << " " << "hostname: " << this->hostname << std::endl;

    fast::agentProperties * agPro = new fast::agentProperties;
    agPro->initialized = false;
    fast::hostname agHost = this->hostname;
    agentMap.insert(std::pair<fast::hostname, fast::agentProperties>(agHost, *agPro));
    delete agPro;
}

YAML::Node initAgent::emit() const {
    YAML::Node n;
    return n;
}

void initAgent::load(const YAML::Node& node) {
    this->hostname = node["source"].as<std::string>();
}

void agentKPI::load(const YAML::Node& node) {
    //this->hostname = node["source"].as<std::string>();
    this->KPIs = node["result"].as<std::string>();
}

YAML::Node agentKPI::emit() const {
    YAML::Node n;
    return n;
}

void agentKPI::execute() {
    std::cout << " KPI received for host " + this->hostname + " result: " + this->KPIs +"\n";
    std::ofstream outputfile;
    std::string directory = configPublic["receive"]["path"].as<std::string>()
            + "/KPIs/";
    boost::filesystem::create_directories(directory);

    std::string fileName = directory + this->hostname;

    outputfile.open(fileName, std::ios::out | std::ios::trunc);
    if (outputfile.is_open()) {
        outputfile << this->KPIs << std::endl;
        outputfile.close();
    } else {
        std::cerr << "agentKPI:: Error opening file" << fileName << std::endl;
    }
}

void agentStatus:: load(const YAML::Node& node) {
    this->hostname = node["source"].as<std::string>();
    this->status = node["status"].as<std::string>();
}

YAML::Node agentStatus::emit() const {
    YAML::Node n;
    return n;
}

void agentStatus:: execute() {
    std::cout <<" Agent status for host " + this->hostname + " " + this->status << std::endl;
    
    std::ofstream outputfile;
    std::string directory = configPublic["receive"]["path"].as<std::string>()
            + "/agents/";
    boost::filesystem::create_directories(directory);

    std::string fileName = directory + this->hostname;

    outputfile.open(fileName, std::ios::out | std::ios::trunc);
    if (outputfile.is_open()) {
        outputfile << this->status << std::endl;
        outputfile.close();
    } else {
        std::cerr << "agentStatus:: Error opening file" << fileName << std::endl;
    }
}
