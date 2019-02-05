/**
  ******************************************************************************
  * @file    main.cpp
  * @author  李星毅
  * @date    2018-10-19
  * @brief   该文件包含了主函数，展示实验流程
  ******************************************************************************
  */

#include "main.h"

Mat g_maskImage, g_srcImage;

int main()
{
	g_srcImage = imread("fruits.jpg");
	Mat maskImage(g_srcImage.size(), CV_32S);
	Mat highlighted_path_image(maskImage.size(), CV_8UC3);
	int count = 0;
	HeapNode heap[MAX_AREA_NUM + 1];
	HeapInit(heap);
	AdjacencyList AL;

	cout << "《数据结构》实验展示\n" << endl;

	// 实验一
	PoissonDisk(maskImage, count);

	// 实验二
	GraphColoring(AL, maskImage, count, heap, highlighted_path_image);

	// 实验三
	Huffman(maskImage, count, heap);

	// 实验四
	cout << "\n【实验四：Dijkstra算法】" << endl;
	while (1)
	{
		if (Dijkstra(AL, highlighted_path_image, maskImage) == EXIT)
		{
			cout << "按任意键进入下一实验..." << endl;
			waitKey(0);
			break;
		}
	}
	// 实验五
	HeapSort(heap, count, AL, highlighted_path_image);

	// 实验六
	cout << "\n【实验六：折半查找】" << endl;
	while (1)
	{
		if (BinarySearch(heap, count, maskImage, highlighted_path_image, AL) == EXIT)
			break;
	}

	cout << "\n《数据结构》实验已经全部展示完毕..." << endl;
	return 0;
}