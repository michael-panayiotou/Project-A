#include "node.h"
#include "nodeGraph.h"
#include "parser.h"
#include "stationList.h"
#include <curses.h>
#include <iostream>
#include <stdio.h>
#include <valarray>
#include <chrono>
#include <ctime>
#include <algorithm>    // std::max
#include <dirent.h>
#include <iomanip>

using namespace::std; 
int main(int argc, char**argv)
{
    
    
    // Prints welcome message...
    std::cout << "Welcome to ALBP1 and ALBP2 algorithms" << std::endl;
    int Algorithm=0;do{
    cout<<"Choose algorithm:\n"<<"1)ALBP1 2)ALBP2\nAlgorithm #:";cin>>Algorithm;}while(Algorithm!=1&&Algorithm!=2);
   
    //creates an instance of a parser object.
    //construction overload to get the name of the file as an input.
    int datasetSize=0;
    std::vector<string> benchmarks;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir ("benchmarks")) != NULL) {
      /* print all the files and directories within directory */
      while ((ent = readdir (dir)) != NULL) {
        benchmarks.push_back(ent->d_name);
      }
      closedir (dir);
    } else {
      /* could not open directory */
      perror ("");
      return EXIT_FAILURE;
    }
    

    const string policies[] = {"LTT","STT","MFT","LNFT","RPW","FCFS","Random","VNS","Heuristic"};
    int policyChoice =0;
    do{
    cout<<endl;
    cout<<"====Choose Policy===="<<endl;
    
    for(int i=0; i<9;i++){
                
                    std::cout<<i+1<<")"<<policies[i]<<std::endl;
               
                
            }
    
    cout<<"Policy #: ";
    cin>>policyChoice;
    }while((policyChoice>10)||(policyChoice<1));
    
    ofstream myfile;
    ofstream stationAlocationFile;
    ofstream statisticsFile;
    
    int timesOptimum=0;

    int totalIterations=0;
    double averDev=0;
    double averBD=0;
    double averSX=0;
    
    if(Algorithm==1)
    {
        stationAlocationFile.open("outputs/ALBP1/"+policies[policyChoice-1]+"_Station_Allocation.txt");
        
        myfile.open ("outputs/ALBP1/"+policies[policyChoice-1]+".txt");
        myfile << "----------------------------------------------------------------------------------------------------------------------------\n";
        myfile<<"Problem\t\tn\tc\tm\tm*\tabs.dev \t%dev \t\tBD \t\tSX\t\tCPU time(sec)"<<endl;
        myfile << "----------------------------------------------------------------------------------------------------------------------------\n";
        
        if (!(std::ifstream("outputs/ALBP1/ALBP1_Statistics.txt")))
            {
               
                statisticsFile.open("outputs/ALBP1/ALBP1_Statistics.txt");
                statisticsFile << "----------------------------------------------------------------------------------------------------------------------------\n";
                statisticsFile<<"Policy\t\tNo. m=m*\t\taver %dev\t\taver BD\t\taver SX"<<endl;
                statisticsFile << "----------------------------------------------------------------------------------------------------------------------------\n";
        
               
            }
        else
        {
            statisticsFile.open("outputs/ALBP1/ALBP1_Statistics.txt", std::ofstream::out | std::ofstream::app);
        
        }
    
    }
        if(Algorithm==2)
    {
        stationAlocationFile.open ("outputs/ALBP2/"+policies[policyChoice-1]+"_Station_Allocation.txt");
        
        myfile.open ("outputs/ALBP2/"+policies[policyChoice-1]+".txt");
        myfile << "-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n\n";
        myfile << "\t\t\t\t\t\t\t\tLower Bound\t\t\t\t\tUpper Bound\t\t\t\t\tBinary Search"<<endl;
        myfile << "\t\t\t\t\t\t---------------------------------------------\t\t---------------------------------------------\t\t---------------------------------------------"<<endl;
        myfile << "Problem\t\tn\tm\tc*\t\tc\t%dev\tBD\tSX\tCPU(time)\t\tc\t%dev\tBD\tSX\tCPU(time)\t\tc\t%dev\tBD\tSX\tCPU(time)"<<endl<<endl;
        myfile << "-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
        
                if (!(std::ifstream("outputs/ALBP2/ALBP2_Statistics.txt")))
            {
               
                statisticsFile.open("outputs/ALBP2/ALBP2_Statistics.txt");
                statisticsFile << "----------------------------------------------------------------------------------------------------------------------------\n";
                statisticsFile<<"Policy\t\tNo. c=c*\t\taver %dev\t\taver BD\t\taver SX"<<endl;
                statisticsFile << "----------------------------------------------------------------------------------------------------------------------------\n";
        
               
            }
        else
        {
            statisticsFile.open("outputs/ALBP1/ALBP1_Statistics.txt", std::ofstream::out | std::ofstream::app);
        
        }
    
    }
    
    int policyQueue[] = {17,5,11,10,16,18,20,9,6,21,13,7,12,8,24,23,25,2,14,15,19,1,3,4,22};
    std::string choice;
    int p=0;
    int benchmarkChoice=-1;
    cout<<"Would you like to manually select benchmarks?(Y)/(N)"<<endl;
    cout<<"Choice: ";cin>>choice;
   
    do{ //The big loop that asks as if we want to continue
        if(choice == "N"){
            if(p==25)
                break;
           
            benchmarkChoice=policyQueue[p];
            p++;
        }
        else{     
    
    do{
    std::cout<<endl<<"Choose a benchmark:"<<std::endl;
    for(int i=3; i<benchmarks.size();i++){
                
                    std::cout<<i-2<<")"<<benchmarks[i]<<std::endl;
               
                
            }
    
    std::cout<<"Benchmark #: ";
    std::cin>>benchmarkChoice;
    
    }while((benchmarkChoice>25)||(benchmarkChoice<1));
    cout<<endl<<"You have chose: "<<benchmarks[benchmarkChoice+2];
    }
    std::vector<dataset1Node> dataset1;
    std::vector<dataset2Node> dataset2;
    parser p1("benchmarks/"+benchmarks[benchmarkChoice+2],Algorithm);
    if(Algorithm==1)
        dataset1=p1.getDataset1();
    else
        dataset2=p1.getDataset2();
    
    datasetSize = p1.getDatasetSize();
    
    stationList s;
    int cycleTime = 0;
    int optimumStations=0;
    int optumumCycleTime=0;
    int m=0;
    
    //Creating the graph and populating it with the parsed data

    for(int i=0; i<p1.getWeightsVector().size();i++)
    {
        s.x.insertNode(p1.getWeightsVector()[i].name,p1.getWeightsVector()[i].value); 
    }
    for(int i=0; i<p1.getDependenciesVector().size();i++)
    {
        s.x.insertEdge(p1.getDependenciesVector()[i].left,p1.getDependenciesVector()[i].right);   
    }
    //
    
    int Pmax=0;  
    int Psum=0; 
    for(int i =0; i<s.x.getNodeList().size();i++)
    {
        Psum=Psum+s.x.getNodeList()[i].getValue();
        if(Pmax<s.x.getNodeList()[i].getValue())
        {
            Pmax=s.x.getNodeList()[i].getValue();
        }
    }
    
    //s.x.printNodes();
    s.x.setFollowingTasks();
    if(Algorithm==1){
        
               
                bool found=false;
                int foundPosition=0;
                for(int k=0; k<dataset1.size();k++){
                    if(benchmarks[benchmarkChoice+2]==dataset1[k].name){
                        found=true; 
                        foundPosition=k;
                    }
                }
                //cin>>cycleTime;
                if (!found){
                cout<<"There no values for the selected benchmark in the dataset: "<<benchmarks[benchmarkChoice+2]<<endl;
                cout<<endl;
                string shallContinue;
                cout<<"Shall we continue?(Y/N)"<<endl;
                cin>>shallContinue; 
                if(shallContinue=="N"){break;}else{continue;}
            
            }
       for (int j=0;j<dataset1[foundPosition].cycletimes.size();j++){
                        s.initStations();
                        s.x.initDone();
                        
                        std::chrono::time_point<std::chrono::system_clock> start, end;
                        start = std::chrono::system_clock::now();
                        
                        cycleTime = dataset1[foundPosition].cycletimes[j];
                        
                        optimumStations = dataset1[foundPosition].optimum[j];
                        if(j==0){   
                           // cout<<"Benchmark:("<<benchmarks[benchmarkChoice+2]<<")\n\n\tn\tc\tm\tm*\tabs.dev \t%dev \tBD \tSX\tCPU time(sec)"<<endl;
//                            cout << "------------------------------------------------------------------------------------------\n";
//                            cout<<"Problem\t\tn\tc\tm\tm*\tabs.dev \t%dev \tBD \tSX\tCPU time(sec)"<<endl<<endl;
//                            cout << "------------------------------------------------------------------------------------------\n";
                        }
                
        s.setCycleTime(cycleTime);

        if(j==0){
        cout<<benchmarks[benchmarkChoice+2]<<setw(4)<<"\t"<<datasetSize;
        myfile<<benchmarks[benchmarkChoice+2]<<setw(4)<<"\t"<<datasetSize;
        }
        else{
            cout<<"\t\t";
            myfile<<"\t\t";
        }      

          if(policyChoice<8){
              s.setPolicy(policies[policyChoice-1]);
              for(int i=0;i<datasetSize;i++)
              {
                  node * decidedNode=s.decideNode(s.x.getQueue());
                  s.pushTaskToStation(decidedNode); 
                  
              }
             // cout<<"Problems and optimal solutions\nPreced.\t\t\ngraph\t\tc\tm\tm*\n--------------------------"<<endl; 
              cout<<"\t"<<cycleTime<<"\t"<<s.getStationList().size()<<"\t"<<optimumStations;
              myfile<<"\t"<<cycleTime<<"\t"<<s.getStationList().size()<<"\t"<<optimumStations;
          }
          else if(policyChoice==8){
              s.VNSpolicy();
             // cout<<"Problems and optimal solutions\nPreced.\t\t\ngraph\t\tc\tm\tm*\n--------------------------"<<endl; 
              cout<<"\t"<<cycleTime<<"\t"<<s.getStationList().size()<<"\t"<<optimumStations;
              myfile<<"\t"<<cycleTime<<"\t"<<s.getStationList().size()<<"\t"<<optimumStations;
              //cout<<endl<<"Optimal solutions vector\n--------------------------"<<endl;
            //  s.printBestSolution();
          }
          else if(policyChoice==9){
             s.Heuristicpolicy();             
              //cout<<"Problems and optimal solutions\nPreced.\t\t\ngraph\t\tc\tm\tm*\n--------------------------"<<endl; 
              cout<<"\t"<<cycleTime<<"\t"<<s.getStationList().size()<<"\t"<<optimumStations;
              myfile<<"\t"<<cycleTime<<"\t"<<s.getStationList().size()<<"\t"<<optimumStations;
            // cout<<endl<<"Optimal solutions vector\n--------------------------"<<endl;
            // s.printBestHeuristicSolution();
          }
        
                myfile<<"\t"<<(int)s.getStationList().size()-(int)optimumStations;
                cout<<"\t"<<(int)s.getStationList().size()-(int)optimumStations;          
                
                
                
                
                end = std::chrono::system_clock::now();
                std::chrono::duration<double> elapsed_seconds = end-start;
                std::cout.unsetf ( std::ios::floatfield ); 
                std::cout.precision(2);
                cout<<"\t\t"<<float((((float)s.getStationList().size()-(float)optimumStations)/(float)optimumStations)*100)<<"\t\t"<<float(s.findBT())<<"\t\t"<<(float)s.findSX()<<setprecision(2)<<"\t\t"<<(float)elapsed_seconds.count()<<endl;
                myfile<<"\t\t"<<float((((float)s.getStationList().size()-(float)optimumStations)/(float)optimumStations)*100)<<setprecision(3)<<"\t\t"<<s.findBT()<<"\t\t"<<(float)s.findSX()<<setprecision(3)<<"\t\t"<<(float)elapsed_seconds.count()<<setprecision(3)<<endl;
                    stationAlocationFile<<"Benchmark:"<<benchmarks[benchmarkChoice+2]<<"\tGiven c:"<<cycleTime<<endl<<endl;
                    stationAlocationFile<<"Task Allocation in Stations:\n\n";
                    s.printStations(stationAlocationFile);
                    stationAlocationFile<<endl;
                    
                    if(s.getStationList().size()==optimumStations)
                        timesOptimum++;
                    totalIterations++;
                    averDev=+double(((((float)s.getStationList().size()-(float)optimumStations)/(float)optimumStations)*100));
                    averBD=+double(s.findBT());
                    averSX=+(double)s.findSX();
                }
    }

     if(Algorithm==2){
        
         //LB =max(p ,p m)
        
            
         int solution=0;
         bool LBsolution=false;
         bool UBsolution=false;   
         bool MIDCsolution=false;
//       if(policyChoice>0&&policyChoice<8) 
//            do{ 
//                     cout<<"Choose a solution :\n1)LB\n2)UB\n3)MIDC"; 
//                     cout<<"\nSolution #:";      
//                     cin>>solution;
//
//             }while(solution!=1&&solution!=2&&solution!=3);  
                
            bool found=false;
            int foundPosition=0;
            for(int k=0; k<dataset2.size();k++){
                    if(benchmarks[benchmarkChoice+2]==dataset2[k].name){
                        found=true; 
                        foundPosition=k;
                        //m = dataset2[k].stations[0];
                        //optumumCycleTime = dataset2[k].optimum[0];
                    }
                }
            
            if (!found){
                if(choice == "N"){continue;}
                else{
                
                cout<<"There no values for the selected benchmark in the dataset: "<<benchmarks[benchmarkChoice+2]<<endl;
                cout<<endl;
                string shallContinue;
                cout<<"Shall we continue?(Y/N)"<<endl;
                cin>>shallContinue; 
                if(shallContinue=="N"){break;}else{continue;}
                }
            }
     
               // cout<<"Stations #: "<<m;      
                
                    
                        for (int j=0;j<dataset2[foundPosition].stations.size();j++){
                        found=true; 
                        m = dataset2[foundPosition].stations[j];
                        optumumCycleTime = dataset2[foundPosition].optimum[j];
                        if(j==0){   
//                            cout<<"Benchmark:("<<benchmarks[benchmarkChoice+2];
//                            cout<<")\n\n\tn\tm\tc*\tc\t%dev\tBD\tSX\tCPU(time)\tc\t%dev\tBD\tSX\tCPU(time)\tc\t%dev\tBD\tSX\tCPU(time)"<<endl;
//                            myfile<<"Benchmark:("<<benchmarks[benchmarkChoice+2];
//                            myfile<<")\n\n\tn\tm\tc*\tc\t%dev\tBD\tSX\tCPU(time)\tc\t%dev\tBD\tSX\tCPU(time)\tc\t%dev\tBD\tSX\tCPU(time)"<<endl;
                            //cout<<"Benchmark:("<<benchmarks[benchmarkChoice+2]<<")\n\n\tm\tc*\tc(LTT)\tc(STT)\tc(MFT)\tc(LNFT)\tc(RPW)\tc(FCFS)\tc(Random)\tc(VNS)\tc(Heuristic)"<<endl;
                            
                        }

           
        
  

         int LB=std::max(Pmax,Psum/m);
         int UB=std::max(Pmax,2*Psum/m);
         int midc=(LB+UB)/2;
         //cout<<"LB:"<<LB<<" UB:"<<UB<<" midc:"<<midc<<" Pmax:"<<Pmax<<" Psum:"<<Psum<<endl;  
         s.setMaxStations(m);
        
         int minc;
         int maxc;
    
         //cout<<"------------------------"<<endl;
         int rememberedOne=0;
         bool enoughtStations=true;
       
         if(j==0){
        myfile<<benchmarks[benchmarkChoice+2]<<setw(4)<<"\t"<<datasetSize;
        cout<<benchmarks[benchmarkChoice+2]<<"\t"<<datasetSize;
        
        }
        else{
            myfile<<"\t\t";
            cout<<"\t\t";
        }
         cout<<"\t"<<m<<"\t"<<optumumCycleTime;
         myfile<<"\t"<<m<<"\t"<<optumumCycleTime;
             s.initStations();
             s.x.initDone();
             s.setAvailableStations(m);
             
             LB=std::max(Pmax,Psum/m);     
             UB=std::max(Pmax,2*Psum/m);
             midc=(LB+UB)/2;
             minc=LB;
             maxc=UB; 
          if(policyChoice<8){
              s.setPolicy(policies[policyChoice-1]);
              for(int j=0; j<3; j++){
                   std::chrono::time_point<std::chrono::system_clock> start, end;
                   start = std::chrono::system_clock::now();
                   s.initStations();
                   s.x.initDone();
                   s.setAvailableStations(m);
                   LB=std::max(Pmax,Psum/m);     
                   UB=std::max(Pmax,2*Psum/m);
                   
                   midc=(LB+UB)/2;
                   minc=LB;
                   maxc=UB; 
                 if(j==0){
                     LBsolution=true;   
                     UBsolution=false;
                     MIDCsolution=false;
                     s.setCycleTime(LB);
                     cycleTime=LB;
                     
                 }

                  if(j==1){
                     LBsolution=false;  
                     UBsolution=true;
                     MIDCsolution=false;
                     s.setCycleTime(UB);
                     cycleTime=UB;
                     //cout<<"<"<<UB<<">";
                     
                  }

                  if(j==2){
                     LBsolution=false;  
                     UBsolution=false;
                     MIDCsolution=true;
                     s.setCycleTime(midc);
                     cycleTime=midc;
                     
                  }
              if(LBsolution||UBsolution){   
                  
                    //cout<<"\tLB:"<<LB<<" UB:"<<UB<<" midc:"<<midc;
                    for(int i=0;i<datasetSize;i++)
                    {
                        node * decidedNode=s.decideNode(s.x.getQueue());
                        enoughtStations=s.pushTaskToStation(decidedNode);

                      if(LBsolution) {
                        if(!enoughtStations){
                            i=-1;
                            LB=LB+1;
                            cycleTime=LB;
                            s.setAvailableStations(m);
                            s.setCycleTime(LB); 
                            s.initStations();
                            s.x.initDone();
                        } 
                      }

                      if(UBsolution){
                         if(enoughtStations&&(i==(datasetSize-1))){  
                            i=-1;
                            UB=UB-1;
                            cycleTime=UB;         
                            s.setAvailableStations(m);
                            s.setCycleTime(UB); 
                            s.initStations();
                            s.x.initDone();
                            rememberedOne=UB; 
                            //cout<<"<"<<UB<<">";
                        } 

                      
                          if(!enoughtStations&&i==datasetSize-1){                
                              s.setCycleTime(rememberedOne);
                              break;
                        }
                      }
                       }  
              }
              else if(MIDCsolution){   
                  while (maxc >= minc) {
                     midc = (maxc+minc)/2;
                     s.setCycleTime(midc);
                    //NODE INITIALIZATION
                     s.initStations();
                     s.x.initDone();
                     //RUN THE GRAPH
                     for(int i=0;i<datasetSize;i++)
                     {
                         node * decidedNode=s.decideNode(s.x.getQueue());
                         s.pushTaskToStation(decidedNode);
                     }
                     
                    //cout<<"min:"<<minc<<" mid:"<<midc<<" max:"<<maxc<<" :size"<<s.getStationList().size()<<" m:"<<m<<endl;  
                     //case we got a small station number then we have to decrease cycle time in order for our algorithm to get a bigger number of startions
                     if(s.getStationList().size() > m){
                      minc = midc +1;
                     }else if(s.getStationList().size() < m){
                      maxc = midc -1;
                    }
                     else if(s.getStationList().size() == m){
                         maxc--;
                     }
                            
                  }
              }
              //cout<<"Problems and optimal solutions\nPreced.\t\t\ngraph\t\tm\tc\tc*\n--------------------------"<<endl; 
                   
             end = std::chrono::system_clock::now();
             std::chrono::duration<double> elapsed_seconds = end-start;
             //cout<<"\t\t"<<(((float)s.getStationList().size()-(float)optimumStations)/(float)optimumStations)*100<<"\t"<<s.findBT()<<"\t"<<s.findSX()<<"\t"<<elapsed_seconds.count()<<endl;
               
             // if(LBsolution||MIDCsolution)
             if(UBsolution)
                 s.setCycleTime(s.getCycleTime()+1);
             //if(j==0){cout<<"\t";myfile<<"\t";}
                 cout<<"\t\t"<<s.getCycleTime()<<"\t"<<(((float)s.getCycleTime()-(float)optumumCycleTime)/(float)optumumCycleTime)*100<<"\t"<<s.findBT()<<"\t"<<s.findSX()<<"\t"<<elapsed_seconds.count();
                 myfile<<"\t\t"<<s.getCycleTime()<<"\t"<<(((float)s.getCycleTime()-(float)optumumCycleTime)/(float)optumumCycleTime)*100<<"\t"<<s.findBT()<<"\t"<<s.findSX()<<"\t"<<elapsed_seconds.count();

                 // if(UBsolution)
                // cout<<"\t"<<s.getCycleTime();
              
              
              }
          }
          else if(policyChoice==8){
              std::chrono::time_point<std::chrono::system_clock> start, end;
              start = std::chrono::system_clock::now();
             do{
                 s.initStations();
                 s.x.initDone();
                 s.setCycleTime(LB); 
                 s.setMaxStations(m);
                 s.setAvailableStations(m);
                 s.VNSpolicy(); 
                 LB=LB+1;
              }while(s.getStationList().size()>m);
              end = std::chrono::system_clock::now();
             std::chrono::duration<double> elapsed_seconds = end-start;
              //cout<<"Problems and optimal solutions\nPreced.\t\t\ngraph\t\tm\tc\tc*\n--------------------------"<<endl; 
              cout<<"\t\t"<<s.getCycleTime()<<"\t"<<(((float)s.getCycleTime()-(float)optumumCycleTime)/(float)optumumCycleTime)*100<<"\t"<<s.findBT()<<"\t"<<s.findSX()<<"\t"<<elapsed_seconds.count();
              myfile<<"\t\t"<<s.getCycleTime()<<"\t"<<(((float)s.getCycleTime()-(float)optumumCycleTime)/(float)optumumCycleTime)*100<<"\t"<<s.findBT()<<"\t"<<s.findSX()<<"\t"<<elapsed_seconds.count();


              //cout<<endl<<"Optimal solutions vector\n--------------------------"<<endl;
              //s.printBestSolution();
          }
          else if(policyChoice==9){    
              do{
                    cout<<" LB:"<<LB<<" m*:"<<m<<" m:"<<s.getStationList().size()<<endl;
                    s.initStations();
                    s.x.initDone();
                    s.setCycleTime(LB); 
                    s.setMaxStations(m);
                    s.setAvailableStations(m);
                    s.Heuristicpolicy(); 
                   LB=LB+1; 
              }while(s.getStationList().size()>m);           
            // cout<<"Problems and optimal solutions\nPreced.\t\t\ngraph\t\tm\tc\tc*\n--------------------------"<<endl; 
             cout<<"\t\t"<<s.getCycleTime();
             myfile<<"\t\t"<<s.getCycleTime();
             //cout<<endl<<"Optimal solutions vector\n--------------------------"<<endl;
             //s.printBestHeuristicSolution();
          }
             myfile<<endl;
             cout<<endl;
             stationAlocationFile<<"Benchmark:"<<benchmarks[benchmarkChoice+2]<<"\tGiven m:"<<m<<endl<<endl;
             stationAlocationFile<<"Task Allocation in Stations:\n\n";
             s.printStations(stationAlocationFile);
             stationAlocationFile<<endl;
             
             if(s.getCycleTime()==optumumCycleTime)
                 timesOptimum++;
             totalIterations++;
             averDev=+double(((((float)s.getStationList().size()-(float)optimumStations)/(float)optimumStations)*100));
             averBD=+double(s.findBT());
             averSX=+(double)s.findSX();
          }
    
    }
    myfile<<endl;

    
    if(choice != "N"){
    string shallContinue;
    cout<<"\nShall we continue?(Y/N)"<<endl;
    cin>>shallContinue; 
    if(shallContinue=="N"){break;}else{continue;}
    }
   
    }while(true);
    statisticsFile<<policies[policyChoice-1]<<"\t\t"<<timesOptimum<<"\t\t\t"<<averDev/totalIterations<<"\t\t"<<averBD/totalIterations<<"\t\t"<<averSX/totalIterations<<endl;
    statisticsFile.close();
    myfile.close();
    stationAlocationFile.close();
    return 8;
}