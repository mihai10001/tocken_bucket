/*
 * source_file.cc
 *
 *  Created on: Dec 16, 2019
 *      Author: iliuta
 */

#include <omnetpp.h>

using namespace omnetpp;

/**
 * Derive the Token_source_file class from cSimpleModule.
 */

class Token_source_file : public cSimpleModule
{
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
  private:
    cMessage *msg;
};

// The module class needs to be registered with OMNeT++
Define_Module(Token_source_file);

void Token_source_file::initialize()
{
    // Initialize is called at the beginning of the simulation.
    // create and send first message on gate "outSource"

    msg = new cMessage("Token");
    send(msg, "outSource");
    EV << "Source: Generated first Token!";
}

void Token_source_file::handleMessage(cMessage *msg)
{
    // The handleMessage() method is called whenever a message arrives
    // at the module. Here, we have a message arriving from the Token_source_file.initialize() function
    // We send out the message to the Token FIFO module

    send(msg, "outSourceTokenFifo");
    EV << "Source: Sending message from Token Source to FIFO";

    // We generate a new message and we send in
    // the new message for the next Token_source_file.handleMessage() function

    msg = new cMessage("Token");
    send(msg, "outSource");
    EV << "Token Source: Generated a new Token!";
}




