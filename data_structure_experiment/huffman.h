/**
  ******************************************************************************
  * @file    huffman.h
  * @author  李星毅
  * @date    2018-10-04
  * @brief   该文件包含了哈夫曼树的所有实现函数原型
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

#pragma once
#define MAX_AREA_NUM MAX_VERTEX_NUM
#define SIBLING_SEPARATION 10
#define NODE_RADIUS 8
#define IMG_HEIGHT 600
#define LEFT 1
#define RIGHT 2


using namespace cv;
using namespace std;

typedef int Status;

typedef struct
{
	unsigned int weight;
	unsigned int parent, lchild, rchild, lsibling, rsibling;
	unsigned int node_level, subtree_depth;
	int prelim, modifier;
	Point pt;
	char left_or_right;
} HTNode, *HuffmanTree;

// 链栈结构体
typedef struct TraverseStack
{
	int data;
	struct TraverseStack *next;
} TraverseStack;

typedef char** HuffmanCode;

Status InitTraverseStack(TraverseStack *(&s));

Status IsTraverseStackEmpty(TraverseStack *s);

Status TraverseStackPush(TraverseStack *(&s), int e);

Status TraverseStackPop(TraverseStack *(&s), int &e);

Status Select(HuffmanTree HT, int n, int &s1, int &s2);

Status HuffmanCoding(HuffmanTree &HT, HuffmanCode &HC, HeapNode *heap, int leaf_node_num, int &node_num, unsigned int &tree_depth);

Status HuffmanDecoding(HuffmanTree HT, HuffmanCode HC, int count);

Status DrawHuffmanTree(HuffmanTree &HT, int count, int node_num, int tree_depth);

Status DepthAugment(HuffmanTree &HT, int start);

Status SplitTree(HuffmanTree &HT, int count, int node_num, int start, int tree_depth);

Status Huffman(Mat maskImage, int count, HeapNode *heap);

