/* 
 * File:   message.h
 * Author: gad
 *
 * Created on June 9, 2015, 10:26 AM
 */

#ifndef MESSAGE_H
#define	MESSAGE_H

#include <string>
#include <vector>
#include <map>

#include <fast-lib/serialization/serializable.hpp>
#include <fast-lib/communication/mqtt_communicator.hpp>
namespace fast {

    enum messageType {
        STARTVM, STOPVM, MIGRATEVM, INITAGENT, STOPMONITOR
    };
    typedef std::string name;
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

        startvm(name hostname, std::vector < machineConf > & vm_conf, std::shared_ptr<fast::MQTT_communicator> comm, int Qos)
        : message(std::string("fast/migfra/") + hostname + std::string("/task"), comm, "scheduler", STARTVM, Qos), vm_configurations(vm_conf), hostname(hostname) {
            this->send();
        }
        std::vector < machineConf > vm_configurations;
        name hostname;

        // Override these two methods to state which members should be serialized
        YAML::Node emit() const override;
        void load(const YAML::Node &node) override;

    };

    class stopvm : public message {
    public:
        name hostname;
        machines vmMachines;

        stopvm(name host, machines vmMachines, std::shared_ptr<fast::MQTT_communicator> comm, int Qos)
        : hostname(host), vmMachines(vmMachines),
        message(std::string("fast/migfra/") + host + std::string("/task")
        , comm, "scheduler", STARTVM, Qos) {
            this->send();
        }

        // Override these two methods to state which members should be serialized
        YAML::Node emit() const override;
        void load(const YAML::Node &node) override;
    };

    class migratevm : public message {
    public:

        migratevm(name host, name vm_name, name destination, parameter par, std::shared_ptr<fast::MQTT_communicator> comm, int Qos)
        : hostname(host), vm_name(vm_name), destination(destination), par(par),
        message(std::string("fast/migfra/") + host + std::string("/task"), comm, "scheduler", MIGRATEVM, Qos) {
           this->send();
        }
        name hostname;
        name vm_name;
        name destination;
        parameter par;
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
        host(host), categories(categories), repeat(repeat),
        message(std::string("fast/agent/") + host + std::string("/task"),
        comm,
        "scheduler", INITAGENT, Qos) {
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
        host(host), job_id(job_id), process_id(process_id),
        message(std::string("fast/agent/") + host + std::string("/task"),
        comm,
        "scheduler", STOPMONITOR, Qos) {
            this->send();
        };
        // Override these two methods to state which members should be serialized
        YAML::Node emit() const override;
        void load(const YAML::Node &node) override;
    };


}
#endif	/* MESSAGE_H */

