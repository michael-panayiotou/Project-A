/* 
 * File:   stationList.h
 * Author: dimitriosboglou
 *
 * Created on February 11, 2015, 11:45 PM
 */

#ifndef STATIONLIST_H
#define	STATIONLIST_H
#include <vector>
#include <string>
#include <iostream>
#include "station.h"
#include "nodeGraph.h"


 enum string_policy {
    LTT,
    STT,
    MFT,
    LNFT,
    RPW,
    LRPW,
    VNS
};


class stationList{

private:
    std::vector<station> stations;
    string_policy policy;
    int cycleTime;

public:
    nodeGraph x; 
    stationList();
    ~stationList();
    void insertStation();
    void pushTaskToStation(node*); 
    void printStations();
    void setPolicy(std::string);
    void initStations();
    node* decideNode(std::vector<node*>);
    node* LTTpolicy(std::vector<node*>);
    node* STTpolicy(std::vector<node*>);
    node* MFTpolicy(std::vector<node*>);
    node* LNFTpolicy(std::vector<node*>);
    node* RPWpolicy(std::vector<node*>);
    node* LRPWpolicy(std::vector<node*>);
    void VNSpolicy();
    std::vector<station> getStationList();
    void setCycleTime(int);



};

#endif	/* STATIONLIST_H */