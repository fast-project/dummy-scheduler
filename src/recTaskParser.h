/* 
 * File:   recTaskParser.h
 * Author: gad
 *
 * Created on June 29, 2015, 10:06 AM
 */

#ifndef RECTASKPARSER_H
#define	RECTASKPARSER_H

#include <fast-lib/serializable.hpp>
#include "task.h"


class recTaskParser
: public fast::Serializable {
public:
    recTaskParser();
    //recTaskParser(std::vector  <std::string> &r):receivedTopic{r}{}
    //recTaskParser(const recTaskParser& orig);
    virtual ~recTaskParser();
    std::string receivedTopic;
    std::vector <std::unique_ptr<task> > tasks;
    std::shared_ptr<fast::MQTT_communicator> comm;
    void setReceiveTopic(std::string topic){
        receivedTopic = topic;
    }
    void execute();
    YAML::Node emit() const override;
    void load(const YAML::Node &node) override;
private:

};
YAML_CONVERT_IMPL(recTaskParser)
#endif	/* RECTASKPARSER_H */

