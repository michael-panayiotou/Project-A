/* 
 * File:   node.h
 * Author: dimitriosboglou
 *
 * Created on February 10, 2015, 6:35 PM
 */

#ifndef NODE_H
#define	NODE_H

 
#include <vector>
#include <string>
#include <iostream>
#include <set>
#ifdef	__cplusplus
extern "C" {
#endif
class node{

 private:
    int weight;
    bool done;
    bool mftDone;
    int name;  
    int numOfFollowingTasks;
    int RPWeight;
 public:
    std::vector<node*> inNodes; 
    std::vector<node*> outNodes; 
    
    node();
    node(int,int);
    int getValue();
    void setValue(int);
    int getName();
    void setName(int);
    int getRPW();
    void setRPW(int);
    bool getDone();
    void setDone(bool); 
    bool getMFTdone();
    void setMFTdone(bool); 
    void setNumFT(int);
    int getNumFT();
    void insertInputNode(node*);
    void insertOutputNode(node*);
};

#ifdef	__cplusplus
}
#endif


#endif	/* NODE_H */

