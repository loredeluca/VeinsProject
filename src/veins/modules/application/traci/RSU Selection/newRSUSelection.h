/*
 * newRSUSelection.h
 *
 *  Created on: 17 mar 2019
 *      Author: lored
 */

//Usiamo questa classe per lo scenario Extraurbano

#ifndef SRC_VEINS_MODULES_APPLICATION_TRACI_NEWRSUSELECTION_H_
#define SRC_VEINS_MODULES_APPLICATION_TRACI_NEWRSUSELECTION_H_

#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"
#define Vehicles 50
#define Rsu 12

class newRSUSelection : public BaseWaveApplLayer {
    private:
        int firstNode=79; //id del primo nodo, varia in base al numero di RSU
        int firstRSU=7;
        int numberNodes= Vehicles;
        bool msgReceived=false;
        bool PowerRecord[Vehicles]={false};
        double myNodes[Vehicles]={-100};
        bool RsuCovered[Rsu]={false};


    protected:
        virtual void onWSM(WaveShortMessage* wsm);
        virtual void onWSA(WaveServiceAdvertisment* wsa);
        virtual void onBSM(BasicSafetyMessage* bsm);
};



#endif /* SRC_VEINS_MODULES_APPLICATION_TRACI_NEWRSUSELECTION_H_ */
