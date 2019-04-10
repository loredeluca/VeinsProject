/*
 * NodeSelection.cc
 *
 *  Created on: 11 mar 2019
 *      Author: lored
 */
#include "veins/modules/application/traci/NodeSelection.h"
#include "veins/base/phyLayer/PhyToMacControlInfo.h"
#include "veins/modules/phy/DeciderResult80211.h"

Define_Module(NodeSelection);

void NodeSelection::initialize(int stage) {
    BaseWaveApplLayer::initialize(stage);
    if (stage == 0) {
        sentMessage = false;
        lastDroveAt = simTime();
        currentSubscribedServiceId = -1;
        this->delayTime = registerSignal("delay");
        this->RSSI = registerSignal("rssi");
        this->RSSI1 = registerSignal("rssi1");
        this->RSSI2 = registerSignal("rssi2");
        this->RSSI3 = registerSignal("rssi3");
        this->RSSI4 = registerSignal("rssi4");
        this->RSSI5 = registerSignal("rssi5");
        this->RSSI6 = registerSignal("rssi6");
        this->RSSI7 = registerSignal("rssi7");
        this->RSSI8 = registerSignal("rssi8");
        this->RSSI9 = registerSignal("rssi9");
        this->RSSI10 = registerSignal("rssi10");
        this->RSSI11 = registerSignal("rssi11");

        this->RSSIRic = registerSignal("rssiRic");
    }
}

void NodeSelection::onWSA(WaveServiceAdvertisment* wsa) {
    if (currentSubscribedServiceId == -1) {
        mac->changeServiceChannel(wsa->getTargetChannel());
        currentSubscribedServiceId = wsa->getPsid();
        if  (currentOfferedServiceId != wsa->getPsid()) {
            stopService();
            startService((Channels::ChannelNumber) wsa->getTargetChannel(), wsa->getPsid(), "Mirrored Traffic Service");
        }
    }
}
void NodeSelection::onBSM(BasicSafetyMessage* bsm) {
    //ignoro beacon che arrivano dalle auto-->cambia in base allo scenario: extraurbano =18 cittadino=11
    if(bsm->getSenderAddress()>18)
        EV<<"ignoro"<<std::endl;
    else{
        //Calcolo l'RSSI con cui riceve beacon
        double rssi = check_and_cast<DeciderResult80211*>(check_and_cast<PhyToMacControlInfo*>(bsm->getControlInfo())->getDeciderResult())->getRecvPower_dBm();
        int senderId = bsm->getSenderAddress();

        //Registro segnale RSSI
        if(senderId==7){
            emit(RSSI, rssi);
        }
        if(senderId==8){
            emit(RSSI1, rssi);
        }
        if(senderId==9){
            emit(RSSI2, rssi);
        }
        if(senderId==10){
            emit(RSSI3, rssi);
        }
        if(senderId==11){
            emit(RSSI4, rssi);
        }
        if(senderId==12){
            emit(RSSI5, rssi);
        }
        if(senderId==13){
            emit(RSSI6, rssi);
        }
        if(senderId==14){
            emit(RSSI7, rssi);
        }
        if(senderId==15){
            emit(RSSI8, rssi);
        }
        if(senderId==16){
            emit(RSSI9, rssi);
        }
        if(senderId==17){
            emit(RSSI10, rssi);
        }
        if(senderId==18){
            emit(RSSI11, rssi);
        }

        //gestione per terminare l'inoltro di beacon tramite la posizione
        //-->si usa nello scenario extraurbano per evitare errore
        /* A = Coord(500,1280);
        double a = A.x;
        double b = A.y;
        double myPosX=mobility->getCurrentPosition().x;
        double myPosY=mobility->getCurrentPosition().y;
        if(myPosX<a && myPosY>b)
            EV<<"stop beacon"<<std::endl;
        else{*/

        //gestione per terminare l'inoltro di beacon tramite il tempo di simulazione
        if(simTime().dbl()<236){
            //Invia Ack quando riceve beacon
            BasicSafetyMessage* ack=new BasicSafetyMessage();
            int senderBeacon=bsm->getSenderAddress();
            ack->setSenderAddress(myId);
            populateWSM(ack);

            //conversione double-to-string rssi per setWsmData
            std::stringstream r;
            r<<rssi;
            std::string R = r.str();
            ack->setRSSI(R);
            ack->setWsmData(ack->getRSSI().c_str());

            //Imposto il ricevente dell'Ack
            ack->setRecipientAddress(senderBeacon-firstRSU);
            sendDelayedDown(ack->dup(), uniform(0.01,0.02));
        }
        else
            EV<<"Stop beacon"<<std::endl;
    }
}

void NodeSelection::onWSM(WaveShortMessage* wsm) {
        if (mobility->getRoadId()[0] != ':') traciVehicle->changeRoute(wsm->getWsmData(), 9999);
        if (!sentMessage) {
            sentMessage = true;

            //Imposto colore del nodo come colore dell'RSU da cui riceve
            if(wsm->getSenderAddress()==7)
                findHost()->getDisplayString().updateWith("r=16,green");
            if(wsm->getSenderAddress()==8)
                findHost()->getDisplayString().updateWith("r=16,blue");
            if(wsm->getSenderAddress()==9)
                findHost()->getDisplayString().updateWith("r=16,brown");
            if(wsm->getSenderAddress()==10)
                findHost()->getDisplayString().updateWith("r=16,orange");
            if(wsm->getSenderAddress()==11)
                findHost()->getDisplayString().updateWith("r=16,yellow");
            if(wsm->getSenderAddress()==12)
                findHost()->getDisplayString().updateWith("r=16,violet");
            if(wsm->getSenderAddress()==13)
                findHost()->getDisplayString().updateWith("r=16,black");
            if(wsm->getSenderAddress()==14)
                findHost()->getDisplayString().updateWith("r=16,purple");
            if(wsm->getSenderAddress()==15)
                findHost()->getDisplayString().updateWith("r=16,pink");
            if(wsm->getSenderAddress()==16)
                findHost()->getDisplayString().updateWith("r=16,white");
            if(wsm->getSenderAddress()==17)
                findHost()->getDisplayString().updateWith("r=16,grey");
            if(wsm->getSenderAddress()==18)
                findHost()->getDisplayString().updateWith("r=16,green");

            //Calcolo l'RSSI con cui riceve wsm
            double rssiRic = check_and_cast<DeciderResult80211*>(check_and_cast<PhyToMacControlInfo*>(wsm->getControlInfo())->getDeciderResult())->getRecvPower_dBm();
            //Calcolo delay di ricezione
            simtime_t delay = simTime().dbl()-wsm->getCreationTime();

            //Registro RSSIRic
            emit(RSSIRic, rssiRic);
            //Registro delay
            emit(delayTime, delay.dbl());
            EV<<"+++++++++++++++++++++++ DELAY +++++++++++++++++++++++++++++++++++"<< delay << std::endl;

            wsm->setSenderAddress(myId);
            scheduleAt(simTime() + uniform(0.0001,0.0002), wsm->dup());
        }
}

void NodeSelection::handleSelfMsg(cMessage* msg) {
    if (WaveShortMessage* wsm = dynamic_cast<WaveShortMessage*>(msg)) {
            //stop service advertisements
            stopService();
            delete(wsm);
    }
    else
        BaseWaveApplLayer::handleSelfMsg(msg);
}

void NodeSelection::handlePositionUpdate(cObject* obj) {
    BaseWaveApplLayer::handlePositionUpdate(obj);

    // stopped for for at least 15s?
    if (mobility->getSpeed() < 1) {
        if (simTime() - lastDroveAt >= 15 && sentMessage == false) {
            findHost()->getDisplayString().updateWith("r=16,red");
            detectorNode=myId;
            sentMessage = true;

            WaveShortMessage* wsm = new WaveShortMessage();
            populateWSM(wsm);
            wsm->setWsmData(mobility->getRoadId().c_str());
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



