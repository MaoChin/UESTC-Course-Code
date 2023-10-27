#include "Tasks.h"

// 方案二：优先级+抢占式
void* ProduceTask(void*);
int main()
{
  cout << "main" << endl;
  // 随机数种子
  srand(time(0));
  Tasks tasks;

  // 1.1 采用多线程，新线程负责生产 Task
  pthread_t thread1;
  pthread_create(&thread1, NULL, ProduceTask, (void*)&tasks);
  // 1.2 分离这个新线程，主线程就不用 join 了
  int ret = pthread_detach(thread1);
  if(ret != 0) cerr << "main: pthread_detach() error!" << endl;

  // 2.1 主线程处理 Task
  tasks.ProcessTasks();

  // 3. 打印各类指标
  tasks.Print();

  // 4. 释放 new 的资源
  while(!tasks.Empty())
  {
    TaskNode* tmp = tasks.Top();
    tasks.Pop();
    delete tmp;
  }
  return 0;
}

void* ProduceTask(void* ptasks)
{
  cout << "Prodeuce Task start!" << endl;
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
    priority = (rand() % 80) + 20;           // 优先级在 [20, 100]之间
    TaskNode* taskNode = new TaskNode(comeTime, processTime, priority);

    // 加锁
    pthread_mutex_lock(&mutex);   // 阻塞式加锁
    ptasksCur->Push(taskNode);
    pthread_mutex_unlock(&mutex);
  }
  processDone = true;
  cout << "Process Task Done!" << endl;
  return NULL;
}

