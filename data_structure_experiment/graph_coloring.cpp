/**
  ******************************************************************************
  * @file    graph_coloring.cpp
  * @author  李星毅
  * @date    2018-09-26
  * @brief   该文件包含了四原色的所有实现函数
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
 * @brief  初始化空队列
 * @param  Q: 链队列
 * @return 函数执行状态
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
 * @brief  入队列
 * @param  Q: 链队列
 * @param  e: 进队列的元素
 * @return 函数执行状态
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
 * @brief  检验该队列是否为空
 * @param  Q: 链队列
 * @return 函数执行状态
 */
Status IsQueueEmpty(LinkQueue Q)
{
	if (Q.front->next == NULL)
		return TRUE;
	else
		return FALSE;
}

/**
 * @brief  出队列
 * @param  Q: 链队列
 * @param  e: 出队列的元素
 * @return 函数执行状态
 */
Status DeQueue(LinkQueue &Q, int &e)
{
	QueuePtr p;
	// 队列已空，出队列不合法
	if (Q.front == Q.rear)
		return ERROR;

	p = Q.front->next;
	e = p->data;
	Q.front->next = p->next;

	// 若队头是队尾	,删除后将rear指向头结点	
	if (p == Q.rear)
		Q.rear = Q.front;
	free(p);
	p = NULL;
	return OK;
}

/**
 * @brief  初始化链栈
 * @param  s: 涂色辅助栈
 * @return 函数执行状态
 */
Status InitColoringStack(ColoringStack *(&s))
{
	if ((s = (ColoringStack *)malloc(sizeof(ColoringStack))) == NULL)
		return ERROR;
	s->next = NULL;
	return OK;
}

/**
 * @brief  获取栈的实际深度
 * @param  s: 涂色辅助栈
 * @return 涂色辅助栈的长度
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
 * @brief  入栈
 * @param  s: 涂色辅助栈
 * @param  area_number: 区域编号
 * @param  color: 在该区域涂的颜色值
 * @return 函数执行状态
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
 * @brief  出栈
 * @param  s: 涂色辅助栈
 * @param  area_number: 区域编号
 * @return 函数执行状态
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
 * @brief  根据maskImage提供的contour信息构造邻接表
 * @param  maskImage: 提供的contour信息的图像矩阵
 * @param  count: 分水岭图像的区域数
 * @return 函数执行状态
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

	// 遍历maskImage来获得区域邻接关系
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

			// 在maskImage中，值为-1的位置为contour，而大于零的位置为某区域，只需关注ref为边界点的情况
			if (ref == -1)
			{
				// 若检测到参考点的值为-1，且周围的点有两种大于零的值，就可以判定有两个区域邻接
				// 并且这两个邻接的区域的编号就是那两个大于零的值
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

				// 得知编号为area1_ID和area2_ID的两个区域邻接
				// 现在只要将他们加入邻接表中即可，以下就是加入邻接表的过程
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

	// 将得到的比较raw的邻接表优化成按相邻区域数从多到少排序的邻接表
	ALOptimization(AL);

	AL.arc_num = arc_num / 2;
	return OK;
}

/**
 * @brief  将得到的比较raw的邻接表优化成按相邻区域数从多到少排序的邻接表
 * @param  AL: 邻接表
 * @return 函数执行状态
 */
Status ALOptimization(AdjacencyList &AL)
{
	// 求出每个区域的相邻区域数，存入adj_area_number_list中
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

	// 将相邻区域数作为属性写入节点中
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

	// 现在开始将相邻区域数多的往前排序，这里采用冒泡排序对单链表进行排序（其他排序方式也可）
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
 * @brief  展示邻接表
 * @param  AL: 邻接表
 * @return 函数执行状态
 */
Status ShowAdjacencyList(AdjacencyList AL)
{
	cout << "邻接表：" << endl;
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
 * @brief  基于图的广度优先遍历，采用队列进行辅助梳理涂色过程，使用堆栈记录涂色过程，优化回退率
 * @param  AL: 邻接表
 * @param  tinting_queue: 涂色路径队列
 * @param  maskImage: 提供的contour信息的图像矩阵
 * @param  highlighted_path_image: 这是需要在后续实验中用到的图像，此处只是多备份一张四原色结果
 * @return 函数执行状态
 */
Status TintingColors(AdjacencyList &AL, LinkQueue tinting_queue, Mat maskImage, Mat &highlighted_path_image)
{
	int pop_record[MAX_VERTEX_NUM] = { 0 };
	// 初始化一个记录颜色是否被用的记录表
	int colors_used_matrix[MAX_VERTEX_NUM][4] = { 0 };
	// 创建一个缓冲队列和一个中转站，前者用于存储回退出来的区域，后者用来调整缓冲队列的顺序
	LinkQueue buffer;
	LinkQueue transfer_station;
	InitQueue(buffer);
	InitQueue(transfer_station);
	// 创建堆栈
	ColoringStack *stack = NULL;
	InitColoringStack(stack);
	int e;

	// 开始涂色，要求尽可能先用同一种颜色去涂，实在冲突再换颜色
	// 如果遇到无法涂色，则回退上一步，换颜色再尝试
	// 如果涂色队列非空，即未完成涂色，则继续尝试
	while (!IsQueueEmpty(tinting_queue) || !IsQueueEmpty(buffer))
	{
		// 如果涂色过程一直卡在某个地方，既不能继续往前涂，又不能往后退，说明陷入了死胡同
		// 那么就往回退50个试试重新制定涂色方案，此过程非常重要
		for (int i = 0; i < AL.vertex_num; i++)
		{
			// 如果某个结点频繁回退，即达到阈值，就直接回退50个结点
			if (pop_record[i] >= 250)
			{
				pop_record[i] = 0;
				for (int j = 0; j < 50; j++)
				{
					int temp_area_number;
					ColoringStackPop(stack, &temp_area_number);
					// 先入中转站
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
		// 检查buffer是否为空，因为要优先处理buffer中的结点，当buffer中无结点，再从tinting_queue中取出待涂色结点
		if (!IsQueueEmpty(buffer))
		{
			bool left_colors_table[4] = { 0 };
			DeQueue(buffer, e);
			// 只有当栈的长度大于0才去检查
			if (GetColoringStackLength(stack))
			{
				// 检查此时将要涂色的区域的邻接区域的颜色是否冲突
				// 遍历现在的栈，看看是否有相邻的区域
				ColoringStack *search_stack = stack;
				while (search_stack != NULL)
				{
					ArcNode *temp = AL.vertices[e - 1].first_arc;
					// 开始从自己的邻接区域去搜索是否含有这个元素
					while (temp != NULL)
					{
						if (temp->arc_serial_number == search_stack->area_number)
							left_colors_table[search_stack->color] = 1;
						temp = temp->next_arc;
					}
					search_stack = search_stack->next;
				}
				// 搜寻结束，现在看看还剩下什么颜色可以涂
				int tinted = FALSE;
				// 如果有颜色剩下，则进行入栈等操作
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
				// 如果还没有颜色剩下，证明再前面的颜色涂的也有问题，又需要回退
				if (tinted == FALSE)
				{
					for (int i = 0; i < 4; i++)
						colors_used_matrix[e - 1][i] = 0;
					

					int temp_area_number;
					ColoringStackPop(stack, &temp_area_number);
					pop_record[temp_area_number - 1]++;

					// 先入中转站
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
			// 如果栈为空
			else
			{
				int flag = FALSE;
				// 说明是第一个区域
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
					cout << "涂色失败！" << endl;
			}
		}


		else if (IsQueueEmpty(buffer))
		{
			// 一个涂色区域序号出队列
			DeQueue(tinting_queue, e);
			// 存放RGBY的使用情况，0表示未使用，1表示已使用
			bool left_colors_table[4] = { 0 };
			// 只有当栈的长度大于0才去检查
			if (GetColoringStackLength(stack))
			{
				// 检查此时将要涂色的区域的邻接区域的颜色是否冲突
				// 遍历现在的栈，看看是否有相邻的区域
				ColoringStack *search_stack = stack;
				while (search_stack != NULL)
				{
					ArcNode *temp = AL.vertices[e - 1].first_arc;
					// 开始从自己的邻接区域去搜索是否含有这个元素
					while (temp != NULL)
					{
						if (temp->arc_serial_number == search_stack->area_number)
							left_colors_table[search_stack->color] = 1;
						temp = temp->next_arc;
					}
					search_stack = search_stack->next;
				}
				// 搜寻结束，现在看看还剩下什么颜色可以涂
				int tinted = FALSE;
				// 如果有颜色剩下，则进行入栈等操作
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
				// 如果没有颜色剩下，证明前面颜色涂的有问题，需要回退
				if (tinted == FALSE)
				{
					int temp_area_number;
					ColoringStackPop(stack, &temp_area_number);
					pop_record[temp_area_number]++;
					EnQueue(buffer, temp_area_number);
					EnQueue(buffer, e);
				}
			}
			// 如果栈为空
			else
			{
				// 说明是第一个区域，无须考察是否冲突，直接入栈
				ColoringStackPush(stack, e, RED);
				colors_used_matrix[e - 1][RED] = 1;
			}
		}
	}

	// 以下将得到的涂色方案应用到图像上，涂色方案存储在堆栈中
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
	imshow("【四原色】", four_primary_colors_image);
	return OK;
}

/**
 * @brief  得到邻接表中某一链表没有被访问结点的个数
 * @param  AL: 邻接表
 * @param  vertex_serial_number: 结点序号
 * @param  visited: 记录是否访问过的标志数组
 * @return 链表中没有被访问结点的个数
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
 * @brief  选取最好的涂色区域
 * @param  AL: 邻接表
 * @param  visited: 记录是否访问过的标志数组
 * @return 最好的涂色区域的编号
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
 * @brief  判断是否所有的结点都被访问过
 * @param  AL: 邻接表
 * @param  visited: 记录是否访问过的标志数组
 * @return 函数执行状态
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
 * @brief  利用队列梳理最佳涂色路径（不唯一，任选一种即可），涂色就按此路径进行
 * @param  AL: 邻接表
 * @param  tinting_queue: 涂色路径队列
 * @return 函数执行状态
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
		// 如果该顶点尚未访问
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
	// 展示涂色最佳路径
	cout << endl << "涂色路径：" << endl;
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
 * @brief  四原色算法实现
 * @param  AL: 邻接表
 * @param  maskImage: 提供的contour信息的图像矩阵
 * @param  count: 分水岭产生的区域数
 * @param  heap: 堆排序需要使用到的堆，这里是用来记录区域面积的
 * @param  highlighted_path_image: 这是需要在后续实验中用到的图像，此处只是多备份一张四原色结果
 * @return 函数执行状态
 */
Status GraphColoring(AdjacencyList &AL, Mat maskImage, int count, HeapNode *heap, Mat &highlighted_path_image)
{
	cout << "\n【实验二：四原色】" << endl;
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
	printf("\n程序用时 = %gs\n", dTime / getTickFrequency());
	cout << "按任意键进入下一实验..." << endl << endl;
	waitKey(0);
	return OK;
}

/**
 * @brief  用于得到区域的质心坐标
 * @param  AL: 邻接表
 * @param  maskImage: 提供的contour信息的图像矩阵
 * @param  four_primary_colors_image: 四原色图像
 * @return 函数执行状态
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

	// 下面需要将得到的质心间距离加入到邻接表中，以便后续的实验使用
	for (int i = 0; i < AL.vertex_num; i++)
	{
		ArcNode *temp = AL.vertices[i].first_arc;
		while (temp != NULL)
		{
			temp->centroids_dist = (unsigned int)sqrt(pow((AL.vertices[i].centroid.x - AL.vertices[temp->arc_serial_number - 1].centroid.x), 2) + pow((AL.vertices[i].centroid.y - AL.vertices[temp->arc_serial_number - 1].centroid.y), 2));
			temp = temp->next_arc;
		}
	}

	// 在区域的质心上标上区域编号
	for (int i = 0; i < AL.vertex_num; i++)
	{
		char str[10];
		sprintf(str, "%d", i + 1);
		putText(four_primary_colors_image, str, AL.vertices[i].centroid, FONT_HERSHEY_PLAIN, 0.8, Scalar(255, 255, 255));
	}
	return OK;
}

/**
 * @brief  通过maskImage得到分水岭区域的相对面积值，从而得到权值集合
 * @param  AL: 邻接表
 * @param  maskImage: 提供的contour信息的图像矩阵
 * @param  count: 分水岭产生的区域数
 * @param  heap: 堆排序需要使用到的堆，这里是用来记录区域面积的
 * @return 函数执行状态
 */
Status GetAreaWeights(AdjacencyList &AL, Mat maskImage, int count, HeapNode *heap)
{
	// 遍历maksImage来获得区域相对面积
	for (int i = 1; i < maskImage.rows - 1; i++)
	{
		int *data = maskImage.ptr<int>(i);
		for (int j = 1; j < maskImage.cols - 1; j++)
		{
			// 要统计相对面积，只需统计区域编号出现的个数即可大致确定区域相对面积
			if (data[j] != -1 && data[j] > 0)
				heap[data[j]].area++;
		}
	}

	// 下面需要将得到的面积加入到邻接表中，以便后续的实验使用
	for (int i = 0; i < AL.vertex_num; i++)
	{
		ArcNode *temp = AL.vertices[i].first_arc;
		while (temp != NULL)
		{
			temp->weight = heap[temp->arc_serial_number].area;
			temp = temp->next_arc;
		}
	}


	//// 展示区域相对面积
	//cout << "区域面积统计结果：" << endl;
	//for (int i = 0; i < count; i++)
	//	cout << i + 1 << ": " << weights[i] << endl;
	return OK;
}