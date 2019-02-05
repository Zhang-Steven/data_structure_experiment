/**
  ******************************************************************************
  * @file    graph_coloring.cpp
  * @author  ������
  * @date    2018-09-26
  * @brief   ���ļ���������ԭɫ������ʵ�ֺ���
  * 		 - InitQueue
  * 		 - EnQueue
  * 		 - IsQueueEmpty
  * 		 - DeQueue
  * 		 - InitColoringStack
  * 		 - GetColoringStackLength
  * 		 - ColoringStackPush
  * 		 - ColoringStackPop
  * 		 - CreateAdjacencyList
  * 		 - ALOptimization
  * 		 - ShowAdjacencyList
  * 		 - TintingColors
  * 		 - GetRowUnvisitedNum
  * 		 - ChooseBestTintingArea
  * 		 - IsAllVerticesVisited
  * 	   	 - FormBestTintingPath
  * 		 - GraphColoring
  * 		 - GetAreaCentroid
  * 		 - GetAreaWeights
  ******************************************************************************
  */

#define _CRT_SECURE_NO_WARNINGS
#include "main.h"

/**
 * @brief  ��ʼ���ն���
 * @param  Q: ������
 * @return ����ִ��״̬
 */
Status InitQueue(LinkQueue &Q)
{
	Q.front = Q.rear = (QueuePtr)malloc(sizeof(QNode));
	if (!Q.front)
		return ERROR;
	Q.front->next = NULL;
	return OK;
}

/**
 * @brief  �����
 * @param  Q: ������
 * @param  e: �����е�Ԫ��
 * @return ����ִ��״̬
 */
Status EnQueue(LinkQueue &Q, int e)
{
	QueuePtr s;
	if ((s = (QueuePtr)malloc(sizeof(QNode))) == NULL)
		return ERROR;
	s->data = e;
	s->next = NULL;
	Q.rear->next = s;
	Q.rear = s;
	return OK;
}

/**
 * @brief  ����ö����Ƿ�Ϊ��
 * @param  Q: ������
 * @return ����ִ��״̬
 */
Status IsQueueEmpty(LinkQueue Q)
{
	if (Q.front->next == NULL)
		return TRUE;
	else
		return FALSE;
}

/**
 * @brief  ������
 * @param  Q: ������
 * @param  e: �����е�Ԫ��
 * @return ����ִ��״̬
 */
Status DeQueue(LinkQueue &Q, int &e)
{
	QueuePtr p;
	// �����ѿգ������в��Ϸ�
	if (Q.front == Q.rear)
		return ERROR;

	p = Q.front->next;
	e = p->data;
	Q.front->next = p->next;

	// ����ͷ�Ƕ�β	,ɾ����rearָ��ͷ���	
	if (p == Q.rear)
		Q.rear = Q.front;
	free(p);
	p = NULL;
	return OK;
}

/**
 * @brief  ��ʼ����ջ
 * @param  s: Ϳɫ����ջ
 * @return ����ִ��״̬
 */
Status InitColoringStack(ColoringStack *(&s))
{
	if ((s = (ColoringStack *)malloc(sizeof(ColoringStack))) == NULL)
		return ERROR;
	s->next = NULL;
	return OK;
}

/**
 * @brief  ��ȡջ��ʵ�����
 * @param  s: Ϳɫ����ջ
 * @return Ϳɫ����ջ�ĳ���
 */
int GetColoringStackLength(ColoringStack *s)
{
	int length = 0;
	ColoringStack *temp = s;
	while (temp != NULL)
	{
		length++;
		temp = temp->next;
	}
	return length - 1;
}

/**
 * @brief  ��ջ
 * @param  s: Ϳɫ����ջ
 * @param  area_number: ������
 * @param  color: �ڸ�����Ϳ����ɫֵ
 * @return ����ִ��״̬
 */
Status ColoringStackPush(ColoringStack *(&s), int area_number, int color)
{
	ColoringStack *p;
	if ((p = (ColoringStack *)malloc(sizeof(ColoringStack))) == NULL)
		return ERROR;
	p->area_number = area_number;
	p->color = color;
	p->next = s;
	s = p;
	return OK;
}

/**
 * @brief  ��ջ
 * @param  s: Ϳɫ����ջ
 * @param  area_number: ������
 * @return ����ִ��״̬
 */
Status ColoringStackPop(ColoringStack *(&s), int *area_number)
{
	ColoringStack *p;
	if (s->next != NULL)
	{
		p = s;
		*area_number = s->area_number;
		s = s->next;
		free(p);
		p = NULL;
		return OK;
	}
	else
		return ERROR;
}

/**
 * @brief  ����maskImage�ṩ��contour��Ϣ�����ڽӱ�
 * @param  maskImage: �ṩ��contour��Ϣ��ͼ�����
 * @param  count: ��ˮ��ͼ���������
 * @return ����ִ��״̬
 */
Status CreateAdjacencyList(AdjacencyList &AL, Mat maskImage, int count)
{
	/*cout << maskImage << endl;*/
	int arc_num = 0;
	AL.vertex_num = count;
	for (int i = 0; i < count; i++)
	{
		AL.vertices[i].vertex_serial_number = i + 1;
		AL.vertices[i].first_arc = NULL;
	}
	int area1_ID, area2_ID;
	/*int edge[8];*/
	int edge[8];

	// ����maskImage����������ڽӹ�ϵ
	for (int i = 1; i < maskImage.rows - 1; i++)
	{
		int *data1 = maskImage.ptr<int>(i - 1);
		int *data2 = maskImage.ptr<int>(i);
		int *data3 = maskImage.ptr<int>(i + 1);
		for (int j = 1; j < maskImage.cols - 1; j++)
		{
			int ref = data2[j];
			edge[0] = data1[j - 1];
			edge[1] = data1[j];
			edge[2] = data1[j + 1];
			edge[3] = data2[j - 1];
			edge[4] = data2[j + 1];
			edge[5] = data3[j - 1];
			edge[6] = data3[j];
			edge[7] = data3[j + 1];

			// ��maskImage�У�ֵΪ-1��λ��Ϊcontour�����������λ��Ϊĳ����ֻ���עrefΪ�߽������
			if (ref == -1)
			{
				// ����⵽�ο����ֵΪ-1������Χ�ĵ������ִ������ֵ���Ϳ����ж������������ڽ�
				// �����������ڽӵ�����ı�ž����������������ֵ
				for (int k = 0; k < 8; k++)
				{
					if (edge[k] > 0)
					{
						area1_ID = edge[k];
						break;
					}
				}

				for (int k = 0; k < 8; k++)
				{
					if (edge[k] > 0 && edge[k] != area1_ID)
					{
						area2_ID = edge[k];
						break;
					}
				}

				// ��֪���Ϊarea1_ID��area2_ID�����������ڽ�
				// ����ֻҪ�����Ǽ����ڽӱ��м��ɣ����¾��Ǽ����ڽӱ�Ĺ���
				if (AL.vertices[area1_ID - 1].first_arc == NULL)
				{
					ArcNode *p = NULL;
					if ((p = (ArcNode *)malloc(sizeof(ArcNode))) == NULL)
					{
						cout << "Error: insufficient memory..." << endl;
						exit(1);
					}
					p->next_arc = NULL;
					p->arc_serial_number = area2_ID;
					AL.vertices[area1_ID - 1].first_arc = p;
					arc_num++;
				}

				else if (AL.vertices[area1_ID - 1].first_arc != NULL)
				{
					int IsDuplicate = FALSE;
					ArcNode *p = NULL;
					ArcNode *temp = AL.vertices[area1_ID - 1].first_arc;
					while (temp != NULL)
					{
						if (temp->arc_serial_number != area2_ID && temp->next_arc != NULL)
							temp = temp->next_arc;
						else if (temp->arc_serial_number == area2_ID || area2_ID == AL.vertices[area1_ID - 1].vertex_serial_number)
						{
							IsDuplicate = TRUE;
							break;
						}
						else
							break;
					}

					if (!IsDuplicate)
					{
						if ((p = (ArcNode *)malloc(sizeof(ArcNode))) == NULL)
						{
							cout << "Error: insufficient memory..." << endl;
							exit(1);
						}
						p->arc_serial_number = area2_ID;
						p->next_arc = NULL;
						temp->next_arc = p;
						arc_num++;
					}
				}

				if (AL.vertices[area2_ID - 1].first_arc == NULL)
				{
					ArcNode *p = NULL;
					if ((p = (ArcNode *)malloc(sizeof(ArcNode))) == NULL)
					{
						cout << "Error: insufficient memory..." << endl;
						exit(1);
					}
					p->next_arc = NULL;
					p->arc_serial_number = area1_ID;
					AL.vertices[area2_ID - 1].first_arc = p;
					arc_num++;
				}

				else if (AL.vertices[area2_ID - 1].first_arc != NULL)
				{
					int IsDuplicate = FALSE;
					ArcNode *p = NULL;
					ArcNode *temp = AL.vertices[area2_ID - 1].first_arc;
					while (temp != NULL)
					{
						if (temp->arc_serial_number != area1_ID && temp->next_arc != NULL)
							temp = temp->next_arc;
						else if (temp->arc_serial_number == area1_ID || area1_ID == AL.vertices[area2_ID - 1].vertex_serial_number)
						{
							IsDuplicate = TRUE;
							break;
						}
						else
							break;
					}
					if (!IsDuplicate)
					{
						if ((p = (ArcNode *)malloc(sizeof(ArcNode))) == NULL)
						{
							cout << "Error: insufficient memory..." << endl;
							exit(1);
						}
						p->arc_serial_number = area1_ID;
						p->next_arc = NULL;
						temp->next_arc = p;
						arc_num++;
					}
				}
			}
		}
	}

	// ���õ��ıȽ�raw���ڽӱ��Ż��ɰ������������Ӷൽ��������ڽӱ�
	ALOptimization(AL);

	AL.arc_num = arc_num / 2;
	return OK;
}

/**
 * @brief  ���õ��ıȽ�raw���ڽӱ��Ż��ɰ������������Ӷൽ��������ڽӱ�
 * @param  AL: �ڽӱ�
 * @return ����ִ��״̬
 */
Status ALOptimization(AdjacencyList &AL)
{
	// ���ÿ�����������������������adj_area_number_list��
	int length = 1;
	int adj_area_number_list[MAX_VERTEX_NUM] = { 0 };
	for (int i = 0; i < AL.vertex_num; i++)
	{
		ArcNode *temp = AL.vertices[i].first_arc;
		while (temp != NULL)
		{
			length++;
			temp = temp->next_arc;
		}
		adj_area_number_list[i] = length;
		length = 1;
	}

	// ��������������Ϊ����д��ڵ���
	for (int i = 0; i < AL.vertex_num; i++)
	{
		AL.vertices[i].adj_area_num = adj_area_number_list[i];
		ArcNode *temp = AL.vertices[i].first_arc;
		while (temp != NULL)
		{
			temp->adj_area_num = adj_area_number_list[temp->arc_serial_number - 1];
			temp = temp->next_arc;
		}
	}

	// ���ڿ�ʼ�����������������ǰ�����������ð������Ե��������������������ʽҲ�ɣ�
	for (int i = 0; i < AL.vertex_num; i++)
	{
		for (int j = 0; j < adj_area_number_list[i] - 2; j++)
		{		
			ArcNode *temp = AL.vertices[i].first_arc;
			ArcNode *shadow = NULL;
			for (int k = 0; k < adj_area_number_list[i] - j - 2; k++)
			{
				if (temp->adj_area_num < temp->next_arc->adj_area_num)
				{
					if (AL.vertices[i].first_arc == temp)
					{
						ArcNode *current = temp->next_arc;
						temp->next_arc = temp->next_arc->next_arc;
						current->next_arc = temp;
						AL.vertices[i].first_arc = current;
						shadow = current;
					}
					else
					{
						ArcNode *current = temp->next_arc;
						temp->next_arc = temp->next_arc->next_arc;
						current->next_arc = temp;
						shadow->next_arc = current;
						shadow = current;
					}
				}
				else
				{
					shadow = temp;
					temp = temp->next_arc;
				}
			}
		}
	}
	return OK;
}

/**
 * @brief  չʾ�ڽӱ�
 * @param  AL: �ڽӱ�
 * @return ����ִ��״̬
 */
Status ShowAdjacencyList(AdjacencyList AL)
{
	cout << "�ڽӱ�" << endl;
	for (int i = 0; i < AL.vertex_num; i++)
	{
		cout << AL.vertices[i].vertex_serial_number << "-";
		ArcNode *temp = AL.vertices[i].first_arc;
		while (temp != NULL && temp->next_arc != NULL)
		{
			cout << temp->arc_serial_number << "-";
			temp = temp->next_arc;
		}
		if (temp != NULL && temp->next_arc == NULL)
			cout << temp->arc_serial_number << endl;
		if (temp == NULL)
			cout << "NULL" << endl;
	}
	return OK;
}

/**
 * @brief  ����ͼ�Ĺ�����ȱ��������ö��н��и�������Ϳɫ���̣�ʹ�ö�ջ��¼Ϳɫ���̣��Ż�������
 * @param  AL: �ڽӱ�
 * @param  tinting_queue: Ϳɫ·������
 * @param  maskImage: �ṩ��contour��Ϣ��ͼ�����
 * @param  highlighted_path_image: ������Ҫ�ں���ʵ�����õ���ͼ�񣬴˴�ֻ�Ƕ౸��һ����ԭɫ���
 * @return ����ִ��״̬
 */
Status TintingColors(AdjacencyList &AL, LinkQueue tinting_queue, Mat maskImage, Mat &highlighted_path_image)
{
	int pop_record[MAX_VERTEX_NUM] = { 0 };
	// ��ʼ��һ����¼��ɫ�Ƿ��õļ�¼��
	int colors_used_matrix[MAX_VERTEX_NUM][4] = { 0 };
	// ����һ��������к�һ����תվ��ǰ�����ڴ洢���˳��������򣬺�����������������е�˳��
	LinkQueue buffer;
	LinkQueue transfer_station;
	InitQueue(buffer);
	InitQueue(transfer_station);
	// ������ջ
	ColoringStack *stack = NULL;
	InitColoringStack(stack);
	int e;

	// ��ʼͿɫ��Ҫ�󾡿�������ͬһ����ɫȥͿ��ʵ�ڳ�ͻ�ٻ���ɫ
	// ��������޷�Ϳɫ���������һ��������ɫ�ٳ���
	// ���Ϳɫ���зǿգ���δ���Ϳɫ�����������
	while (!IsQueueEmpty(tinting_queue) || !IsQueueEmpty(buffer))
	{
		// ���Ϳɫ����һֱ����ĳ���ط����Ȳ��ܼ�����ǰͿ���ֲ��������ˣ�˵������������ͬ
		// ��ô��������50�����������ƶ�Ϳɫ�������˹��̷ǳ���Ҫ
		for (int i = 0; i < AL.vertex_num; i++)
		{
			// ���ĳ�����Ƶ�����ˣ����ﵽ��ֵ����ֱ�ӻ���50�����
			if (pop_record[i] >= 250)
			{
				pop_record[i] = 0;
				for (int j = 0; j < 50; j++)
				{
					int temp_area_number;
					ColoringStackPop(stack, &temp_area_number);
					// ������תվ
					EnQueue(transfer_station, temp_area_number);

					while (!IsQueueEmpty(buffer))
					{
						int transfer;
						DeQueue(buffer, transfer);
						EnQueue(transfer_station, transfer);
					}

					while (!IsQueueEmpty(transfer_station))
					{
						int transfer;
						DeQueue(transfer_station, transfer);
						EnQueue(buffer, transfer);
					}
				}
			}
		}
		// ���buffer�Ƿ�Ϊ�գ���ΪҪ���ȴ���buffer�еĽ�㣬��buffer���޽�㣬�ٴ�tinting_queue��ȡ����Ϳɫ���
		if (!IsQueueEmpty(buffer))
		{
			bool left_colors_table[4] = { 0 };
			DeQueue(buffer, e);
			// ֻ�е�ջ�ĳ��ȴ���0��ȥ���
			if (GetColoringStackLength(stack))
			{
				// ����ʱ��ҪͿɫ��������ڽ��������ɫ�Ƿ��ͻ
				// �������ڵ�ջ�������Ƿ������ڵ�����
				ColoringStack *search_stack = stack;
				while (search_stack != NULL)
				{
					ArcNode *temp = AL.vertices[e - 1].first_arc;
					// ��ʼ���Լ����ڽ�����ȥ�����Ƿ������Ԫ��
					while (temp != NULL)
					{
						if (temp->arc_serial_number == search_stack->area_number)
							left_colors_table[search_stack->color] = 1;
						temp = temp->next_arc;
					}
					search_stack = search_stack->next;
				}
				// ��Ѱ���������ڿ�����ʣ��ʲô��ɫ����Ϳ
				int tinted = FALSE;
				// �������ɫʣ�£��������ջ�Ȳ���
				for (int i = 0; i < 4; i++)
				{
					if (left_colors_table[i] != 1 && colors_used_matrix[e - 1][i] == 0)
					{
						ColoringStackPush(stack, e, i);
						colors_used_matrix[e - 1][i] = 1;
						tinted = TRUE;
						break;
					}
				}
				// �����û����ɫʣ�£�֤����ǰ�����ɫͿ��Ҳ�����⣬����Ҫ����
				if (tinted == FALSE)
				{
					for (int i = 0; i < 4; i++)
						colors_used_matrix[e - 1][i] = 0;
					

					int temp_area_number;
					ColoringStackPop(stack, &temp_area_number);
					pop_record[temp_area_number - 1]++;

					// ������תվ
					EnQueue(transfer_station, temp_area_number);
					EnQueue(transfer_station, e);
					
					while (!IsQueueEmpty(buffer))
					{
						int transfer;
						DeQueue(buffer, transfer);
						EnQueue(transfer_station, transfer);
					}

					while (!IsQueueEmpty(transfer_station))
					{
						int transfer;
						DeQueue(transfer_station, transfer);
						EnQueue(buffer, transfer);
					}
				}
			}
			// ���ջΪ��
			else
			{
				int flag = FALSE;
				// ˵���ǵ�һ������
				for (int i = 0; i < 4; i++)
				{
					if (colors_used_matrix[e - 1][i] != 1)
					{
						ColoringStackPush(stack, e, i);
						colors_used_matrix[e - 1][i] = 1;
						flag = TRUE;
						break;
					}
				}
				if (flag == FALSE)
					cout << "Ϳɫʧ�ܣ�" << endl;
			}
		}


		else if (IsQueueEmpty(buffer))
		{
			// һ��Ϳɫ������ų�����
			DeQueue(tinting_queue, e);
			// ���RGBY��ʹ�������0��ʾδʹ�ã�1��ʾ��ʹ��
			bool left_colors_table[4] = { 0 };
			// ֻ�е�ջ�ĳ��ȴ���0��ȥ���
			if (GetColoringStackLength(stack))
			{
				// ����ʱ��ҪͿɫ��������ڽ��������ɫ�Ƿ��ͻ
				// �������ڵ�ջ�������Ƿ������ڵ�����
				ColoringStack *search_stack = stack;
				while (search_stack != NULL)
				{
					ArcNode *temp = AL.vertices[e - 1].first_arc;
					// ��ʼ���Լ����ڽ�����ȥ�����Ƿ������Ԫ��
					while (temp != NULL)
					{
						if (temp->arc_serial_number == search_stack->area_number)
							left_colors_table[search_stack->color] = 1;
						temp = temp->next_arc;
					}
					search_stack = search_stack->next;
				}
				// ��Ѱ���������ڿ�����ʣ��ʲô��ɫ����Ϳ
				int tinted = FALSE;
				// �������ɫʣ�£��������ջ�Ȳ���
				for (int i = 0; i < 4; i++)
				{
					if (left_colors_table[i] != 1)
					{
						ColoringStackPush(stack, e, i);
						colors_used_matrix[e - 1][i] = 1;
						tinted = TRUE;
						break;
					}
				}
				// ���û����ɫʣ�£�֤��ǰ����ɫͿ�������⣬��Ҫ����
				if (tinted == FALSE)
				{
					int temp_area_number;
					ColoringStackPop(stack, &temp_area_number);
					pop_record[temp_area_number]++;
					EnQueue(buffer, temp_area_number);
					EnQueue(buffer, e);
				}
			}
			// ���ջΪ��
			else
			{
				// ˵���ǵ�һ���������뿼���Ƿ��ͻ��ֱ����ջ
				ColoringStackPush(stack, e, RED);
				colors_used_matrix[e - 1][RED] = 1;
			}
		}
	}

	// ���½��õ���Ϳɫ����Ӧ�õ�ͼ���ϣ�Ϳɫ�����洢�ڶ�ջ��
	Mat srcImage = imread("fruits.jpg");
	Mat grayImage;
	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);
	cvtColor(grayImage, grayImage, COLOR_GRAY2BGR);
	
	Mat four_primary_colors_image(maskImage.size(), CV_8UC3);
	for (int i = 0; i < maskImage.rows; i++)
		for (int j = 0; j < maskImage.cols; j++)
		{
			int index = maskImage.at<int>(i, j);
			ColoringStack *temp = stack;
			if (index > 0)
			{
				while (temp->next != NULL)
				{
					if (temp->area_number == index)
					{
						switch (temp->color)
						{
							case RED:
								four_primary_colors_image.at<Vec3b>(i, j) = Vec3b(0, 0, 255);
								highlighted_path_image.at<Vec3b>(i, j) = Vec3b(0, 0, 255);
								break;
							case GREEN:
								four_primary_colors_image.at<Vec3b>(i, j) = Vec3b(0, 255, 0);
								highlighted_path_image.at<Vec3b>(i, j) = Vec3b(0, 255, 0);
								break;
							case BLUE:
								four_primary_colors_image.at<Vec3b>(i, j) = Vec3b(255, 0, 0);
								highlighted_path_image.at<Vec3b>(i, j) = Vec3b(255, 0, 0);
								break;
							case YELLOW:
								four_primary_colors_image.at<Vec3b>(i, j) = Vec3b(0, 255, 255);
								highlighted_path_image.at<Vec3b>(i, j) = Vec3b(0, 255, 255);
								break;
						}
					}
					temp = temp->next;
				}
			}
			else if (index == -1)
			{
				four_primary_colors_image.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
				highlighted_path_image.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
			}
		}

	four_primary_colors_image = four_primary_colors_image * 0.5 + grayImage * 0.5;
	
	GetAreaCentroid(AL, maskImage, four_primary_colors_image);
	imshow("����ԭɫ��", four_primary_colors_image);
	return OK;
}

/**
 * @brief  �õ��ڽӱ���ĳһ����û�б����ʽ��ĸ���
 * @param  AL: �ڽӱ�
 * @param  vertex_serial_number: ������
 * @param  visited: ��¼�Ƿ���ʹ��ı�־����
 * @return ������û�б����ʽ��ĸ���
 */
int GetRowUnvisitedNum(AdjacencyList AL, int vertex_serial_number, const int *visited)
{
	int length = 0;
	if (visited[AL.vertices[vertex_serial_number - 1].vertex_serial_number] == FALSE)
		length++;
	ArcNode *temp = AL.vertices[vertex_serial_number - 1].first_arc;
	if (temp == NULL)
		return length;
	while (temp != NULL)
	{
		if (visited[temp->arc_serial_number - 1] == FALSE)
			length++;
		temp = temp->next_arc;
	}
	return length;
}

/**
 * @brief  ѡȡ��õ�Ϳɫ����
 * @param  AL: �ڽӱ�
 * @param  visited: ��¼�Ƿ���ʹ��ı�־����
 * @return ��õ�Ϳɫ����ı��
 */
int ChooseBestTintingArea(AdjacencyList AL, const int *visited)
{
	int index = NONE;
	int length = 0;
	for (int i = 0; i < AL.vertex_num; i++)
	{
		if (visited[i] == FALSE)
		{
			int temp = GetRowUnvisitedNum(AL, i + 1, visited);
			if (temp > length)
			{			
				index = i;
				length = temp;
			}
		}
	}
	if (index == NONE)
		return ERROR;
	else
		return index;
}

/**
 * @brief  �ж��Ƿ����еĽ�㶼�����ʹ�
 * @param  AL: �ڽӱ�
 * @param  visited: ��¼�Ƿ���ʹ��ı�־����
 * @return ����ִ��״̬
 */
Status IsAllVerticesVisited(AdjacencyList AL, const int *visited)
{
	for (int i = 0; i < AL.vertex_num; i++)
	{
		if (visited[i] == FALSE)
			return FALSE;
	}
	return TRUE;
}

/**
 * @brief  ���ö����������Ϳɫ·������Ψһ����ѡһ�ּ��ɣ���Ϳɫ�Ͱ���·������
 * @param  AL: �ڽӱ�
 * @param  tinting_queue: Ϳɫ·������
 * @return ����ִ��״̬
 */
Status FormBestTintingPath(AdjacencyList AL, LinkQueue &tinting_queue)
{
	LinkQueue Q;
	int visited[MAX_VERTEX_NUM] = { FALSE };
	for (int i = 0; i < AL.vertex_num; i++)
	{
		ArcNode *temp = AL.vertices[i].first_arc;
		if (temp == NULL)
			visited[i] = TRUE;
	}
	InitQueue(tinting_queue);
	InitQueue(Q);

	while (!IsAllVerticesVisited(AL, visited))
	{
		int v = ChooseBestTintingArea(AL, visited);
		// ����ö�����δ����
		if (!visited[v])
		{
			visited[v] = TRUE;

			EnQueue(tinting_queue, v + 1);
			EnQueue(Q, v);
			while (!IsQueueEmpty(Q))
			{
				int u;
				DeQueue(Q, u);
				ArcNode *temp = AL.vertices[u].first_arc;
				if (temp == NULL)
					break;
				while (temp != NULL)
				{
					if (!visited[(temp->arc_serial_number) - 1])
					{
						visited[(temp->arc_serial_number) - 1] = TRUE;
						EnQueue(tinting_queue, temp->arc_serial_number);
						EnQueue(Q, (temp->arc_serial_number) - 1);
					}
					temp = temp->next_arc;
				}
			}
		}
	}
	// չʾͿɫ���·��
	cout << endl << "Ϳɫ·����" << endl;
	LinkQueue temp = tinting_queue;
	temp.front = temp.front->next;
	while (temp.front != temp.rear)
	{
		cout << temp.front->data << "->";
		temp.front = temp.front->next;
	}
	cout << temp.front->data << endl;
	return OK;
}

/**
 * @brief  ��ԭɫ�㷨ʵ��
 * @param  AL: �ڽӱ�
 * @param  maskImage: �ṩ��contour��Ϣ��ͼ�����
 * @param  count: ��ˮ�������������
 * @param  heap: ��������Ҫʹ�õ��Ķѣ�������������¼���������
 * @param  highlighted_path_image: ������Ҫ�ں���ʵ�����õ���ͼ�񣬴˴�ֻ�Ƕ౸��һ����ԭɫ���
 * @return ����ִ��״̬
 */
Status GraphColoring(AdjacencyList &AL, Mat maskImage, int count, HeapNode *heap, Mat &highlighted_path_image)
{
	cout << "\n��ʵ�������ԭɫ��" << endl;
	double dTime = (double)getTickCount();

	CreateAdjacencyList(AL, maskImage, count);
	Mat contours_image;
	GetAreaWeights(AL, maskImage, count, heap);
	ShowAdjacencyList(AL);
	LinkQueue tinting_queue;
	tinting_queue.front = NULL;
	tinting_queue.rear = NULL;
	FormBestTintingPath(AL, tinting_queue);
	TintingColors(AL, tinting_queue, maskImage, highlighted_path_image);

	dTime = (double)getTickCount() - dTime;
	printf("\n������ʱ = %gs\n", dTime / getTickFrequency());
	cout << "�������������һʵ��..." << endl << endl;
	waitKey(0);
	return OK;
}

/**
 * @brief  ���ڵõ��������������
 * @param  AL: �ڽӱ�
 * @param  maskImage: �ṩ��contour��Ϣ��ͼ�����
 * @param  four_primary_colors_image: ��ԭɫͼ��
 * @return ����ִ��״̬
 */
Status GetAreaCentroid(AdjacencyList &AL, Mat maskImage, Mat four_primary_colors_image)
{
	for (int i = 0; i < AL.vertex_num; i++)
	{
		int centroid_x = 0, centroid_y = 0, number = 0;
		for (int j = 1; j < maskImage.rows - 1; j++)
		{
			int *data = maskImage.ptr<int>(j);
			for (int k = 1; k < maskImage.cols - 1; k++)
			{
				if (data[k] == i + 1)
				{
					centroid_x += k;
					centroid_y += j;
					number++;
				}
			}
		}
		AL.vertices[i].centroid.x = centroid_x / number;
		AL.vertices[i].centroid.y = centroid_y / number;
	}

	// ������Ҫ���õ������ļ������뵽�ڽӱ��У��Ա������ʵ��ʹ��
	for (int i = 0; i < AL.vertex_num; i++)
	{
		ArcNode *temp = AL.vertices[i].first_arc;
		while (temp != NULL)
		{
			temp->centroids_dist = (unsigned int)sqrt(pow((AL.vertices[i].centroid.x - AL.vertices[temp->arc_serial_number - 1].centroid.x), 2) + pow((AL.vertices[i].centroid.y - AL.vertices[temp->arc_serial_number - 1].centroid.y), 2));
			temp = temp->next_arc;
		}
	}

	// ������������ϱ���������
	for (int i = 0; i < AL.vertex_num; i++)
	{
		char str[10];
		sprintf(str, "%d", i + 1);
		putText(four_primary_colors_image, str, AL.vertices[i].centroid, FONT_HERSHEY_PLAIN, 0.8, Scalar(255, 255, 255));
	}
	return OK;
}

/**
 * @brief  ͨ��maskImage�õ���ˮ�������������ֵ���Ӷ��õ�Ȩֵ����
 * @param  AL: �ڽӱ�
 * @param  maskImage: �ṩ��contour��Ϣ��ͼ�����
 * @param  count: ��ˮ�������������
 * @param  heap: ��������Ҫʹ�õ��Ķѣ�������������¼���������
 * @return ����ִ��״̬
 */
Status GetAreaWeights(AdjacencyList &AL, Mat maskImage, int count, HeapNode *heap)
{
	// ����maksImage���������������
	for (int i = 1; i < maskImage.rows - 1; i++)
	{
		int *data = maskImage.ptr<int>(i);
		for (int j = 1; j < maskImage.cols - 1; j++)
		{
			// Ҫͳ����������ֻ��ͳ�������ų��ֵĸ������ɴ���ȷ������������
			if (data[j] != -1 && data[j] > 0)
				heap[data[j]].area++;
		}
	}

	// ������Ҫ���õ���������뵽�ڽӱ��У��Ա������ʵ��ʹ��
	for (int i = 0; i < AL.vertex_num; i++)
	{
		ArcNode *temp = AL.vertices[i].first_arc;
		while (temp != NULL)
		{
			temp->weight = heap[temp->arc_serial_number].area;
			temp = temp->next_arc;
		}
	}


	//// չʾ����������
	//cout << "�������ͳ�ƽ����" << endl;
	//for (int i = 0; i < count; i++)
	//	cout << i + 1 << ": " << weights[i] << endl;
	return OK;
}