#pragma once 

// #define TO_CLOCK(remainProcessTime) 
//   ((clock_t)(((float)(remainProcessTime * 1000)) / CLOCKS_PER_SEC))

class CPU
{
public:
  CPU(TaskNode* curProcessTask  = nullptr)
    : curProcessTask_(curProcessTask)
  {}

  void RunTask(Tasks* const tasks, pthread_mutex_t* pmutex);

  TaskNode* curProcessTask_;
};

void CPU::RunTask(Tasks* const tasks, pthread_mutex_t* pmutex)
{
  if(pmutex == nullptr)
    std::cerr << "the pmutex is nullptr" << std::endl;
  // 1. 获取当前Task剩余运行时间 单位：ms
  int remainProcessTime = curProcessTask_->getRemainProcessTime();
  // 2. 处理task
  if(remainProcessTime > TIMESLICE)
  {
    usleep(TIMESLICE * 1000);
    int remainTime = remainProcessTime - TIMESLICE;
    curProcessTask_->setRemainProcessTime(remainTime);

    if(remainTime < 300) curProcessTask_->priority_ = 80; // 剩余时间很少了，提高优先级
    else --(curProcessTask_->priority_);

    std::cout << "current task's remain process time: " << 
      curProcessTask_->getRemainProcessTime() << "ms, exchange a new task!" << std::endl;

    // 加锁，把这个任务放回去
    pthread_mutex_lock(pmutex);
    tasks->Push(curProcessTask_);
    pthread_mutex_unlock(pmutex);
  }
  else 
  {
    usleep(remainProcessTime * 1000);
    curProcessTask_->setRemainProcessTime(0);
    curProcessTask_->setCompleteTime(steady_clock::now());
    std::cout << "process a task done" << std::endl;
  }
}