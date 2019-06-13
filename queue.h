#define TRUE 1
#define FALSE 0

#define MAXNUM 10	
#define _type int

typedef struct {
	_type queue[MAXNUM];
	int front;					//����ָ��Ԫ��
	int rear;					//��β��ָ��Ԫ��
	unsigned char flag;			//1Ϊ������
}SeqQueue;

//��ʼ������
unsigned char initQueue(SeqQueue* q)
{
	q->front = q->rear = q->flag = 0;
	return TRUE;
}

//�����Ƿ�Ϊ��
unsigned char qEmpty(SeqQueue* q)
{
	if (q->front == q->rear && !q->flag)
		return TRUE;
	else return FALSE;
}

//���ʶ���Ԫ��
_type qFront(SeqQueue * q)
{
	if (q->front == q->rear && !q->flag)
		return -1;
	else return q->queue[q->front];
}

//����
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

//���
unsigned char qPush(SeqQueue * q, _type e)
{
	if (q->flag)return FALSE;
	q->queue[q->rear] = e;
	if (++q->rear >= MAXNUM) q->rear = 0;
	if (q->front == q->rear)q->flag = 1;
	return TRUE;
}

//������г���
int qLength(SeqQueue * q)
{
	if (q->flag)return MAXNUM;
	else if (q->front > q->rear)
		return q->rear + MAXNUM - q->front;
	else return q->rear - q->front;
}

//void travelque(SeqQueue* q)
//{
//	if (q->flag || q->front > q->rear) q->rear += MAXNUM;
//	for (int i = q->front; i < q->rear; i++)
//	{
//		std::cout << q->queue[i % MAXNUM];
//	}
//}

#undef MAXNUM
#undef _type