/**
  ******************************************************************************
  * @file    Dijkstra.h
  * @author  李星毅
  * @date    2018-10-13
  * @brief   该文件包含了Dijkstra算法的所有实现函数原型
  * 		 - InitPathRecordStack
  * 		 - GetPathRecordStackLength
  * 		 - PathRecordStackPush
  * 		 - PathRecordStackPop
  * 		 - GetMinDistVertex
  * 		 - HighlightShortestPath
  * 		 - Dijkstra
  ******************************************************************************
  */

#pragma once

using namespace cv;
using namespace std;

typedef int Status;

// 链栈结构体
typedef struct PathRecordStack
{
	int vertex_ID;
	struct PathRecordStack *next;
} PathRecordStack;

typedef struct ProcessMatrixCell
{
	unsigned int distance;
	unsigned int subscript;
} ProcessMatrixCell;

Status InitPathRecordStack(PathRecordStack *(&s));

int GetPathRecordStackLength(PathRecordStack *s);

Status PathRecordStackPush(PathRecordStack *(&s), int vertex_ID);

Status PathRecordStackPop(PathRecordStack *(&s), int *vertex_ID);

int GetMinDistVertex(AdjacencyList AL, ProcessMatrixCell* PMRow, const int *visited);

Status Dijkstra(AdjacencyList AL, Mat highlighted_path_image, Mat maskImage);

Status HighlightShortestPath(AdjacencyList AL, Mat maskImage, int *path_vertex_ID_record, Mat highlighted_path_image);