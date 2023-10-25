#include <cstdio>
#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <cstring>

#include <stdlib.h>

using std::cin;
using std::cout;
using std::cerr;
using std::endl;

#define N 1001
#define M 1001

int n, m;

// 所有的空间静态开辟好！
int Max[N][M], Allocation[N][M], Need[N][M];
int Available[M], Finish[N];
int Work[M], Request[M];
// 系统中 m 类资源的总量  
int Sum[M];


// 判断 Allocation矩阵的合法性
bool JudgeAllocation()
{
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1 ;j <= m; j++)
		{
			if (Allocation[i][j] > Max[i][j])
				return false;
		}
	}
	return true;
}
// 判断当前 work向量是否大于Need[i]向量
bool JudgeNeed(int k)
{
	for (int j = 1; j <= m; j++)
	{
		if (Need[k][j] > Work[j])
			return false;
	}
	return true;
}

// 递归处理
void _SecurityCheck(std::queue<int>& securityQueue, int& securityProcessNum)
{
	if(securityProcessNum == n) return;
	for(int i = 1; i <= n; ++i)
	{
		if(!Finish[i] && JudgeNeed(i))
		{
			++securityProcessNum;
			securityQueue.push(i);
			// 释放持有的资源
			for (int j = 1; j <= m; j++)
				Work[j] += Allocation[i][j];
			Finish[i] = true;

			_SecurityCheck(securityQueue, securityProcessNum);

			if(securityProcessNum == n) return;
			// 回溯
			--securityProcessNum;
			securityQueue.pop();
			for(int j = 1; j <= m; ++j)
				Work[j] -= Allocation[i][j];
			Finish[i] = false;
		}
	}
}

// 安全性检查  DFS+回溯
bool SecurityCheck(std::queue<int>& securityQueue)
{
	memcpy(Work, Available, sizeof(Available[0]) * (m+1));
	memset(Finish, 0, sizeof(Finish[0]) * (n+1));

  int securityProcessNum = 0;
	_SecurityCheck(securityQueue, securityProcessNum);
	return securityProcessNum == n;
}

// 预分配资源
void TryAlloc(int k)
{
	for (int j = 1; j <= m; j++)
	{
		Allocation[k][j] += Request[j];
		Need[k][j] -= Request[j];
		Available[j] -= Request[j];
	}
}
// 恢复预分配的资源
void RecoverAlloc(int k)
{
	for (int j = 1; j <= m; j++)
	{
		Allocation[k][j] -= Request[j];
		Need[k][j] += Request[j];
		Available[j] += Request[j];
	}
}
// 用户输入某个进程要请求的资源信息
void QuerySource()
{
	int processID;
  printf("请输入要请求资源的进程ID(1~%d)# ", n);
	cin >> processID;
	cout << "请输入一行" << m << "个整数表示该进程请求每种资源的数量# " << endl;
	for (int j = 1; j <= m; j++) 
	cin >> Request[j];
	for (int j = 1; j <= m; j++)
	{
		if (Request[j] > Need[processID][j])
		{
      printf("要请求的%d号资源大于该进程需求的资源总量，本次请求无效！\n", j);
			return;
		}
		else if (Request[j] > Available[j])
		{
      printf("要请求的%d号资源大于系统所剩余的资源总量，本次请求无效！\n", j);
			return;
		}
	}

	// 资源的预分配
	TryAlloc(processID);
	std::queue<int> q;
	if (SecurityCheck(q))
	{
		cout << "此次请求不会造成不安全状态，分配资源成功！" << endl;
		cout << "其中的一个安全序列# ";
		while (!q.empty())
		{
			cout << q.front() << "->";
			q.pop();
		}
		cout << endl;
	}
	else
	{
		cout << "此次请求会造成不安全状态！本次请求无效！" << endl;
		// 若分配资源进入不安全状态！恢复预分配
		RecoverAlloc(processID);
	}
}

void Print()
{
  cout << "系统资源剩余总量(Available)# ";
  for(int i = 1; i <= m; ++i)
    cout << Available[i] << " ";
  cout << endl;

  printf("%d个进程对每种资源的总需求量(Max)# \n", n);
  int num = 1;
  for(int i = 1; i <= n; ++i)
  {
    printf("进程%d: ", num++);
    for(int j = 1; j <= m; ++j)
      cout << Max[i][j] << " ";
    cout << endl;
  }

  printf("%d个进程对每种资源已经申请的数量(Allocation)# \n", n);
  num = 1;
  for(int i = 1; i <= n; ++i)
  {
    printf("进程%d: ", num++);
    for(int j = 1; j <= m; ++j)
      cout << Allocation[i][j] << " ";
    cout << endl;
  }
}