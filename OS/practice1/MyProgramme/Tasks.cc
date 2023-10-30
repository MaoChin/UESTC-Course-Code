#include "Tasks.h"
#include "CPU.hpp"

void Tasks::Print()
{
  int num = 1;
  for(auto& elem : printVector_)
  {
    printf("第%d个task:\n", num++);
    elem->PrintIndicator();
  }
}

void Tasks::ProcessTasks()
{
  cout << "start process task" << endl;
  CPU cpu;
  auto start = steady_clock::now();
  while(!processDone)
  {
    while(!Empty())
    {
      // 1.1 从 tasks 中取出 tasknode
      pthread_mutex_lock(&mutex);           // 加锁
      TaskNode* curTaskNode = Top();
      Pop();
      pthread_mutex_unlock(&mutex);         // 解锁
      cpu.curProcessTask_ = curTaskNode;

      // 1.2  交给cpu执行curTask
      cpu.RunTask(this, &mutex);
    }
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

  int sleepTime, processTime, priority; 
  Tasks* ptasksCur = (Tasks*)ptasks;

  for(int i = 0; i < TASKNUM; ++i)
  {
    // 1.获取 [2000, 3000] 之间的随机数
    sleepTime = (rand() % 1000) + 2000;
    // cout << "sleepNum: " << sleepNum << endl;
    
    // 2.休眠 sleepNum 毫秒，usleep单位是微秒！这里就是休眠 2~3秒
    usleep(sleepTime * 1000);

    // 3.构造Task
    auto comeTime = steady_clock::now();
    processTime = (rand() % 5000) + 1000;    // 处理时间在 1~6s之间 

    // 短任务优先！
    if(processTime < 1000) priority = 100;
    else priority = (rand() % 5) + 20;           // 优先级在 [20, 25]之间
    TaskNode* taskNode = new TaskNode(comeTime, processTime, priority);

    // 加锁
    pthread_mutex_lock(&mutex);   // 阻塞式加锁
    ptasksCur->Push(taskNode);
    pthread_mutex_unlock(&mutex);

    ptasksCur->Push(taskNode, 0);
  }
  processDone = true;
  return NULL;
}