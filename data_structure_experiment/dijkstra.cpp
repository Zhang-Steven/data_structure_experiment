/**
  ******************************************************************************
  * @file    Dijkstra.cpp
  * @author  ������
  * @date    2018-10-13
  * @brief   ���ļ�������Dijkstra�㷨������ʵ�ֺ���
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
 * @brief  ��ʼ����ջ
 * @param  s: ·����¼ջ
 * @return ����ִ��״̬
 */
Status InitPathRecordStack(PathRecordStack *(&s))
{
	if ((s = (PathRecordStack *)malloc(sizeof(PathRecordStack))) == NULL)
		return ERROR;
	s->next = NULL;
	return OK;
}

/**
 * @brief  ��ȡջ��ʵ�����
 * @param  s: ·����¼ջ
 * @return ·����¼ջ�ĳ���
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
 * @brief  ��ջ
 * @param  s: ·����¼ջ
 * @param  vertex_ID: ���ID
 * @return ����ִ��״̬
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
 * @brief  ��ջ
 * @param  s: ·����¼ջ
 * @param  vertex_ID: ���ID
 * @return ����ִ��״̬
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
 * @brief  ��δ�����ʹ��ĵ���ѡȡ������С�ĵ�
 * @param  AL: �ڽӱ�
 * @param  PMRow: ���̼�¼�������ָ��
 * @param  visited: ��¼�Ƿ����Ƿ񱻷��ʹ�������
 * @return ������С�ĵ��ID
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
 * @brief  ͻ�����·��
 * @param  AL: �ڽӱ�
 * @param  maskImage: �ṩ��contour��Ϣ��ͼ�����
 * @param  path_vertex_ID_record: ·�����ID��¼��
 * @param  highlighted_path_image: ����֮ǰ�౸�ݵ�һ����ԭɫ��������ڸ������·��
 * @return ����ִ��״̬
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

	// ������������ϱ���������
	for (int i = 0; i < AL.vertex_num; i++)
	{
		char str[10];
		sprintf(str, "%d", i + 1);
		putText(highlighted_path_image_copy, str, AL.vertices[i].centroid, FONT_HERSHEY_PLAIN, 0.8, Scalar(255, 255, 255));
	}
	imshow("��Dijkstra��", highlighted_path_image_copy);
	return OK;
}

/**
 * @brief  Dijkstra�㷨ʵ��
 * @param  AL: �ڽӱ�
 * @param  highlighted_path_image: ����֮ǰ�౸�ݵ�һ����ԭɫ��������ڸ������·��
 * @param  maskImage: �ṩ��contour��Ϣ��ͼ�����
 * @return ����ִ��״̬
 */
Status Dijkstra(AdjacencyList AL, Mat highlighted_path_image, Mat maskImage)
{
	int source_vertex = 0;
	
	while (1)
	{
		cout << "������ŷ�Χ�ǣ�1~" << AL.vertex_num << endl;
		cout << "\n��������ʼ������ţ�����-1�˳���ʵ�飩��";
		cin >> source_vertex;
		if (source_vertex == -1)
			return EXIT;
		else if (source_vertex > AL.vertex_num || source_vertex < 1)
		{
			cout << "��ʼ������ű������1~" << AL.vertex_num << endl << endl;
			source_vertex = 0;
			continue;
		}
		break;
	}

	double dTime1 = (double)getTickCount();

	// ·����¼ջ
	PathRecordStack *s = NULL;
	InitPathRecordStack(s);

	// Ѱ�����·���Ĺ��̼�¼����
	ProcessMatrixCell **PM;
	// ��̬�����ά����
	if ((PM = (ProcessMatrixCell **)malloc(AL.vertex_num * sizeof(ProcessMatrixCell *))) == NULL)
		return ERROR;
	for (int i = 0; i < AL.vertex_num; i++)
		if ((PM[i] = (ProcessMatrixCell *)malloc(AL.vertex_num * sizeof(ProcessMatrixCell))) == NULL)
			return ERROR;

	// �����еľ��붼��Ϊ�����
	for (int i = 0; i < AL.vertex_num; i++)
		for (int j = 0; j < AL.vertex_num; j++)
			PM[i][j].distance = INF;
	
	// ���ʱ�־����
	int visited[MAX_VERTEX_NUM];
	// ��ʼ��Ϊ���е��δ���ʹ�
	for (int i = 0; i < AL.vertex_num; i++)
		visited[i] = FALSE;
	
	// ��source vertex�ľ�������Ϊ0���±�����Ϊsource_vertex
	// �˴��±��������Ϊ�˸�������������·��
	PM[0][source_vertex - 1].distance = 0;
	PM[0][source_vertex - 1].subscript = source_vertex;

	int i = 0;
	// ��û�з��������н�㣬����ѭ��
	while (!IsAllVerticesVisited(AL, visited))
	{
		// ��δ���ʵĽ����ѡȡ������С�Ľ��
		int current_vertex = GetMinDistVertex(AL, PM[i], visited);
		// ������Ϊ���ʹ�
		visited[current_vertex - 1] = 1;
		ArcNode *temp = AL.vertices[current_vertex - 1].first_arc;
		// �����ڽӱ�����ý�����ڵ�ÿһ����㶼���¾���
		while (temp != NULL)
		{
			if (visited[temp->arc_serial_number - 1] == FALSE)
			{
				// ֻ�е�current vertex�ľ������Ȩֵ�����ľ��룩С�����ڵ�ľ���Ÿ���
				if (temp->centroids_dist + PM[i][current_vertex - 1].distance < PM[i][temp->arc_serial_number - 1].distance)
				{
					PM[i][temp->arc_serial_number - 1].distance = temp->centroids_dist + PM[i][current_vertex - 1].distance;
					PM[i][temp->arc_serial_number - 1].subscript = current_vertex;
				}
			}
			temp = temp->next_arc;
		}

		// ����������ǽ�Ŀǰ��״����������һ��copy
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
		cout << "������Ŀ��������ţ�����-1�˳���ʵ�飬����-2ѡ���µ���ʼ���򣩣�";
		cin >> destination_vertex;
		if (destination_vertex == -1)
			return EXIT;
		else if (destination_vertex == -2)
			return OK;
		else if (destination_vertex > AL.vertex_num || destination_vertex < 1)
		{
			cout << "Ŀ��������ű������1~" << AL.vertex_num << endl;
			destination_vertex = 0;
			continue;
		}

		double dTime2 = (double)getTickCount();
	
		int temp = destination_vertex;
		PathRecordStackPush(s, temp);

		// ��ջ����Ӹո�Dijkstra�㷨�ĵ������·��
		while (temp != source_vertex)
		{
			temp = PM[AL.vertex_num - 1][temp - 1].subscript;
			PathRecordStackPush(s, temp);
		}

		int stack_length = GetPathRecordStackLength(s);
		int *path_vertex_ID_record = NULL;
		if ((path_vertex_ID_record = (int *)malloc(AL.vertex_num * sizeof(int))) == NULL)
			return ERROR;

		// ��¼���·���к�����Щ��㣬Ϊ��������ܹ�ͻ�����·��
		for (int i = 0; i < AL.vertex_num; i++)
			path_vertex_ID_record[i] = 0;

		cout << "���·����";
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
		cout << "\t��̾��루���أ���" << PM[AL.vertex_num - 1][destination_vertex - 1].distance << "\t������������" << stack_length - 1;
		HighlightShortestPath(AL, maskImage, path_vertex_ID_record, highlighted_path_image);
		dTime2 = (double)getTickCount() - dTime2;
		printf("\n������ʱ = %gs\n", (dTime1 + dTime2) / getTickFrequency());
		waitKey(0);
	}
	return OK;
}