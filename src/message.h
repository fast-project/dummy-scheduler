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
        STARTVM, STOPVM, MIGRATEVM
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

        startvm(name topic, std::vector < machineConf > vm_conf, std::shared_ptr<fast::MQTT_communicator> comm, int Qos)
        : message(topic, comm, "scheduler", STARTVM, Qos), vm_configurations(vm_conf) {
            this->send();
        }
        std::vector < machineConf > vm_configurations;

        // Override these two methods to state which members should be serialized
        YAML::Node emit() const override;
        void load(const YAML::Node &node) override;

    };

    class stopvm : public message {
    public:

        stopvm(name topic, machines vmMachines, std::shared_ptr<fast::MQTT_communicator> comm, int Qos)
        : message(topic, comm, "scheduler", STARTVM, Qos), vmMachines(vmMachines) {
            this->send();
        }
        machines vmMachines;

        // Override these two methods to state which members should be serialized
        YAML::Node emit() const override;
        void load(const YAML::Node &node) override;
    };

    class migratevm : public message {
    public:

        migratevm(name topic, name vm_name, name destination, parameter par, std::shared_ptr<fast::MQTT_communicator> comm, int Qos)
        : message(topic, comm, "scheduler", MIGRATEVM, Qos), vm_name(vm_name), destination(destination), par(par) {
            this->send();
        }
        name vm_name;
        name destination;
        parameter par;
        // Override these two methods to state which members should be serialized
        YAML::Node emit() const override;
        void load(const YAML::Node &node) override;
    };
}
#endif	/* MESSAGE_H */

