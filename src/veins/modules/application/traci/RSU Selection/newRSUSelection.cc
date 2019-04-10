/*
 * newRSUSelection.cc
 *
 *  Created on: 17 mar 2019
 *      Author: lored
 */

#include "veins/modules/application/traci/newRSUSelection.h"
#include "veins/base/phyLayer/PhyToMacControlInfo.h"
#include "veins/modules/phy/DeciderResult80211.h"

Define_Module(newRSUSelection);

void newRSUSelection::onWSA(WaveServiceAdvertisment* wsa) {
    //if this RSU receives a WSA for service 42, it will tune to the chan
    if (wsa->getPsid() == 42) {
        mac->changeServiceChannel(wsa->getTargetChannel());
    }
}
void newRSUSelection::onBSM(BasicSafetyMessage* bsm) {
    //Memorizzo gli RSU nell'area di copertura in base ai beacon ricevuti
    if(bsm->getSenderAddress()<=firstRSU+Rsu){
        RsuCovered[bsm->getSenderAddress()-firstRSU]=true;
    }
    else{
        //RSSI con cui ha ricevuto il destinatario del beacon
        double RSSI = check_and_cast<DeciderResult80211*>(check_and_cast<PhyToMacControlInfo*>(bsm->getControlInfo())->getDeciderResult())->getRecvPower_dBm();
        myNodes[(bsm->getSenderAddress()-firstNode)/6]=RSSI;

        //valuto se il nodo è nell'area di copertura dell'RSU
        if(myId==16){
            if(myNodes[(bsm->getSenderAddress()-firstNode)/6]<-85)
                PowerRecord[(bsm->getSenderAddress()-firstNode)/6]=false;
            else
                PowerRecord[(bsm->getSenderAddress()-firstNode)/6]=true;
        }
        else{
            if(myNodes[(bsm->getSenderAddress()-firstNode)/6]<-88)
                PowerRecord[(bsm->getSenderAddress()-firstNode)/6]=false;
            else
                PowerRecord[(bsm->getSenderAddress()-firstNode)/6]=true;
        }
    }
}

void newRSUSelection::onWSM(WaveShortMessage* wsm) {
    //Aggiorno la lista dei nodi nell'area di copertura
    if(myId==16){
        for(int i=0;i<numberNodes;i++){
            if(myNodes[i]<-85||myNodes[i]==0)
                PowerRecord[i]=false;
            else
                PowerRecord[i]=true;
        }
    }
    else{
        for(int i=0;i<numberNodes;i++){
            if(myNodes[i]<-87.9||myNodes[i]==0)
                PowerRecord[i]=false;
            else
                PowerRecord[i]=true;
        }
    }
    //Visualizzazione dei nodi nell'area di copertura dell'RSU
    for(int i=0; i<numberNodes; i++){
        if(PowerRecord[i])
            EV<<"L'RSU["<<myId-firstRSU<<"] ha nella sua area di copertura il Nodo["<< i << "] con RSSI: "<<myNodes[i]<<std::endl;
    }

    if (!msgReceived){
        msgReceived=true;
        if(myId==7)
            findHost()->getDisplayString().updateWith("r=16,green");
        if(myId==8)
            findHost()->getDisplayString().updateWith("r=16,blue");
        if(myId==9)
            findHost()->getDisplayString().updateWith("r=16,brown");
        if(myId==10)
            findHost()->getDisplayString().updateWith("r=16,orange");
        if(myId==11)
             findHost()->getDisplayString().updateWith("r=16,yellow");
        if(myId==12)
             findHost()->getDisplayString().updateWith("r=16,violet");
        if(myId==13)
             findHost()->getDisplayString().updateWith("r=16,black");
        if(myId==14)
             findHost()->getDisplayString().updateWith("r=16,purple");
        if(myId==15)
             findHost()->getDisplayString().updateWith("r=16,pink");
        if(myId==16)
             findHost()->getDisplayString().updateWith("r=16,white");
        if(myId==17)
             findHost()->getDisplayString().updateWith("r=16,gray");
        if(myId==18)
             findHost()->getDisplayString().updateWith("r=16,green");

        //Inoltro wsm alle auto nell'area di copertura
        for(int i=0; i<numberNodes; i++){
            if(PowerRecord[i]){
                //invia solo a nodi che ricevono con RSSI>-80
                if(myNodes[i]>-81){
                    wsm->setSenderAddress(myId);
                    wsm->setRecipientAddress(i);
                    sendDelayedDown(wsm->dup(), uniform(0.0001,0.0002));
                    EV<<"L'RSU["<<myId-firstRSU<<"] inoltra il msg al nodo["<< i << "] che lo riceve con RSSI"<<myNodes[i]<<std::endl;
                }
            }
        }

        //Inoltro wsm alle RSU nell'area di copertura
        for(int i=0; i<Rsu; i++){
            if(RsuCovered[i]){
                wsm->setRecipientAddress(i);
                sendDelayedDown(wsm->dup(), uniform(0.001,0.002));
                EV<<"L'RSU["<<myId-firstRSU<<"] inoltra il msg all'RSU["<< i << "]"<<std::endl;
            }
        }
    }
}


