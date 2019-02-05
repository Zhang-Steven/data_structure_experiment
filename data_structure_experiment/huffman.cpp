/**
  ******************************************************************************
  * @file    huffman.cpp
  * @author  ������
  * @date    2018-10-04
  * @brief   ���ļ������˹�������������ʵ�ֺ���
  * 		 - InitTraverseStack
  * 		 - IsTraverseStackEmpty
  * 		 - TraverseStackPush
  * 		 - TraverseStackPop
  * 		 - Select
  * 		 - HuffmanCoding
  * 		 - HuffmanDecoding
  * 		 - DepthAugment
  * 		 - DrawHuffmanTree
  * 		 - SplitTree
  * 		 - Huffman
  ******************************************************************************
  */

#define _CRT_SECURE_NO_WARNINGS
#include "main.h"

/**
 * @brief  ��ʼ����������ջ
 * @param  s: ��������ջ
 * @return ����ִ��״̬
 */
Status InitTraverseStack(TraverseStack *(&s))
{
	if ((s = (TraverseStack *)malloc(sizeof(TraverseStack))) == NULL)
		return ERROR;
	s->next = NULL;
	return OK;
}

/**
 * @brief  �жϱ�������ջ�Ƿ�Ϊ��
 * @param  s: ��������ջ
 * @return ����ִ��״̬
 */
Status IsTraverseStackEmpty(TraverseStack *s)
{
	if (s->next == NULL)
		return TRUE;
	else
		return FALSE;
}

/**
 * @brief  ��ջ
 * @param  s: ��������ջ
 * @param  e: ��ջԪ��
 * @return ����ִ��״̬
 */
Status TraverseStackPush(TraverseStack *(&s), int e)
{
	TraverseStack *p;
	if ((p = (TraverseStack *)malloc(sizeof(TraverseStack))) == NULL)
		return ERROR;
	p->data = e;
	p->next = s;
	s = p;
	return OK;
}

/**
 * @brief  ��ջ
 * @param  s: ��������ջ
 * @param  e: ��ջԪ��
 * @return ����ִ��״̬
 */
Status TraverseStackPop(TraverseStack *(&s), int &e)
{
	TraverseStack *p;
	if (s->next != NULL)
	{
		p = s;
		e = s->data;
		s = s->next;
		free(p);
		p = NULL;
		return OK;
	}
	else
		return ERROR;
}

/**
 * @brief  ÿ�ζ�����Ҷ�ӽ����ѡȡȨֵ��С��������㣬��������˫�׵�ֵΪ0
 * @param  HT: ��������
 * @param  n: Ҷ�ӽ�����
 * @param  s1: Ȩֵ��С�Ľ�����
 * @param  s2: Ȩֵ��С�Ľ�����
 * @return ����ִ��״̬
 */
Status Select(HuffmanTree HT, int n, int &s1, int &s2)
{
	HTNode n1, n2;
	n1.node_level = 0;
	n2.node_level = 0;
	n1.weight = INF;
	n2.weight = INF;
	s1 = s2 = 0;

	// ѡ��parentΪ0��Ȩֵ��С���������
	for (int i = 1; i < n + 1; i++)
	{
		if (HT[i].parent == 0)
		{
			if (HT[i].weight <= n1.weight)
			{
				// ���ȱȽ�Ȩֵ
				if (HT[i].weight < n1.weight)
				{
					n2.weight = n1.weight;
					n2.node_level = n1.node_level;
					s2 = s1;
					n1.weight = HT[i].weight;
					n1.node_level = HT[i].node_level;
					s1 = i;
				}
				// ���Ȩֵһ��С������Ҫ�Ƚ���ȣ�Ϊ��ʹ���뷽�����С��Ӧ����ѡ����Ƚ�ǳ��ȥ�ϲ�
				else
				{
					if (HT[i].node_level >= n1.node_level)
						continue;
					else if (HT[i].node_level < n1.node_level)
					{
						n2.weight = n1.weight;
						n2.node_level = n1.node_level;
						s2 = s1;
						n1.weight = HT[i].weight;
						n1.node_level = HT[i].node_level;
						s1 = i;
					}
				}
			}
			else if (HT[i].weight <= n2.weight)
			{
				// ���ȱȽ�Ȩֵ
				if (HT[i].weight < n2.weight)
				{
					n2.weight = HT[i].weight;
					n2.node_level = HT[i].node_level;
					s2 = i;
				}
				// ���Ȩֵһ��С������Ҫ�Ƚ���ȣ�Ϊ��ʹ���뷽�����С��Ӧѡ����Ƚ�ǳ�ĵ�ȥ�ϲ�
				else
				{
					if (HT[i].node_level >= n2.node_level)
						continue;
					else if (HT[i].node_level < n2.node_level)
					{
						n2.weight = HT[i].weight;
						n2.node_level = HT[i].node_level;
						s2 = i;
					}
				}
			}
		}
	}
	return OK;
}

/**
 * @brief  	���������룬������ȵ����⼴���뷽��Ҫ������С��ʹ�ù�����������Ⱦ�����С
			���ڽ����뷽��뷽��ԽС����������ĳ���Խ�������һ������£������ºϲ����֧·�ŵ��ȸ���/��Ȩֵ������֧·���������������볤����ұ��������ӽ��ڵȳ���
			����أ�Ҳ����Ҫ�ڶ��Ȩֵ��ȵĽ���У������ܲ�Ҫѡȡ�Ѿ��ϲ��˵Ľ�㣬��Ҫͨ���Ż�Select����ʵ��
 * @param  HT: ��������
 * @param  HC: ��������
 * @param  heap: ��������Ҫʹ�õ��Ķѣ�������������¼���������
 * @param  leaf_node_num: Ҷ�ӽ����Ŀ
 * @param  node_num: �������
 * @param  tree_depth: �������
 * @return ����ִ��״̬
 */
Status HuffmanCoding(HuffmanTree &HT, HuffmanCode &HC, HeapNode *heap, int leaf_node_num, int &node_num, unsigned int &tree_depth)
{
	if (leaf_node_num <= 1)
		return ERROR;

	// ���ڹ�������û�ж�Ϊ1�Ľ�㣬��һ����n��Ҷ�ӽ��Ĺ�����������2n-1�����
	node_num = 2 * leaf_node_num - 1;
	// ��̬��������洢�շ������������һ����ŵ�Ԫ����
	HT = (HuffmanTree)malloc((node_num + 1) * sizeof(HTNode));
	HuffmanTree p = HT + 1;

	heap++;
	// �ȸ�ÿ��Ҷ�ӽ��assignȨֵ�����Һ��ӡ�˫�׾���ֵΪ0����ȸ�ֵΪ1
	for (int i = 1; i <= leaf_node_num; i++, p++, heap++)
		*p = { heap->area, 0, 0, 0, 0, 0, 1, 1, 0, 0, {0, 0}, NONE };
	// Ȼ���ٳ�ʼ��������
	for (int i = leaf_node_num + 1; i <= node_num; i++, p++)
		*p = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0, 0}, NONE };

	// ���������Ĺ���
	for (int i = leaf_node_num + 1; i <= node_num; i++)
	{
		int s1, s2;
		// ÿ�ζ�����Ҷ�ӽ����ѡȡȨֵ��С��������㣬��������˫�׵�ֵΪ0
		Select(HT,i - 1, s1, s2);
		HT[i].subtree_depth = HT[s1].subtree_depth > HT[s2].subtree_depth ? HT[s1].subtree_depth + 1 : HT[s2].subtree_depth + 1;
		HT[s1].parent = i;
		HT[s2].parent = i;
		if (HT[s1].subtree_depth <= HT[s2].subtree_depth)
		{
			HT[i].lchild = s1;
			HT[i].rchild = s2;
			HT[s1].rsibling = s2;
			HT[s2].lsibling = s1;
		}
		else if (HT[s1].subtree_depth > HT[s2].subtree_depth)
		{
			HT[i].lchild = s2;
			HT[i].rchild = s1;
			HT[s2].rsibling = s1;
			HT[s1].lsibling = s2;
		}
		HT[i].weight = HT[s1].weight + HT[s2].weight;
		HT[i].node_level++;


		// ������Ҫ��s1��s2��������ӣ�����Ҫ��s1��s2������������н����ȶ����ӣ���������˺���ǵݹ����
		DepthAugment(HT, s1);
		DepthAugment(HT, s2);
	}
	// ͳ�����Ź������������
	for (int i = 1; i <= node_num; i++)
	{
		if (HT[i].node_level > tree_depth)
			tree_depth = HT[i].node_level;
	}

	//// TEST
	//for (int i = 1; i <= node_num; i++)
	//{
	//	cout << i << ": " << endl;
	//	cout << "weight: " << HT[i].weight << endl;
	//	cout << "lchild: " << HT[i].lchild << endl;
	//	cout << "rchild: " << HT[i].rchild << endl;
	//	cout << "parent: " << HT[i].parent << endl;
	//	cout << "node_level: " << HT[i].node_level << endl;
	//	cout << "lsibling: " << HT[i].lsibling << endl;
	//	cout << "rsibling: " << HT[i].rsibling << endl;
	//	cout << "prelim: " << HT[i].prelim << endl;
	//	cout << "modifier: " << HT[i].modifier << endl;
	//}

	// ��Ҷ�ӵ���������ÿ���ַ��Ĺ���������
	if ((HC = (HuffmanCode)malloc((leaf_node_num + 1) * sizeof(char *))) == NULL)
		return ERROR;
	char *encoding_workspace = (char *)malloc(leaf_node_num * sizeof(char));
	encoding_workspace[leaf_node_num - 1] = '\0';
	for (int i = 1; i <= leaf_node_num; i++)
	{
		int start = leaf_node_num - 1;
		for (int c = i, f = HT[i].parent; f != 0; c = f, f = HT[f].parent)
		{
			if (HT[f].lchild == c)
				encoding_workspace[--start] = '0';
			else
				encoding_workspace[--start] = '1';

		}
		HC[i] = (char *)malloc((leaf_node_num - start) * sizeof(char));
		strcpy(HC[i], &encoding_workspace[start]);
	}
	free(encoding_workspace);
	encoding_workspace = NULL;

	cout << "���������룺" << endl;
	for (int i = 1; i <= leaf_node_num; i++)
		cout << "Area " << i << "(" << HT[i].weight << ")" << ": " << HC[i] << endl;
	return OK;
}


/**
 * @brief  ����Ĺ��̾��ǴӸ������������ַ���0����1��ȷ�������ӻ����Һ��ӣ�ֱ��Ҷ�ӽ�㣬������˸ù��������Ӧ���ַ�
 * @param  HT: ��������
 * @param  HC: ��������
 * @param  count: Ҷ�ӽ����
 * @return ����ִ��״̬
 */
Status HuffmanDecoding(HuffmanTree HT, HuffmanCode HC, int count)
{
	cout << endl;
	cout << "���������룺" << endl;
	for (int i = 1; i <= count; i++)
	{
		char *temp = HC[i];
		int c = 2 * count - 1;
		while (*temp != '\0')
		{
			if (*temp == '0')
				c = HT[c].lchild;
			else
				c = HT[c].rchild;
			temp++;
		}
		cout << HC[i] << ": " << "Area "<< i << "(" << HT[i].weight << ")"<< "->" << c << endl;
	}
	return OK;
}

/**
 * @brief  ������Ҫ��s1��s2��������ӣ�����Ҫ��s1��s2������������н����ȶ����ӣ���������˺���ǵݹ����
 * @param  HT: ��������
 * @param  start: ���±�start�ĸ��ڵ��������ʼ�������
 * @return ����ִ��״̬
 */
Status DepthAugment(HuffmanTree &HT, int start)
{
	// ����һ����ջ
	TraverseStack *s;
	InitTraverseStack(s);
	int pre = 0;
	int cur = 0;
	int node_pop = 0;

	if (HT == NULL)
		return ERROR;
	// �Ƚ����������ĸ������ջ
	TraverseStackPush(s, start);
	// ֱ��ջΪ��ʱ������ѭ��
	while (!IsTraverseStackEmpty(s))
	{
		// ��ǰ�������Ϊջ�����
		cur = s->data;
		if ((HT[cur].lchild == 0 && HT[cur].rchild == 0) || (pre != 0 && (HT[cur].lchild == pre || HT[cur].rchild == pre)))
		{
			HT[cur].node_level++;
			TraverseStackPop(s, node_pop);
			pre = cur;
		}
		else
		{
			if (HT[cur].rchild != 0)
				TraverseStackPush(s, HT[cur].rchild);
			if (HT[cur].lchild != 0)
				TraverseStackPush(s, HT[cur].lchild);
		}
	}
	return OK;
}

/**
 * @brief  ����������
 * @param  HT: ��������
 * @param  count: Ҷ�ӽ����Ŀ
 * @param  node_num: �������
 * @param  tree_depth: �������
 * @return ����ִ��״̬
 */
Status DrawHuffmanTree(HuffmanTree &HT, int count, int node_num, int tree_depth)
{
	// �Ⱥ�����������������ɳ���ȷ��ÿ�����ĺ����꼴prelim
	// Ȼ��ͨ��modifierȥ���������໥����ĵط�
	TraverseStack *s;
	InitTraverseStack(s);
	int pre = 0;			// ָʾǰһ�����ʹ��Ľ��
	int cur = 0;			// ��ǰָ��Ľ��
	int node_pop = 0;		// ���ڽ��ճ�ջ�Ľ�㣬����Ҫ�������κβ���

	if (HT == NULL)
		return ERROR;
	// �Ƚ����������ĸ������ջ
	TraverseStackPush(s, node_num);
	// ֱ��ջΪ��ʱ������ѭ��
	while (!IsTraverseStackEmpty(s))
	{
		// ��ǰ�������Ϊջ�����
		cur = s->data;
		// ����ж����������ں�������Ĳ��֣�����������Է��ʸý��
		if ((HT[cur].lchild == 0 && HT[cur].rchild == 0) || (pre != 0 && (HT[cur].lchild == pre || HT[cur].rchild == pre)))
		{
			// �����Ǽ���prelim��modifier���㷨����
			// �����ǰ����Ǹ��ڵ������
			if (cur == HT[node_num].lchild)
			{
				if (HT[cur].lchild == 0 && HT[cur].rchild == 0)
				{
					HT[cur].prelim = 0;
					HT[cur].modifier = 0;
				}
				else
				{
					HT[cur].prelim = (HT[HT[cur].lchild].prelim + HT[HT[cur].rchild].prelim) / 2;
					HT[cur].modifier = 0;
				}	
			}
			// �����ǰ����Ǹ��ڵ���Һ���
			else if (cur == HT[node_num].rchild)
			{
				if (HT[cur].lchild == 0 && HT[cur].rchild == 0)
				{
					HT[cur].prelim = 0;
					HT[cur].modifier = 0;
				}
				else
				{
					HT[cur].prelim = (HT[HT[cur].lchild].prelim + HT[HT[cur].rchild].prelim) / 2;
					HT[cur].modifier = 0;
				}
			}
			// ��ǰ���Ȳ��Ǹ��ڵ������Ҳ�����Һ���
			else
			{
				// ���û�����ֵܣ����ý������˫�׵����ӣ�������û�����Һ���
				if (HT[cur].lsibling == 0 && (HT[cur].lchild == 0 && HT[cur].rchild == 0))
				{
					HT[cur].prelim = 0;
					HT[cur].modifier = 0;
				}
				// ���û�����ֵܣ����ý������˫�׵����ӣ������������Һ���
				else if (HT[cur].lsibling == 0 && (HT[cur].lchild != 0 && HT[cur].rchild != 0))
				{
					HT[cur].prelim = (HT[HT[cur].lchild].prelim + HT[HT[cur].rchild].prelim) / 2;
					HT[cur].modifier = 0;
				}
				// ��������ֵܣ����ý������˫�׵��Һ��ӣ�������û�����Һ���
				else if (HT[cur].lsibling != 0 && (HT[cur].lchild == 0 && HT[cur].rchild == 0))
				{
					HT[cur].prelim = (HT[HT[cur].lsibling].prelim + 2 * NODE_RADIUS + SIBLING_SEPARATION) * 1 / HT[cur].node_level;
					HT[cur].modifier = 0;
				}
				// ��������ֵܣ����ý������˫�׵��Һ��ӣ���Ҳ�����Һ���
				else if (HT[cur].lsibling != 0 && (HT[cur].lchild != 0 && HT[cur].rchild != 0))
				{
					HT[cur].prelim = (HT[HT[cur].lsibling].prelim + 2 * NODE_RADIUS + SIBLING_SEPARATION) * 1 / HT[cur].node_level;
					HT[cur].modifier = HT[cur].prelim - (HT[HT[cur].lchild].prelim + HT[HT[cur].rchild].prelim) / 2;
				}
			}
			// �ý����ʹ��ˣ���ջ
			TraverseStackPop(s, node_pop);
			pre = cur;
		}
		// ����������Ϸ�if�������Һ��ӡ�����������ջ
		else
		{
			if (HT[cur].rchild != 0)
				TraverseStackPush(s, HT[cur].rchild);
			if (HT[cur].lchild != 0)
				TraverseStackPush(s, HT[cur].lchild);
		}
	}

	// ����ĺ������ֻ������˳�����λ��ȷ��������Ҫ��modifier������������������λ��
	for (int i = 1; i < node_num; i++)
	{
		int temp = i;
		int parent_modifier = 0;
		// ��Զ�ǰѵ�ǰ����ancestors��modifier�����������˸��ڵ㣩��Ȼ����µ���ǰ����modifier��
		while (HT[temp].parent != node_num)
		{
			parent_modifier += HT[HT[temp].parent].modifier;
			temp = HT[temp].parent;
		}
		HT[i].pt.x = HT[i].prelim + parent_modifier + 2 * NODE_RADIUS;
		// �������ǵȲ�����
		HT[i].pt.y = 2 * NODE_RADIUS + ((IMG_HEIGHT - 2 * (2 * NODE_RADIUS)) / tree_depth) / 2 * (2 * HT[i].node_level - 1);
	}
	
	// ʹ�������㷨��Ȼ�޷���ȫ��������������������
	// ��˻���Ҫ���һ������������֮��ļ��
	// ��Ȼ��Ҫ���к������

	pre = 0;
	cur = 0;
	node_pop = 0;

	// �Ƚ����������ĸ������ջ
	TraverseStackPush(s, node_num);
	// ֱ��ջΪ��ʱ������ѭ��
	while (!IsTraverseStackEmpty(s))
	{
		// ��ǰ�������Ϊջ�����
		cur = s->data;
		if ((HT[cur].lchild == 0 && HT[cur].rchild == 0) || (pre != 0 && (HT[cur].lchild == pre || HT[cur].rchild == pre)))
		{
			// �����������Ѳ���
			SplitTree(HT, count, node_num, cur, tree_depth);
			TraverseStackPop(s, node_pop);
			pre = cur;
		}
		else
		{
			if (HT[cur].rchild != 0)
				TraverseStackPush(s, HT[cur].rchild);
			if (HT[cur].lchild != 0)
				TraverseStackPush(s, HT[cur].lchild);
		}
	}
	// �����������̹�������ÿ�����λ����ȷ��
	// ������Ҫ����������������Ļ�Ϻ��ʵ�λ��
	// ��ͳ�ƹ��������������������Һ����꣬��ȷ����Ҫ�೤����Ļ
	int min_x = INF, max_x = -INF;
	for (int i = 1; i <= node_num; i++)
	{
		if (HT[i].pt.x < min_x)
			min_x = HT[i].pt.x;
		else if (HT[i].pt.x > max_x)
			max_x = HT[i].pt.x;
	}
	int huffman_tree_width = max_x - min_x + 2 * NODE_RADIUS;
	int dx = NODE_RADIUS - min_x;
	if (huffman_tree_width < 600)
		huffman_tree_width = 600;
	Mat huffman_tree_image(IMG_HEIGHT, huffman_tree_width, CV_8UC3, Scalar(255, 255, 255));

	// �ƶ����еĽ�㣬ʹ������ߵĽ��λ����Ļ�����
	for (int i = 1; i <= node_num; i++)
		HT[i].pt.x += dx;

	// ����
	for (int i = 1; i <= node_num; i++)
	{
		if (HT[i].parent > 0)
			line(huffman_tree_image, HT[i].pt, HT[HT[i].parent].pt, Scalar(0, 0, 0));
		if (HT[i].lchild > 0)
			line(huffman_tree_image, HT[i].pt, HT[HT[i].lchild].pt, Scalar(0, 0, 0));
		if (HT[i].rchild > 0)
			line(huffman_tree_image, HT[i].pt, HT[HT[i].rchild].pt, Scalar(0, 0, 0));
	}
	
	// ������Լ�����š�Ȩֵ
	for (int i = 1; i <= node_num; i++)
	{
		Point temp = HT[i].pt;
		temp.x -= NODE_RADIUS;
		temp.y += NODE_RADIUS / 2;
		circle(huffman_tree_image, HT[i].pt, NODE_RADIUS, Scalar(255, 255, 255), -1);
		char str[10];
		if (i >= 1 && i <= 99)
			sprintf(str, " %d", i);
		else
			sprintf(str, "%d", i);
		putText(huffman_tree_image, str, temp, FONT_HERSHEY_PLAIN, 0.5, Scalar(255, 0, 0));
		circle(huffman_tree_image, HT[i].pt, NODE_RADIUS, Scalar(0, 0, 0));

		if (i >= 1 && i <= count)
		{
			temp.y += (int)(1.5 * NODE_RADIUS);
			sprintf(str, "%d", HT[i].weight);
			putText(huffman_tree_image, str, temp, FONT_HERSHEY_PLAIN, 0.5, Scalar(0, 0, 255));
		}
	}
	imwrite("huffman.jpg", huffman_tree_image);
	cout << "\n��������ͼƬ�ѱ����ڹ���Ŀ¼��..." << endl;
	//moveWindow("������������", 200, 100);
	return OK;
}

/**
 * @brief  �������֮���Ƿ񽻲棬�����潫���߻����ƿ�
 * @param  HT: ��������
 * @param  count: Ҷ�ӽ����Ŀ
 * @param  node_num: �������
 * @param  start: ���±�start�ĸ��ڵ����������Ƿ񽻲�
 * @param  tree_depth: �������
 * @return ����ִ��״̬
 */
Status SplitTree(HuffmanTree &HT, int count, int node_num, int start, int tree_depth)
{
	for (int i = 1; i <= count; i++)
		if (start == i)
			return OK;
	for (int i = 1; i < node_num; i++)
	{
		int temp = i;
		while (HT[temp].parent != start)
		{
			temp = HT[temp].parent;
			if (temp == 0)
			{
				break;
			}
		}
			
		if (temp == HT[start].lchild)
			HT[i].left_or_right = LEFT;
		else if (temp == HT[start].rchild)
			HT[i].left_or_right = RIGHT;
	}

	int max_modifier = 0;

	for (int depth = HT[start].node_level; depth <= tree_depth; depth++)
	{
		int max_left_x = 0, min_right_x = INF;
		for (int i = 1; i < node_num; i++)
		{
			if (HT[i].node_level == depth)
			{
				if (HT[i].left_or_right == LEFT)
				{
					if (HT[i].pt.x > max_left_x)
						max_left_x = HT[i].pt.x;
				}
				else if (HT[i].left_or_right == RIGHT)
				{
					if (HT[i].pt.x < min_right_x)
						min_right_x = HT[i].pt.x;
				}
			}
		}
		if (max_left_x - max_modifier / 2 + NODE_RADIUS >= min_right_x + max_modifier / 2 - NODE_RADIUS)
			max_modifier += (max_left_x - max_modifier / 2 + NODE_RADIUS) - (min_right_x + max_modifier / 2 - NODE_RADIUS) + 2 * NODE_RADIUS;
	}

	if (max_modifier != 0)
	{
		for (int i = 1; i < node_num; i++)
		{
			if (HT[i].left_or_right == LEFT)
				HT[i].pt.x -= max_modifier / 2;
			else if (HT[i].left_or_right == RIGHT)
				HT[i].pt.x += max_modifier / 2;
		}
	}

	HT[start].pt.x = (HT[HT[start].lchild].pt.x + HT[HT[start].rchild].pt.x) / 2;
	HT[start].pt.y = 2 * NODE_RADIUS + ((IMG_HEIGHT - 2 * (2 * NODE_RADIUS)) / tree_depth) / 2 * (2 * HT[start].node_level - 1);

	for (int i = 1; i < node_num; i++)
		HT[i].left_or_right = NONE;
	return OK;
}

/**
 * @brief  ���������㷨ʵ��
 * @param  maskImage: �ṩ��contour��Ϣ��ͼ�����
 * @param  count: Ҷ�ӽ����Ŀ
 * @param  heap: ��������Ҫʹ�õ��Ķѣ�������������¼���������
 * @return ����ִ��״̬
 */
Status Huffman(Mat maskImage, int count, HeapNode *heap)
{
	double dTime = (double)getTickCount();
	HuffmanTree HT;
	HuffmanCode HC;
	unsigned int tree_depth = 0;
	int node_num = 0;
	cout << "��ʵ����������������" << endl;
	if (!HuffmanCoding(HT, HC, heap, count, node_num, tree_depth))
		return ERROR;
	HuffmanDecoding(HT, HC, count);
	DrawHuffmanTree(HT, count, node_num, tree_depth);

	dTime = (double)getTickCount() - dTime;
	printf("������ʱ = %gs\n", dTime / getTickFrequency());
	cout << "�������������һʵ��..." << endl;
	waitKey(0);
	return OK;
}