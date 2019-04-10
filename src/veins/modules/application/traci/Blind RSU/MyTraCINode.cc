/*
 * MyTraCINode.cc
 *
 *  Created on: 03 mar 2019
 *      Author: lored
 */
#include "veins/modules/application/traci/MyTraCINode.h"
#include "veins/base/phyLayer/PhyToMacControlInfo.h"
#include "veins/modules/phy/DeciderResult80211.h"
#include <random>

Define_Module(MyTraCINode);
int MyTraCINode::detectorNode;

void MyTraCINode::initialize(int stage) {
    BaseWaveApplLayer::initialize(stage);
    if (stage == 0) {
        sentMessage = false;
        lastDroveAt = simTime();
        currentSubscribedServiceId = -1;
        this->delayTime = registerSignal("delay");
        this->RSSI = registerSignal("rssi");
    }
}

void MyTraCINode::onWSA(WaveServiceAdvertisment* wsa) {
    if (currentSubscribedServiceId == -1) {
        mac->changeServiceChannel(wsa->getTargetChannel());
        currentSubscribedServiceId = wsa->getPsid();
        if  (currentOfferedServiceId != wsa->getPsid()) {
            stopService();
            startService((Channels::ChannelNumber) wsa->getTargetChannel(), wsa->getPsid(), "Mirrored Traffic Service");
        }
    }
}

void MyTraCINode::onWSM(WaveShortMessage* wsm) {
   //ignoro wsm del detectorNode
   if(wsm->getSenderAddress()>= detectorNode){
      EV<<"ricevo msg dal nodo["<<detectorNode<<"]"<<std::endl;
   }
   //processo wsm mandato da RSU
   else{
       if (mobility->getRoadId()[0] != ':') traciVehicle->changeRoute(wsm->getWsmData(), 9999);
       if (!sentMessage) {
            findHost()->getDisplayString().updateWith("r=16,green");
            sentMessage = true;
            //Calcolo la potenza di ricezione
            double rssi = check_and_cast<DeciderResult80211*>(check_and_cast<PhyToMacControlInfo*>(wsm->getControlInfo())->getDeciderResult())->getRecvPower_dBm();
            //Calcolo il ritardo di ricezione
            simtime_t delay = simTime().dbl()-wsm->getCreationTime();

            //Registro RSSI
            emit(RSSI, rssi);
            //Registro delay
            emit(delayTime, delay.dbl());
            EV<<"Ho ricevuto con un delay: "<< delay <<"sec"<< std::endl;

            wsm->setSenderAddress(myId);
            scheduleAt(simTime()+uniform(0.0001,0.0002), wsm->dup());
        }
    }
}

void MyTraCINode::handleSelfMsg(cMessage* msg) {
    if (WaveShortMessage* wsm = dynamic_cast<WaveShortMessage*>(msg)) {
        //stop service advertisements
        stopService();
        delete(wsm);
    }
    else
        BaseWaveApplLayer::handleSelfMsg(msg);
}

void MyTraCINode::handlePositionUpdate(cObject* obj) {
    BaseWaveApplLayer::handlePositionUpdate(obj);

    // stopped for for at least 15s?
    if (mobility->getSpeed() < 1) {
        if (simTime() - lastDroveAt >= 15 && sentMessage == false) {
            findHost()->getDisplayString().updateWith("r=16,red");
            detectorNode=myId;
            sentMessage = true;

            //creo un nuovo msg di warning
            WaveShortMessage* wsm = new WaveShortMessage();
            populateWSM(wsm);
            wsm->setWsmData(mobility->getRoadId().c_str());

            //Scelta dell'RSU a cui inviare
            wsm->setRecipientAddress(0);

            if (dataOnSch) {
                startService(Channels::SCH2, 42, "Traffic Information Service");
                //started service and server advertising, schedule message to self to send later
                scheduleAt(computeAsynchronousSendingTime(1,type_SCH),wsm);
            }
            else {
                //send right away on CCH, because channel switching is disabled
                sendDown(wsm);
            }
        }
    }
    else {
        lastDroveAt = simTime();
    }
}
int MyTraCINode::getDetectorNode(){
    return detectorNode;
}


