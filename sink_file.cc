/*
 * source_file.cc
 *
 *  Created on: Dec 16, 2019
 *      Author: iliuta
 */

#include <omnetpp.h>

using namespace omnetpp;

/**
 * Derive the Sink_file class from cSimpleModule.
 */

class Sink_file : public cSimpleModule
{
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
  private:
    cMessage *sink_message_array;
    int max_array_size;
    int index;
};

// The module class needs to be registered with OMNeT++
Define_Module(Sink_file);

void Sink_file::initialize()
{
    max_array_size = 200;
    sink_message_array = new cMessage[max_array_size];
    index = 0;
}

void Sink_file::handleMessage(cMessage *msg)
{
    // Here, we have a message arriving from the FIFO file
    // If there is space left in the sink_array, we insert the message

    if (index < 100) {
        sink_message_array[index] = *msg;
        index++;
        EV << "Sink: Received message from FIFO, adding to sink_array";
    } else {
       EV << "Sink: Sink_array is full! Discarding message: " << msg->getName();
       delete msg;
    }
}




