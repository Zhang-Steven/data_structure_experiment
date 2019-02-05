/**
  ******************************************************************************
  * @file    Dijkstra.cpp
  * @author  李星毅
  * @date    2018-10-13
  * @brief   该文件包含了Dijkstra算法的所有实现函数
  * 		 - InitPathRecordStack
  * 		 - GetPathRecordStackLength
  * 		 - PathRecordStackPush
  * 		 - PathRecordStackPop
  * 		 - GetMinDistVertex
  * 		 - HighlightShortestPath
  * 		 - Dijkstra
  ******************************************************************************
  */

#define _CRT_SECURE_NO_WARNINGS
#include "main.h"

/**
 * @brief  初始化链栈
 * @param  s: 路径记录栈
 * @return 函数执行状态
 */
Status InitPathRecordStack(PathRecordStack *(&s))
{
	if ((s = (PathRecordStack *)malloc(sizeof(PathRecordStack))) == NULL)
		return ERROR;
	s->next = NULL;
	return OK;
}

/**
 * @brief  获取栈的实际深度
 * @param  s: 路径记录栈
 * @return 路径记录栈的长度
 */
int GetPathRecordStackLength(PathRecordStack *s)
{
	int length = 0;
	PathRecordStack *temp = s;
	while (temp != NULL)
	{
		length++;
		temp = temp->next;
	}
	return length - 1;
}

/**
 * @brief  入栈
 * @param  s: 路径记录栈
 * @param  vertex_ID: 结点ID
 * @return 函数执行状态
 */
Status PathRecordStackPush(PathRecordStack *(&s), int vertex_ID)
{
	PathRecordStack *p;
	if ((p = (PathRecordStack *)malloc(sizeof(PathRecordStack))) == NULL)
		return ERROR;
	p->vertex_ID = vertex_ID;
	p->next = s;
	s = p;
	return OK;
}


/**
 * @brief  出栈
 * @param  s: 路径记录栈
 * @param  vertex_ID: 结点ID
 * @return 函数执行状态
 */
Status PathRecordStackPop(PathRecordStack *(&s), int *vertex_ID)
{
	PathRecordStack *p;
	if (s->next != NULL)
	{
		p = s;
		*vertex_ID = s->vertex_ID;
		s = s->next;
		free(p);
		p = NULL;
		return OK;
	}
	else
		return ERROR;
}

/**
 * @brief  从未被访问过的点中选取距离最小的点
 * @param  AL: 邻接表
 * @param  PMRow: 过程记录矩阵的行指针
 * @param  visited: 记录是否结点是否被访问过的数组
 * @return 距离最小的点的ID
 */
int GetMinDistVertex(AdjacencyList AL, ProcessMatrixCell* PMRow, const int *visited)
{
	unsigned int min_dist = INF;
	int min_dist_vertex_ID = 0;

	for (int i = 0; i < AL.vertex_num; i++)
	{
		if (visited[i] == FALSE)
		{
			if (PMRow[i].distance < min_dist)
			{
				min_dist = PMRow[i].distance;
				min_dist_vertex_ID = i;
			}
		}
	}
	return min_dist_vertex_ID + 1;
}

/**
 * @brief  突显最短路径
 * @param  AL: 邻接表
 * @param  maskImage: 提供的contour信息的图像矩阵
 * @param  path_vertex_ID_record: 路径结点ID记录表
 * @param  highlighted_path_image: 这是之前多备份的一张四原色结果，用于高亮最短路径
 * @return 函数执行状态
 */
Status HighlightShortestPath(AdjacencyList AL, Mat maskImage, int *path_vertex_ID_record, Mat highlighted_path_image)
{
	Mat srcImage = imread("fruits.jpg");
	Mat grayImage;
	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);
	cvtColor(grayImage, grayImage, COLOR_GRAY2BGR);

	Mat highlighted_path_image_copy;
	highlighted_path_image.copyTo(highlighted_path_image_copy);

	for (int i = 0; i < maskImage.rows; i++)
		for (int j = 0; j < maskImage.cols; j++)
		{
			int index = maskImage.at<int>(i, j);
			if (index > 0)
			{
				int is_target = FALSE;
				for (int k = 0; k < AL.vertex_num; k++)
				{
					if (index == path_vertex_ID_record[k])
					{
						is_target = TRUE;
						break;
					}
				}
				if (!is_target)
					highlighted_path_image_copy.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
			}
			else if (index == -1)
				highlighted_path_image_copy.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
		}
	
	highlighted_path_image_copy = highlighted_path_image_copy * 0.5 + grayImage * 0.5;

	// 在区域的质心上标上区域编号
	for (int i = 0; i < AL.vertex_num; i++)
	{
		char str[10];
		sprintf(str, "%d", i + 1);
		putText(highlighted_path_image_copy, str, AL.vertices[i].centroid, FONT_HERSHEY_PLAIN, 0.8, Scalar(255, 255, 255));
	}
	imshow("【Dijkstra】", highlighted_path_image_copy);
	return OK;
}

/**
 * @brief  Dijkstra算法实现
 * @param  AL: 邻接表
 * @param  highlighted_path_image: 这是之前多备份的一张四原色结果，用于高亮最短路径
 * @param  maskImage: 提供的contour信息的图像矩阵
 * @return 函数执行状态
 */
Status Dijkstra(AdjacencyList AL, Mat highlighted_path_image, Mat maskImage)
{
	int source_vertex = 0;
	
	while (1)
	{
		cout << "区域序号范围是：1~" << AL.vertex_num << endl;
		cout << "\n请输入起始区域序号（输入-1退出该实验）：";
		cin >> source_vertex;
		if (source_vertex == -1)
			return EXIT;
		else if (source_vertex > AL.vertex_num || source_vertex < 1)
		{
			cout << "起始区域序号必须介于1~" << AL.vertex_num << endl << endl;
			source_vertex = 0;
			continue;
		}
		break;
	}

	double dTime1 = (double)getTickCount();

	// 路径记录栈
	PathRecordStack *s = NULL;
	InitPathRecordStack(s);

	// 寻找最短路径的过程记录矩阵
	ProcessMatrixCell **PM;
	// 动态分配二维数组
	if ((PM = (ProcessMatrixCell **)malloc(AL.vertex_num * sizeof(ProcessMatrixCell *))) == NULL)
		return ERROR;
	for (int i = 0; i < AL.vertex_num; i++)
		if ((PM[i] = (ProcessMatrixCell *)malloc(AL.vertex_num * sizeof(ProcessMatrixCell))) == NULL)
			return ERROR;

	// 将所有的距离都置为无穷大
	for (int i = 0; i < AL.vertex_num; i++)
		for (int j = 0; j < AL.vertex_num; j++)
			PM[i][j].distance = INF;
	
	// 访问标志数组
	int visited[MAX_VERTEX_NUM];
	// 初始化为所有点均未访问过
	for (int i = 0; i < AL.vertex_num; i++)
		visited[i] = FALSE;
	
	// 将source vertex的距离设置为0，下标设置为source_vertex
	// 此处下标的作用是为了更方便地梳理最短路径
	PM[0][source_vertex - 1].distance = 0;
	PM[0][source_vertex - 1].subscript = source_vertex;

	int i = 0;
	// 当没有访问完所有结点，继续循环
	while (!IsAllVerticesVisited(AL, visited))
	{
		// 从未访问的结点中选取距离最小的结点
		int current_vertex = GetMinDistVertex(AL, PM[i], visited);
		// 将其置为访问过
		visited[current_vertex - 1] = 1;
		ArcNode *temp = AL.vertices[current_vertex - 1].first_arc;
		// 查找邻接表，对与该结点相邻的每一个结点都更新距离
		while (temp != NULL)
		{
			if (visited[temp->arc_serial_number - 1] == FALSE)
			{
				// 只有当current vertex的距离加上权值（质心距离）小于相邻点的距离才更新
				if (temp->centroids_dist + PM[i][current_vertex - 1].distance < PM[i][temp->arc_serial_number - 1].distance)
				{
					PM[i][temp->arc_serial_number - 1].distance = temp->centroids_dist + PM[i][current_vertex - 1].distance;
					PM[i][temp->arc_serial_number - 1].subscript = current_vertex;
				}
			}
			temp = temp->next_arc;
		}

		// 这里的作用是将目前的状况往矩阵下一行copy
		for (int j = 0; j < AL.vertex_num; j++)
		{
			if (i < AL.vertex_num - 1)
			{
				PM[i + 1][j].distance = PM[i][j].distance;
				PM[i + 1][j].subscript = PM[i][j].subscript;
			}
		}
		i++;
	}
	/*for (int i = 0; i < AL.vertex_num; i++)
	{
		for (int j = 0; j < AL.vertex_num; j++)
			cout << PM[i][j].distance << "/" << PM[i][j].subscript << "  ";

		cout << endl;
	}*/
	dTime1 = (double)getTickCount() - dTime1;
	int destination_vertex = 0;
	while (1)
	{
		cout << endl;
		cout << "请输入目标区域序号（输入-1退出该实验，输入-2选择新的起始区域）：";
		cin >> destination_vertex;
		if (destination_vertex == -1)
			return EXIT;
		else if (destination_vertex == -2)
			return OK;
		else if (destination_vertex > AL.vertex_num || destination_vertex < 1)
		{
			cout << "目标区域序号必须介于1~" << AL.vertex_num << endl;
			destination_vertex = 0;
			continue;
		}

		double dTime2 = (double)getTickCount();
	
		int temp = destination_vertex;
		PathRecordStackPush(s, temp);

		// 用栈梳理从刚刚Dijkstra算法的到的最短路径
		while (temp != source_vertex)
		{
			temp = PM[AL.vertex_num - 1][temp - 1].subscript;
			PathRecordStackPush(s, temp);
		}

		int stack_length = GetPathRecordStackLength(s);
		int *path_vertex_ID_record = NULL;
		if ((path_vertex_ID_record = (int *)malloc(AL.vertex_num * sizeof(int))) == NULL)
			return ERROR;

		// 记录最短路径中含有哪些结点，为的是最后能够突显最短路径
		for (int i = 0; i < AL.vertex_num; i++)
			path_vertex_ID_record[i] = 0;

		cout << "最短路径：";
		while (GetPathRecordStackLength(s) != 0)
		{
			int vertex_ID;
			PathRecordStackPop(s, &vertex_ID);
			path_vertex_ID_record[vertex_ID - 1] = vertex_ID;
			if (GetPathRecordStackLength(s) != 0)
				cout << vertex_ID << "->";
			else
				cout << vertex_ID;
		}
		cout << "\t最短距离（像素）：" << PM[AL.vertex_num - 1][destination_vertex - 1].distance << "\t经过区域数：" << stack_length - 1;
		HighlightShortestPath(AL, maskImage, path_vertex_ID_record, highlighted_path_image);
		dTime2 = (double)getTickCount() - dTime2;
		printf("\n程序用时 = %gs\n", (dTime1 + dTime2) / getTickFrequency());
		waitKey(0);
	}
	return OK;
}