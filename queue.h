#define TRUE 1
#define FALSE 0

#define MAXNUM 10	
#define _type int

typedef struct {
	_type queue[MAXNUM];
	int front;					//队首指向元素
	int rear;					//队尾不指向元素
	unsigned char flag;			//1为满队列 0为非满队列
}SeqQueue;

//初始化队列
unsigned char initQueue(SeqQueue* q)
{
	q->front = q->rear = q->flag = 0;
	return TRUE;
}

//队列是否为空
unsigned char qEmpty(SeqQueue* q)
{
	if (q->front == q->rear && !q->flag)
		return TRUE;
	else return FALSE;
}

//访问队首元素
_type qFront(SeqQueue * q)
{
	if (q->front == q->rear && !q->flag)
		return -1;
	else return q->queue[q->front];
}

//出队
_type qPop(SeqQueue * q)
{
	if (q->front == q->rear && !q->flag)
		return -1;
	if (q->flag)q->flag = 0;
	_type e = q->queue[q->front];
	q->front++;
	if (q->front >= MAXNUM) q->front = 0;
	return e;
}

//入队
unsigned char qPush(SeqQueue * q, _type e)
{
	if (q->flag)return FALSE;
	q->queue[q->rear] = e;
	if (++q->rear >= MAXNUM) q->rear = 0;
	if (q->front == q->rear)q->flag = 1;
	return TRUE;
}

//计算队列长度
int qLength(SeqQueue * q)
{
	if (q->flag)return MAXNUM;
	else if (q->front > q->rear)
		return q->rear + MAXNUM - q->front;
	else return q->rear - q->front;
}

//更新队列
//队列不满时入队 队列满时先出队再入队
void qUpdateQueue(SeqQueue * q, _type e)
{
	if (q->flag)qPop(q);
	qPush(q, e);
}

//获取队列中元素最大值
_type qGetMax(SeqQueue * q)
{
	if (q->flag)
	{
		_type tmp = q->queue[0];
		for (int i = 1; i < MAXNUM; i++)
		{
			if (q->queue[i] > tmp)
				tmp = q->queue[i];
		}
		return tmp;
	}
	else if (q->front > q->rear)
	{
		_type tmp = q->queue[0];
		for (int i = 1; i < q->rear; i++)
		{
			if (q->queue[i] > tmp)
				tmp = q->queue[i];
		}
		for (int i = q->front; i < MAXNUM; i++)
		{
			if (q->queue[i] > tmp)
				tmp = q->queue[i];
		}
		return tmp;
	}
	else
	{
		_type tmp = q->queue[q->front];
		for (int i = q->front + 1; i < q->rear; i++)
		{
			if (q->queue[i] > tmp)
				tmp = q->queue[i];
		}
		return tmp;
	}
}

//获取队列中元素最小值
_type qGetMin(SeqQueue * q)
{
	if (q->flag)
	{
		_type tmp = q->queue[0];
		for (int i = 1; i < MAXNUM; i++)
		{
			if (q->queue[i] < tmp)
				tmp = q->queue[i];
		}
		return tmp;
	}
	else if (q->front > q->rear)
	{
		_type tmp = q->queue[0];
		for (int i = 1; i < q->rear; i++)
		{
			if (q->queue[i] < tmp)
				tmp = q->queue[i];
		}
		for (int i = q->front; i < MAXNUM; i++)
		{
			if (q->queue[i] < tmp)
				tmp = q->queue[i];
		}
		return tmp;
	}
	else
	{
		_type tmp = q->queue[q->front];
		for (int i = q->front + 1; i < q->rear; i++)
		{
			if (q->queue[i] < tmp)
				tmp = q->queue[i];
		}
		return tmp;
	}
}
//void travelque(SeqQueue * q)
//{
//	int tmp_rear = 0;
//	if (q->flag || q->front > q->rear) tmp_rear = q->rear + MAXNUM;
//	else tmp_rear = q->rear;
//	for (int i = q->front; i < tmp_rear; i++)
//	{
//		std::cout << q->queue[i % MAXNUM];
//	}
//}

#undef MAXNUM
#undef _type