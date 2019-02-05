/**
  ******************************************************************************
  * @file    heap_sort.cpp
  * @author  李星毅
  * @date    2018-10-17
  * @brief   该文件包含了堆排序算法的所有实现函数
  * 		 - HeapInit
  * 		 - MaxHeapify
  * 		 - MinHeapify
  * 		 - CreateMaxHeap
  * 		 - CreateMinHeap
  * 		 - AscendingSort
  * 		 - DescendingSort
  * 		 - HeapSort
  ******************************************************************************
  */

#define _CRT_SECURE_NO_WARNINGS
#include "main.h"

/**
 * @brief  初始化堆
 * @param  heap: 堆
 * @return 函数执行状态
 */
Status HeapInit(HeapNode *heap)
{
	for (int i = 0; i <= MAX_AREA_NUM; i++)
	{
		heap[i].area = 0;
		heap[i].area_ID = i;
	}
	return OK;
}

/**
 * @brief  大顶堆化
 * @param  heap: 堆
 * @param  particular_area_index: 某个区域的下标
 * @param  area_num: 区域数量
 * @return 函数执行状态
 */
Status MaxHeapify(HeapNode *heap, int particular_area_index, unsigned int area_num)
{
	unsigned int item = heap[particular_area_index].area;
	unsigned int index = heap[particular_area_index].area_ID;
	// Index of left child
	unsigned int j = 2 * particular_area_index;

	while (j <= area_num)
	{
		// Compare to select the larger one between left child and right child
		if (j < area_num && heap[j].area < heap[j + 1].area)
			j = j + 1;
		// If the reference area maintains largest, then the adjustment is needless
		if (item >= heap[j].area)
			break;
		// Adjustment condition satified, execute the exchange
		heap[j / 2] = heap[j];
		// Once adjustment occured between item and its larger child
		// Something may happen to destruct the heap structure
		// i.e. continue to check the subtree of the larger child to assure 
		// heap structure undamaged
		j *= 2;
	}
	heap[j / 2].area = item;
	heap[j / 2].area_ID = index;
	return OK;
}

/**
 * @brief  小顶堆化
 * @param  heap: 堆
 * @param  particular_area_index: 某个区域的下标
 * @param  area_num: 区域数量
 * @return 函数执行状态
 */
Status MinHeapify(HeapNode *heap, int particular_area_index, unsigned int area_num)
{
	unsigned int item = heap[particular_area_index].area;
	unsigned int index = heap[particular_area_index].area_ID;
	// Index of left child
	unsigned int j = 2 * particular_area_index;

	while (j <= area_num)
	{
		// Compare to select the smaller one between left child and right child
		if (j < area_num && heap[j].area > heap[j + 1].area)
			j = j + 1;
		// If the reference area maintains smallest, then the adjustment is needless
		if (item <= heap[j].area)
			break;
		// Adjustment condition satified, execute the exchange
		heap[j / 2] = heap[j];
		// Once adjustment occured between item and its larger child
		// Something may happen to destruct the heap structure
		// i.e. continue to check the subtree of the larger child to assure 
		// heap structure undamaged
		j *= 2;
	}
	heap[j / 2].area = item;
	heap[j / 2].area_ID = index;
	return OK;
}

/**
 * @brief  创建大顶堆
 * @param  heap: 堆
 * @param  area_num: 区域数量
 * @return 函数执行状态
 */
Status CreateMaxHeap(HeapNode *heap, unsigned int area_num)
{
	// Heapify from area_num/2 to root node
	// as from area_num/2 to area_num all always
	// satisfy heap property
	for (int i = area_num / 2; i >= 1; i--)
		MaxHeapify(heap, i, area_num);
	return OK;
}

/**
 * @brief  创建小顶堆
 * @param  heap: 堆
 * @param  area_num: 区域数量
 * @return 函数执行状态
 */
Status CreateMinHeap(HeapNode *heap, unsigned int area_num)
{
	// Heapify from area_num/2 to root node
	// as from area_num/2 to area_num all always
	// satisfy heap property
	for (int i = area_num / 2; i >= 1; i--)
		MinHeapify(heap, i, area_num);
	return OK;
}

/**
 * @brief  升序排序
 * @param  heap: 堆
 * @param  area_num: 区域数量
 * @return 函数执行状态
 */
Status AscendingSort(HeapNode *heap, unsigned int area_num)
{
	// Ascending sort
	CreateMaxHeap(heap, area_num);
	for (int i = area_num; i >= 2; i--)
	{
		// Exchange root and the last element
		HeapNode temp = heap[1];
		heap[1] = heap[i];
		heap[i] = temp;
		// Discarding the last element and heapify
		MaxHeapify(heap, 1, i - 1);
	}
	return OK;
}

/**
 * @brief  降序排序
 * @param  heap: 堆
 * @param  area_num: 区域数量
 * @return 函数执行状态
 */
Status DescendingSort(HeapNode *heap, unsigned int area_num)
{
	// Descending sort
	CreateMinHeap(heap, area_num);
	for (int i = area_num; i >= 2; i--)
	{
		// Exchange root and the last element
		HeapNode temp = heap[1];
		heap[1] = heap[i];
		heap[i] = temp;
		// Discarding the last element and heapify
		MinHeapify(heap, 1, i - 1);
	}
	return OK;
}

/**
 * @brief  堆排序实现
 * @param  heap: 堆
 * @param  area_num: 区域数量
 * @param  AL: 邻接表
 * @param  highlighted_path_image: 这是之前多备份的一张四原色结果，用于重新进行区域标号
 * @return 函数执行状态
 */
Status HeapSort(HeapNode *heap, unsigned int area_num, AdjacencyList AL, Mat highlighted_path_image)
{
	double dTime = (double)getTickCount();
	Mat srcImage = imread("fruits.jpg");
	Mat grayImage;
	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);
	cvtColor(grayImage, grayImage, COLOR_GRAY2BGR);

	Mat highlighted_path_image_copy1, highlighted_path_image_copy2;
	highlighted_path_image.copyTo(highlighted_path_image_copy1);
	highlighted_path_image.copyTo(highlighted_path_image_copy2);

	DescendingSort(heap, area_num);

	cout << "\n【实验五：堆排序】" << endl;
	cout << "堆排序（降序）：" << endl;
	for (unsigned int i = 1; i <= area_num; i++)
		cout << heap[i].area_ID << ": " << heap[i].area << endl;
	// 在区域的质心上标上区域编号
	for (int i = 1; i <= AL.vertex_num; i++)
	{
		char str[10];
		sprintf(str, "%d", i);
		putText(highlighted_path_image_copy1, str, AL.vertices[heap[i].area_ID - 1].centroid, FONT_HERSHEY_PLAIN, 0.8, Scalar(0, 0, 0));
	}
	highlighted_path_image_copy1 = highlighted_path_image_copy1 * 0.5 + grayImage * 0.5;
	imshow("【堆排序（降序）】", highlighted_path_image_copy1);

	AscendingSort(heap, area_num);
	cout << "\n堆排序（升序）：" << endl;
	for (unsigned int i = 1; i <= area_num; i++)
		cout << heap[i].area_ID << ": " << heap[i].area << endl;
	// 在区域的质心上标上区域编号
	for (int i = 1; i <= AL.vertex_num; i++)
	{
		char str[10];
		sprintf(str, "%d", i);
		putText(highlighted_path_image_copy2, str, AL.vertices[heap[i].area_ID - 1].centroid, FONT_HERSHEY_PLAIN, 0.8, Scalar(0, 0, 0));
	}
	highlighted_path_image_copy2 = highlighted_path_image_copy2 * 0.5 + grayImage * 0.5;
	imshow("【堆排序（升序）】", highlighted_path_image_copy2);

	dTime = (double)getTickCount() - dTime;
	printf("\n程序用时 = %gs\n", dTime / getTickFrequency());
	cout << "按任意键进入下一实验..." << endl;
	waitKey(0);
	
	return OK;
}