/*
 * RSUSelection.h
 *
 *  Created on: 11 mar 2019
 *      Author: lored
 */

//Usiamo questa classe per lo scenario Cittadino

#ifndef SRC_VEINS_MODULES_APPLICATION_TRACI_RSUSELECTION_H_
#define SRC_VEINS_MODULES_APPLICATION_TRACI_RSUSELECTION_H_

#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"
#define Vehicles 30
#define Rsu 5

class RSUSelection : public BaseWaveApplLayer {
    private:
        int firstNode=37; //id del primo nodo, varia in base al numero di RSU
        int firstRSU=7;
        int numberNodes= Vehicles;
        bool msgReceived=false;
        bool PowerRecord[Vehicles]={false};
        int PowerCounter[Vehicles]={0};
        double myNodes[Vehicles]={-100};
        bool RsuCovered[Rsu]={false};

    protected:
        virtual void onWSM(WaveShortMessage* wsm);
        virtual void onWSA(WaveServiceAdvertisment* wsa);
        virtual void onBSM(BasicSafetyMessage* bsm);
};



#endif /* SRC_VEINS_MODULES_APPLICATION_TRACI_RSUSELECTION_H_ */
