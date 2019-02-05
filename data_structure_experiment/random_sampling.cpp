/**
  ******************************************************************************
  * @file    random_sampling.cpp
  * @author  ������
  * @date    2018-09-19
  * @brief   ���ļ�����������㷨������ʵ�ֺ���
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

#define _CRT_SECURE_NO_WARNINGS
#include "main.h"

extern Mat g_srcImage;
extern Mat g_maskImage;

/**
 * @brief  ����һ�����񣬴˺�����ֱ������˵�һ���㣬λ������ͼ������
 * @param  grid: ����
 * @param  w: ÿ�����ӵĿ��
 * @param  samples_number: ����������
 * @param  actives: ������б�
 * @param  r: ���ӵ�֮�����С����
 * @return ����ִ��״̬
 */
Status GridInit(Mat &grid, double w, int &samples_number, Active *actives, int r)
{
	Point original(250, 250);
	Vec2b cell_pos;
	
	GetCellPos(original, cell_pos, w);
	short *data = grid.ptr<short>(cell_pos[1]);
	data[cell_pos[0] * grid.channels()] = original.x;
	data[cell_pos[0] * grid.channels() + 1] = original.y;
	data[cell_pos[0] * grid.channels() + 2] = 0;
	samples_number += 1;
	ActiveAppend(actives, original, 0);
	/*int j = 0;*/
	/*for (int i = 1; i < M - 2; i += r, j++)
	{
		Point pt(i, 1);
		GetCellPos(pt, cell_pos, w);
		data = grid.ptr<short>(cell_pos[1]);
		data[cell_pos[0] * grid.channels()] = pt.x;
		data[cell_pos[0] * grid.channels() + 1] = pt.y;
		data[cell_pos[0] * grid.channels() + 2] = 0;
		samples_number += 1;
		ActiveAppend(actives, pt, j);
	}
	for (int i = 1; i < M - 2; i += r, j++)
	{
		Point pt(i, N - 2);
		GetCellPos(pt, cell_pos, w);
		data = grid.ptr<short>(cell_pos[1]);
		data[cell_pos[0] * grid.channels()] = pt.x;
		data[cell_pos[0] * grid.channels() + 1] = pt.y;
		data[cell_pos[0] * grid.channels() + 2] = 0;
		samples_number += 1;
		ActiveAppend(actives, pt, j);
	}
	for (int i = 1; i < N - 2; i += r, j++)
	{
		Point pt(1, i);
		GetCellPos(pt, cell_pos, w);
		data = grid.ptr<short>(cell_pos[1]);
		data[cell_pos[0] * grid.channels()] = pt.x;
		data[cell_pos[0] * grid.channels() + 1] = pt.y;
		data[cell_pos[0] * grid.channels() + 2] = 0;
		samples_number += 1;
		ActiveAppend(actives, pt, j);
	}
	for (int i = 1; i < N - 2; i += r, j++)
	{
		Point pt(M - 2, i);
		GetCellPos(pt, cell_pos, w);
		data = grid.ptr<short>(cell_pos[1]);
		data[cell_pos[0] * grid.channels()] = pt.x;
		data[cell_pos[0] * grid.channels() + 1] = pt.y;
		data[cell_pos[0] * grid.channels() + 2] = 0;
		samples_number += 1;
		ActiveAppend(actives, pt, j);
	}*/

	return OK;
}

/**
 * @brief  �ú����õ�ĳ���������е�λ��
 * @param  pt: ���ӵ�
 * @param  cell_pos: ���ӵ��������е�λ�ñ���
 * @param  w: ÿ�����ӵĿ��
 * @return ����ִ��״̬
 */
Status GetCellPos(Point pt, Vec2b &cell_pos, double w)
{
	cell_pos[0] = (int)(pt.x / w);
	cell_pos[1] = (int)(pt.y / w);
	return OK;
}

/**
 * @brief  ����һ��������б�����������
 * @param  actives: ������б�
 * @return ����ִ��״̬
 */
Status ActiveInit(Active *&actives)
{
	if ((actives = (Active *)malloc(sizeof(Active))) == NULL)
		return ERROR;
	actives->next = NULL;
	return OK;
}

/**
 * @brief  ����һ������㣨����Ĳ��룩
 * @param  p: ָ�򼤻���ָ��
 * @param  pt: ���ӵ�
 * @param  sample_index: �����±�
 * @return ����ִ��״̬
 */
Status ActiveAppend(Active *p, Point pt, int sample_index)
{
	Active *temp = NULL;

	while (p->next != NULL)
		p = p->next;

	if ((temp = (Active *)malloc(sizeof(Active))) == NULL)
		return ERROR;
	p->next = temp;
	temp->next = NULL;
	temp->active_point = pt;
	temp->active_index = sample_index;
	return OK;
}

/**
 * @brief  �Ƴ�����㣨�����ɾ����
 * @param  p: ĳ�������
 * @param  sample_index: �����±�
 * @return ����ִ��״̬
 */
Status ActiveRemove(Active *p, int sample_index)
{
	if (p->next == NULL)
		return ERROR;
	Active *temp = p->next;

	while (temp->next != NULL && temp->active_index != sample_index)
	{
		p = temp;
		temp = temp->next;
	}

	if (temp->active_index == sample_index)
	{
		p->next = temp->next;
		free(temp);
		temp = NULL;
	}
	return OK;
}

/**
 * @brief  ��ĳ�������Ϊ��׼���õ�һ����Ч���µ�
 * @param  grid: ����
 * @param  ref_pt: ĳ���ο������
 * @param  result_pt: ��������Ч���µ�
 * @param  iteration_limit: ������������
 * @param  r: ���ӵ�֮�����С����
 * @param  w: ÿ�����ӵĿ��
 * @return ����ִ��״̬
 */
Status GetPoint(Mat grid, Active ref_pt, Point &result_pt, int iteration_limit, int r, double w)
{
	int i = 0;
	RNG rng((unsigned)time(NULL));
	double theta = 0, rand_radius;
	Point pt;

	while (i < iteration_limit)
	{
		theta = rng.uniform((double)0, (double)(2 * PI));
		rand_radius = r + 1;
		//rand_radius = rng.uniform(r, 2 * r);

		pt.x = ref_pt.active_point.x + (int)(rand_radius * cos(theta));
		pt.y = ref_pt.active_point.y + (int)(rand_radius * sin(theta));

		// If this new point is out of the background, discard it
		if (pt.x < 0 || pt.x > M || pt.y < 0 || pt.y > N)
			continue;
		// If this new point is inside the background, consider whether it is valid or not
		if (IsPointValid(pt, r, w, grid))
		{
			result_pt.x = pt.x;
			result_pt.y = pt.y;
			return OK;
		}
		i++;
	}
	return ERROR;
}

/**
 * @brief  ���������µ��Ƿ���Ч
 * @param  pt: ĳ�����ӵ�
 * @param  r: ���ӵ�֮�����С����
 * @param  w: ÿ�����ӵĿ��
 * @param  grid: ����
 * @return ����ִ��״̬
 */
Status IsPointValid(Point pt, int r, double w, Mat grid)
{
	Mat neighbours(5, 5, CV_16SC3, Scalar(NONE, NONE, NONE));
	Point nearby_point;
	double distance2;

	// Is the point a valid point to emit as a sample?
	// It must be no closer than r from any other point: check the cells in its
	// immediate neighbourhood
	Vec2b cell_pos;
	GetCellPos(pt, cell_pos, w);

	GetNeighbours(grid, cell_pos, neighbours);

	for (int i = 0; i < neighbours.rows; i++)
	{
		for (int j = 0; j < neighbours.cols; j++)
		{
			short *data = neighbours.ptr<short>(i);
			if (data[j * neighbours.channels()] == NONE && data[j * neighbours.channels() + 1] == NONE && data[j * neighbours.channels() + 2] == NONE)
				continue;
			distance2 = pow((data[j * neighbours.channels()] - pt.x), 2) + pow((data[j * neighbours.channels() + 1] - pt.y), 2);
			if (distance2 < pow(r, 2))
				return FALSE;
		}
	}
	return TRUE;
}

/**
 * @brief  �õ�ĳһ������ھӵ�������±�
 * @param  grid: ����
 * @param  cell_pos: ���ӵ��������е�λ�ñ���
 * @param  neighbours: ĳ������ھӾ���
 * @return ����ִ��״̬
 */
Status GetNeighbours(Mat grid, Vec2b cell_pos, Mat &neighbours)
{
	int j = 0, i;
	int offset[25][2]
	{
		{-2, -2}, {-1, -2}, {0, -2}, {1, -2}, {2, -2},
		{-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {2, -1},
		{-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}, 
		{-2, 1}, {-1, 1}, {0, 1}, {1, 1}, {2, 1}, 
		{-2, 2}, {-1, 2}, {0, 2}, {1, 2}, {2, 2},
	};
	for (i = 0; i < 25; i++)
	{
		if ((cell_pos[0] + offset[i][0]) > -3 && (cell_pos[0] + offset[i][0]) < grid.cols * grid.channels() && (cell_pos[1] + offset[i][1] >= 0 && (cell_pos[1] + offset[i][1]) < grid.rows))
		{
			short *grid_data = grid.ptr<short>(cell_pos[1] + offset[i][1]);
			if (i % 5 == 0 && i != 0)
				j++;
			short *neighbours_data = neighbours.ptr<short>(j);
			neighbours_data[(i % 5) * neighbours.channels()] = grid_data[(cell_pos[0] + offset[i][0]) * grid.channels()];
			neighbours_data[(i % 5) * neighbours.channels() + 1] = grid_data[(cell_pos[0] + offset[i][0]) * grid.channels() + 1];
			neighbours_data[(i % 5) * neighbours.channels() + 2] = grid_data[(cell_pos[0] + offset[i][0]) * grid.channels() + 2];
		}
		else
			continue;
	}
	//cout << neighbours << endl;
	return OK;
}

/**
 * @brief  ���ؼ�����б�ĳ���
 * @param  p: ָ�򼤻���ָ��
 * @return ������б���
 */
int GetActiveLength(Active *p)
{
	int length = 0;

	while (p->next != NULL)
	{
		length++;
		p = p->next;
	}

	return length;
}

/**
 * @brief  �ڼ�����б������ѡ��һ�������
 * @param  actives: ������б�
 * @param  ref_pt: ĳ���ο������
 * @return ����ִ��״̬
 */
Status GetRandomActive(Active *actives, Active &ref_pt)
{
	RNG rng((unsigned)time(NULL));
	int i;
	int rand_index = rng.uniform(0, GetActiveLength(actives));

	for (i = 0; i < rand_index; i++)
		actives = actives->next;

	ref_pt.active_point = actives->next->active_point;
	ref_pt.active_index = actives->next->active_index;
	return OK;
}

/**
 * @brief  ���������µ���ӵ�������
 * @param  grid: ����
 * @param  result_pt: ���ɵ���Ч��
 * @param  cell_pos: ĳ����������е�λ�ñ���
 * @param  sample_index: �����±� 
 * @return ����ִ��״̬
 */
Status SampleAppend(Mat &grid, Point result_pt, Vec2b cell_pos, int sample_index)
{
	short *data = grid.ptr<short>(cell_pos[1]);
	data[cell_pos[0] * grid.channels()] = result_pt.x;
	data[cell_pos[0] * grid.channels() + 1] = result_pt.y;
	data[cell_pos[0] * grid.channels() + 2] = sample_index;
	return OK;
}

/**
 * @brief  ����Բ��ʵ�ֺ��������滹�����˷�ˮ���㷨
 * @param  maskImage: ��¼��ˮ�����������ľ���
 * @param  count: ��¼�ܹ����������ӵ����
 * @return ����ִ��״̬
 */
Status PoissonDisk(Mat &maskImage, int &count)
{
	Mat grayImage, srcImage;
	g_srcImage.copyTo(srcImage);
	cvtColor(g_srcImage, g_maskImage, COLOR_BGR2GRAY);
	cvtColor(g_maskImage, grayImage, COLOR_GRAY2BGR);
	g_maskImage = Scalar::all(0);
	Mat img = imread("fruits.jpg");
	RNG rng((unsigned)time(NULL));
	int K;
	cout << "��ʵ��һ������㷨��" << endl;
	while (1)
	{
		cout << "��������Ҫ���������������� ";
		cin >> K;
		if (K <= 0)
			cout << "�������󣬱���������������..." << endl;
		else
			break;
	}

	double dTime = (double)getTickCount();

	int iteration_limit = 100;
	int samples_number = 0;
	int r = (int)sqrt(M * N / K);
	double w = r / sqrt(2);
	int grid_cols = (int)(M / w) + 1;
	int grid_rows = (int)(N / w) + 1;


	Mat grid(grid_cols, grid_rows, CV_16SC3, Scalar(NONE, NONE, NONE));
	Active *actives = NULL;

	if (!ActiveInit(actives))
	{
		cout << "Error: insufficient memory to initialize an active list..." << endl;
		exit(1);
	}

	if (!GridInit(grid, w, samples_number, actives, r))
	{
		cout << "Error: insufficient memory to initialize a grid..." << endl;
		exit(1);
	}



	while (GetActiveLength(actives))
	{
		Active ref_pt;
		Point result_pt;
		// Choose a random reference point from the active list
		GetRandomActive(actives, ref_pt);

		if (GetPoint(grid, ref_pt, result_pt, iteration_limit, r, w))
		{
			Vec2b cell_pos;
			GetCellPos(result_pt, cell_pos, w);

			SampleAppend(grid, result_pt, cell_pos, samples_number);
			ActiveAppend(actives, result_pt, samples_number);
			samples_number++;
		}
		else
		{
			circle(img, ref_pt.active_point, 2, Scalar(255, 255, 255), -1);
			circle(g_maskImage, ref_pt.active_point, 2, Scalar(255, 255, 255), -1);
			circle(g_srcImage, ref_pt.active_point, 2, Scalar(255, 255, 255), -1);
			//char str[10];
			//sprintf(str, "%d", count);
			//putText(img, str, ref_pt.active_point, FONT_HERSHEY_PLAIN, 0.8, Scalar(0, 0, 0));
			ActiveRemove(actives, ref_pt.active_index);
			count++;
		}
	}
	cout << "ʵ�ʲ�������������" << count << endl;
	imshow("������Բ�̡�", img);

	// ��ˮ���㷨
	while (1)
	{
		int i, j, compCount = 0;
		vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;

		findContours(g_maskImage, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);

		if (contours.empty())
			continue;

		maskImage = Scalar::all(0);

		for (int index = 0; index >= 0; index = hierarchy[index][0], compCount++)
			drawContours(maskImage, contours, index, Scalar::all(compCount + 1), -1, 8, hierarchy, INT_MAX);

		if (compCount == 0)
			continue;

		vector<Vec3b> colorTab;
		for (i = 0; i < compCount; i++)
		{
			int b = theRNG().uniform(0, 255);
			int g = theRNG().uniform(0, 255);
			int r = theRNG().uniform(0, 255);

			colorTab.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
		}

		watershed(srcImage, maskImage);

		Mat watershedImage(maskImage.size(), CV_8UC3);
		for (i = 0; i < maskImage.rows; i++)
			for (j = 0; j < maskImage.cols; j++)
			{
				int index = maskImage.at<int>(i, j);
				if (index == -1)
					watershedImage.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
				else if (index <= 0 || index > compCount)
					watershedImage.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
				else
					watershedImage.at<Vec3b>(i, j) = colorTab[index - 1];
			}

		watershedImage = watershedImage * 0.5 + grayImage * 0.5;
		imshow("����ˮ���㷨��", watershedImage);

		dTime = (double)getTickCount() - dTime;
		printf("\n������ʱ = %gs\n", dTime / getTickFrequency());

		cout << "�������������һʵ��..." << endl;
		waitKey(0);
		break;
	}
	return OK;
}


