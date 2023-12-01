#pragma once 
#include <iostream>
#include <chrono>
#include <queue>
#include <atomic>

#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

using namespace std::chrono;
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::atomic_bool;

// 共产生 TASKNUM 个 Task
#define TASKNUM 5
// 所有任务生产完成的标记
atomic_bool processDone;
// 整把全局互斥锁，保护临界资源 tasks
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

class TaskNode
{
public:
  // 构造
  TaskNode(const steady_clock::time_point comeTime, int processTime)
    : comeTime_(comeTime)
    , totalProcessTime_(processTime)
    , remainProcessTime_(processTime)
  {}

  void setCompleteTime(const steady_clock::time_point completeTime) 
  {
    completeTime_ = completeTime;
  }
  int getRemainProcessTime() const
  {
    return remainProcessTime_;
  }
  void setRemainProcessTime(const int remainProcessTime)
  {
    remainProcessTime_ = remainProcessTime;
  }
  void PrintIndicator()
  {
    // 周转时间 = 任务完成时刻 - 任务到达时刻
    // 带权周转时间 = 周转时间 / 实际服务时间
    duration<double> time_span = duration_cast<duration<double>>(completeTime_-comeTime_);
    cout << "周转时间：" << time_span.count() << " seconds" << endl;
    cout << "带权周转时间：" << time_span.count() / (double)(totalProcessTime_ / 1000) << " seconds"<< endl; 
  }

private:
  int totalProcessTime_;                         // 总处理时间  单位：ms
  int remainProcessTime_;                        // 剩余处理时间  单位：ms
  steady_clock::time_point comeTime_;            // Task到达时间
  steady_clock::time_point completeTime_;        // Task 处理完成时间
};

class Tasks
{
public:
  Tasks()
  {}

  ~Tasks()
  {
    // 释放 new 的资源
    while(!Empty())
    {
      TaskNode* tmp = Front();
      Pop();
      delete tmp;
    }
  }

  void Push(TaskNode* task)
  {
    readyQueue_.push(task);
  }
  void Pop()
  {
    readyQueue_.pop();
  }
  bool Empty()
  {
    return readyQueue_.empty();
  }
  TaskNode* Front()
  {
    return readyQueue_.front();
  }

  void Print();
  void ProcessTask();

private:
  // 1. FCFS,非抢占
  std::queue<TaskNode*> readyQueue_;
};

void Tasks::Print()
{
  int num = 1;
  Tasks tmpTasks;
  while(!Empty())
  {
    TaskNode* cur = Front();
    printf("第%d个task:\n", num++);
    cur->PrintIndicator();
    tmpTasks.Push(cur);
    Pop();
  }
  while(!tmpTasks.Empty())
  {
    Push(tmpTasks.Front());
    tmpTasks.Pop();
  }
}

void Tasks::ProcessTask()
{
  cout << "start process task" << endl;
  Tasks tmpTasks;
  auto start = steady_clock::now();
  while(!processDone)
  {
    while(!Empty())
    {
      // 1.1 从 tasks 中取出 tasknode
      pthread_mutex_lock(&mutex);          // 访问临界资源 加锁
      TaskNode* curTaskNode = Front();
      Pop();
      pthread_mutex_unlock(&mutex);        // 解锁

      tmpTasks.Push(curTaskNode);
      // 1.2 模拟执行 Task  非抢占式
      usleep(curTaskNode->getRemainProcessTime() * 1000);
      curTaskNode->setRemainProcessTime(0);
      curTaskNode->setCompleteTime(steady_clock::now());
      cout << "process a task done" << endl;
    }
  }

  // 这里所有task都处理完了，就不加锁了
  while(!tmpTasks.Empty())
  {
    Push(tmpTasks.Front());
    tmpTasks.Pop();
  }
  auto end = steady_clock::now();
  duration<double> time_span = duration_cast<duration<double>>(end - start);
  cout << "process all tasks done, total process time: " << time_span.count() << " seconds" << endl;
}

void* ProduceTask(void* ptasks)
{
  if(!ptasks)
  {
    cerr << "ptasks is nullptr!" << endl;
    exit(1);
  } 

  int sleepTime, processTime; 
  Tasks* ptasksCur = (Tasks*)ptasks;

  for(int i = 0; i < TASKNUM; ++i)
  {
    // 1.获取 [2000, 3000] 之间的随机数
    sleepTime = (rand() % 1000) + 2000;
    // cout << "sleepNum: " << sleepNum << endl;
    
    // 2.休眠 sleepNum 毫秒，usleep单位是微秒！这里就是休眠 2~3秒
    usleep(sleepTime * 1000);

    // 获取 Task 的到来时间和处理时间
    auto comeTime = steady_clock::now();
    processTime = (rand() % 1000) + 1000;    // 处理时间在 1~6s之间 
    TaskNode* taskNode = new TaskNode(comeTime, processTime);
    // 加锁
    pthread_mutex_lock(&mutex);   // 阻塞式加锁
    ptasksCur->Push(taskNode);
    pthread_mutex_unlock(&mutex);

  }
  processDone = true;
  return NULL;
}