#pragma once 
#include <chrono>
#include <queue>

using namespace std::chrono;
class TaskNode
{
public:
  // 构造
  TaskNode(const steady_clock::time_point comeTime)
    : comeTime_(comeTime)
  {
  }

  void setCompleteTime(const steady_clock::time_point completeTime) 
  {
    completeTime_ = completeTime;
  }
  int getRemainProcessTime() const
  {
    return remainProcessTime_;
  }
  int setRemainProcessTime(const int remainProcessTime)
  {
    remainProcessTime_ = remainProcessTime;
  }

private:
  int remainProcessTime_;                        // 单位：ms
  steady_clock::time_point comeTime_;            // Task到达时间
  steady_clock::time_point completeTime_;        // Task 处理完成时间
};

class Task
{
public:
  Task()
  {
    readyQueue_ = new std::queue<TaskNode*>;
  }

  ~Task()
  {
    delete readyQueue_;
    readyQueue_ = nullptr;
  }

  void Push(TaskNode* task)
  {
    readyQueue_->push(task);
  }


private:
  // 1. FCFS,非抢占
  std::queue<TaskNode*>* readyQueue_;
};