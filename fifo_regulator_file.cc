/*
 * source_file.cc
 *
 *  Created on: Dec 16, 2019
 *      Author: iliuta
 */

#include <omnetpp.h>

using namespace omnetpp;

/**
 * Derive the FIFORegulator_file class from cSimpleModule.
 */

class FIFORegulator_file : public cSimpleModule
{
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
  private:
    cQueue *buffer_message_array;
    int max_size;
    cMessage *msg;
    int statistic_regulator_ip_counter;
    int statistic_regulator_token_counter;

    int limit_LP;
    int no_of_LP;
    int no_of_HP;
    char temp_flc_msg[20];
    cMessage *flc_msg;
};

// The module class needs to be registered with OMNeT++
Define_Module(FIFORegulator_file);

void FIFORegulator_file::initialize()
{
    // Initialize is called at the beginning of the simulation.

    buffer_message_array = new cQueue("queue");
    max_size = 100;
    statistic_regulator_ip_counter = 0;
    statistic_regulator_token_counter = 0;

    limit_LP = max_size / 2;
    no_of_LP = 0;
    no_of_HP = 0;
}

void FIFORegulator_file::handleMessage(cMessage *msg)
{
    // The handleMessage() method is called whenever a message arrives
    // at the module. Here, we have messages arriving from either the Source or the Token FIFO module
    // If there is space left in the FIFO queue, we insert the message
    // If there is no space left, we discard the message
    // Then we check if there are any tokens left for delivery
    // If there are tokens left, we send the message forward to the Sink module
    // If there are no tokens left, we discard the message

    // * UPDATE: Added FLC logic, 2 variables denoting the count of LOW and HIGH
    // priority packages, and also another variable limit_LP, which denotes
    // the limit of the LOW priority packets that can be in the queue at any time


    if (strcmp(msg->getName(), "IP Packet LOW") == 0 ||
        strcmp(msg->getName(), "IP Packet HIGH") == 0) {
        statistic_regulator_ip_counter++;

        if (buffer_message_array->getLength() < max_size) {
            if (strcmp(msg->getName(), "IP Packet LOW") == 0)
                no_of_LP++;
            else if (strcmp(msg->getName(), "IP Packet HIGH") == 0)
                no_of_HP++;

            sprintf(temp_flc_msg, "%d-%d", no_of_LP, no_of_HP);
            flc_msg = new cMessage(temp_flc_msg);
            send(flc_msg, "outFifoRegFLC");

            buffer_message_array->insert(msg);
            EV << "FIFO/Regulator: Received message from Source, adding to queue";
        } else {
            EV << "FIFO/Regulator: Queue is full ! Discarding message: " << msg->getName();
            delete msg;
        }
    } else if (strcmp(msg->getName(), "Token Found") == 0) {
        statistic_regulator_token_counter++;

        if (!buffer_message_array->isEmpty()) {
            cMessage *popped =  (cMessage *)buffer_message_array->pop();
            send(popped, "outFifoReg");
            EV << "FIFO/Regulator: Token found! Sending message from FIFO/Regulator to Sink";
        } else if (buffer_message_array->isEmpty()) {
            EV << "FIFO/Regulator: Token found, but there is no message in queue!";
        }
    } else if (strcmp(msg->getName(), "Token Not Found") == 0) {
        if (!buffer_message_array->isEmpty()) {
            EV << "FIFO/Regulator: Token not found, discarding the last message: " << msg->getName();
            cMessage *popped =  (cMessage *)buffer_message_array->pop();
            delete popped;
        }
    }
}

void FIFORegulator_file::finish()
{
    printf("\nFIFO/Regulator: Since start, there have been received a number of %d IP Packets in the FIFO/Regulator!\n\n", statistic_regulator_ip_counter);
    printf("\nFIFO/Regulator: Since start, there have been received a number of %d Tokens in the FIFO/Regulator!\n\n", statistic_regulator_token_counter);
}



