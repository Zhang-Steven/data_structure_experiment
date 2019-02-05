/**
  ******************************************************************************
  * @file    binary_search.cpp
  * @author  李星毅
  * @date    2018-10-18
  * @brief   该文件包含了堆排序算法的所有实现函数
  * 		 - HighlightArea
  * 		 - BinarySearch
  ******************************************************************************
  */

#define _CRT_SECURE_NO_WARNINGS
#include "main.h"

/**
 * @brief  高亮符合面积区间的区域
 * @param  heap: 堆
 * @param  inf_index: 下确界的区域的序号
 * @param  sup_index: 上确界的区域的序号
 * @param  maskImage: 提供的contour信息的图像矩阵
 * @param  highlighted_path_image: 用于高亮符合面积区间的区域的图像
 * @param  AL: 邻接表
 * @return 函数执行状态
 */
Status HighlightArea(HeapNode *heap, unsigned int inf_index, unsigned int sup_index, Mat maskImage, Mat highlighted_path_image, AdjacencyList AL)
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
				for (unsigned int k = inf_index; k <= sup_index; k++)
				{
					if (index == heap[k].area_ID)
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
	for (unsigned int i = inf_index; i <= sup_index; i++)
	{
		char str[10];
		sprintf(str, "%d", heap[i].area_ID);
		putText(highlighted_path_image_copy, str, AL.vertices[heap[i].area_ID - 1].centroid, FONT_HERSHEY_PLAIN, 0.8, Scalar(255, 255, 255));
	}
	imshow("【折半查找】", highlighted_path_image_copy);
	return OK;
}

/**
 * @brief  折半查找实现
 * @param  heap: 堆
 * @param  count: 区域总数
 * @param  maskImage: 提供的contour信息的图像矩阵
 * @param  highlighted_path_image: 用于高亮符合面积区间的区域的图像
 * @param  AL: 邻接表
 * @return 函数执行状态
 */
Status BinarySearch(HeapNode *heap, int count, Mat maskImage, Mat highlighted_path_image, AdjacencyList AL)
{
	unsigned int sup, inf;

	while (1)
	{
		int temp;
		cout << "\n区域面积区间为：" << heap[1].area << "~" << heap[count].area << endl;
		cout << "\n请输入查找下确界（输入-1退出该实验）：";
		cin >> temp;
		if (temp == -1)
			return EXIT;
		else if (temp < (int)heap[1].area)
		{
			cout << "下确界必须大于等于" << heap[1].area << endl;
			continue;
		}
		inf = (unsigned int)temp;

		cout << "请输入查找上确界（输入-1退出该实验）：";
		cin >> temp;
		if (temp == -1)
			return EXIT;
		else if (temp < (int)inf || temp > (int)heap[count].area)
		{
			cout << "上确界必须大于等于" << inf << "且小于等于" << heap[count].area << endl;
			continue;
		}
		sup = (unsigned int)temp;
		break;
	}

	double dTime = (double)getTickCount();

	// Binary Search implementaion
	unsigned int inf_index = 0;
	unsigned int low = 1, high = count, mid;
	while (low <= high)
	{
		mid = (low + high) / 2;
		if (heap[mid].area == inf)
		{
			inf_index = mid;
			break;
		}
		else if (heap[mid].area < inf)
		{
			low = mid + 1;
			inf_index = low;
		}
		else
			high = mid - 1;
	}

	unsigned int sup_index = 0;
	high = count;
	while (low <= high)
	{
		mid = (low + high) / 2;
		if (heap[mid].area == sup)
		{
			sup_index = mid;
			break;
		}
		else if (heap[mid].area < sup)
			low = mid + 1;
		else
		{
			high = mid - 1;
			sup_index = high;
		}
	}

	if (inf_index > sup_index)
		cout << "\n无区域面积介于" << inf << "~" << sup << "的区域..." << endl;
	else if (inf_index <= sup_index)
	{
		cout << "\n区域面积介于" << inf << "~" << sup << "的区域有：" << endl;
		for (unsigned int i = inf_index; i <= sup_index; i++)
			cout << heap[i].area_ID << ": " << heap[i].area << endl;
	}
	

	HighlightArea(heap, inf_index, sup_index, maskImage, highlighted_path_image, AL);
	dTime = (double)getTickCount() - dTime;
	printf("\n程序用时 = %gs\n", dTime / getTickFrequency());
	waitKey(0);
	return OK;
}