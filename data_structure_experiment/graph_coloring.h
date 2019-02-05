/**
  ******************************************************************************
  * @file    graph_coloring.h
  * @author  ������
  * @date    2018-09-26
  * @brief   ���ļ���������ԭɫ������ʵ�ֺ�����ԭ��
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

#pragma once
#define MAX_VERTEX_NUM 1200
#define RED	0
#define GREEN 1
#define BLUE 2
#define YELLOW 3
#define NOT_TINTED -1
#define M 500
#define N 500
#define PI 3.1415926


using namespace cv;
using namespace std;

typedef int Status;

typedef struct HeapNode
{
	unsigned int area;
	int area_ID;
} HeapNode;

// ���ڵ�
typedef struct ArcNode
{
	int arc_serial_number;			        // �û������
	int adj_area_num;						// �û�������������
	int weight;								// ���Ȩ��
	unsigned int centroids_dist;			// �û������ĵ���������ĵľ���
	struct ArcNode *next_arc;				// ָ����һ������ָ��				
} ArcNode;

// ����ڵ�
typedef struct VertexNode
{
	int vertex_serial_number;					// �������
	int adj_area_num;							// ���������������
	Point centroid;								// ����
	ArcNode *first_arc;							// ָ���һ�������ö���Ļ���ָ��
} VertexNode, AdjList[MAX_VERTEX_NUM];			

// ����ͼ���ڽӱ�
typedef struct AdjacencyList
{
	AdjList vertices;					
	int vertex_num, arc_num;			// ͼ�ĵ�ǰ�������ͻ���
} AdjacencyList;


// ���нڵ�ṹ
typedef struct QNode
{
	int data;
	bool color_used[4];
	struct QNode *next;
} QNode, *QueuePtr;

// ����ָ�뼯��
typedef struct LinkQueue
{
	QueuePtr front;			// ��ͷָ��
	QueuePtr rear;			// ��βָ��
} LinkQueue;

// ��ջ�ṹ��
typedef struct ColoringStack
{
	int area_number;
	int color;
	struct ColoringStack *next;
} ColoringStack;


Status InitQueue(LinkQueue &Q);

Status EnQueue(LinkQueue &Q, int e);

Status IsQueueEmpty(LinkQueue Q);

Status DeQueue(LinkQueue &Q, int &e);

Status InitColoringStack(ColoringStack *(&s));

int GetColoringStackLength(ColoringStack *s);

Status ColoringStackPush(ColoringStack *(&s), int area_number, int color);

Status ColoringStackPop(ColoringStack *(&s), int *area_number);

Status CreateAdjacencyList(AdjacencyList &AL, Mat maskImage, int count);

Status ShowAdjacencyList(AdjacencyList AL);

Status TintingColors(AdjacencyList &AL, LinkQueue tinting_queue, Mat maskImage, Mat &highlighted_path_image);

int ChooseBestTintingArea(AdjacencyList AL, const int *visited);

int GetRowUnvisitedNum(AdjacencyList AL, int vertex_serial_number, const int *visited);

Status IsAllVerticesVisited(AdjacencyList AL, const int *visited);

Status ALOptimization(AdjacencyList &AL);

Status FormBestTintingPath(AdjacencyList AL, LinkQueue &tinting_queue);

Status GraphColoring(AdjacencyList &AL, Mat maskImage, int count, HeapNode *heap, Mat &highlighted_path_image);

Status GetAreaCentroid(AdjacencyList &AL, Mat maskImage, Mat four_primary_colors_image);

Status GetAreaWeights(AdjacencyList &AL, Mat maskImage, int count, HeapNode *heap);

