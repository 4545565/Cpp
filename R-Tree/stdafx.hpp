#pragma once

#include "targetver.hpp"

#include<stdio.h>
#include<tchar.h>
#include<math.h>
#include<iostream>
#include<stdlib.h>
#include<cmath>

#define Invalid_Rect(x) ((x).bound[0]>(x).bound[2])	//判定左右边界是否相反
#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))
#define Maxfill 3			//叶子节点包含对象的最大值,及R树分值的最大值
#define Minfill (Maxfill/2)	//同上,最小值
#define FALSE 0
#define TRUE 1
using namespace std;