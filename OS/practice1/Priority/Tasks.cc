#include "CPU.h"

void Tasks::Print()
{
  int num = 1;
  Tasks tmpTasks;
  while(!Empty())
  {
    TaskNode* cur = Top();
    printf("第%d个task:\n", num++);
    cur->PrintIndicator();
    tmpTasks.Push(cur);
    Pop();
  }
  while(!tmpTasks.Empty())
  {
    Push(tmpTasks.Top());
    tmpTasks.Pop();
  }
}

void Tasks::ProcessTasks()
{
  cout << "start process task" << endl;
  Tasks tmpTasks;
  CPU cpu;
  auto start = steady_clock::now();
  while(!processDone)
  {
    while(!Empty())
    {
      // 1.1 从 tasks 中取出 tasknode
      TaskNode* curTaskNode = Top();
      cpu.curProcessTask_ = curTaskNode;
      // 1.2  交给cpu执行curTask
      cpu.RunTask(this);

      if(cpu.isTrip_)
      {
        // 1.3 被剥夺
        // 1.3.1 把当前process优先级降1
        if(curTaskNode->priority_ > 20) (curTaskNode->priority_)--;
        // 1.3.2 
      }
      else
      {
        tmpTasks.Push(curTaskNode);
        Pop();
        curTaskNode->setRemainProcessTime(0);
        curTaskNode->setCompleteTime(steady_clock::now());
        cout << "process a task done" << endl;
      }
    }
  }

  while(!tmpTasks.Empty())
  {
    Push(tmpTasks.Top());
    tmpTasks.Pop();
  }

  auto end = steady_clock::now();
  duration<double> time_span = duration_cast<duration<double>>(end - start);
  cout << "process tasks done, total process time: " << time_span.count() << " seconds" << endl;
}