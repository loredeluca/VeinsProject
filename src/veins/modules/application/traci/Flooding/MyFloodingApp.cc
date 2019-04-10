/*
 * MyFloodingApp.cc
 *
 *  Created on: 02 mar 2019
 *      Author: lored
 */

#include "veins/modules/application/traci/MyFloodingApp.h"
#include "veins/base/phyLayer/PhyToMacControlInfo.h"
#include "veins/modules/phy/DeciderResult80211.h"

Define_Module(MyFloodingApp);

void MyFloodingApp::initialize(int stage) {
    BaseWaveApplLayer::initialize(stage);
    if (stage == 0) {
        sentMessage = false;
        lastDroveAt = simTime();
        currentSubscribedServiceId = -1;
        this->delayTime = registerSignal("delay");
        this->RSSI = registerSignal("rssi");
    }
}

void MyFloodingApp::onWSA(WaveServiceAdvertisment* wsa) {
    if (currentSubscribedServiceId == -1) {
        mac->changeServiceChannel(wsa->getTargetChannel());
        currentSubscribedServiceId = wsa->getPsid();
        if  (currentOfferedServiceId != wsa->getPsid()) {
            stopService();
            startService((Channels::ChannelNumber) wsa->getTargetChannel(), wsa->getPsid(), "Mirrored Traffic Service");
        }
    }
}
//onWSM gestisce la ricezione di un msg di warning
void MyFloodingApp::onWSM(WaveShortMessage* wsm) {
    if (mobility->getRoadId()[0] != ':') traciVehicle->changeRoute(wsm->getWsmData(), 9999);
    if (!sentMessage){
        findHost()->getDisplayString().updateWith("r=16,green");
        sentMessage = true;
        //Calcolo la potenza di ricezione
        double rssi = check_and_cast<DeciderResult80211*>(check_and_cast<PhyToMacControlInfo*>(wsm->getControlInfo())->getDeciderResult())->getRecvPower_dBm();
        //Calcolo del ritardo di ricezione
        simtime_t delay = simTime().dbl()-wsm->getCreationTime();

        //Registro RSSI
        emit(RSSI, rssi);
        //Registro Delay
        emit(delayTime, delay.dbl());
        EV<<"HO RICEVUTO IL MSG CON UN RITARDO DI "<< delay <<"sec"<< std::endl;

        //repeat the received traffic update once with some random delay to avoid collisions
        wsm->setSenderAddress(myId);
        wsm->setSerial(3);
        scheduleAt(simTime() + uniform(0.01,0.02), wsm->dup());
    }
}

//handleSelfMsg gestice i msg per inviarli ai livelli inferiori
void MyFloodingApp::handleSelfMsg(cMessage* msg) {
    if (WaveShortMessage* wsm = dynamic_cast<WaveShortMessage*>(msg)) {
        //send this message on the service channel until the counter is 3 or higher.
        //this code only runs when channel switching is enabled
        sendDown(wsm->dup());
        wsm->setSerial(wsm->getSerial() +1);
        if (wsm->getSerial() >= 3) {
            //stop service advertisements
            stopService();
            delete(wsm);
        }
        else {
            scheduleAt(simTime()+1, wsm);
        }
    }
    else {
        BaseWaveApplLayer::handleSelfMsg(msg);
    }
}

void MyFloodingApp::handlePositionUpdate(cObject* obj) {
    BaseWaveApplLayer::handlePositionUpdate(obj);

    // stopped for for at least 15s?
    if (mobility->getSpeed() < 1) {
        if (simTime() - lastDroveAt >= 15 && sentMessage == false){
            findHost()->getDisplayString().updateWith("r=16,red");
            sentMessage = true;

            //Creo un msg di warning per notificare l'incidente
            WaveShortMessage* wsm = new WaveShortMessage();
            populateWSM(wsm);
            wsm->setWsmData(mobility->getRoadId().c_str());

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
