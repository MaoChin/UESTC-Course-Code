#pragma once 

#include "Tasks.h"

class CPU
{
public:
  CPU(TaskNode* curProcessTask  = nullptr, bool isTrip = false)
    : curProcessTask_(curProcessTask)
    , isTrip_(isTrip)
  {}

  void RunTask(Tasks* tasks);

  TaskNode* curProcessTask_;
  bool isTrip_;
};

void CPU::RunTask(Tasks* const tasks)
{
  isTrip_ = false;
  // 1. 获取当前Task运行时间 单位：ms
  int remainProcessTime = curProcessTask_->getRemainProcessTime();
  // clock 的单位是 微秒，准确来说 clock ticks
  clock_t end = clock() + (clock_t)(remainProcessTime * 1000);

  // 2. 处理Task，同时检测是否会被剥夺
  while(true)
  {
    // 2.1 正常处理完成
    if(clock() >= end) break;

    if(tasks->Top()->priority_ > curProcessTask_->priority_)
    {
      // 2.2 被高优先级Task剥夺
      isTrip_ = true;
      curProcessTask_->setRemainProcessTime((end - clock()) / 1000);
      break;
    }    
  }
}