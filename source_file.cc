/*
 * source_file.cc
 *
 *  Created on: Dec 16, 2019
 *      Author: iliuta
 */

#include <omnetpp.h>

using namespace omnetpp;

/**
 * Derive the Source_file class from cSimpleModule.
 */

class Source_file : public cSimpleModule
{
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

// The module class needs to be registered with OMNeT++
Define_Module(Source_file);

void Source_file::initialize()
{
    // Initialize is called at the beginning of the simulation.
    // create and send first message on gate "in"

    cMessage *msg = new cMessage("IP Packet");
    send(msg, "outSource");
}

void Source_file::handleMessage(cMessage *msg)
{
    // The handleMessage() method is called whenever a message arrives
    // at the module. Here, we have a message arriving from the Source_file.initialize() function

    send(msg, "outSourceFifo"); // send out the message to the Fifo module

    cMessage *new_msg = new cMessage("IP Packet"); // we create a new message
    send(new_msg, "outSource"); // send in the new message for the next Source_file.handleMessage() function
}





