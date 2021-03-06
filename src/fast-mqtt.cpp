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
#include <ostream>

#include <sys/types.h> 
#include <sys/stat.h>
#include <fcntl.h>

#include "pluginConfiguration.h"
#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp>
#include "message.h"
#include "recMessageHandler.h"
#include "agents.h"
#include "utilites.h"
//#include <fast-lib/log.hpp>
//FASTLIB_LOG_INIT(comm_log, "SHE")

//FASTLIB_LOG_SET_LEVEL_GLOBAL(comm_log, trace);
//Global Variable
std::map<fast::hostname, fast::agentProperties> agentMap;
YAML::Node configPublic;
//tLIB_LOG_INIT(comm_test_log, "communication tests")

int main(int argc, char *argv[]) {

    //FASTLIB_LOG_SET_LEVEL(comm_test_log, trace);

    try {
        namespace bo = boost::program_options;
        bo::options_description desc("Options");
        bo::positional_options_description p;
        p.add("Command Parameter", -1);
        desc.add_options()
                ("help,h", "produce help message")
                ("config,c", bo::value<std::string>(), "path to config file, default path is /cluster/MQTT/dummy-scheduler/scheduler.conf")
                ("startvm", "<hostname> <vmname> <XML configuration>")
                ("stopvm", "(<hostname> <vmname>)  or (<delay in seconds> <hostname> <vmname>)")
                ("migratevm", "<vmname> <source hostname> <destination hostname> <cpu-map>")
                ("repinvm", "<hostname> <vmname> <cpu-map>")
                ("listen", "the scheduler MQTT related channel for the nodes defined in the node list")
                ("stopAgent", "<hostname> <jobID> <processID>")
                ("initAgents", "<hostlist>")
                ("statusAgents", " ")
                ("statusStartvm", "<vmname>")
                ("statusStopvm", "<vmname>")
                ("statusMigratevm", "<vmname>")
                ("collectKPI", "<nodelist file> <periode>")
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
        std::string config_file_name = "/cluster/MQTT/dummy-scheduler/scheduler.conf";
        if (vm.count("config"))
            config_file_name = vm["config"].as<std::string>();


        configPublic = YAML::LoadFile(config_file_name);
        if (vm.count("statusStartvm")) {
            fast::name vmname;

            if (vm.count("Command Parameter")) {

                std::vector<std::string> arguments = vm["Command Parameter"].as<std::vector < std::string >> ();
                vmname = {arguments[0]};
                std::string directory = configPublic["receive"]["path"].as<std::string>()
                        + "/status/" + vmname;
                std::ifstream ofS;

                ofS.open(directory + "/vmStarted");

                if (ofS.is_open()) {
                    //std::cout << "Openning "<< directory + "/vmStarted" << std::endl;
                    std::string outbuffer;
                    std::getline(ofS, outbuffer);
                    if (outbuffer.size() < 2) {
                        std::cout << "Undefined" << std::endl;
                    } else {
                        std::cout << outbuffer << std::endl;
                        ofS.close();
                        clearFile(directory + "/vmStarted");
                    }
                } else {
                    std::cerr << "Error statusStartvm: unable to open " << directory + "/vmStarted\n";
                }
            }
            return 0;
        }

        if (vm.count("statusStopvm")) {
            fast::name vmname;
            if (vm.count("Command Parameter")) {
                std::vector<std::string> arguments = vm["Command Parameter"].as<std::vector < std::string >> ();
                vmname = {arguments[0]};
                std::string directory = configPublic["receive"]["path"].as<std::string>()
                        + "/status/" + vmname;
                std::ifstream ofS;
                ofS.open(directory + "/vmStopped");
                if (ofS.is_open()) {
                    std::string outbuffer;
                    std::getline(ofS, outbuffer);
                    if (outbuffer.size() < 2) {
                        std::cout << "Undefined" << std::endl;
                    } else {
                        std::cout << outbuffer << std::endl;
                        ofS.close();
                        clearFile(directory + "/vmStopped");
                    }
                } else {
                    std::cerr << "Error statusStopvm: unable to open " << directory + "vmStopped\n";
                }
            }
            return 0; // end the program
        }

        if (vm.count("statusMigratevm")) {
            fast::name vmname;
            if (vm.count("Command Parameter")) {
                std::vector<std::string> arguments = vm["Command Parameter"].as<std::vector < std::string >> ();
                vmname = {arguments[0]};
                std::string directory = configPublic["receive"]["path"].as<std::string>()
                        + "/status/" + vmname;
                std::ifstream ofS;
                ofS.open(directory + "/vmMigrated");
                if (ofS.is_open()) {
                    std::string outbuffer;
                    std::getline(ofS, outbuffer);
                    if (outbuffer.size() < 2) {
                        std::cout << "Undefined" << std::endl;
                    } else {
                        std::cout << outbuffer << std::endl;
                        ofS.close();
                        clearFile(directory + "/vmMigrated");
                    }
                } else {
                    std::cerr << "Error statusMigratevm: unable to open " << directory + "/vmMigrated\n";
                }
            }

            return 0; // end the program
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

        pluginConfiguration conf(config_file_name);

        if (vm.count("collectKPI")) {
            int maxCores{16};
            if (vm.count("Command Parameter")) {
                std::vector<std::string> arguments = vm["Command Parameter"].as<std::vector < std::string >> ();
                //arguments[0] ==> node list, arguments[1] ==> periode
                std::cout << "Periode " << arguments[1] << " Seconds" << std::endl;
                std::cout << "Reading host file  " << arguments[0] << std::endl;
                std::ifstream nodelist{arguments[0]};
                if (nodelist) {
                    std::cout << "Found:" << std::endl;
                    std::string item;
                    std::vector<std::string> nodelistVector;
                    while (nodelist >> item) {
                        std::cout << "\t" << item << std::endl;
                        nodelistVector.push_back(item);
                    }
                    std::vector<fast::name> corelist;
                    for (int i = 8; i <= 15; i++) {
                        std::stringstream outstr;
                        outstr << i;
                        corelist.push_back(outstr.str());
                    }
                    for (int i = 24; i <= 31; i++) {
                        std::stringstream outstr;
                        outstr << i;
                        corelist.push_back(outstr.str());
                    }
                    if (nodelistVector.size() != 0) {
                        while (1) {
                            for (auto &node : nodelistVector) {
                                fast::requestKPI(node, corelist, conf.comm, 2);
                            }
                            std::string delay = arguments[1];
                            sleep(std::atoi(delay.c_str()));
                        }
                    }
                } else {

                    std::cerr << "error with file " << arguments[0] << std::endl;
                    return 0;
                }

                /*s
                if (arguments.size() == 2) {

                    fast::stopvm(arguments[0], configPublic["vm"]["UUID"].as<std::string>(), {
                        arguments[1]
                    }, conf.comm, 2);
                    sleep(1);
                } else if (arguments.size() == 3)
                {
                    sleep(std::stoi (arguments[0]));
                    fast::stopvm(arguments[1], configPublic["vm"]["UUID"].as<std::string>(), {
                        arguments[2]
                    }, conf.comm, 2);
                    sleep(1);
                }*/
                //conf.comm->disconnect_from_broker();
            }
        }
        if (vm.count("startvm")) {
            if (vm.count("Command Parameter")) {
                YAML::Node confs;
                std::vector<std::string> arguments = vm["Command Parameter"].as<std::vector < std::string >> ();
                //std::vector<fast::machineConf> confs;
                // start using vm-name
                /*now the VM-name is not sent at all*/
                //std::string vm_name = arguments[1];
                //if (vm_name != "") {
                //    YAML::Node n;
                //    n["vm-name"] = vm_name;
                //    confs.push_back(n);
                // }
                // start using xml
                std::fstream inFile;
                inFile.open(arguments[2]); //open the input file
                std::stringstream strStream, pc_ids;
                strStream << inFile.rdbuf(); //read the file
                std::string xml_str = strStream.str(); //str holds the content of the file
                if (xml_str != "") {
                    YAML::Node n;
                    n["xml"] = xml_str;
                    YAML::Node pci_id;
                    //			pci_id["vendor"] = "0x15b3";
                    pci_id["vendor"] = configPublic["vm"]["vendor"].as<std::string>();
                    //			pci_id["device"] = "0x1004";
                    //			pci_id["device"] = "0x673c";
                    pci_id["device"] = configPublic["vm"]["device"].as<std::string>();
                    std::vector<YAML::Node> pci_id_vec;
                    pci_id_vec.push_back(pci_id);
                    n["pci-ids"] = pci_id_vec;
                    n["vm-name"] = arguments[1];
                    confs.push_back(n);
                }
                /*
                confs.push_back({  // confs  is a vector of maps
                    {"name", arguments[1]}
                    //{"xml", str}// the xml should be loaded now from a file
                   // {"overlay-image", configPublic["vm"]["overlay-image"].as<std::string>() + arguments[1]},
                   // {base-image", configPublic["vm"]["base-image"].as<std::string>()}
                });*/
                //pci_ids << "- vendor: 0x15b3"<<std::endl;
                //pci_ids << "- device: 0x673c"
                /*
                confs.push_back( {
                    {"xml", str},// the xml should be loaded now from a file
                    {"pci-ids", pci_ids}
                });*/
                fast::startvm(arguments[0], configPublic["vm"]["UUID"].as<std::string>(), confs, conf.comm, 2);
                sleep(1);
                //conf.comm->disconnect_from_broker();
            }
        }



        if (vm.count("stopvm")) {
            if (vm.count("Command Parameter")) {
                std::vector<std::string> arguments = vm["Command Parameter"].as<std::vector < std::string >> ();
                if (arguments.size() == 2) {

                    fast::stopvm(arguments[0], configPublic["vm"]["UUID"].as<std::string>(), {
                        arguments[1]
                    }, conf.comm, 2);
                    sleep(1);
                } else if (arguments.size() == 3) {
                    sleep(std::stoi(arguments[0]));

                    fast::stopvm(arguments[1], configPublic["vm"]["UUID"].as<std::string>(), {
                        arguments[2]
                    }, conf.comm, 2);
                    sleep(1);
                }
                //conf.comm->disconnect_from_broker();
            }
        }


        if (vm.count("repinvm")) {
            if (vm.count("Command Parameter")) {
                std::vector<std::string> arguments = vm["Command Parameter"].as<std::vector < std::string >> ();
                    fast::repinvm(arguments[0], configPublic["vm"]["UUID"].as<std::string>(), arguments[1], {
                        arguments[2]
                    }, conf.comm, 2);
                    sleep(1);

                //conf.comm->disconnect_from_broker();
            }
        }


        if (vm.count("migratevm")) {
            if (vm.count("Command Parameter")) {
                std::vector<std::string> arguments = vm["Command Parameter"].as<std::vector < std::string >> ();
                fast::migratevm(arguments[1], configPublic["vm"]["UUID"].as<std::string>(), arguments[0], arguments[2], arguments[3],{
                    {"retry-counter", configPublic["vm"]["retry-counter"].as<std::string>()},
                    {"migration-type", configPublic["vm"]["migration-type"].as<std::string>()},
                    {"rdma-migration", configPublic["vm"]["rdma-migration"].as<std::string>()}
                }, conf.comm, 2);
                sleep(1);
                //conf.comm->disconnect_from_broker();
            }
        }

        if (vm.count("listen")) {
            recMessageHandler receive(true, conf.comm);
            /* Updating things to comply with simon*/
            //receive.addTopic("fast/migfra/+/status", 2);

            /* unused for now
            receive.addTopic("fast/migfra/+/result", 2);
            receive.addTopic("fast/agent/+/status", 2);
             * /
            // just add the following lines to solve subscri problm*/
            /* unused for now
            receive.addTopic("fast/migfra/fast-01/result", 2);
            receive.addTopic("fast/agent/fast-01/status", 2);
            receive.addTopic("fast/migfra/fast-02/result", 2);
            receive.addTopic("fast/agent/fast-02/status", 2);
            receive.addTopic("fast/migfra/fast-03/result", 2);
            receive.addTopic("fast/agent/fast-03/status", 2);
            receive.addTopic("fast/migfra/fast-04/result", 2);
            receive.addTopic("fast/agent/fast-04/status", 2);
            */
            receive.addTopic("fast/agent/+/mmbwmon/response",2);

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
