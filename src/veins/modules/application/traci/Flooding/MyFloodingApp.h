/*
 * MyFloodingApp.h
 *
 *  Created on: 02 mar 2019
 *      Author: lored
 */

#ifndef SRC_VEINS_MODULES_APPLICATION_TRACI_MYFLOODINGAPP_H_
#define SRC_VEINS_MODULES_APPLICATION_TRACI_MYFLOODINGAPP_H_

#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"

class MyFloodingApp : public BaseWaveApplLayer {
    public:
        virtual void initialize(int stage);
    protected:
        simtime_t lastDroveAt;
        bool sentMessage;
        int currentSubscribedServiceId;
        simsignal_t delayTime;
        simsignal_t RSSI;
        int detectorNode=0;
    protected:
        virtual void onWSM(WaveShortMessage* wsm);
        virtual void onWSA(WaveServiceAdvertisment* wsa);

        virtual void handleSelfMsg(cMessage* msg);
        virtual void handlePositionUpdate(cObject* obj);
};

#endif /* SRC_VEINS_MODULES_APPLICATION_TRACI_MYFLOODINGAPP_H_ */
