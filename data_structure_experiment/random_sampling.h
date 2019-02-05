/**
  ******************************************************************************
  * @file    random_sampling.h
  * @author  李星毅
  * @date    2018-09-19
  * @brief   该文件包含了随机算法的所有实现函数的原型
  * 		 - GridInit
  *   		 - GetCellPos
  * 		 - ActiveInit
  * 		 - ActiveAppend
  * 		 - ActiveRemove
  * 	     - GetPoint
  * 		 - IsPointValid
  * 		 - GetNeighbours
  * 		 - GetActiveLength
  * 		 - GetRandomActive
  * 	  	 - SampleAppend
  * 		 - PoissonDisk
  ******************************************************************************
  */

#pragma once

using namespace cv;
using namespace std;

typedef int Status;

typedef struct Active
{
	Point active_point;
	int active_index;
	struct Active *next;
} Active;


Status GridInit(Mat &grid, double w, int &samples_number, Active *actives, int r);

Status GetCellPos(Point pt, Vec2b &cell_pos, double w);

Status ActiveInit(Active *&actives);

Status ActiveAppend(Active *p, Point pt, int sample_index);

Status ActiveRemove(Active *p, int sample_index);

int GetActiveLength(Active *p);

Status GetNeighbours(Mat grid, Vec2b cell_pos, Mat &neighbours);

Status IsPointValid(Point pt, int r, double w, Mat grid);

Status GetPoint(Mat grid, Active ref_pt, Point &result_pt, int iteration_limit, int r, double w);

Status GetRandomActive(Active *actives, Active &ref_pt);

Status SampleAppend(Mat &grid, Point result_pt, Vec2b cell_pos, int sample_index);

Status PoissonDisk(Mat &maskImage, int &count);
