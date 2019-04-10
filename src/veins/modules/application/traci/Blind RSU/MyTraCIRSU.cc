/*
 * MyTraCIRSU.cc
 *
 *  Created on: 03 mar 2019
 *      Author: lored
 */
#include "veins/modules/application/traci/MyTraCIRSU.h"
Define_Module(MyTraCIRSU);

void MyTraCIRSU::onWSA(WaveServiceAdvertisment* wsa) {
    //if this RSU receives a WSA for service 42, it will tune to the chan
    if (wsa->getPsid() == 42) {
        mac->changeServiceChannel(wsa->getTargetChannel());
    }
}

void MyTraCIRSU::onWSM(WaveShortMessage* wsm) {
    if (!msgReceived){
        msgReceived=true;
        findHost()->getDisplayString().updateWith("r=16,yellow");
        //inoltro wsm in broadcast
        wsm->setSenderAddress(myId);
        wsm->setRecipientAddress(-1);
        sendDelayedDown(wsm->dup(), uniform(0.001,0.002));
    }
}



