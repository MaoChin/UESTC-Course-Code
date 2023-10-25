#include "banker.hpp"

void Init()
{
	cout << "请输入请求资源的进程个数# "; 
  cin >> n;
	cout << "请输入资源的种类个数# ";
  cin >> m;
	cout << endl;

	cout << "初始化Max矩阵" << endl;
  printf("请输入一个%d行%d列的矩阵表示%d个进程对%d类资源的总需求量(Max矩阵)#\n", n, m, n, m);
	for (int i = 1;i <= n;i++)
		for (int j = 1;j <= m;j++)
			cin >> Max[i][j];

  cout << endl;

	cout << "初始化Allocation矩阵" << endl;
  printf("请输入一个%d行%d列的矩阵表示%d个进程对%d类资源已经申请量(Allocation矩阵)#\n", n, m, n, m);
	for (int i = 1;i <= n;i++)
		for (int j = 1;j <= m;j++)
			cin >> Allocation[i][j];
  cout << endl;

	cout << "初始化Available向量" << endl;
	cout << "请输入一行" << m << "个整数表示每种资源的总量(Available向量)#" << endl;
	for (int i = 1;i <= m;i++) cin >> Available[i];

	// 初始化Need矩阵
	for (int i = 1;i <= n;i++)
	{
		for (int j = 1;j <= m;j++)
			Need[i][j] = Max[i][j] - Allocation[i][j];
	}
	// 初始化Sum矩阵
	for (int j = 1;j <= m;j++)
	{
		for (int i = 1;i <= n;i++)
			Sum[j] += Allocation[i][j];

		Sum[j] += Available[j];
	}
	cout << "所有初始化已完成！" << endl;
}

void Menu()
{
	while (true)
	{
		cout << endl;
		cout << "*********************************************" << endl;
		cout << "************ 选项1: 请求资源 ****************" << endl;
		cout << "************ 选项2: 检查当前状态是否安全 ****" << endl;
		cout << "************ 选项3: 查看各类资源信息 ********" << endl;
		cout << "************ 选项0: 退出程序 ****************" << endl;
		cout << "*********************************************" << endl;
		cout << "请输入选项# ";
		int optional; 
		cin >> optional;
		switch(optional)
		{
			case 0:
			{
				cout << "退出程序！" << endl;
				return;
			}
			case 1:
			{
				QuerySource();
				break;
			}
			case 2:
			{
				// 存放安全序列
				std::queue<int> securityQueue;
				if (SecurityCheck(securityQueue))
				{
					cout << "当前状态安全" << endl;
					cout << "其中的一个安全序列是# ";
					while (!securityQueue.empty())
					{
						cout << securityQueue.front() << "->";
						securityQueue.pop();
					}
					cout << endl;
				}
				else 
					cout << "当前状态不安全，有可能进入死锁状态" << endl;
				break;
			}
			case 3:
			{
				Print();
				break;
			}
			default:
			{
				cout << "输入的选项错误，请按照要求输入！！！" << endl;
				break;
			}
		}
	}
}

int main()
{
	Init();
	Menu();
	return 0;
}

/*
测试用例：
n = 5, m = 3
Max
7 5 3
3 2 2
9 0 2
2 2 2
4 3 3
Allocation
0 1 0
2 0 0
3 0 2
2 1 1 
0 0 2
Available
3 3 2
第一次查询是否安全：安全

第一次请求：2号进程申请资源：1 0 2 -> 可以申请！
申请后Allocation
0 1 0
3 0 2
3 0 2
2 1 1 
0 0 2
Available
2 3 0

第二次请求：5号进程申请资源：3 3 0 -> 申请失败！

第三次请求：1号进程申请资源：0 2 0 -> 申请失败(不安全)！
申请后Allocation
0 3 0
3 0 2
3 0 2
2 1 1 
0 0 2
Available
2 1 0
*/
