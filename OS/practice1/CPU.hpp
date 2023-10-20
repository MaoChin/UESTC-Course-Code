#pragma once 

#include "Task.hpp"

class CPU
{
public:
  CPU(TaskNode* curProcessTask = nullptr, bool isTrip = false)
    : curProcessTask_(curProcessTask)
    , isTrip_(isTrip)
  {}

  void RunTask();

  void setIsTrip(bool isTrip)
  {
    isTrip_ = isTrip;
  }

private:
  TaskNode* curProcessTask_;
  bool isTrip_;
};

void CPU::RunTask()
{
  // 1. 获取当前Task运行时间
  int remainProcessTime = curProcessTask_->getRemainProcessTime();
  clock_t end = clock() + (clock_t)remainProcessTime;
  // 2. 处理Task，同时检测是否会被剥夺
  while(true)
  {
    // 2.1 正常处理完成
    if(clock() >= end) break;

    if(isTrip_)
    {
      // 2.2 被高优先级Task剥夺
      // curProcessTask_->setRemainProcessTime(end - clock());
      break;
    }    
  }
  if(!isTrip_)
    curProcessTask_->setCompleteTime(steady_clock::now());
  else 
    curProcessTask_->setRemainProcessTime(end - clock());
}