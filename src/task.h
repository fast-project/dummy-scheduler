/* 
 * File:   task.h
 * Author: gad
 *
 * Created on June 26, 2015, 12:20 PM
 */

#ifndef TASK_H
#define	TASK_H
#include "message.h"
#include <vector>
#include <stdio.h>
#include <string>

#include <fast-lib/serializable.hpp>

//namespace fast {

    class task
    : public fast::Serializable {
    public:
        task();
        std::shared_ptr<fast::MQTT_communicator> comm;
        virtual void execute() = 0;
    private:

    };
    //YAML_CONVERT_IMPL(task)

    class vmStarted
    : public task {
    public:
        vmStarted(){} ;
        void execute() override;
        YAML::Node emit() const override;
        void load(const YAML::Node &node) override;
        std::vector < fast::machineConf > vm_configurations;
        fast::name hostname;
    };
    YAML_CONVERT_IMPL(vmStarted)
    
    
    
    class vmStopped
    : public task {
    public:
       vmStopped() {};
        void execute() override;
        YAML::Node emit() const override;
        void load(const YAML::Node &node) override;
        std::vector < fast::machineConf > vm_configurations;
        fast::name hostname;
    };
    YAML_CONVERT_IMPL(vmStopped)
    
    
    class vmMigrated
    : public task {
    public:
        vmMigrated(){};
        void execute() override;
        YAML::Node emit() const override;
        void load(const YAML::Node &node) override;
        fast::name vmName;
        fast::name status;
        fast::name processID;  
    };
    YAML_CONVERT_IMPL(vmMigrated)
            
    class initAgent
    : public task {
    public:
        initAgent(){};
        void execute() override;
        YAML::Node emit() const override;
        void load(const YAML::Node &node) override;
        fast::name hostname;
    };
    YAML_CONVERT_IMPL(initAgent)
    
    class agentKPI
    :   public task {
    public:
        agentKPI(){};
        void execute() override;
        YAML::Node emit() const override;
        void load(const YAML::Node &node) override;
        fast::name hostname;
        fast::name KPIs;
    };
    YAML_CONVERT_IMPL(agentKPI)
            
    class agentStatus
    : public task {
    public:
        agentStatus(){};
        void execute() override;
        YAML::Node emit() const override;
        void load(const YAML::Node &node) override;
        fast::name hostname;
        fast::name status;
    };
    YAML_CONVERT_IMPL(agentStatus)        
//}


#endif	/* TASK_H */

