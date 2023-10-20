#include <iostream>

#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#include "Task.hpp"

// 共产生 TASKNUM 个 Task
#define TASKNUM 5

// 所有任务生产完成的标记
static bool processDone = false;


void* ProduceTask(void*);
void ProcessTask();
int main()
{
  // 随机数种子
  srand(time(0));

  // 1.1 采用多线程，新线程负责生产 Task
  pthread_t thread1;
  pthread_create(&thread1, NULL, ProduceTask, NULL);
  // 1.2 分离这个新线程，主进程就不用 join 了
  int ret = pthread_detach(thread1);
  if(ret != 0) std::cerr << "main: pthread_detach() error!" << std::endl;

  // 2.1 主线程处理 Task
  ProcessTask();


  // 3. 打印各类指标


  return 0;
}

void* ProduceTask(void*)
{
  int sleepNum; 
  Task task;

  for(int i = 0; i < TASKNUM; ++i)
  {
    // 1.获取 [2000, 3000] 之间的随机数
    sleepNum = (rand() % 1000) + 2000;
    // std::cout << "sleepNum: " << sleepNum << std::endl;
    
    // 2.休眠 sleepNum 毫秒，usleep单位是微秒！这里就是休眠 2~3秒
    usleep(sleepNum * 1000);

    auto inReadyQueue = steady_clock::now();
    TaskNode* taskNode = new TaskNode(inReadyQueue);
    task.Push(taskNode);
  }
  processDone = true;
  return NULL;
}

void ProcessTask()
{
  std::cout << "start process task" << std::endl;
  auto start = steady_clock::now();
  while(!processDone)
  {
    while(!readyQueue.empty())
    {
      // 1.1 从 readyQueue 中取出 Task
      Task* curTask = readyQueue.front();
      readyQueue.pop();
      curTask->setOutReadyQueue(std::chrono::steady_clock::now());
      // 1.2 模拟执行 Task
      usleep(curTask->getProcessTime() * 1000);
      // 把堆空间释放掉！
      delete curTask;
    }
  }
  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> time_span = 
    std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
  std::cout << "process time: " << time_span.count() << " seconds" << std::endl;
  std::cout << "process task done" << std::endl;
}