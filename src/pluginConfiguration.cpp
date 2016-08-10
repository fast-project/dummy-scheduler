/* 
 * File:   pluginConfiguration.cpp
 * Author: gad
 * 
 * Created on June 8, 2015, 6:18 PM
 */

#include <fast-lib/mqtt_communicator.hpp>
#include <mosquittopp.h>

#include "pluginConfiguration.h"
#include <exception>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <sys/types.h>
std::string random_string( size_t length );
pluginConfiguration::pluginConfiguration(const std::string &config_file) {
    // Convert config file to string
    std::ifstream file_stream(config_file);
    std::stringstream string_stream;
    string_stream << file_stream.rdbuf(); // Filestream to stingstream conversion
    auto conf_str = string_stream.str();
    // Load config from string
    from_string(conf_str);
}

YAML::Node pluginConfiguration::emit() const {
    throw std::runtime_error("Task_handler::emit() is not implemented.");
    YAML::Node node;
    return node;
}

void pluginConfiguration::load(const YAML::Node &node) {
    if (!node["communicator"])
        throw std::invalid_argument("No configuration for communication interface.");
    {
        auto comm_node = node["communicator"];
        if (!comm_node["type"])
            throw std::invalid_argument("No type for communication interface in configuration found.");
        auto type = comm_node["type"].as<std::string>();
        if (type == "mqtt") {
            if (!comm_node["id"] || !comm_node["subscribe-topic"] || !comm_node["publish-topic"]
                    || !comm_node["host"] || !comm_node["port"] || !comm_node["keepalive"])
                throw std::invalid_argument("Defective configuration for mqtt communicator.");
            size_t len{100};
            char    hostname[len];
            std::stringstream Idstream;
            gethostname(hostname, len);
            Idstream<<"Current ID : fast-mqtt-"<<hostname<<"-"<< getpid()<<std::endl;
            std::cout << Idstream.str();
            comm = std::make_shared<fast::MQTT_communicator>(
                    //comm_node["id"].as<std::string>(),
                    //random_string(10), // now the ID is generated as random string
		    //std::string{},//giving and empty string
                    Idstream.str(), //now the is set to fast-mqtt-<hostname>-<pid>
                    comm_node["subscribe-topic"].as<std::string>(),
                    comm_node["publish-topic"].as<std::string>(),
                    comm_node["host"].as<std::string>(),
                    comm_node["port"].as<int>(),
                    comm_node["keepalive"].as<int>());
        } else {
            throw std::invalid_argument("Unknown communcation type in configuration found");
        }
    }
}

std::string random_string( size_t length )
{
    auto randchar = []() -> char
    {
        const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}

