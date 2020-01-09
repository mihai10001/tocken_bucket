/*
 * source_file.cc
 *
 *  Created on: Dec 16, 2019
 *      Author: iliuta
 */

#include <omnetpp.h>

using namespace omnetpp;

/**
 * Derive the Fifo_file class from cSimpleModule.
 */

class Fifo_file : public cSimpleModule
{
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
  private:
    cQueue *buffer_message_array;
    int max_size;
};

// The module class needs to be registered with OMNeT++
Define_Module(Fifo_file);

void Fifo_file::initialize()
{
    // No second parameter in constructor => queue has FIFO behavior

    buffer_message_array = new cQueue("queue");
    max_size = 100;
}

void Fifo_file::handleMessage(cMessage *msg)
{
    // Here, we have a message arriving from the Source file
    // If there is space left in the FIFO queue, we insert the message
    // If there is no space left, we discard the message

    if (buffer_message_array->getLength() < max_size) {
        buffer_message_array->insert(msg);
        EV << "FIFO: Received message from Source, adding to queue";
    } else {
        EV << "FIFO: Queue is full! Discarding message: " << msg->getName();
        delete msg;
    }

    // If the FIFO queue is not empty, we pop the first element with queue->pop()
    // and we send it out to the Regulator module

    if (!buffer_message_array->isEmpty()) {
        cMessage *popped =  (cMessage *)buffer_message_array->pop();
        send(popped, "outFifo");
        EV << "FIFO: Sending message from FIFO to Regulator";
    }
}
