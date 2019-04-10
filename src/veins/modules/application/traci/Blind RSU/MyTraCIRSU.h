/*
 * MyTraCIRSU.h
 *
 *  Created on: 03 mar 2019
 *      Author: lored
 */

#ifndef SRC_VEINS_MODULES_APPLICATION_TRACI_MYTRACIRSU_H_
#define SRC_VEINS_MODULES_APPLICATION_TRACI_MYTRACIRSU_H_

#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"

class MyTraCIRSU : public BaseWaveApplLayer {
    private:
        bool msgReceived=false;
    protected:
        virtual void onWSM(WaveShortMessage* wsm);
        virtual void onWSA(WaveServiceAdvertisment* wsa);
};




#endif /* SRC_VEINS_MODULES_APPLICATION_TRACI_MYTRACIRSU_H_ */
