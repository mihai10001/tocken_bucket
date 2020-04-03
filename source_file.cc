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
    virtual void finish() override;
  private:
    cMessage *msg_LP;
    cMessage *msg_HP;
    int statistic_source_ip_counter;
};

// The module class needs to be registered with OMNeT++
Define_Module(Source_file);

void Source_file::initialize()
{
    // Initialize is called at the beginning of the simulation.
    // create and send first message on gate "outSource"

    msg_LP = new cMessage("IP Packet LOW");
    msg_HP = new cMessage("IP Packet HIGH");
    statistic_source_ip_counter = 2;

    send(msg_LP, "outSource");
    send(msg_HP, "outSource");
    EV << "Source: Generated first two IP Packets!";
}

void Source_file::handleMessage(cMessage *msg)
{
    // The handleMessage() method is called whenever a message arrives
    // at the module. Here, we have a message arriving from the Source_file.initialize() function
    // We send out the message to the FIFO Regulator module

    statistic_source_ip_counter++;

    send(msg, "outSourceFifo");
    EV << "Source: Sending message from Source to FIFO";

    // We generate a new message and we send in
    // the new message for the next Source_file.handleMessage() function

    msg_LP = new cMessage("IP Packet LOW");
    msg_HP = new cMessage("IP Packet HIGH");
    send(msg_LP, "outSource");
    send(msg_HP, "outSource");
    EV << "Source: Generated two new IP Packets!";
}

void Source_file::finish()
{
    printf("\nSource: Since start, there have been created a number of %d IP Packets!\n\n", statistic_source_ip_counter);
}





