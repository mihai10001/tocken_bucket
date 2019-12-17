/*
 * source_file.cc
 *
 *  Created on: Dec 16, 2019
 *      Author: iliuta
 */

#include <omnetpp.h>
// #include <iostream>
// #include <queue>

using namespace omnetpp;

/**
 * Derive the Source_file class from cSimpleModule.
 */

class Fifo_file : public cSimpleModule
{
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
  private:
    // int max_size = 100;
    // std::queue<cMessage> buffer_message_array;
};

// The module class needs to be registered with OMNeT++
Define_Module(Fifo_file);

void Fifo_file::initialize()
{
}

void Fifo_file::handleMessage(cMessage *msg)
{
    // The handleMessage() method is called whenever a message arrives
    // at the module. Here, we have a message arriving from the Source file

    // if (buffer_message_array.size() < max_size)
    //     buffer_message_array.push(*msg);
    // else
    //     std::cout << "Queue is full ! Discarding message: " << *msg;

    send(msg, "outFifo"); // send out the message from queue.pop() to the Sink module
}
