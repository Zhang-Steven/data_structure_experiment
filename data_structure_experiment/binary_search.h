/**
  ******************************************************************************
  * @file    binary_search.h
  * @author  李星毅
  * @date    2018-10-18
  * @brief   该文件包含了堆排序算法的所有实现函数原型
  * 		 - HighlightArea
  * 		 - BinarySearch
  ******************************************************************************
  */

#pragma once

using namespace cv;
using namespace std;

typedef int Status;

Status HighlightArea(HeapNode *heap, unsigned int inf_index, unsigned int sup_index, Mat maskImage, Mat highlighted_path_image, AdjacencyList AL);

Status BinarySearch(HeapNode *heap, int count, Mat maskImage, Mat highlighted_path_image, AdjacencyList AL);