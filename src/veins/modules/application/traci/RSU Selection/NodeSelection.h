/*
 * NodeSelection.h
 *
 *  Created on: 11 mar 2019
 *      Author: lored
 */

#ifndef SRC_VEINS_MODULES_APPLICATION_TRACI_NODESELECTION_H_
#define SRC_VEINS_MODULES_APPLICATION_TRACI_NODESELECTION_H_

#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"

class NodeSelection : public BaseWaveApplLayer{
    public:
        virtual void initialize(int stage);
    protected:
        simtime_t lastDroveAt;
        bool sentMessage;
        int currentSubscribedServiceId;
        simsignal_t delayTime;
        simsignal_t RSSI;
        simsignal_t RSSI1;
        simsignal_t RSSI2;
        simsignal_t RSSI3;
        simsignal_t RSSI4;
        simsignal_t RSSI5;
        simsignal_t RSSI6;
        simsignal_t RSSI7;
        simsignal_t RSSI8;
        simsignal_t RSSI9;
        simsignal_t RSSI10;
        simsignal_t RSSI11;

        simsignal_t RSSIRic;
        int firstRSU = 7;
        //cambia in base al numero di RSU: scenario cittadino=37, scenario extraurbano=79
        int detectorNode=79;//37

    protected:
        virtual void onWSM(WaveShortMessage* wsm);
        virtual void onWSA(WaveServiceAdvertisment* wsa);
        virtual void onBSM(BasicSafetyMessage* bsm);

        virtual void handleSelfMsg(cMessage* msg);
        virtual void handlePositionUpdate(cObject* obj);
};




#endif /* SRC_VEINS_MODULES_APPLICATION_TRACI_NODESELECTION_H_ */
