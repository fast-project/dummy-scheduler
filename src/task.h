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

#include <fast-lib/serialization/serializable.hpp>

//namespace fast {

    class task
    : public fast::Serializable {
    public:
        task();
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
    
    
    
    class vmStoped
    : public task {
    public:
       vmStoped() {};
        void execute() override;
        YAML::Node emit() const override;
        void load(const YAML::Node &node) override;
        std::vector < fast::machineConf > vm_configurations;
        fast::name hostname;
    };
    YAML_CONVERT_IMPL(vmStoped)
    
    
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
            
//}


#endif	/* TASK_H */

