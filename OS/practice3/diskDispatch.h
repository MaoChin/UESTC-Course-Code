#include <iostream>
#include <vector>
#include <list>
#include <ctime>
#include <unistd.h>

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::list;

#define PCB_NUM 10

struct PCB
{
  PCB(int arriveTime = 0, int requestTrack = 0)
    :arriveTime_(arriveTime), requestTrack_(requestTrack)
  {}
  int arriveTime_;
  int requestTrack_;
};

void creatAllPcb(list<PCB*>& pcbs);

void printInfo(list<PCB*>& pcbs, list<PCB*>::iterator nextPcb, 
      int& curTrack, int& curTime, int& sumAcrossTrack);

void SSTF(list<PCB*> pcbs, int& curTrack, int& curTime, int& sumAcrossTrack);

void SCAN(list<PCB*> pcbs, int& curTrack, int& curTime, int& sumAcrossTrack);