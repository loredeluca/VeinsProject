/*
 * MyTraCINode.h
 *
 *  Created on: 03 mar 2019
 *      Author: lored
 */

#ifndef SRC_VEINS_MODULES_APPLICATION_TRACI_MYTRACINODE_H_
#define SRC_VEINS_MODULES_APPLICATION_TRACI_MYTRACINODE_H_

#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"


class MyTraCINode : public BaseWaveApplLayer {
    public:
        virtual void initialize(int stage);
        int getDetectorNode();
    protected:
        simtime_t lastDroveAt;
        bool sentMessage;
        int currentSubscribedServiceId;
        simsignal_t delayTime;
        simsignal_t RSSI;
    protected:
        virtual void onWSM(WaveShortMessage* wsm);
        virtual void onWSA(WaveServiceAdvertisment* wsa);

        virtual void handleSelfMsg(cMessage* msg);
        virtual void handlePositionUpdate(cObject* obj);
    private:
        static int detectorNode;
};


#endif /* SRC_VEINS_MODULES_APPLICATION_TRACI_MYTRACINODE_H_ */
