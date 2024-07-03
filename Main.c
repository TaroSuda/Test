#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Process
{
    int id;
    bool flag;
    int arrivalTime;
    int serveTime;
    int startTime;
    int endTime;
    int waitTime;
    float turnaroundTime;
    struct Process *next;
} Process;

int SJF();
int SJF1();
int SJF2();
int SRTF();
int SRTF1();
int SRTF2();
int RR();
int RR1();
int RR2();
int BANK();
int BANK1();
int BANK2();
int Compare1(const void *a, const void *b);

void RunSJF(Process *p, int *time);

int SJF()
{
label1:
    system("cls");
    printf("SJF算法\n\n");
    printf("1:默认数据\n");
    printf("2:自定义数据\n");
    printf("3:返回\n");
label2:
    int a;
    scanf("%d", &a);
    switch (a)
    {
    case 1:
        SJF1();
        goto label1;
        break;
    case 2:
        SJF2();
        goto label1;
        break;
    case 3:
        return 0;
    default:
        printf("请输入正确的数字:\n");
        goto label2;
        break;
    }
}

int SJF1()
{
    float sumTime = 0;
    Process p[30];
    p[0].arrivalTime = 0;
    p[1].arrivalTime = 0;
    p[2].arrivalTime = 0;
    p[3].arrivalTime = 0;
    p[0].serveTime = 6;
    p[1].serveTime = 8;
    p[2].serveTime = 7;
    p[3].serveTime = 3;
    printf("各进程的到达时间和工作时间:\n");
    for (int i = 0; i < 4; i++)
    {
        p[i].id = i;
        p[i].flag = false;
        p[i].startTime = 0;
        p[i].endTime = 0;
        p[i].turnaroundTime = 0;
        p[i].waitTime = 0;
        printf("P%d      %d         %d\n", p[i].id, p[i].arrivalTime, p[i].serveTime);
    }
    qsort(p, 4, sizeof(p[0]), Compare1);
    int time = p[0].arrivalTime;
    printf("进程名  开始时间  结束时间\n");
    for (int i = 0; i < 4; i++)
    {
        if (!p[i].flag)
        {
            if (i == 0)
            {
                RunSJF(&p[i], &time);
            }
            else if (p[i].arrivalTime < time)
            {
                int t = i;
                while (p[t].arrivalTime < time && t < 4)
                {
                    t++;
                }
                int m = 1;
                for (int j = i; j < t - 1; j++)
                {
                    for (int k = i; k < t - m; k++)
                    {
                        if (p[k].serveTime > p[k + 1].serveTime)
                        {
                            Process temp = p[k];
                            p[k] = p[k + 1];
                            p[k + 1] = temp;
                        }
                    }
                    m++;
                }
                RunSJF(&p[i], &time);
            }
            else
            {
                time = p[i].arrivalTime;
                RunSJF(&p[i], &time);
            }
        }
    }

    for (int i = 0; i < 4; i++)
    {
        sumTime += p[i].waitTime;
    }
    printf("平均等待时间为:%.2f\n", sumTime / 4);
    system("pause");
    return 0;
}

int SJF2()
{
    int n;
    float sumTime = 0;
    Process p[30];
    printf("请输入进程数:");
    scanf("%d", &n);
    printf("请输入各进程的到达时间和工作时间:\n");
    for (int i = 0; i < n; i++)
    {
        printf("P%d:", i);
        scanf("%d %d", &p[i].arrivalTime, &p[i].serveTime);
        p[i].id = i;
        p[i].flag = false;
        p[i].startTime = 0;
        p[i].endTime = 0;
        p[i].turnaroundTime = 0;
        p[i].waitTime = 0;
    }
    qsort(p, n, sizeof(p[0]), Compare1);
    int time = p[0].arrivalTime;
    printf("进程名  开始时间  结束时间\n");
    for (int i = 0; i < n; i++)
    {
        if (!p[i].flag)
        {
            if (i == 0)
            {
                RunSJF(&p[i], &time);
            }
            else if (p[i].arrivalTime < time)
            {
                int t = i;
                while (p[t].arrivalTime < time && t < n)
                {
                    t++;
                }
                int m = 1;
                for (int j = i; j < t - 1; j++)
                {
                    for (int k = i; k < t - m; k++)
                    {
                        if (p[k].serveTime > p[k + 1].serveTime)
                        {
                            Process temp = p[k];
                            p[k] = p[k + 1];
                            p[k + 1] = temp;
                        }
                    }
                    m++;
                }
                RunSJF(&p[i], &time);
            }
            else
            {
                time = p[i].arrivalTime;
                RunSJF(&p[i], &time);
            }
        }
    }
    for (int i = 0; i < n; i++)
    {
        sumTime += p[i].waitTime;
    }
    printf("平均等待时间为:%.2f\n", sumTime / n);
    system("pause");
    return 0;
}

void RunSJF(Process *p, int *time)
{
    p->flag = true;
    p->startTime = *time;
    p->endTime = p->startTime + p->serveTime;
    *time = p->endTime;
    p->turnaroundTime = p->endTime - p->arrivalTime;
    p->waitTime = p->turnaroundTime - p->serveTime;
    printf("P%d      %d         %d\n", p->id, p->startTime, p->endTime);
}

int SRTF()
{
label1:
    system("cls");
    printf("SRTF算法\n\n");
    printf("1:默认数据\n");
    printf("2:自定义数据\n");
    printf("3:返回\n");
label2:
    int a;
    scanf("%d", &a);
    switch (a)
    {
    case 1:
        SRTF1();
        goto label1;
        break;
    case 2:
        SRTF2();
        goto label1;
        break;
    case 3:
        return 0;
    default:
        printf("请输入正确的数字:\n");
        goto label2;
        break;
    }
    return 0;
}

int SRTF1()
{
    return 0;
}

int SRTF2()
{
    return 0;
}

int RR()
{
label1:
    system("cls");
    printf("RR算法\n\n");
    printf("1:默认数据\n");
    printf("2:自定义数据\n");
    printf("3:返回\n");
label2:
    int a;
    scanf("%d", &a);
    switch (a)
    {
    case 1:
        RR1();
        goto label1;
        break;
    case 2:
        RR2();
        goto label1;
        break;
    case 3:
        return 0;
    default:
        printf("请输入正确的数字:\n");
        goto label2;
        break;
    }
    return 0;
}

int RR1()
{
    return 0;
}

int RR2()
{
    return 0;
}

int BANK()
{
label1:
    system("cls");
    printf("银行家算法\n\n");
    printf("1:默认数据\n");
    printf("2:自定义数据\n");
    printf("3:返回\n");
label2:
    int a;
    scanf("%d", &a);
    switch (a)
    {
    case 1:
        BANK1();
        goto label1;
        break;
    case 2:
        BANK2();
        goto label1;
        break;
    case 3:
        return 0;
    default:
        printf("请输入正确的数字:\n");
        goto label2;
        break;
    }
    return 0;
}

int BANK1()
{
    return 0;
}

int BANK2()
{
    return 0;
}

int Compare1(const void *a, const void *b)
{
    if (((Process *)a)->arrivalTime - ((Process *)b)->arrivalTime != 0)
    {
        return ((Process *)a)->arrivalTime - ((Process *)b)->arrivalTime;
    }
    else
    {
        return ((Process *)a)->serveTime - ((Process *)b)->serveTime;
    }
}

int main()
{
label1:
    system("cls");
    printf("1:SJF\n");
    printf("2:SRTF\n");
    printf("3:RR\n");
    printf("4:银行家算法\n");
    printf("5:退出\n");
label2:
    int t;
    scanf("%d", &t);
    switch (t)
    {
    case 1:
        SJF();
        goto label1;
        break;
    case 2:
        SRTF();
        goto label1;
        break;
    case 3:
        RR();
        goto label1;
        break;
    case 4:
        BANK();
        goto label1;
    case 5:
        return 0;
    default:
        printf("请输入正确的数字:\n");
        goto label2;
        break;
    }
}