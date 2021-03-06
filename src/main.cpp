/*
# This file is a part of dummy scheduler.
# Copyright (C) 2015 Datenverarbeitung (ZDV) Uni-Mainz.
#
# This file is licensed under the GNU Lesser General Public License Version 3
# Version 3, 29 June 2007. For details see 'LICENSE.md' in the root directory.
 */



#include <boost/program_options.hpp>

#include <unistd.h>
#include <cstdlib>
#include <string>
#include <exception>
#include <iostream>
#include <unistd.h>
#include "pluginConfiguration.h"
#include "message.h"
#include "recMessageHandler.h"
#include "agents.h"
//Global Variable
std::map<fast::hostname, fast::agentProperties> agentMap;
YAML::Node configPublic;
int main(int argc, char *argv[]) {
    try {
        namespace bo = boost::program_options;
        bo::options_description desc("Options");
        desc.add_options()
                ("help,h", "produce help message")
                ("config,c", bo::value<std::string>(), "path to config file");
        bo::variables_map vm;
        bo::store(bo::parse_command_line(argc, argv, desc), vm);
        bo::notify(vm);
        if (vm.count("help")) {
            std::cout << desc << std::endl;
            return EXIT_SUCCESS;
        }
        std::string config_file_name = "scheduler.conf";
        if (vm.count("config"))
            config_file_name = vm["config"].as<std::string>();

        pluginConfiguration conf(config_file_name);
        //fast::globalPlgConf = &conf;
        recMessageHandler receive(false, conf.comm);
        //std::cout << "debug1\n";
        receive.addTopic("fast/migfra/+/status", 2);
        //std::cout << "debug2\n";
        receive.addTopic("fast/agent/+/status", 2);
        //std::cout << "debug3\n";

        //while (1) {
        std::vector<fast::machineConf> confs;
        confs.push_back({
            {"name", "anthe1"},
            {"vcpus", "1"},
            {"memory", "1048576"}
        });
        confs.push_back({
            {"name", "centos660"},
            {"vcpus", "1"},
            {"memory", "524288"}
        });



        //fast::startvm("test","UUID1", confs, conf.comm, 2);

        fast::stopvm("test","UUID2",{"anthe1", "centos660"}, conf.comm, 2);

//        fast::migratevm("test", "UUID3", "anthe1", "node45",{
//            {"live-migration", "false"}
//        }, conf.comm, 2);

        //fast::startvm("test", confs, conf.comm, 2);

        //fast::initAgent("test", {{{"energy consumption"},{"100"}},{{"IO intensity"},{"low"}}}, "120", conf.comm ,2);

        //fast::stopMonitor("test","ID4256", "154268", conf.comm ,2 );

        while (1) {
            //std::cout << "receiving" << std::endl;
            receive.run();
            sleep(1);
            fast::initializeAgents(conf.comm);
            fast::printInitializedAgents();
            fast::sendRandomStopMonitor(conf.comm);

        }
        //}
    } catch (const std::exception &e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    return EXIT_SUCCESS;

}
