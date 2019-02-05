/**
  ******************************************************************************
  * @file    binary_search.h
  * @author  ������
  * @date    2018-10-18
  * @brief   ���ļ������˶������㷨������ʵ�ֺ���ԭ��
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