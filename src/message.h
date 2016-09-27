/* 
 * File:   message.h
 * Author: gad
 *
 * Created on June 9, 2015, 10:26 AM
 */

#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <vector>
#include <map>

#include <fast-lib/serializable.hpp>
#include <fast-lib/mqtt_communicator.hpp>
namespace fast {

    enum messageType {
        STARTVM, STOPVM, MIGRATEVM, INITAGENT, STOPMONITOR, REQUESTKPI };
    typedef std::string name;
    typedef std::string hostname;
    typedef std::vector<name> machines;
    typedef std::map <name, name > machineConf;
    typedef std::map <name, name > parameter;

    class message : public fast::Serializable {
    public:
        message() = default;

        message(name topic, std::shared_ptr<fast::MQTT_communicator> comm, name source, messageType type, int Qos)
        : source(source), topic(topic), type(type), Qos(Qos), comm(comm) {
        }

        virtual ~message() {
        };

        name source;
        name topic;
        messageType type;
        int Qos;

        std::shared_ptr<fast::MQTT_communicator > comm;


        void send();
        // Override these two methods to state which members should be serialized
        //YAML::Node emit() const override;
        //void load(const YAML::Node &node) override;
    };


    // type (STARTVM), source("scheduler"), topic(topic), comm(comm),  , Qos(Qos)

    class startvm : public message {
    public:

        startvm(name hostname,name UUID, YAML::Node & vm_conf, std::shared_ptr<fast::MQTT_communicator> comm, int Qos)
        : message(std::string("fast/migfra/") + hostname + std::string("/task"), comm, "scheduler", STARTVM, Qos), UUID(UUID) ,vm_configurations(vm_conf), hostname(hostname) {
            this->send();
        }
        name UUID;

        YAML::Node vm_configurations;
        name hostname;

        // Override these two methods to state which members should be serialized
        YAML::Node emit() const override;
        void load(const YAML::Node &node) override;

    };

    class stopvm : public message {
    public:
        name hostname;
        machines vmMachines;
        name UUID;
        stopvm(name host, name UUID , machines vmMachines, std::shared_ptr<fast::MQTT_communicator> comm, int Qos)
        : message(std::string("fast/migfra/") + host + std::string("/task")
        , comm, "scheduler", STARTVM, Qos),hostname(host), vmMachines(vmMachines), UUID(UUID)
         {
            this->send();
        }

        // Override these two methods to state which members should be serialized
        YAML::Node emit() const override;
        void load(const YAML::Node &node) override;
    };

    class migratevm : public message {
    public:

        migratevm(name host, name UUID, name vm_name, name destination, parameter par, std::shared_ptr<fast::MQTT_communicator> comm, int Qos)
        : message(std::string("fast/migfra/") + host + std::string("/task"), comm, "scheduler", MIGRATEVM, Qos),
        hostname(host), vm_name(vm_name), destination(destination), par(par), UUID(UUID) {
            this->send();
        }
        name hostname;
        name vm_name;
        name destination;
        parameter par;
        name UUID;
        // Override these two methods to state which members should be serialized
        YAML::Node emit() const override;
        void load(const YAML::Node &node) override;
    };

    class initAgent : public message {
    public:
        name host;
        parameter categories;
        name repeat;

        initAgent(name host, parameter categories, name repeat, std::shared_ptr<fast::MQTT_communicator> comm, int Qos) :
        message(std::string("fast/agent/") + host + std::string("/task/init_agent"),
        comm,
        "scheduler", INITAGENT, Qos),
        host(host), categories(categories), repeat(repeat)
         {
            this->send();

        };
        // Override these two methods to state which members should be serialized
        YAML::Node emit() const override;
        void load(const YAML::Node &node) override;
    };

    class stopMonitor : public message {
    public:
        name host;
        name job_id;
        name process_id;

        stopMonitor(name host, name job_id, name process_id, std::shared_ptr<fast::MQTT_communicator> comm, int Qos) :
        message(std::string("fast/agent/") + host + std::string("/task/stop_monitoring"),
        comm,
        "scheduler", STOPMONITOR, Qos),
        host(host), job_id(job_id), process_id(process_id) {
            this->send();
        };
        // Override these two methods to state which members should be serialized
        YAML::Node emit() const override;
        void load(const YAML::Node &node) override;
    };

    class requestKPI: public message {
    public:
        fast::name host;
        std::vector<fast::name> corelist;
        requestKPI(name host, std::vector<name> corelist, std::shared_ptr<fast::MQTT_communicator> comm, int Qos) :   
        message( std::string("fast/agent/") + host +std::string("/mmbwmon/request"),
        comm,"scheduler",REQUESTKPI,Qos),
        host(host),
         corelist(corelist){
            this->send();
        };
        // Override these two methods to state which members should be serialized
        YAML::Node emit() const override;
        void load(const YAML::Node &node) override;
    };

}
#endif /* MESSAGE_H */

