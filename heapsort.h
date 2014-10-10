/*  Author:Tony yin
 *  Date:2014-10-9
 *  Description:堆排序的C语言实现版本，堆排序的数组约定从0开始计算。
 */


#ifndef _HEAPSORT_H
#define _HEAPSORT_H


/* 因为我们约定的数组是从0开始计算，所以需要对给定节点的左右孩子和父节点的index变换计算 */
#define LEFT(i) (((i) << 1) + 1)
#define RIGHT(i) (((i) + 1) << 1)
#define PARENT(i) (((i) - 1) >> 1)

#endif