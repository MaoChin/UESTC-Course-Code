#include "Tasks.h"

// 方案二：优先级 + 抢占式
void* ProduceTask(void*);
int main()
{
  // 随机数种子
  srand(time(0));
  Tasks tasks;
  processDone = false;

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
  return 0;
}
