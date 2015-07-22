/* 
 * File:   recMessageHandler.h
 * Author: gad
 *
 * Created on June 29, 2015, 11:38 AM
 */

#ifndef RECMESSAGEHANDLER_H
#define	RECMESSAGEHANDLER_H
#define TIMEOUT 100
#include <fast-lib/communication/mqtt_communicator.hpp>
#include "recTaskParser.h"


class recMessageHandler {
public:

    recMessageHandler(bool l, std::shared_ptr<fast::MQTT_communicator> c) : loop(l), comm(c) {
    };
    virtual ~recMessageHandler();
    void addTopic(std::string topic , int qos);
    void run(void);
    bool loop;
    std::vector< std::string > topics;
    std::shared_ptr<fast::MQTT_communicator> comm;
private:

};

#endif	/* RECMESSAGEHANDLER_H */

