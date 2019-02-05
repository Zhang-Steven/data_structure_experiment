/**
  ******************************************************************************
  * @file    huffman.cpp
  * @author  李星毅
  * @date    2018-10-04
  * @brief   该文件包含了哈夫曼树的所有实现函数
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
 * @brief  初始化遍历辅助栈
 * @param  s: 遍历辅助栈
 * @return 函数执行状态
 */
Status InitTraverseStack(TraverseStack *(&s))
{
	if ((s = (TraverseStack *)malloc(sizeof(TraverseStack))) == NULL)
		return ERROR;
	s->next = NULL;
	return OK;
}

/**
 * @brief  判断遍历辅助栈是否为空
 * @param  s: 遍历辅助栈
 * @return 函数执行状态
 */
Status IsTraverseStackEmpty(TraverseStack *s)
{
	if (s->next == NULL)
		return TRUE;
	else
		return FALSE;
}

/**
 * @brief  入栈
 * @param  s: 遍历辅助栈
 * @param  e: 入栈元素
 * @return 函数执行状态
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
 * @brief  出栈
 * @param  s: 遍历辅助栈
 * @param  e: 出栈元素
 * @return 函数执行状态
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
 * @brief  每次都从在叶子结点中选取权值最小的两个结点，并且满足双亲的值为0
 * @param  HT: 哈夫曼树
 * @param  n: 叶子结点个数
 * @param  s1: 权值最小的结点序号
 * @param  s2: 权值次小的结点序号
 * @return 函数执行状态
 */
Status Select(HuffmanTree HT, int n, int &s1, int &s2)
{
	HTNode n1, n2;
	n1.node_level = 0;
	n2.node_level = 0;
	n1.weight = INF;
	n2.weight = INF;
	s1 = s2 = 0;

	// 选择parent为0且权值最小的两个结点
	for (int i = 1; i < n + 1; i++)
	{
		if (HT[i].parent == 0)
		{
			if (HT[i].weight <= n1.weight)
			{
				// 优先比较权值
				if (HT[i].weight < n1.weight)
				{
					n2.weight = n1.weight;
					n2.node_level = n1.node_level;
					s2 = s1;
					n1.weight = HT[i].weight;
					n1.node_level = HT[i].node_level;
					s1 = i;
				}
				// 如果权值一样小，则需要比较深度，为了使得码方差尽可能小，应优先选出深度较浅的去合并
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
				// 优先比较权值
				if (HT[i].weight < n2.weight)
				{
					n2.weight = HT[i].weight;
					n2.node_level = HT[i].node_level;
					s2 = i;
				}
				// 如果权值一样小，则需要比较深度，为了使得码方差尽可能小，应选出深度较浅的的去合并
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
 * @brief  	哈夫曼编码，考虑深度的问题即是码方差要尽可能小才使得哈夫曼树的深度尽可能小
			对于降低码方差（码方差越小，哈夫曼码的长度越相近），一般情况下，若将新合并后的支路排到等概率/等权值的最上支路，将有利于缩短码长方差，且编出的码更接近于等长码
			具体地，也就是要在多个权值相等的结点中，尽可能不要选取已经合并了的结点，主要通过优化Select函数实现
 * @param  HT: 哈夫曼树
 * @param  HC: 哈夫曼码
 * @param  heap: 堆排序需要使用到的堆，这里是用来记录区域面积的
 * @param  leaf_node_num: 叶子结点数目
 * @param  node_num: 结点总数
 * @param  tree_depth: 树的深度
 * @return 函数执行状态
 */
Status HuffmanCoding(HuffmanTree &HT, HuffmanCode &HC, HeapNode *heap, int leaf_node_num, int &node_num, unsigned int &tree_depth)
{
	if (leaf_node_num <= 1)
		return ERROR;

	// 由于哈夫曼数没有度为1的结点，故一棵有n个叶子结点的哈夫曼树共有2n-1个结点
	node_num = 2 * leaf_node_num - 1;
	// 动态分配数组存储赫夫曼树，多分配一个零号单元不用
	HT = (HuffmanTree)malloc((node_num + 1) * sizeof(HTNode));
	HuffmanTree p = HT + 1;

	heap++;
	// 先给每个叶子结点assign权值，左右孩子、双亲均赋值为0，深度赋值为1
	for (int i = 1; i <= leaf_node_num; i++, p++, heap++)
		*p = { heap->area, 0, 0, 0, 0, 0, 1, 1, 0, 0, {0, 0}, NONE };
	// 然后再初始化其余结点
	for (int i = leaf_node_num + 1; i <= node_num; i++, p++)
		*p = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0, 0}, NONE };

	// 哈夫曼树的构建
	for (int i = leaf_node_num + 1; i <= node_num; i++)
	{
		int s1, s2;
		// 每次都从在叶子结点中选取权值最小的两个结点，并且满足双亲的值为0
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


		// 除了需要将s1，s2的深度增加，还需要将s1，s2的子树里的所有结点深度都增加，这里采用了后序非递归遍历
		DepthAugment(HT, s1);
		DepthAugment(HT, s2);
	}
	// 统计整颗哈夫曼树的深度
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

	// 从叶子到根逆向求每个字符的哈夫曼编码
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

	cout << "哈夫曼编码：" << endl;
	for (int i = 1; i <= leaf_node_num; i++)
		cout << "Area " << i << "(" << HT[i].weight << ")" << ": " << HC[i] << endl;
	return OK;
}


/**
 * @brief  解码的过程就是从根结点出发，按字符‘0’或‘1’确定找左孩子或是右孩子，直到叶子结点，就求得了该哈夫曼码对应的字符
 * @param  HT: 哈夫曼树
 * @param  HC: 哈夫曼码
 * @param  count: 叶子结点数
 * @return 函数执行状态
 */
Status HuffmanDecoding(HuffmanTree HT, HuffmanCode HC, int count)
{
	cout << endl;
	cout << "哈夫曼解码：" << endl;
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
 * @brief  除了需要将s1，s2的深度增加，还需要将s1，s2的子树里的所有结点深度都增加，这里采用了后序非递归遍历
 * @param  HT: 哈夫曼树
 * @param  start: 以下标start的根节点的子树开始增加深度
 * @return 函数执行状态
 */
Status DepthAugment(HuffmanTree &HT, int start)
{
	// 创建一个空栈
	TraverseStack *s;
	InitTraverseStack(s);
	int pre = 0;
	int cur = 0;
	int node_pop = 0;

	if (HT == NULL)
		return ERROR;
	// 先将哈夫曼树的根结点入栈
	TraverseStackPush(s, start);
	// 直到栈为空时，结束循环
	while (!IsTraverseStackEmpty(s))
	{
		// 当前结点设置为栈顶结点
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
 * @brief  画哈夫曼树
 * @param  HT: 哈夫曼树
 * @param  count: 叶子结点数目
 * @param  node_num: 结点总数
 * @param  tree_depth: 树的深度
 * @return 函数执行状态
 */
Status DrawHuffmanTree(HuffmanTree &HT, int count, int node_num, int tree_depth)
{
	// 先后序遍历哈夫曼树，可初步确定每个结点的横坐标即prelim
	// 然后通过modifier去初步调整相互交叉的地方
	TraverseStack *s;
	InitTraverseStack(s);
	int pre = 0;			// 指示前一个访问过的结点
	int cur = 0;			// 当前指向的结点
	int node_pop = 0;		// 用于接收出栈的结点，不需要对其做任何操作

	if (HT == NULL)
		return ERROR;
	// 先将哈夫曼树的根结点入栈
	TraverseStackPush(s, node_num);
	// 直到栈为空时，结束循环
	while (!IsTraverseStackEmpty(s))
	{
		// 当前结点设置为栈顶结点
		cur = s->data;
		// 这个判断条件是属于后序遍历的部分，若满足则可以访问该结点
		if ((HT[cur].lchild == 0 && HT[cur].rchild == 0) || (pre != 0 && (HT[cur].lchild == pre || HT[cur].rchild == pre)))
		{
			// 以下是计算prelim和modifier的算法部分
			// 如果当前结点是根节点的左孩子
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
			// 如果当前结点是根节点的右孩子
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
			// 当前结点既不是根节点的左孩子也不是右孩子
			else
			{
				// 如果没有左兄弟（即该结点是其双亲的左孩子），并且没有左右孩子
				if (HT[cur].lsibling == 0 && (HT[cur].lchild == 0 && HT[cur].rchild == 0))
				{
					HT[cur].prelim = 0;
					HT[cur].modifier = 0;
				}
				// 如果没有左兄弟（即该结点是其双亲的左孩子），但是有左右孩子
				else if (HT[cur].lsibling == 0 && (HT[cur].lchild != 0 && HT[cur].rchild != 0))
				{
					HT[cur].prelim = (HT[HT[cur].lchild].prelim + HT[HT[cur].rchild].prelim) / 2;
					HT[cur].modifier = 0;
				}
				// 如果有左兄弟（即该结点是其双亲的右孩子），并且没有左右孩子
				else if (HT[cur].lsibling != 0 && (HT[cur].lchild == 0 && HT[cur].rchild == 0))
				{
					HT[cur].prelim = (HT[HT[cur].lsibling].prelim + 2 * NODE_RADIUS + SIBLING_SEPARATION) * 1 / HT[cur].node_level;
					HT[cur].modifier = 0;
				}
				// 如果有左兄弟（即该结点是其双亲的右孩子），也有左右孩子
				else if (HT[cur].lsibling != 0 && (HT[cur].lchild != 0 && HT[cur].rchild != 0))
				{
					HT[cur].prelim = (HT[HT[cur].lsibling].prelim + 2 * NODE_RADIUS + SIBLING_SEPARATION) * 1 / HT[cur].node_level;
					HT[cur].modifier = HT[cur].prelim - (HT[HT[cur].lchild].prelim + HT[HT[cur].rchild].prelim) / 2;
				}
			}
			// 该结点访问过了，出栈
			TraverseStackPop(s, node_pop);
			pre = cur;
		}
		// 如果不满足上方if条件则将右孩子、左孩子依次入栈
		else
		{
			if (HT[cur].rchild != 0)
				TraverseStackPush(s, HT[cur].rchild);
			if (HT[cur].lchild != 0)
				TraverseStackPush(s, HT[cur].lchild);
		}
	}

	// 上面的后序遍历只是完成了初步的位置确定，下面要将modifier加入结点的坐标调整结点的位置
	for (int i = 1; i < node_num; i++)
	{
		int temp = i;
		int parent_modifier = 0;
		// 永远是把当前结点的ancestors的modifier加起来（除了根节点），然后更新到当前结点的modifier里
		while (HT[temp].parent != node_num)
		{
			parent_modifier += HT[HT[temp].parent].modifier;
			temp = HT[temp].parent;
		}
		HT[i].pt.x = HT[i].prelim + parent_modifier + 2 * NODE_RADIUS;
		// 纵坐标是等差数列
		HT[i].pt.y = 2 * NODE_RADIUS + ((IMG_HEIGHT - 2 * (2 * NODE_RADIUS)) / tree_depth) / 2 * (2 * HT[i].node_level - 1);
	}
	
	// 使用上述算法仍然无法完全解决哈夫曼树交叉的问题
	// 因此还需要最后一步，调整子树之间的间隔
	// 依然需要进行后序遍历

	pre = 0;
	cur = 0;
	node_pop = 0;

	// 先将哈夫曼树的根结点入栈
	TraverseStackPush(s, node_num);
	// 直到栈为空时，结束循环
	while (!IsTraverseStackEmpty(s))
	{
		// 当前结点设置为栈顶结点
		cur = s->data;
		if ((HT[cur].lchild == 0 && HT[cur].rchild == 0) || (pre != 0 && (HT[cur].lchild == pre || HT[cur].rchild == pre)))
		{
			// 进行子树分裂操作
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
	// 经过上述过程哈夫曼树每个结点位置已确定
	// 现在需要将哈夫曼树画在屏幕上合适的位置
	// 先统计哈夫曼树最左横坐标和最右横坐标，来确定需要多长的屏幕
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

	// 移动所有的结点，使得最左边的结点位于屏幕最左边
	for (int i = 1; i <= node_num; i++)
		HT[i].pt.x += dx;

	// 画线
	for (int i = 1; i <= node_num; i++)
	{
		if (HT[i].parent > 0)
			line(huffman_tree_image, HT[i].pt, HT[HT[i].parent].pt, Scalar(0, 0, 0));
		if (HT[i].lchild > 0)
			line(huffman_tree_image, HT[i].pt, HT[HT[i].lchild].pt, Scalar(0, 0, 0));
		if (HT[i].rchild > 0)
			line(huffman_tree_image, HT[i].pt, HT[HT[i].rchild].pt, Scalar(0, 0, 0));
	}
	
	// 画结点以及标序号、权值
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
	cout << "\n哈夫曼树图片已保存在工程目录下..." << endl;
	//moveWindow("【哈夫曼树】", 200, 100);
	return OK;
}

/**
 * @brief  检查子树之间是否交叉，若交叉将二者互相移开
 * @param  HT: 哈夫曼树
 * @param  count: 叶子结点数目
 * @param  node_num: 结点总数
 * @param  start: 以下标start的根节点的子树检查是否交叉
 * @param  tree_depth: 树的深度
 * @return 函数执行状态
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
 * @brief  哈夫曼树算法实现
 * @param  maskImage: 提供的contour信息的图像矩阵
 * @param  count: 叶子结点数目
 * @param  heap: 堆排序需要使用到的堆，这里是用来记录区域面积的
 * @return 函数执行状态
 */
Status Huffman(Mat maskImage, int count, HeapNode *heap)
{
	double dTime = (double)getTickCount();
	HuffmanTree HT;
	HuffmanCode HC;
	unsigned int tree_depth = 0;
	int node_num = 0;
	cout << "【实验三：哈夫曼树】" << endl;
	if (!HuffmanCoding(HT, HC, heap, count, node_num, tree_depth))
		return ERROR;
	HuffmanDecoding(HT, HC, count);
	DrawHuffmanTree(HT, count, node_num, tree_depth);

	dTime = (double)getTickCount() - dTime;
	printf("程序用时 = %gs\n", dTime / getTickFrequency());
	cout << "按任意键进入下一实验..." << endl;
	waitKey(0);
	return OK;
}