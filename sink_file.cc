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

class Sink_file : public cSimpleModule
{
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
  private:
    int index = 0;
    cMessage sink_message_array[100];
};

// The module class needs to be registered with OMNeT++
Define_Module(Sink_file);

void Sink_file::initialize()
{
}

void Sink_file::handleMessage(cMessage *msg)
{
    // The handleMessage() method is called whenever a message arrives
    // at the module. Here, we have a message arriving from the Fifo file

    if (index < 100) {
        sink_message_array[index] = *msg;
        index++;
    }
}




