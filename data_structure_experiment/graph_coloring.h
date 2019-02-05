/**
  ******************************************************************************
  * @file    graph_coloring.h
  * @author  李星毅
  * @date    2018-09-26
  * @brief   该文件包含了四原色的所有实现函数的原型
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

// 弧节点
typedef struct ArcNode
{
	int arc_serial_number;			        // 该弧的序号
	int adj_area_num;						// 该弧的相邻区域数
	int weight;								// 面积权重
	unsigned int centroids_dist;			// 该弧的质心到顶点的质心的距离
	struct ArcNode *next_arc;				// 指向下一条弧的指针				
} ArcNode;

// 顶点节点
typedef struct VertexNode
{
	int vertex_serial_number;					// 顶点序号
	int adj_area_num;							// 顶点的相邻区域数
	Point centroid;								// 质心
	ArcNode *first_arc;							// 指向第一条依附该顶点的弧的指针
} VertexNode, AdjList[MAX_VERTEX_NUM];			

// 无向图的邻接表
typedef struct AdjacencyList
{
	AdjList vertices;					
	int vertex_num, arc_num;			// 图的当前顶点数和弧数
} AdjacencyList;


// 队列节点结构
typedef struct QNode
{
	int data;
	bool color_used[4];
	struct QNode *next;
} QNode, *QueuePtr;

// 队列指针集合
typedef struct LinkQueue
{
	QueuePtr front;			// 队头指针
	QueuePtr rear;			// 队尾指针
} LinkQueue;

// 链栈结构体
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

