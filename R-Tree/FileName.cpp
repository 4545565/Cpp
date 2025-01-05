#include"stdafx.hpp"

struct RTreeMBR;
struct RTreeBranch;
struct RTreeNode;

struct RTreeNode* RTreeCreate();
double Distance(RTreeNode* node1, RTreeNode* node2);
void LeafInsert(RTreeNode* father, RTreeNode* son, RTreeNode* leaf);
void LeafCreate(RTreeNode* root, float x, float y, int ID);
void RTreeInitMBR(RTreeMBR* rc);
struct RTreeMBR RTreeCombineMBR(RTreeMBR rc1, RTreeMBR rc2);
void RTreeInitBranch(RTreeBranch& br);
void RTreeInitNode(RTreeNode* node);
void RTreeNodeSplit(RTreeNode* father, RTreeNode* son, int i);

RTreeNode* root = RTreeCreate();

struct RTreeMBR		//定义矩形框的四个界值
{
	float bound[4];	//(bound[0],bound[1])为矩形框左下角坐标,(bound[2],bound[3])为矩形框右上角坐标
};

struct RTreeBranch		//定义R树节点分支的结构体
{
	RTreeMBR mbr;		//分支所在的矩形框
	RTreeNode* child;	//分支指向的孩子节点
	int leafchild;		//若为叶子节点,存储对象的ID
};

struct RTreeNode						//定义R树节点的结构体
{
	RTreeMBR mbr;						//矩形框边界
	int mbrcount;						//节点矩形框的个数
	int level;							//节点的高度
	RTreeBranch branch[Maxfill + 1];	//节点的分支,为节点分裂冗余一位
	int ID;
};

struct RTreeNode* RTreeCreate()									//创建R树,初始化根
{
	RTreeNode* root = (RTreeNode*)malloc(sizeof(RTreeNode));	//构建节点
	RTreeInitNode(root);										//调用节点初始化函数
	return root;
}

double Distance(RTreeNode* node1, RTreeNode* node2)				//求两个矩形框质心距离
{
	double x1 = (node1->mbr.bound[0] + node1->mbr.bound[2]) / 2;
	double y1 = (node1->mbr.bound[1] + node1->mbr.bound[3]) / 2;
	double x2 = (node2->mbr.bound[0] + node2->mbr.bound[2]) / 2;
	double y2 = (node2->mbr.bound[1] + node2->mbr.bound[3]) / 2;
	double distance = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
	return distance;
}

void LeafInsert(RTreeNode* father, RTreeNode* son, RTreeNode* leaf)		//将已生成的数据插入树
{
	int min = 0;
	if (son->level == 1)		//如叶子节点则插入
	{
		son->branch[son->mbrcount].child = leaf;
		son->branch[son->mbrcount].leafchild = leaf->ID;
		son->branch[son->mbrcount].mbr = son->branch[son->mbrcount].child->mbr;
		son->mbrcount++;
		son->mbr = RTreeCombineMBR(son->mbr, son->branch[son->mbrcount].mbr);
		min = son->mbrcount - 1;
	}
	else		//如非叶子节点则寻找质心距离最近的子节点
	{

		double mindistance = Distance(son->branch[0].child, leaf);
		double distance;
		for (int i = 1; i < son->mbrcount; ++i) {
			distance = Distance(son->branch[i].child, leaf);
			if (distance < mindistance) {
				min = i;
				mindistance = distance;
			}
		}
		LeafInsert(son, son->branch[min].child, leaf);			//对质心距离最近的字节点执行递归
		son->branch[min].mbr = son->branch[min].child->mbr;
		son->mbr = RTreeCombineMBR(son->mbr, son->branch[min].mbr);
	}
	if (son->mbrcount > Maxfill)		//检测节点是否溢出,如溢出,执行节点分裂
	{
		RTreeNodeSplit(father, son, min);
	}
}

void LeafCreate(RTreeNode* root, float x, float y, int ID) {
	RTreeNode* leaf = (RTreeNode*)malloc(sizeof(RTreeNode));
	RTreeInitNode(leaf);
	leaf->level = 0;
	leaf->mbr.bound[0] = leaf->mbr.bound[2] = x;	//数据节点只占用一个点坐标
	leaf->mbr.bound[1] = leaf->mbr.bound[3] = y;
	leaf->ID = ID;
	LeafInsert(NULL, root, leaf);
}

void RTreeInitMBR(RTreeMBR* rc)	//初始化矩形框
{
	int i;
	for (i = 0; i < 4; ++i)
		rc->bound[i] = -1;
}

/*
double RTreeRectSurfaceArea(RTreeMBR rc)	//求矩形框面积
{
	double ans = abs((rc.bound[0] - rc.bound[2]) * (rc.bound[1] - rc.bound[3]));
	return ans;
}
*/

struct RTreeMBR RTreeCombineMBR(RTreeMBR rc1, RTreeMBR rc2)	//合并两个矩形框
{
	RTreeMBR rc;
	if (rc1.bound[0] < 0)
		rc = rc2;
	else		//取两者公共MBR
	{
		rc.bound[0] = MIN(rc1.bound[0], rc2.bound[0]);
		rc.bound[1] = MIN(rc1.bound[1], rc2.bound[1]);
		rc.bound[2] = MAX(rc1.bound[2], rc2.bound[2]);
		rc.bound[3] = MAX(rc1.bound[3], rc2.bound[3]);
	}
	return rc;
}

void RTreeInitBranch(RTreeBranch& br)	//初始化一个分支的矩形框和孩子指针
{
	RTreeInitMBR(&br.mbr);
	br.child = NULL;			//新建节点暂无孩子
	br.leafchild = -1;		//-1表示非叶子节点
}

void RTreeInitNode(RTreeNode* node)		//初始化一个R树节点的分支个数和层数以及分支
{
	node->mbrcount = 0;
	node->level = 1;
	node->ID = -1;
	RTreeInitMBR(&node->mbr);
	int i;
	for (i = 0; i <= Maxfill; ++i)
		RTreeInitBranch(node->branch[i]);
}

void RTreeNodeSplit(RTreeNode* father, RTreeNode* son, int x)		//节点分裂
{
	int i = (Maxfill + 1) / 2;
	int j;
	RTreeNode* son1 = (RTreeNode*)malloc(sizeof(RTreeNode));		//son1,son2为由son分裂得来的两个节点
	RTreeNode* son2 = (RTreeNode*)malloc(sizeof(RTreeNode));
	RTreeInitNode(son1);
	RTreeInitNode(son2);
	for (j = 0; j < i; ++j)		//son1继承son的前半部分属性
	{
		son1->branch[j] = son->branch[j];
		son1->mbr = RTreeCombineMBR(son1->mbr, son->branch[j].mbr);
		son1->mbrcount++;
	}
	for (j = i; j < Maxfill + 1; ++j)									//son2继承son的后半部分属性
	{
		son2->branch[j - i] = son->branch[j];
		son2->mbr = RTreeCombineMBR(son2->mbr, son->branch[j].mbr);
		son2->mbrcount++;
	}
	son1->level = son2->level = son->level;
	if (father != NULL)			//当son存在父节点(即son非根节点)时,son1继承son的位置,son2接在其父节点的第一个空位上
	{
		son = son1;
		father->branch[x].mbr = son1->mbr;
		father->branch[father->mbrcount].child = son2;
		father->branch[father->mbrcount].mbr = son2->mbr;
		father->mbr = RTreeCombineMBR(father->mbr, father->branch[father->mbrcount].mbr);
		father->mbrcount++;
	}
	else			//反之,创建新的根节点作为son1和son2的父节点,同时层数+1
	{
		father = RTreeCreate();
		father->branch[0].child = son1;
		father->branch[1].child = son2;
		father->branch[0].mbr = son1->mbr;
		father->branch[1].mbr = son2->mbr;
		father->mbr = RTreeCombineMBR(father->branch[0].mbr, father->branch[1].mbr);
		father->mbrcount = 2;
		father->level = son->level + 1;
		root = father;
	}
}

int main() {
	int i = 1;
	int ID, x, y;
	while (i) {
	ERROR_CHOICE:
		printf("请选择:\t0退出\t1插入\n");
		scanf_s("%d", &i);
		if (i < 0 || i>1) {
			printf("选项有误,请重新选择\n");
			goto ERROR_CHOICE;
		}
		if (i) {
		ID_ERROR:
			printf("请输入ID:");
			scanf_s("%d", &ID);
			if (ID < 0) {
				printf("ID输入有误,请重新输入\n");
				goto ID_ERROR;

			}
		COORDINATE_ERROR:
			printf("请输入坐标(x,y):");
			scanf_s("%d,%d", &x, &y);
			if (x < 0 || y < 0) {
				printf("坐标输入有误,请重新输入\n");
				goto COORDINATE_ERROR;
			}
			LeafCreate(root, x, y, ID);
		}
	}
	return 0;
}