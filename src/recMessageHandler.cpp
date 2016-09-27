/* 
 * File:   recMessageHandler.cpp
 * Author: gad
 * 
 * Created on June 29, 2015, 11:38 AM
 */

#include "recMessageHandler.h"
#include <iostream>

recMessageHandler::~recMessageHandler() {
}

void recMessageHandler::addTopic(std::string topic, int qos) {
    comm->add_subscription(topic, qos);
    topics.push_back(topic);
}

void recMessageHandler::run() {
    do {
        for (auto &item : topics) {


            try {
                recTaskParser taskParser;
                taskParser.comm = this->comm;
                //std::cout << "For this topic " << item << std::endl;
                //std::string s = this->comm->get_message(item, std::chrono::seconds(1));//item,
                std::string actualTopic;
                std::string s = this->comm->get_message(item,std::chrono::seconds(1), &actualTopic);
                //std::cout << "message received" << std::endl;
                for( char &c : actualTopic){
                    if(c =='/') c =' ';
                }
                //std::stringstream ss{actualTopic};
                //std::vector<std::string> receivedTopic;
                //for(std::string t; ss>>t;)
                //{
                //    receivedTopic.push_back(t);
                //}
                //for(auto i : receivedTopic){
                //    std::cout << i<<" $$$ ";
                //}
                //std::cout <<std::endl;
                //t.
                //std::cout << "message received from Topic "<< actualTopic << std::endl;
                //std::cout << "message: "<< std::endl<<s<<std::endl;
                taskParser.setReceiveTopic(actualTopic);
                taskParser.from_string(s);
                taskParser.execute();
            } catch (const std::runtime_error &e) {
                if (e.what() != std::string("Timeout while waiting for message."))
                {
                    //std::cout << "Timeout while waiting for message." <<std::endl;
                    continue;
                }
            }

        }
    } while (loop);
}
