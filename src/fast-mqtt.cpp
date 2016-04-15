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
#include <vector>
#include <fstream>
#include <istream>
#include "pluginConfiguration.h"
#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp>
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
        bo::positional_options_description p;
        p.add("Command Parameter", -1);
        desc.add_options()
                ("help,h", "produce help message")
                ("config,c", bo::value<std::string>(), "path to config file")
                ("startvm", "startvm <hostname> <vmname> <XML configuration>")
                ("stopvm", "stopvm <hostname> <vmname>")
                ("migratevm", "migratevm <vmname> <source hostname> <destination hostname>")
                ("listen", "listen the scheduler MQTT related channel for the nodes defined in the node list")
                ("stopAgent", "stopAgent <hostname> <jobID> <processID>")
                ("initAgents", "initAgents <hostlist>")
                ("statusAgents", " ")
                ("statusStartvm", "statusStartvm <vmname>")
                ("statusStopvm", "statusStopvm <vmname>")
                ("statusMigratevm", "statusMigratevm <vmname>")
                //("input-file",bo::value< std::vector<std::string> >(),"test")
                ;
        bo::options_description hidden("Hidden options");
        hidden.add_options()
                ("Command Parameter", bo::value< std::vector<std::string> >(), "Command Parameter")
                ;
        bo::options_description visible("Allowed options");
        visible.add(desc);
        bo::options_description cmdline_options;
        cmdline_options.add(desc).add(hidden);
        bo::variables_map vm;
        //bo::store(bo::parse_command_line(argc, argv, desc), vm);
        bo::store(bo::command_line_parser(argc, argv).options(cmdline_options).positional(p).run(), vm);
        bo::notify(vm);
        if (vm.count("help")) {
            std::cout << desc << std::endl;
            return EXIT_SUCCESS;
        }
        std::string config_file_name = "scheduler.conf";
        if (vm.count("config"))
            config_file_name = vm["config"].as<std::string>();

        pluginConfiguration conf(config_file_name);
        configPublic = YAML::LoadFile(config_file_name);




        if (vm.count("startvm")) {
            if (vm.count("Command Parameter")) {

                std::vector<std::string> arguments = vm["Command Parameter"].as<std::vector < std::string >> ();
                std::vector<fast::machineConf> confs;
                confs.push_back({
                    {"name", arguments[1]},
                    {"xml", arguments[2]},
                    {"overlay-image", configPublic["vm"]["overlay-image"].as<std::string>() + arguments[1]},
                    {"base-image", configPublic["vm"]["base-image"].as<std::string>()}
                });
                fast::startvm(arguments[0], configPublic["vm"]["UUID"].as<std::string>(), confs, conf.comm, 2);
            }
        }



        if (vm.count("stopvm")) {
            if (vm.count("Command Parameter")) {
                std::vector<std::string> arguments = vm["Command Parameter"].as<std::vector < std::string >> ();

                fast::stopvm(arguments[0], configPublic["vm"]["UUID"].as<std::string>(), {
                    arguments[1]
                }, conf.comm, 2);
            }
        }



        if (vm.count("migratevm")) {
            if (vm.count("Command Parameter")) {
                std::vector<std::string> arguments = vm["Command Parameter"].as<std::vector < std::string >> ();
                fast::migratevm(arguments[1], configPublic["vm"]["UUID"].as<std::string>(), arguments[0], arguments[2],{
                    {"retry-counter", configPublic["vm"]["retry-counter"].as<std::string>()},
                    {"migration-type", configPublic["vm"]["migration-type"].as<std::string>()},
                    {"rdma-migration", configPublic["vm"]["rdma-migration"].as<std::string>()}
                }, conf.comm, 2);
            }
        }

        if (vm.count("listen")) {
            recMessageHandler receive(true, conf.comm);
            receive.addTopic("fast/migfra/+/status", 2);
            receive.addTopic("fast/agent/+/status", 2);
            receive.run();
        }

        if (vm.count("stopAgent")) {
            if (vm.count("Command Parameter")) {
                std::vector<std::string> arguments = vm["Command Parameter"].as<std::vector < std::string >> ();
                fast::stopMonitor(arguments[0], arguments[1], arguments[2], conf.comm, 2);
            }
        }

        if (vm.count("initAgents")) {
            fast::name hostfile;
            if (vm.count("Command Parameter")) {
                std::vector<std::string> arguments = vm["Command Parameter"].as<std::vector < std::string >> ();
                hostfile = {arguments[0]};
            } else {
                hostfile = {configPublic["agent"]["hostlist"].as<std::string>()};
            }
            /*std::ifstream ifstream_hostlist;
            ifstream_hostlist.open(hostfile);
            if(ifstream_hostlist.is_open()){
                
            } else
            {
                std::cout << "initAgents:: Error unable to open hostlist " << hostfile << std::endl;
            }*/
            YAML::Node nHostfile = YAML::LoadFile(hostfile);
            std::vector<fast::name> vhostlist;
            assert(nHostfile.IsSequence());
            for (std::size_t i = 0; i < nHostfile.size(); i++) {
                vhostlist.push_back(nHostfile[0].as<fast::name>());
            }
            fast::parameter AgentConf{
                {"energy consumption", configPublic["agent"]["defaultConf"]["energy consumption"].as<std::string>()},
                {"compute intensity", configPublic["agent"]["defaultConf"]["compute intensity"].as<std::string>()},
                {"IO intensity", configPublic["agent"]["defaultConf"]["IO intensity"].as<std::string>()},
                {"communication intensity (network)", configPublic["agent"]["defaultConf"]["communication intensity (network)"].as<std::string>()},
                {"expected runtime", configPublic["agent"]["defaultConf"]["expected runtime"].as<std::string>()}
            };

            for (auto &host : vhostlist) {
                fast::initAgent(host, AgentConf, configPublic["agent"]["repeat"].as<std::string>(), conf.comm, 2);
            }
        }

        if (vm.count("statusStartvm")) {
            fast::name vmname;
            if (vm.count("Command Parameter")) {
                std::vector<std::string> arguments = vm["Command Parameter"].as<std::vector < std::string >> ();
                vmname = {arguments[0]};
                std::string directory = configPublic["receive"]["path"].as<std::string>()
                        + "/status/" + vmname;
                std::ifstream ofS;
                ofS.open(directory + "vmStarted");
                if (ofS.is_open()) {
                    std::string outbuffer;
                    std::getline(ofS, outbuffer);
                    if (outbuffer.size() < 2) {
                        std::cout << "Undefined" << std::endl;
                    } else {
                        std::cout << outbuffer << std::endl;
                    }
                } else {
                    std::cerr << "Error statusStartvm: unable to open " << directory + "vmStarted\n";
                }
            }
        }

        if (vm.count("statusStopvm")) {
            fast::name vmname;
            if (vm.count("Command Parameter")) {
                std::vector<std::string> arguments = vm["Command Parameter"].as<std::vector < std::string >> ();
                vmname = {arguments[0]};
                std::string directory = configPublic["receive"]["path"].as<std::string>()
                        + "/status/" + vmname;
                std::ifstream ofS;
                ofS.open(directory + "vmStopped");
                if (ofS.is_open()) {
                    std::string outbuffer;
                    std::getline(ofS, outbuffer);
                    if (outbuffer.size() < 2) {
                        std::cout << "Undefined" << std::endl;
                    } else {
                        std::cout << outbuffer << std::endl;
                    }
                } else {
                    std::cerr << "Error statusStopvm: unable to open " << directory + "vmStopped\n";
                }
            }
        }

        if (vm.count("statusMigratevm")) {
            fast::name vmname;
            if (vm.count("Command Parameter")) {
                std::vector<std::string> arguments = vm["Command Parameter"].as<std::vector < std::string >> ();
                vmname = {arguments[0]};
                std::string directory = configPublic["receive"]["path"].as<std::string>()
                        + "/status/" + vmname;
                std::ifstream ofS;
                ofS.open(directory + "vmMigrated");
                if (ofS.is_open()) {
                    std::string outbuffer;
                    std::getline(ofS, outbuffer);
                    if (outbuffer.size() < 2) {
                        std::cout << "Undefined" << std::endl;
                    } else {
                        std::cout << outbuffer << std::endl;
                    }
                } else {
                    std::cerr << "Error statusStopvm: unable to open " << directory + "vmMigrated\n";
                }
            }


        }

        if (vm.count("statusAgents")) {
            fast::name hostfile;
            if (vm.count("Command Parameter")) {
                std::vector<std::string> arguments = vm["Command Parameter"].as<std::vector < std::string >> ();
                hostfile = {arguments[0]};
            } else {
                hostfile = {configPublic["agent"]["hostlist"].as<std::string>()};
            }
            YAML::Node nHostfile = YAML::LoadFile(hostfile);
            std::vector<fast::name> vhostlist;
            assert(nHostfile.IsSequence());
            for (std::size_t i = 0; i < nHostfile.size(); i++) {
                vhostlist.push_back(nHostfile[0].as<fast::name>());
            }

            std::string directory = configPublic["receive"]["path"].as<std::string>()
                    + "/agents/";
            boost::filesystem::create_directories(directory);
            bool inProgress{false};
            bool failed{false};
            for (fast::name & item : vhostlist) {
                std::ifstream inputfile;

                std::string fileName = directory + item;

                inputfile.open(fileName);
                if (inputfile.is_open()) {
                    fast::name status;
                    inputfile >> status;
                    if (status == "accepted") {
                        ;
                    } else if (status == "denied") {
                        std::cout << "Host " << item << "denied \n";
                        failed = {true};
                    } else {
                        std::cerr << "statusAgents: unknown status";
                        return 0;
                    }

                } else {
                    inProgress = {true};

                }
            }
            if (failed) {
                std::cout << "Failed\n";
                return 0;
            }
            if (inProgress) {
                std::cout << "In progress" << std::endl;
                return 0;
            }
            std::cout << "success\n";
            return 0;
        }
        /*
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



        fast::startvm("test","UUID1", confs, conf.comm, 2);

        fast::stopvm("test","UUID2",{"anthe1", "centos660"}, conf.comm, 2);

        fast::migratevm("test", "UUID3", "anthe1", "node45",{
            {"live-migration", "false"}
        }, conf.comm, 2);

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
         */
        //}
    } catch (const std::exception &e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }

    return EXIT_SUCCESS;

}