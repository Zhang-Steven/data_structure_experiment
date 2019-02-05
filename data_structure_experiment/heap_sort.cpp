/**
  ******************************************************************************
  * @file    heap_sort.cpp
  * @author  ������
  * @date    2018-10-17
  * @brief   ���ļ������˶������㷨������ʵ�ֺ���
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
 * @brief  ��ʼ����
 * @param  heap: ��
 * @return ����ִ��״̬
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
 * @brief  �󶥶ѻ�
 * @param  heap: ��
 * @param  particular_area_index: ĳ��������±�
 * @param  area_num: ��������
 * @return ����ִ��״̬
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
 * @brief  С���ѻ�
 * @param  heap: ��
 * @param  particular_area_index: ĳ��������±�
 * @param  area_num: ��������
 * @return ����ִ��״̬
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
 * @brief  �����󶥶�
 * @param  heap: ��
 * @param  area_num: ��������
 * @return ����ִ��״̬
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
 * @brief  ����С����
 * @param  heap: ��
 * @param  area_num: ��������
 * @return ����ִ��״̬
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
 * @brief  ��������
 * @param  heap: ��
 * @param  area_num: ��������
 * @return ����ִ��״̬
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
 * @brief  ��������
 * @param  heap: ��
 * @param  area_num: ��������
 * @return ����ִ��״̬
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
 * @brief  ������ʵ��
 * @param  heap: ��
 * @param  area_num: ��������
 * @param  AL: �ڽӱ�
 * @param  highlighted_path_image: ����֮ǰ�౸�ݵ�һ����ԭɫ������������½���������
 * @return ����ִ��״̬
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

	cout << "\n��ʵ���壺������" << endl;
	cout << "�����򣨽��򣩣�" << endl;
	for (unsigned int i = 1; i <= area_num; i++)
		cout << heap[i].area_ID << ": " << heap[i].area << endl;
	// ������������ϱ���������
	for (int i = 1; i <= AL.vertex_num; i++)
	{
		char str[10];
		sprintf(str, "%d", i);
		putText(highlighted_path_image_copy1, str, AL.vertices[heap[i].area_ID - 1].centroid, FONT_HERSHEY_PLAIN, 0.8, Scalar(0, 0, 0));
	}
	highlighted_path_image_copy1 = highlighted_path_image_copy1 * 0.5 + grayImage * 0.5;
	imshow("�������򣨽��򣩡�", highlighted_path_image_copy1);

	AscendingSort(heap, area_num);
	cout << "\n���������򣩣�" << endl;
	for (unsigned int i = 1; i <= area_num; i++)
		cout << heap[i].area_ID << ": " << heap[i].area << endl;
	// ������������ϱ���������
	for (int i = 1; i <= AL.vertex_num; i++)
	{
		char str[10];
		sprintf(str, "%d", i);
		putText(highlighted_path_image_copy2, str, AL.vertices[heap[i].area_ID - 1].centroid, FONT_HERSHEY_PLAIN, 0.8, Scalar(0, 0, 0));
	}
	highlighted_path_image_copy2 = highlighted_path_image_copy2 * 0.5 + grayImage * 0.5;
	imshow("�����������򣩡�", highlighted_path_image_copy2);

	dTime = (double)getTickCount() - dTime;
	printf("\n������ʱ = %gs\n", dTime / getTickFrequency());
	cout << "�������������һʵ��..." << endl;
	waitKey(0);
	
	return OK;
}