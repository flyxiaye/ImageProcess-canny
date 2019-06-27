#define TRUE 1
#define FALSE 0

typedef struct {
	_type queue[MAXNUM];
	int front;					//����ָ��Ԫ��
	int rear;					//��β��ָ��Ԫ��
	unsigned char flag;			//1Ϊ������ 0Ϊ��������
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

//���¶���
//���в���ʱ��� ������ʱ�ȳ��������
void qUpdateQueue(SeqQueue * q, _type e)
{
	if (q->flag)qPop(q);
	qPush(q, e);
}

//��ȡ������Ԫ�����ֵ
_type qGetMax(SeqQueue * q)
{
	int tmp = q->front;
	int i = q->front + 1;
	while (i != q->rear)
	{
		if (i > MAXNUM) i = 0;
		if (q->queue[i] > q->queue[tmp])
		{
			tmp = i;
		}
		i++;
	}
	return q->queue[tmp];
}

//��ȡ������Ԫ����Сֵ
_type qGetMin(SeqQueue * q)
{
	int tmp = q->front;
	int i = q->front + 1;
	while (i != q->rear)
	{
		if (i > MAXNUM) i = 0;
		if (q->queue[i] < q->queue[tmp])
		{
			tmp = i;
		}
		i++;
	}
	return q->queue[tmp];
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