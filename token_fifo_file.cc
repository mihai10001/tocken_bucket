/*
 * source_file.cc
 *
 *  Created on: Dec 16, 2019
 *      Author: iliuta
 */

#include <omnetpp.h>

using namespace omnetpp;

/**
 * Derive the Token_fifo_file class from cSimpleModule.
 */

class Token_fifo_file : public cSimpleModule
{
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
  private:
    cQueue *buffer_message_array;
    int max_size;
};

// The module class needs to be registered with OMNeT++
Define_Module(Token_fifo_file);

void Token_fifo_file::initialize()
{
    // No second parameter in constructor => queue has FIFO behavior

    buffer_message_array = new cQueue("token_queue");
    max_size = 100;
}

void Token_fifo_file::handleMessage(cMessage *msg)
{
    // Here, we have a message arriving from the Source file
    // If there is space left in the FIFO queue, we insert the message
    // If there is no space left, we discard the message

    if (strcmp(msg->getName(), "Token") == 0) {
        if (buffer_message_array->getLength() < max_size) {
            buffer_message_array->insert(msg);
            EV << "Token FIFO: Received message from Token Source, adding to queue";
        } else {
            EV << "Token FIFO: Queue is full ! Discarding message: " << msg->getName();
            delete msg;
        }
    }

    // If the FIFO queue is not empty, we pop the first element with queue->pop()
    // and we send out the response the Regulator module, Token Found or Token Not Found

    if (!buffer_message_array->isEmpty()) {
        cMessage *popped =  (cMessage *)buffer_message_array->pop();
        send("Token Found", "outTokenFifo");
        EV << "Token FIFO: Sending Token Found message from Token FIFO to Regulator";
    } else if (buffer_message_array->isEmpty()) {
        send("Token Not Found", "outTokenFifo");
        EV << "Token FIFO: Sending Token Not Found message from Token FIFO to Regulator";
    }
}
