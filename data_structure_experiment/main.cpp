/**
  ******************************************************************************
  * @file    main.cpp
  * @author  ������
  * @date    2018-10-19
  * @brief   ���ļ���������������չʾʵ������
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

	cout << "�����ݽṹ��ʵ��չʾ\n" << endl;

	// ʵ��һ
	PoissonDisk(maskImage, count);

	// ʵ���
	GraphColoring(AL, maskImage, count, heap, highlighted_path_image);

	// ʵ����
	Huffman(maskImage, count, heap);

	// ʵ����
	cout << "\n��ʵ���ģ�Dijkstra�㷨��" << endl;
	while (1)
	{
		if (Dijkstra(AL, highlighted_path_image, maskImage) == EXIT)
		{
			cout << "�������������һʵ��..." << endl;
			waitKey(0);
			break;
		}
	}
	// ʵ����
	HeapSort(heap, count, AL, highlighted_path_image);

	// ʵ����
	cout << "\n��ʵ�������۰���ҡ�" << endl;
	while (1)
	{
		if (BinarySearch(heap, count, maskImage, highlighted_path_image, AL) == EXIT)
			break;
	}

	cout << "\n�����ݽṹ��ʵ���Ѿ�ȫ��չʾ���..." << endl;
	return 0;
}