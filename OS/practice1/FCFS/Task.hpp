#pragma once 
#include <iostream>
#include <chrono>
#include <queue>

// 共产生 TASKNUM 个 Task
#define TASKNUM 5
using namespace std::chrono;
using std::cout;
using std::cin;
using std::cerr;
using std::endl;

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
    duration<double> time_span = duration_cast<duration<double>>(completeTime_-comeTime_);
    cout << "周转时间：" << time_span.count() << " seconds" << endl;
    cout << "带权周转时间：" << time_span.count() / (double)(totalProcessTime_ / 1000) << " seconds"<< endl; 
  }

private:
  int totalProcessTime_;                         // 总处理时间  单位：s
  int remainProcessTime_;                        // 剩余处理时间
  steady_clock::time_point comeTime_;            // Task到达时间
  steady_clock::time_point completeTime_;        // Task 处理完成时间
};

class Tasks
{
public:
  Tasks()
  {
  }

  ~Tasks()
  {
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

  void Print()
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

private:
  // 1. FCFS,非抢占
  std::queue<TaskNode*> readyQueue_;
};