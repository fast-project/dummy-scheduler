/* 
 * File:   recMessageHandler.cpp
 * Author: gad
 * 
 * Created on June 29, 2015, 11:38 AM
 */

#include "recMessageHandler.h"

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
                std::string s = this->comm->get_message(item, std::chrono::seconds(1));//item,
                //std::cout << "message received" << std::endl;
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
