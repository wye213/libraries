/*  Author:Tony yin
 *  Date:2014-10-9
 *  Description:堆排序的C语言实现版本，堆排序的数组约定从0开始计算。
 */


#include <stdio.h>
#include <stdlib.h>

#include "heapsort.h"

/* 交换数组中的两个元素 */
void swap(int array[], int a, int b) {
	int temp;

	temp = array[a];
	array[a] = array[b];
	array[b] = temp;
}

/* 维持大根堆的特性，当有节点不满足大根堆的特性时，调用该函数即可修正,index为要修正的节点索引 */
void heap_max_balance(int array[], int index, int heapsize) {
	int l = LEFT(index);
	int r = RIGHT(index);
	int largest = index;

	if (l <= heapsize && array[l] > array[index]) largest = l;
	if (r <= heapsize && array[r] > array[largest]) largest = r;

	if (largest != index) {
		swap(array, largest, index);
		heap_max_balance(array, largest, heapsize);
	}
}

/* 维持小根堆的特性，当有节点不满足小根堆的特性时，调用该函数即可修正,index为要修正的节点索引 */
void heap_min_balance(int array[], int index, int heapsize) {
	int l = LEFT(index);
	int r = RIGHT(index);
	int largest = index;

	if (l <= heapsize && array[l] < array[index]) largest = l;
	if (r <= heapsize && array[r] < array[largest]) largest = r;

	if (largest != index) {
		swap(array, largest, index);
		heap_min_balance(array, largest, heapsize);
	}
}

/* 对给定的数组建立大根堆 num为数组最后元素的索引 */
void build_max_heap(int array[], int num) {
	int i;

	for (i=PARENT(num); i>=0; i--) {
		heap_max_balance(array, i, num);
	}
}

/* 对给定的数组建立小根堆 num为数组最后元素的索引 */
void build_min_heap(int array[], int num) {
	int i;

	for (i=PARENT(num); i>=0; i--) {
		heap_min_balance(array, i, num);
	}
}

/* 对建好的堆按照顺序排序 注意num实际上为数组最后一个元素的索引,通常小根堆适合从打到小排序，大根堆从小到大排序 这里使用大根堆从小到大排序 */
void heapsort(int array[], int num) {
	int i;
	int heapsize = num;

	build_max_heap(array, num);

	for (i=num; i>0; i--) {
		heapsize--;
		swap(array, i, 0);
		heap_max_balance(array, 0, heapsize);
	}

}

/* 测试 */
int main() {
	int array[] = {4, 8, 6, 10};
	int i;

	for (i=0; i<4; i++) {
		printf("%d--", array[i]);
	}
	printf("\n");

	heapsort(array, 3);
	for (i=0; i<4; i++) {
		printf("%d--", array[i]);
	}
	printf("\n");

}