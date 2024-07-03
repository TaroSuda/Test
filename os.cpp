#include <windows.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

// banker data structions
typedef struct
{
	int proceeding;				// number of proceedings.
	int resource;				// number of resources.
	int max_r[6];				// stand for the max number of each kind of resources .
	int Pro_maxResource[10][6]; // Pro_maxResource is mean each proceeding needs the max number of each resource
	int allocation[10][6];		// allocation[i][j]=k stand for proceeding[i] has been allocated k resource[j].
	int need[10][6];			// need[i][j]=k stands for proceeding[i] need k resource[j] for finishing.
	int finish[10];				// finish[i]=1 => proceeding[i] have been finished .
	int available[6];			// available each kind of resource;
} systemInformation;
typedef struct
{
	int PID;
	int need[6];
} currentInformation;

// some banker functions
void f1();
void f2();
bool check(systemInformation *p);
void banker(systemInformation *p, systemInformation *P);
int BANKER();

// 文件操作
int fileoperation();
// some CPU structions
typedef struct
{
	int id;
	// workTime1表示副本
	double arrive, workTime, waitTime, Turnaround, workTime1;
	bool FLAG;
} PCB;
typedef struct L
{
	int id;
	double time;
	double start;
	struct L *next;
} information;

// some CPU functions
int SJF1();
int SJF2();
int SJF();
int SJF_preemptive1();
int SJF_preemptive2();
int SJF_preemptive();
int RR1();
int RR2();
int RR();
int comp(const void *p, const void *q);
int compp(const void *p, const void *q);
int rrcomp(const void *p, const void *q);
int main()
{
	while (1)
	{
		system("cls");
		cout << "1:RR" << endl;
		cout << "2:SJF" << endl;
		cout << "3:SJF抢占式" << endl;
		cout << "4:赢行家算法" << endl;
		cout << "5:文件操作" << endl;
		cout << "6:退出" << endl;
		cout << "请选择（1-6）" << endl;
		int a;
		cin >> a;
		switch (a)
		{
		case 1:
			RR();
			break;
		case 2:
			SJF();
			break;
		case 3:
			SJF_preemptive();
			break;
		case 4:
			BANKER();
			break;
		case 5:
			fileoperation();
			break;
		case 6:
			return 0;
		default:
			cout << "请在1-6之间做选择" << endl;
			system("pause");
		}
	}
}
// 文件追加
int fileoperation()
{
	system("cls");
	cout << "==========================================" << endl;
	cout << "=              file operation            =" << endl;
	cout << "==========================================" << endl;
	string f1, f2;
	ofstream fp1;
	ifstream fp2;
	getchar();
label2:
	cout << "请输入第一个文件的路径和文件:" << endl;
	getline(cin, f1);
	fp1.open(f1, ios::app);
	if (!fp1.is_open())
	{
		cout << "文件不能打开，请核实路径以及文件！" << endl;
		goto label2;
	}
label1:
	cout << "请输入第二个文件的路径:" << endl;
	getline(cin, f2);
	fp2.open(f2);
	if (!fp2.is_open())
	{
		cout << "文件不能打开，请核实路径以及文件！" << endl;
		goto label1;
	}
	while (getline(fp2, f1))
	{
		fp1 << f1 << endl;
	}
	fp1.close();
	fp2.close();
	cout << "第二个文件追加到第一个文件中" << endl;
	cout << "文件操作完毕" << endl;
	system("pause");
	return 0;
}

// CPU调度
int RR()
{
label2:
	system("cls");
	cout << "==========================================" << endl;
	cout << "=                RR 算法                 =" << endl;
	cout << "==========================================" << endl;
	cout << endl;
	cout << "1.使用默认数据" << endl;
	cout << "2.输入数据" << endl;
	cout << "3.返回" << endl;
	int choice;
label1:
	cin >> choice;
	if (choice != 1 && choice != 2 && choice != 3)
	{
		cout << "输入有误，请重新输入（1-3):" << endl;
		goto label1;
	}
	system("cls");
	switch (choice)
	{
	case 1:
		RR2();
		goto label2;
		break;
	case 2:
		RR1();
		goto label2;
		break;
	case 3:
		return 0;
	}
}
int RR1()
{
	PCB p[20];
	information head, *cur = &head;
	bool flag = FALSE;
	int n, flag2;
	double jilu, seg;
	double avgWaitTime = 0;
	cout << "请输入进程数量:" << endl;
	cin >> n;
	cout << "请输入各进程到达时间和工作时间:" << endl;
	for (int i = 0; i < n; i++)
	{
		cout << "P" << i << ':';
		cin >> p[i].arrive >> p[i].workTime;
		p[i].id = i;
		p[i].workTime1 = p[i].workTime;
		p[i].FLAG = FALSE;
	}
	cout << "请输入时间片大小:" << endl;
	cin >> seg;
	qsort(p, n, sizeof(p[0]), rrcomp);
	jilu = 0;
	while (!flag)
	{
		for (int i = 0; i < n; i++)
		{
			if (p[i].workTime == 0)
				continue;
			else
			{
				flag2 = i;
				break;
			}
		}
		for (int i = 0; i < n; i++)
		{
			if (p[i].workTime == 0)
				continue;
			if (jilu < p[i].arrive)
			{
				if (i == flag2)
					jilu = p[i].arrive;
				else
					break;
			}
			information *m = (information *)malloc(sizeof(information));
			m->id = p[i].id;
			m->start = jilu;
			m->time = min(seg, p[i].workTime);
			cur->next = m;
			m->next = NULL;
			cur = m;
			p[i].workTime -= m->time;
			jilu += m->time;
			if (p[i].workTime == 0)
			{
				p[i].Turnaround = jilu - p[i].arrive;
				p[i].waitTime = p[i].Turnaround - p[i].workTime1;
			}
		}
		flag = TRUE;
		for (int i = 0; i < n; i++)
		{
			if (p[i].workTime != 0)
			{
				flag = FALSE;
				break;
			}
		}
	}
	cur = head.next;
	cout << endl;
	cout << "甘特图如下表：" << endl;
	while (cur != NULL)
	{
		cout << 'p' << cur->id << ' ' << cur->start << ' ' << cur->start + cur->time << endl;
		cur = cur->next;
	}
	for (int i = 0; i < n; i++)
	{
		avgWaitTime += p[i].waitTime;
	}
	avgWaitTime /= n;
	cout << "平均等待时间是:" << avgWaitTime << endl;
	cout << endl;
	system("pause");
	system("cls");
	return 0;
}
int RR2()
{
	PCB p[20];
	information head, *cur = &head;
	bool flag = FALSE;
	int n, flag2;
	double jilu, seg;
	double avgWaitTime = 0;
	n = 3;
	p[0].arrive = 0;
	p[1].arrive = 0;
	p[2].arrive = 0;
	p[0].workTime = 24;
	p[1].workTime = 3;
	p[2].workTime = 3;
	cout << "各进程到达时间和工作时间如下所示:" << endl;
	for (int i = 0; i < n; i++)
	{
		cout << "P" << i << ':';
		cout << p[i].arrive << ' ' << p[i].workTime << endl;
		p[i].id = i;
		p[i].workTime1 = p[i].workTime;
		p[i].FLAG = FALSE;
	}
	seg = 4;
	cout << "时间片大小:";
	cout << seg << endl;
	//................................................................this
	qsort(p, n, sizeof(p[0]), rrcomp);
	jilu = 0;
	while (!flag)
	{
		for (int i = 0; i < n; i++)
		{
			if (p[i].workTime == 0)
				continue;
			else
			{
				flag2 = i;
				break;
			}
		}
		for (int i = 0; i < n; i++)
		{
			if (p[i].workTime == 0)
				continue;
			if (jilu < p[i].arrive)
			{
				if (i == flag2)
					jilu = p[i].arrive;
				else
					break;
			}
			information *m = (information *)malloc(sizeof(information));
			m->id = p[i].id;
			m->start = jilu;
			m->time = min(seg, p[i].workTime);
			cur->next = m;
			m->next = NULL;
			cur = m;
			p[i].workTime -= m->time;
			jilu += m->time;
			if (p[i].workTime == 0)
			{
				p[i].Turnaround = jilu - p[i].arrive;
				p[i].waitTime = p[i].Turnaround - p[i].workTime1;
			}
		}
		flag = TRUE;
		for (int i = 0; i < n; i++)
		{
			if (p[i].workTime != 0)
			{
				flag = FALSE;
				break;
			}
		}
	}
	cur = head.next;
	cout << endl;
	cout << "甘特图如下表：" << endl;
	while (cur != NULL)
	{
		cout << 'p' << cur->id << ' ' << cur->start << ' ' << cur->start + cur->time << endl;
		cur = cur->next;
	}
	for (int i = 0; i < n; i++)
	{
		avgWaitTime += p[i].waitTime;
	}
	avgWaitTime /= n;
	cout << "平均等待时间是:" << avgWaitTime << endl;
	cout << endl;
	system("pause");
	system("cls");
	return 0;
}
int SJF_preemptive1()
{
	PCB p[20];
	information head, *cur = &head;
	int n;
	double jilu, avgWaitTime = 0;
	bool flag = TRUE;
	cout << "请输入进程数量:" << endl;
	cin >> n;
	cout << "请输入各进程到达时间和工作时间:" << endl;
	for (int i = 0; i < n; i++)
	{
		cout << "P" << i << ':';
		cin >> p[i].arrive >> p[i].workTime;
		p[i].id = i;
		p[i].workTime1 = p[i].workTime;
		p[i].FLAG = FALSE;
	}
	qsort(p, n, sizeof(p[0]), comp);
	int c = 0;
	jilu = p[0].arrive;
	for (int i = 0; i < n; i++)
	{
		flag = TRUE;
		for (int j = 0; j < n; j++)
		{
			if (p[j].workTime != 0)
			{
				flag = FALSE;
				break;
			}
		}
		if (flag)
			break;
		for (int j = 0; j < n; j++)
		{ // 寻找要运行的PCB
			if (p[j].workTime != 0)
			{
				i = j;
				break;
			}
		}
		c = 0;
		for (int j = 0; j < n; j++)
		{
			if (jilu >= p[j].arrive)
				c++;
		}
		if (i == c)
		{ // 当i==c时，也就是出现空缺的时候
			jilu = p[i].arrive;
			c++;
		}
		qsort(p, c, sizeof(p[0]), compp);
		if (c != n)
		{
			information *m = (information *)malloc(sizeof(information));
			m->id = p[i].id;
			m->start = jilu;
			m->time = min(p[c].arrive - jilu, p[i].workTime);
			cur->next = m;
			m->next = NULL;
			cur = m;
			p[i].workTime -= m->time;
			jilu += m->time;
			if (p[i].workTime == 0)
			{
				p[i].Turnaround = jilu - p[i].arrive;
				p[i].waitTime = p[i].Turnaround - p[i].workTime1;
			}
			i = -1;
		}
		else
		{ // 如果c==n，表示i是最后一个要运行的进程
			information *m = (information *)malloc(sizeof(information));
			m->id = p[i].id;
			m->start = jilu;
			m->time = p[i].workTime;
			cur->next = m;
			m->next = NULL;
			cur = m;
			p[i].workTime -= m->time;
			jilu += m->time;
			if (p[i].workTime == 0)
			{
				p[i].Turnaround = jilu - p[i].arrive;
				p[i].waitTime = p[i].Turnaround - p[i].workTime1;
			}
			i = -1;
		}
	}
	cur = head.next;
	cout << endl;
	cout << "甘特图如下表：" << endl;
	while (cur != NULL)
	{
		cout << 'p' << cur->id << ' ' << cur->start << ' ' << cur->start + cur->time << endl;
		cur = cur->next;
	}
	for (int i = 0; i < n; i++)
	{
		avgWaitTime += p[i].waitTime;
	}
	avgWaitTime /= n;
	cout << "平均等待时间是:" << avgWaitTime << endl;
	cout << endl;
	system("pause");
	system("cls");
	return 0;
}
int SJF_preemptive2()
{
	PCB p[20];
	information head, *cur = &head;
	int n = 4;
	double jilu, avgWaitTime = 0;
	bool flag = TRUE;
	p[0].arrive = 0;
	p[1].arrive = 1;
	p[2].arrive = 2;
	p[3].arrive = 3;
	p[0].workTime = 8;
	p[1].workTime = 4;
	p[2].workTime = 9;
	p[3].workTime = 5;
	cout << "各进程到达时间和工作时间如下所示:" << endl;
	for (int i = 0; i < n; i++)
	{
		cout << "P" << i << ':';
		cout << p[i].arrive << ' ' << p[i].workTime << endl;
		p[i].id = i;
		p[i].workTime1 = p[i].workTime;
		p[i].FLAG = FALSE;
	}
	qsort(p, n, sizeof(p[0]), comp);
	int c = 0;
	jilu = p[0].arrive;
	for (int i = 0; i < n; i++)
	{
		flag = TRUE;
		for (int j = 0; j < n; j++)
		{
			if (p[j].workTime != 0)
			{
				flag = FALSE;
				break;
			}
		}
		if (flag)
			break;
		for (int j = 0; j < n; j++)
		{ // 寻找要运行的PCB
			if (p[j].workTime != 0)
			{
				i = j;
				break;
			}
		}
		c = 0;
		for (int j = 0; j < n; j++)
		{
			if (jilu >= p[j].arrive)
				c++;
		}
		if (i == c)
		{ // 当i==c时，也就是出现空缺的时候
			jilu = p[i].arrive;
			c++;
		}
		qsort(p, c, sizeof(p[0]), compp);
		if (c != n)
		{
			information *m = (information *)malloc(sizeof(information));
			m->id = p[i].id;
			m->start = jilu;
			m->time = min(p[c].arrive - jilu, p[i].workTime);
			cur->next = m;
			m->next = NULL;
			cur = m;
			p[i].workTime -= m->time;
			jilu += m->time;
			if (p[i].workTime == 0)
			{
				p[i].Turnaround = jilu - p[i].arrive;
				p[i].waitTime = p[i].Turnaround - p[i].workTime1;
			}
			i = -1;
		}
		else
		{ // 如果c==n，表示i是最后一个要运行的进程
			information *m = (information *)malloc(sizeof(information));
			m->id = p[i].id;
			m->start = jilu;
			m->time = p[i].workTime;
			cur->next = m;
			m->next = NULL;
			cur = m;
			p[i].workTime -= m->time;
			jilu += m->time;
			if (p[i].workTime == 0)
			{
				p[i].Turnaround = jilu - p[i].arrive;
				p[i].waitTime = p[i].Turnaround - p[i].workTime1;
			}
			i = -1;
		}
	}
	cur = head.next;
	cout << endl;
	cout << "甘特图如下表：" << endl;
	while (cur != NULL)
	{
		cout << 'p' << cur->id << ' ' << cur->start << ' ' << cur->start + cur->time << endl;
		cur = cur->next;
	}
	for (int i = 0; i < n; i++)
	{
		avgWaitTime += p[i].waitTime;
	}
	avgWaitTime /= n;
	cout << "平均等待时间是:" << avgWaitTime << endl;
	cout << endl;
	system("pause");
	system("cls");
	return 0;
}
int SJF_preemptive()
{
label2:
	system("cls");
	cout << "==========================================" << endl;
	cout << "=           SJF_preemptive 算法          =" << endl;
	cout << "==========================================" << endl;
	cout << endl;
	cout << "1.使用默认数据" << endl;
	cout << "2.输入数据" << endl;
	cout << "3.返回" << endl;
	int choice;
label1:
	cin >> choice;
	if (choice != 1 && choice != 2 && choice != 3)
	{
		cout << "输入有误，请重新输入（1-3):" << endl;
		goto label1;
	}
	system("cls");
	switch (choice)
	{
	case 1:
		SJF_preemptive2();
		goto label2;
		break;
	case 2:
		SJF_preemptive1();
		goto label2;
		break;
	case 3:
		return 0;
	}
}
int SJF1()
{
	PCB p[20];
	information head, *cur = &head;
	int n;
	double jilu, avgWaitTime = 0;
	cout << "请输入进程数量:" << endl;
	cin >> n;
	cout << "请输入各进程到达时间和工作时间:" << endl;
	for (int i = 0; i < n; i++)
	{
		cout << "P" << i << ':';
		cin >> p[i].arrive >> p[i].workTime;
		p[i].workTime1 = p[i].workTime;
		p[i].id = i;
		p[i].FLAG = FALSE;
	}
	qsort(p, n, sizeof(p[0]), comp);
	int c = 0;
	jilu = p[0].arrive;
	for (int i = 0; i < n; i++)
	{ // 寻找要运行的PCB
		if (p[i].FLAG != TRUE)
		{
			if (c != 0)
			{
				if (i > c - 1)
					jilu = p[i].arrive;
			}
			information *m = (information *)malloc(sizeof(information));
			m->id = p[i].id;
			m->start = jilu;
			m->time = p[i].workTime;
			cur->next = m;
			m->next = NULL;
			cur = m;
			jilu += p[i].workTime;
			p[i].Turnaround = jilu - p[i].arrive;
			p[i].waitTime = p[i].Turnaround - p[i].workTime1;
			p[i].FLAG = TRUE;
			c = 0;
			for (int j = 0; j < n; j++)
			{
				if (jilu >= p[j].arrive)
					c++;
			}
			qsort(p, c, sizeof(p[0]), compp);
			i = -1;
		}
	}
	cur = head.next;
	cout << endl;
	cout << "甘特图如下表：" << endl;
	while (cur != NULL)
	{
		cout << 'p' << cur->id << ':' << cur->start << ' ' << cur->start + cur->time << endl;
		cur = cur->next;
	}
	for (int i = 0; i < n; i++)
	{
		avgWaitTime += p[i].waitTime;
	}
	avgWaitTime /= n;
	cout << "平均等待时间是:" << avgWaitTime << endl;
	cout << endl;
	system("pause");
	system("cls");
	return 0;
}
int SJF2()
{
	PCB p[20];
	information head, *cur = &head;
	int n = 4;
	double jilu, avgWaitTime = 0;
	p[0].arrive = 0;
	p[1].arrive = 0;
	p[2].arrive = 0;
	p[3].arrive = 0;
	p[0].workTime = 6;
	p[1].workTime = 8;
	p[2].workTime = 7;
	p[3].workTime = 3;
	cout << "各进程到达时间和工作时间如下所示:" << endl;
	for (int i = 0; i < n; i++)
	{
		cout << "P" << i << ':';
		cout << p[i].arrive << ' ' << p[i].workTime << endl;
		p[i].workTime1 = p[i].workTime;
		p[i].id = i;
		p[i].FLAG = FALSE;
	}
	qsort(p, n, sizeof(p[0]), comp);
	int c = 0;
	jilu = p[0].arrive;
	for (int i = 0; i < n; i++)
	{ // 寻找要运行的PCB
		if (p[i].FLAG != TRUE)
		{
			if (c != 0)
			{
				if (i > c - 1)
					jilu = p[i].arrive;
			}
			information *m = (information *)malloc(sizeof(information));
			m->id = p[i].id;
			m->start = jilu;
			m->time = p[i].workTime;
			cur->next = m;
			m->next = NULL;
			cur = m;
			jilu += p[i].workTime;
			p[i].Turnaround = jilu - p[i].arrive;
			p[i].waitTime = p[i].Turnaround - p[i].workTime1;
			p[i].FLAG = TRUE;
			c = 0;
			for (int j = 0; j < n; j++)
			{
				if (jilu >= p[j].arrive)
					c++;
			}
			qsort(p, c, sizeof(p[0]), compp);
			i = -1;
		}
	}
	cur = head.next;
	cout << endl;
	cout << "甘特图如下表：" << endl;
	while (cur != NULL)
	{
		cout << 'p' << cur->id << ':' << cur->start << ' ' << cur->start + cur->time << endl;
		cur = cur->next;
	}
	for (int i = 0; i < n; i++)
	{
		avgWaitTime += p[i].waitTime;
	}
	avgWaitTime /= n;
	cout << "平均等待时间是:" << avgWaitTime << endl;
	cout << endl;
	system("pause");
	system("cls");
	return 0;
}
int SJF()
{
label2:
	system("cls");
	cout << "==========================================" << endl;
	cout << "=               SJF 算法                 =" << endl;
	cout << "==========================================" << endl;
	cout << endl;
	cout << "1.使用默认数据" << endl;
	cout << "2.输入数据" << endl;
	cout << "3.返回" << endl;
	int choice;
label1:
	cin >> choice;
	if (choice != 1 && choice != 2 && choice != 3)
	{
		cout << "输入有误，请重新输入（1-3):" << endl;
		goto label1;
	}
	system("cls");
	switch (choice)
	{
	case 1:
		SJF2();
		goto label2;
		break;
	case 2:
		SJF1();
		goto label2;
		break;
	case 3:
		return 0;
	}
}
int comp(const void *p, const void *q)
{
	if (((PCB *)p)->arrive != ((PCB *)q)->arrive)
		return ((PCB *)p)->arrive < ((PCB *)q)->arrive ? -1 : 1;
	else
		return ((PCB *)p)->workTime < ((PCB *)q)->workTime ? -1 : 1;
}
int compp(const void *p, const void *q)
{
	return ((PCB *)p)->workTime < ((PCB *)q)->workTime ? -1 : 1;
}
int rrcomp(const void *p, const void *q)
{
	if (((PCB *)p)->arrive != ((PCB *)q)->arrive)
		return ((PCB *)p)->arrive < ((PCB *)q)->arrive ? -1 : 1;
}

int BANKER()
{
label2:
	system("cls");
	cout << "==========================================" << endl;
	cout << "=                银行家算法              =" << endl;
	cout << "==========================================" << endl;
	cout << endl;
	cout << "1.使用默认数据" << endl;
	cout << "2.输入数据" << endl;
	cout << "3.返回" << endl;
	int choice;
label1:
	cin >> choice;
	if (choice != 1 && choice != 2 && choice != 3)
	{
		cout << "输入有误，请重新输入（1-3):" << endl;
		goto label1;
	}
	system("cls");
	switch (choice)
	{
	case 1:
		f1();
		system("pause");
		goto label2;
		break;
	case 2:
		f2();
		system("pause");
		goto label2;
		break;
	case 3:
		return 0;
	}
}
void f1()
{
	systemInformation P;
	P.proceeding = 5;
	P.resource = 3;
	P.max_r[0] = 10;
	P.max_r[1] = 5;
	P.max_r[2] = 6;
	P.allocation[0][0] = 0;
	P.allocation[0][1] = 1;
	P.allocation[0][2] = 0;
	P.allocation[1][0] = 2;
	P.allocation[1][1] = 0;
	P.allocation[1][2] = 0;
	P.allocation[2][0] = 3;
	P.allocation[2][1] = 0;
	P.allocation[2][2] = 2;
	P.allocation[3][0] = 2;
	P.allocation[3][1] = 1;
	P.allocation[3][2] = 1;
	P.allocation[4][0] = 0;
	P.allocation[4][1] = 0;
	P.allocation[4][2] = 2;
	P.need[0][0] = 7;
	P.need[0][1] = 4;
	P.need[0][2] = 3;
	P.need[1][0] = 1;
	P.need[1][1] = 2;
	P.need[1][2] = 2;
	P.need[2][0] = 6;
	P.need[2][1] = 0;
	P.need[2][2] = 0;
	P.need[3][0] = 0;
	P.need[3][1] = 1;
	P.need[3][2] = 1;
	P.need[4][0] = 4;
	P.need[4][1] = 3;
	P.need[4][2] = 1;
	P.Pro_maxResource[0][0] = 7;
	P.Pro_maxResource[0][1] = 5;
	P.Pro_maxResource[0][2] = 3;
	P.Pro_maxResource[1][0] = 3;
	P.Pro_maxResource[1][1] = 2;
	P.Pro_maxResource[1][2] = 2;
	P.Pro_maxResource[2][0] = 9;
	P.Pro_maxResource[2][1] = 0;
	P.Pro_maxResource[2][2] = 2;
	P.Pro_maxResource[3][0] = 2;
	P.Pro_maxResource[3][1] = 2;
	P.Pro_maxResource[3][2] = 2;
	P.Pro_maxResource[4][0] = 4;
	P.Pro_maxResource[4][1] = 3;
	P.Pro_maxResource[4][2] = 3;
	P.available[0] = 3;
	P.available[1] = 3;
	P.available[2] = 2;
	cout << "使用数据如下所示:" << endl
		 << endl;
	cout << setw(7) << ' ' << "MAX" << setw(3) << ' ' << "ALLOCATION" << setw(4) << ' ' << "NEED" << endl;
	for (int i = 0; i < P.proceeding; i++)
	{
		cout << "P" << i + 1 << setw(5);
		for (int j = 0; j < P.resource; j++)
		{
			cout << P.Pro_maxResource[i][j] << " ";
		}
		cout << setw(5);
		for (int j = 0; j < P.resource; j++)
		{
			cout << P.allocation[i][j] << " ";
		}
		cout << setw(5);
		for (int j = 0; j < P.resource; j++)
		{
			cout << P.need[i][j] << " ";
		}
		cout << endl;
	}
	cout << "以下是可用资源数量:" << endl;
	for (int i = 0; i < P.resource; i++)
		cout << P.available[i] << " ";
	cout << endl
		 << endl;
	systemInformation fbP = P;
	banker(&fbP, &P);
}
void f2()
{
	systemInformation P;
	cout << "请输入进程数量（max 10）和资源类型数量（max 6):" << endl;
	cin >> P.proceeding >> P.resource;
	cout << "请输入每种资源的总个数:" << endl;
	for (int i = 0; i < P.resource; i++)
	{
		cin >> P.max_r[i];
		P.available[i] = P.max_r[i];
	}
	for (int i = 0; i < P.proceeding; i++)
	{
		cout << "请依次输入P" << i + 1 << "的每个资源MAX数量:" << endl;
		for (int j = 0; j < P.resource; j++)
			cin >> P.Pro_maxResource[i][j];
		cout << "请依次输入P" << i + 1 << "的每个资源ALLOCATION数量:" << endl;
		for (int j = 0; j < P.resource; j++)
		{
			cin >> P.allocation[i][j];
			P.need[i][j] = P.Pro_maxResource[i][j] - P.allocation[i][j];
			P.available[j] -= P.allocation[i][j];
		}
	}
	cout << "使用数据如下所示:" << endl
		 << endl;
	cout << setw(7) << ' ' << "MAX" << setw(3) << ' ' << "ALLOCATION" << setw(4) << ' ' << "NEED" << endl;
	for (int i = 0; i < P.proceeding; i++)
	{
		cout << "P" << i + 1 << setw(5);
		for (int j = 0; j < P.resource; j++)
		{
			cout << P.Pro_maxResource[i][j] << " ";
		}
		cout << setw(5);
		for (int j = 0; j < P.resource; j++)
		{
			cout << P.allocation[i][j] << " ";
		}
		cout << setw(5);
		for (int j = 0; j < P.resource; j++)
		{
			cout << P.need[i][j] << " ";
		}
		cout << endl;
	}
	cout << "以下是可用资源数量:" << endl;
	for (int i = 0; i < P.resource; i++)
		cout << P.available[i] << " ";
	cout << endl
		 << endl;
	systemInformation fbP = P;
	banker(&fbP, &P);
}
bool check(systemInformation *p)
{
	int temp[10] = {};
	int cnt = 0;
	for (int i = 0; i < p->proceeding; i++)
	{
		for (int j = 0; j < p->proceeding; j++)
		{
			int flag = 1;
			for (int k = 0; k < p->resource; k++)
			{
				if (p->need[j][k] > p->available[k])
				{
					flag = 0;
					break;
				}
			}
			if (flag == 1 && p->finish[j] != 1)
			{
				p->finish[j] = 1;
				temp[cnt++] = j + 1;
				for (int k = 0; k < p->resource; k++)
					p->available[k] += p->allocation[j][k];
				break;
			}
		}
	}
	if (cnt == p->proceeding)
	{
		cout << "状态安全" << endl;
		cout << "以下是一条安全序列:" << endl;
		for (int i = 0; i < p->proceeding; i++)
			cout << temp[i] << ' ';
		cout << endl;
		return TRUE;
	}
	else
	{
		cout << "状态不安全！" << endl;
		return FALSE;
	}
}
void banker(systemInformation *p, systemInformation *P)
{
	currentInformation a;
	if (check(p))
	{
		int f = 1;
		cout << "请输入要申请资料的进程:" << endl;
		cin >> a.PID;
		a.PID -= 1;
		cout << "P" << a.PID + 1 << "要申请各个资源的数量:" << endl;
		for (int i = 0; i < P->resource; i++)
			cin >> a.need[i];
		for (int i = 0; i < P->resource; i++)
		{
			if (a.need[i] > P->need[a.PID][i])
				f = 0;
			if (a.need[i] > P->available[i])
				f = 0;
		}
		for (int i = 0; i < P->resource; i++)
		{
			P->available[i] -= a.need[i];
			P->need[a.PID][i] -= a.need[i];
			P->allocation[a.PID][i] += a.need[i];
		}
		if (f == 0)
		{
			cout << "申请有问题，系统不安全!" << endl;
		}
		else
		{
			check(P);
		}
	}
}