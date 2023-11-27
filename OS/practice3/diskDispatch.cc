#include "diskDispatch.h"

// 初始化所有 PCB
void creatAllPcb(list<PCB*>& pcbs)
{
  int prevArriveTime = 0;
  int arriveTime = 0;
  int requestTrack = (rand() % 200) + 1;
  pcbs.push_back(new PCB(arriveTime, requestTrack));

  for(int i = 0; i < PCB_NUM - 1; ++i)
  {
    // 间隔[10, 40)
    arriveTime = (rand() % 30) + 10;
    requestTrack = (rand() % 200) + 1;
    pcbs.push_back(new PCB(arriveTime + prevArriveTime, requestTrack));
    prevArriveTime += arriveTime;
  }
}
// 打印信息
void printInfo(list<PCB*>& pcbs, list<PCB*>::iterator nextIt,
      int& curTrack, int& curTime, int& sumAcrossTrack)
{
  // nextPcb 先解引用，才是 PCB*
  cout << "当前磁道: " << curTrack << "; 当前时间: " << curTime 
    << "; 下一个要访问的磁道: " << (*nextIt)->requestTrack_ 
    << "; 横跨的磁道数: " << abs(curTrack - (*nextIt)->requestTrack_) 
    << "; 横跨的总磁道数: " << sumAcrossTrack << endl;

  cout << "PCB信息: " << endl;
  int i = 0;
  for(const auto& pcb : pcbs)
  {
    cout << "第" << ++i << "个pcb: 请求磁道数：" << pcb->requestTrack_ 
      << ", 到达时间: " << pcb->arriveTime_ << endl;
  }
  cout << endl;
}

void update(list<PCB*>& pcbs, list<PCB*>::iterator nextIt, 
      int& curTrack, int& curTime, int& sumAcrossTrack)
{
  int requestTrack = (*nextIt)->requestTrack_;
  curTime += (abs(curTrack - requestTrack));
  // 横跨的总磁道数也是加这个磁道差
  sumAcrossTrack += (abs(curTrack - requestTrack));  
  curTrack = requestTrack;
  pcbs.erase(nextIt);
  nextIt = pcbs.end();
}

// SSTF 方案：最短服务时间优先，就是每次找最近的磁道
void SSTF(list<PCB*> pcbs, int& curTrack, int& curTime, int& sumAcrossTrack)
{
  // 第一个磁道最先访问，访问完后删除掉
  // 考虑这样一种情况：任务生成的比较慢，在访问完一个磁道后没有新任务了！！但是总任务还没执行完！！！
  auto nextIt = pcbs.begin();
  printInfo(pcbs, nextIt, curTrack, curTime, sumAcrossTrack);
  update(pcbs, nextIt, curTrack, curTime, sumAcrossTrack);
  nextIt = pcbs.end();
  while(!pcbs.empty())
  {
    int minDis = 200;
    // 1.遍历找下一个磁道
    auto iter = pcbs.begin();
    while(iter != pcbs.end())
    {
      PCB* curPcb = (*iter);
      int curDis = abs(curTrack - curPcb->requestTrack_);
      // 找当前已经到达的最近的PCB
      if(curPcb->arriveTime_ <= curTime && curDis < minDis)
      {
        minDis = curDis;
        nextIt = iter;
      }
      ++iter;
    }
    // 2.访问下一个磁道，先打印
    if(nextIt != pcbs.end())
    {
      printInfo(pcbs, nextIt, curTrack, curTime, sumAcrossTrack);
      update(pcbs, nextIt, curTrack, curTime, sumAcrossTrack);
      nextIt = pcbs.end();
    }
    else
    {
      cout << "当前没有任务！！" << endl;
      // 简单处理一下！
      curTime += 30;
      sleep(1);
    }
  }
}

// SCAN 方案
void SCAN(list<PCB*> pcbs, int& curTrack, int& curTime, int& sumAcrossTrack)
{
  // 0-->200: true  200->0: false    
  bool flag = true;
  auto nextIt = pcbs.end();
  while(!pcbs.empty())
  {
    int minDis = 200;
    // 1.遍历找下一个磁道
    auto iter = pcbs.begin();
    while(iter != pcbs.end())
    {
      PCB* curPcb = (*iter);
      int curDis = abs(curTrack - curPcb->requestTrack_);
      if(flag)
      {
        // 0->200
        if(curPcb->arriveTime_ <= curTime && curPcb->requestTrack_ >= curTrack 
            && curDis < minDis)
        {
          minDis = curDis;
          nextIt = iter;
        }
      }
      else 
      {
        // 200->0
        if(curPcb->arriveTime_ <= curTime && curPcb->requestTrack_ < curTrack 
            && curDis < minDis)
        {
          minDis = curDis;
          nextIt = iter;
        }
      }
      ++iter;
    }
    // 2.访问下一个磁道，先打印
    if(nextIt != pcbs.end())
    {
      printInfo(pcbs, nextIt, curTrack, curTime, sumAcrossTrack);
      update(pcbs, nextIt, curTrack, curTime, sumAcrossTrack);
      nextIt = pcbs.end();
    }
    else
    {
      // 这里也有可能遇到任务生成的比较慢，在访问完一个磁道后没有新任务了,但是总任务还没执行完的情况
      if(flag)
      {
        cout << "没有正向任务，开始反向寻道" << endl;
        curTime += 20;
        flag = false; 
      }
      else 
      {
        cout << "没有反向任务，开始正向寻道" << endl;
        curTime += 20;
        flag = true; 
      }
      sleep(1);
    }
  }
}