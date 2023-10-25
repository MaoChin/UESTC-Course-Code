#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#include "Task.hpp"

// 所有任务生产完成的标记
static bool processDone = false;

// 整把全局互斥锁，保护临界资源 tasks
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* ProduceTask(void*);
void ProcessTask(Tasks*);
int main()
{
  // 随机数种子
  srand(time(0));
  Tasks tasks;

  // 1.1 采用多线程，新线程负责生产 Task
  pthread_t thread1;
  pthread_create(&thread1, NULL, ProduceTask, (void*)&tasks);
  // 1.2 分离这个新线程，主进程就不用 join 了
  int ret = pthread_detach(thread1);
  if(ret != 0) cerr << "main: pthread_detach() error!" << endl;

  // 2.1 主线程处理 Task
  ProcessTask(&tasks);


  // 3. 打印各类指标
  tasks.Print();

  return 0;
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
    processTime = (rand() % 5000) + 1000;    // 处理时间在 1~6s之间 
    TaskNode* taskNode = new TaskNode(comeTime, processTime);
    // 加锁
    pthread_mutex_lock(&mutex);   // 阻塞式加锁
    ptasksCur->Push(taskNode);
    pthread_mutex_unlock(&mutex);

  }
  processDone = true;
  return NULL;
}

void ProcessTask(Tasks* ptasks)
{
  if(!ptasks)
  {
    cerr << "ptasks is nullptr!" << endl;

    exit(1);
  } 
  cout << "start process task" << endl;
  Tasks tmpTasks;
  auto start = steady_clock::now();
  while(!processDone)
  {
    while(!ptasks->Empty())
    {
      // 1.1 从 tasks 中取出 tasknode
      TaskNode* curTaskNode = ptasks->Front();
      tmpTasks.Push(curTaskNode);
      ptasks->Pop();
      // 1.2 模拟执行 Task  非抢占式
      usleep(curTaskNode->getRemainProcessTime() * 1000);
      curTaskNode->setRemainProcessTime(0);
      curTaskNode->setCompleteTime(steady_clock::now());
      cout << "process a task done" << endl;
    }
  }

  while(!tmpTasks.Empty())
  {
    ptasks->Push(tmpTasks.Front());
    tmpTasks.Pop();
  }
  auto end = steady_clock::now();
  duration<double> time_span = duration_cast<duration<double>>(end - start);
  cout << "process tasks done, total process time: " << time_span.count() << " seconds" << endl;
}