/* 
 * File:   pluginConfiguration.h
 * Author: gad
 *
 * Created on June 8, 2015, 6:18 PM
 */

#ifndef PLUGINCONFIGURATION_H
#define	PLUGINCONFIGURATION_H

#include <fast-lib/communication/communicator.hpp>
#include <fast-lib/serialization/serializable.hpp>

class pluginConfiguration : fast::Serializable {
public:
    pluginConfiguration() = default;
    pluginConfiguration(const std::string &config_file);
    /**
     * \brief Emits Task_handler to YAML::Node.
     *
     * Implements fast::Serializable::emit().
     * Dummy implementation which always throws, due to lack of need for this method.
     */
    YAML::Node emit() const override;
    /**
     * \brief Loads Task_handler from YAML::Node.
     *
     * Implements fast::Serializable::load().
     * Creates the Communicator and Hypervisor from YAML.
     */
    void load(const YAML::Node &node) override;



    std::shared_ptr<fast::Communicator> comm;
private:

};

#endif	/* PLUGINCONFIGURATION_H */

