#include "diskDispatch.h"

// 磁盘调度算法
// 大致思路：用整型递增表示时间，磁盘每横跨一个磁道时间为1。共有200个磁道 1~200；初始磁道固定为100
// 随机生成10个PCB，请求磁道随机产生，每 10~40 时间间隔产生一个PCB
// 由于规定磁盘每横跨一个磁道的时间是1，所以在没有等待的情况下，横跨的总磁道数就是总时间

static int curTrack = 100;
static int curTime = 0;
// 计算总的横跨磁道数
static int sumAcrossTrack = 0;  

int main()
{
  // 随机数种子
  srand(time(nullptr));
  // 用list存放所有PCB
  list<PCB*> pcbs;
  // 随机生成PCB
  creatAllPcb(pcbs);
   
  // SSFT 方案: 每次访问最近的磁道
  cout << "SSFT方案开始!" << endl;
  SSTF(pcbs, curTrack, curTime, sumAcrossTrack);

  cout << "SSFT方案所有磁道均访问完毕，横跨的总磁道数: " << sumAcrossTrack
    << "; 平均寻道长度: " << (double)sumAcrossTrack / 10 << endl;
  cout << "*******************************************************************************************" << endl;

  // 把这些值初始化一下
  curTrack = 100;
  curTime = 0;
  sumAcrossTrack = 0;
  
  // SCAN 方案：从100磁道-->200磁道-->0磁道-->200磁道循环往复
  cout << "SCAN方案开始!" << endl;
  SCAN(pcbs, curTrack, curTime, sumAcrossTrack);

  cout << "SCAN方案所有磁道均访问完毕，横跨的总磁道数: " << sumAcrossTrack
    << "; 平均寻道长度: " << (double)sumAcrossTrack / 10 << endl;

  // 释放资源
  for(auto& pcb : pcbs)
    delete pcb;
  return 0;
}