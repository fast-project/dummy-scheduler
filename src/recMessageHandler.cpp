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
            
            //std::string message = comm->get_message();

            recTaskParser taskParser;
            std::cout << "before get message" << std::endl;
            std::string s = comm->get_message(item);
            std::cout << "after get message" << std::endl;
            taskParser.from_string(s);
            taskParser.execute();
        }
    } while (loop);
}
