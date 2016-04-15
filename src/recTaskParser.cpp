/* 
 * File:   recTaskParser.cpp
 * Author: gad
 * 
 * Created on June 29, 2015, 10:06 AM
 */

#include "recTaskParser.h"
#include "task.h"
#include <thread>

recTaskParser::recTaskParser() {
}

/*
recTaskParser::recTaskParser(const recTaskParser& orig) {
    
}*/

recTaskParser::~recTaskParser() {
}

void recTaskParser::load(const YAML::Node& node) {
    //fast::load
    if (node["result"]) {
        if (node["result"].as<std::string>() == std::string("vm started")) {

            std::unique_ptr<task> p(new vmStarted);
            p->load(node);
            p->comm = this->comm;
            tasks.push_back(std::move(p));

        } else if (node["result"].as<std::string>() == std::string("vm stopped")) {

            std::unique_ptr<task> p(new vmStopped());
            p->load(node);
            p->comm = this->comm;
            tasks.push_back(std::move(p));
        } else
            if (node["result"].as<std::string>() == std::string("vm migrated")) {

            std::unique_ptr<task> p(new vmMigrated());
            p->load(node);
            p->comm = this->comm;
            tasks.push_back(std::move(p));
        }
    } else if (node["task"]) {
        if (node["task"].as<std::string>() == std::string("init")) {
            std::unique_ptr<task> p(new initAgent());
            p->load(node);
            p->comm = this->comm;
            tasks.push_back(std::move(p));
        } else if (node["task"].as<std::string>() == std::string("KPI")) {
            std::unique_ptr<task> p(new agentKPI());
            p->load(node);
            p->comm = this->comm;
            tasks.push_back(std::move(p));
        }
        else if(node["task"].as<std::string>() == std::string("configuration")) {
            std::unique_ptr<task> p(new agentStatus());
            p->load(node);
            p->comm = this->comm;
            tasks.push_back(std::move(p));
        }
    }
    else {
        throw std::runtime_error("receiving Unsupported message\n");
        //std::cout << "receiving Unsupported message\n";
    }
}

void recTaskParser::execute() {
    for (auto & item : tasks) {
        //item->execute();
        std::thread t(&task::execute, std::move(item));
        t.detach();
    }
}

YAML::Node recTaskParser::emit() const {
    YAML::Node t;
    return t;
}
