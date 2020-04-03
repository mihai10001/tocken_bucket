/*
 * source_file.cc
 *
 *  Created on: Mar 30, 2020
 *      Author: iliuta
 */

#include <omnetpp.h>

using namespace omnetpp;

/**
 * Derive the FLC class from cSimpleModule.
 */

class FLC : public cSimpleModule
{
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
  private:
    int max_size;
    int limit_LP;
    int no_of_LP;
    int no_of_HP;

    char fifo_msg[20];
    char *parts_msg;
    cMessage *msg;
};

// The module class needs to be registered with OMNeT++
Define_Module(FLC);

void FLC::initialize()
{
    max_size = 100;
    limit_LP = max_size / 2;
    no_of_LP = 0;
    no_of_HP = 0;
}

void FLC::handleMessage(cMessage *msg)
{
    // The handleMessage() method is called whenever a message arrives
    // at the module. Here, we have messages arriving from the FIFO Regulator module
    // The message will be a concatenated string containing the representation
    // of the number of the LOW and HIGH priority packages contained in the FIFO Regulator
    // Using fuzzy logic, and assuming HIGH priority packages are 30% more important
    // than LOW priority packages ( arbitrary chosen value ), the FLC module will output
    // a new limit for the LOW priority packages. The limit defaults to max_size / 2
    // This limit will be recalculated at each new arrived package
    // By concatenating, I can easily send a message containing 2 variables without much hassle
    // Message: ' no_of_LP - no_of_HP ' ( - is the separator )

    strcpy(fifo_msg, msg->getName());

    parts_msg = strtok(fifo_msg, "-");
    no_of_LP = atoi(parts_msg);

    parts_msg = strtok(NULL, "-");
    no_of_HP = atoi(parts_msg);

    EV << "FLC: Number of LOW priority packets received from the FIFO Regulator: " << no_of_LP;
    EV << "FLC: Number of HIGH priority packets received from the FIFO Regulator: " << no_of_HP;


    if ( ( no_of_LP < (2 * no_of_HP) / 3) && limit_LP < max_size ) {
        limit_LP++;
        EV << "FLC: Limit LP increased";
        msg = new cMessage("LimitLPIncreased");
        send(msg, "outFLC");
    } else if ( ( no_of_LP >= (2 * no_of_HP) / 3) && limit_LP > 0) {
        limit_LP--;
        EV << "FLC: Limit LP decreased";
        msg = new cMessage("LimitLPDecreased");
        send(msg, "outFLC");
    }
}

void FLC::finish()
{
    printf("\nFLC: At the end, the limit of LOW priority packets is: %d\n\n ", limit_LP);

}

