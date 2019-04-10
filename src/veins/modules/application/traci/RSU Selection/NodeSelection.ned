//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package org.car2x.veins.modules.application.traci;
import org.car2x.veins.modules.application.ieee80211p.BaseWaveApplLayer;

simple NodeSelection extends BaseWaveApplLayer
{
    parameters:
        @signal[delay](type="double");
        @statistic[delayTime](title="delay Time"; source="delay";record=max,vector);
        @signal[rssi](type="double");
        @statistic[RSSI](title="RSSI"; source="rssi";record=max,vector);
        
        @signal[rssi1](type="double");
        @statistic[RSSI1](title="RSSI1"; source="rssi1";record=max,vector);
        @signal[rssi2](type="double");
        @statistic[RSSI2](title="RSSI2"; source="rssi2";record=max,vector);
        @signal[rssi3](type="double");
        @statistic[RSSI3](title="RSSI3"; source="rssi3";record=max,vector);
        @signal[rssi4](type="double");
        @statistic[RSSI4](title="RSSI4"; source="rssi4";record=max,vector);
        @signal[rssi5](type="double");
        @statistic[RSSI5](title="RSSI5"; source="rssi5";record=max,vector);
        @signal[rssi6](type="double");
        @statistic[RSSI6](title="RSSI6"; source="rssi6";record=max,vector);
        @signal[rssi7](type="double");
        @statistic[RSSI7](title="RSSI7"; source="rssi7";record=max,vector);
        @signal[rssi8](type="double");
        @statistic[RSSI8](title="RSSI8"; source="rssi8";record=max,vector);
        @signal[rssi9](type="double");
        @statistic[RSSI9](title="RSSI9"; source="rssi9";record=max,vector);
        @signal[rssi10](type="double");
        @statistic[RSSIpenultimo](title="RSSI10"; source="rssi10";record=max,vector);
        @signal[rssi11](type="double");
        @statistic[RSSI11](title="RSSI11"; source="rssi11";record=max,vector);
        
        @signal[rssiRic](type="double");
        @statistic[RSSIRic](title="RSSIRic"; source="rssiRic";record=max,vector);
    @class(NodeSelection);
    @display("i=block/app2");
}
