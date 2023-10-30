#pragma once 
#include <iostream>
#include <chrono>
#include <queue>

#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

// 共产生 TASKNUM 个 Task
#define TASKNUM 5
// 定义时间片：2000ms(2s)
#define TIMESLICE 2000

// 整把全局互斥锁，保护临界资源 tasks
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// 所有任务生产完成的标记
bool processDone = false;

using namespace std::chrono;
using std::cout;
using std::cin;
using std::cerr;
using std::endl;

class TaskNode
{
public:
  // 构造
  TaskNode(const steady_clock::time_point comeTime, int processTime, int priority)
    : comeTime_(comeTime)
    , totalProcessTime_(processTime)
    , remainProcessTime_(processTime)
    , priority_(priority)
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
    duration<double> time_span = duration_cast<duration<double>>(completeTime_-comeTime_);
    cout << "周转时间：" << time_span.count() << " seconds" << endl;
    cout << "带权周转时间：" << time_span.count() / (double)(totalProcessTime_ / 1000) << " seconds"<< endl; 
  }

  // 优先级设置成 public
  int priority_;                                 // 优先级
private:
  int totalProcessTime_;                         // 总处理时间   单位：ms
  int remainProcessTime_;                        // 剩余处理时间 单位：ms
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
      TaskNode* tmp = Top();
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
  TaskNode* Top()
  {
    return readyQueue_.top();
  }
  void Print();
  void ProcessTasks();

private:
  // 1. 优先级+抢占式
  std::priority_queue<TaskNode*> readyQueue_;
};
