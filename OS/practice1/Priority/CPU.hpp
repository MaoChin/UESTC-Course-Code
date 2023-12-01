#pragma once 

#define TO_CLOCK(remainProcessTime) ((clock_t)(((float)(remainProcessTime / 1000)) * CLOCKS_PER_SEC))

struct CPU
{
  CPU(TaskNode* curProcessTask  = nullptr, bool beDeprive = false)
    : curProcessTask_(curProcessTask)
    , beDeprive_(beDeprive)
  {}

  void RunTask(Tasks* const tasks)
  {
    beDeprive_ = false;
    // 1. 获取当前Task运行时间 单位：ms
    int remainProcessTime = curProcessTask_->getRemainProcessTime();
    // clock 的单位是clicks
    clock_t end = clock() + TO_CLOCK(remainProcessTime);
    // clock_t end = clock() + ((clock_t)(((float)(remainProcessTime / 1000)) * CLOCKS_PER_SEC));

    // 2. 处理Task，同时检测是否会被剥夺
    while(true)
    {
      // 2.1 正常处理完成
      if(clock() > end) break;

      // 这里也加锁可能会好一些，把mutex作为参数传进来。但考虑到这是死循环，所以不加锁也可以。
      if(tasks->Top()->priority_ > curProcessTask_->priority_)
      {
        // 2.2 被高优先级Task剥夺
        beDeprive_ = true;
        curProcessTask_->setRemainProcessTime(((end - clock()) / CLOCKS_PER_SEC) * 1000);
        break;
      }    
    }
  }

  TaskNode* curProcessTask_;
  bool beDeprive_;
};
