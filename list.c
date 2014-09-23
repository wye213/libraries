/* Author:Tony yin
 * Date:2014-9-22
 * Description:模仿Redis的双端链表的实现
 */

#include <stdlib.h>
#include "list.h"

/* 创建一个链表结构初始状态不包含一个节点 */
struct listhdr *list_create() {
	struct listhdr *list;

	list = malloc(sizeof(struct listhdr));
	if (list == NULL) return NULL;

	list->head = NULL;
	list->tail = NULL;
	list->num = 0;

	return list;
}

/* 在链表头部添加一个节点 */
struct listhdr *add_head_node(struct listhdr *list, void *value) {
	struct node *node;

	node = (struct node *)malloc(sizeof(struct node));
	if (node == NULL) return NULL;

	node->value = value;

	if (list->head == NULL) {
		list->head = node;
		node->prev = node->next = NULL;
	}
	else {
		list->head->prev = node;
		node->next = list->head;
		list->head = node;
		node->prev = NULL;
	}

	list->num++;

	return list;
}

/* 在链表尾部添加一个节点 */
struct listhdr *add_tail_node(struct listhdr *list, void *value) {
	struct node *node;

	node = (struct node *)malloc(sizeof(struct node));
	if (node == NULL) return NULL;

	if (list->tail == NULL) {
		list->tail = node;
		node->prev = node->next = NULL;
	}
	else {
		list->tail->next = node;
		node->prev = list->tail;
		list->tail = node;
		node->next = NULL;
	}

	list->num++;

	return list;
}

/* 在一个链表中插入一个节点的方式有两种，一种是给定一个index 插入节点， 或是给定一个待插入的节点的前驱结点或是后继结点 */
struct listhdr *insert_node(struct listhdr *list, struct node *old_node, void *value, int after) {
	struct node *node;

	node = (struct node *)malloc(sizeof(struct node));
	if (node == NULL) return NULL;

	if (after) {
		node->prev = old_node;
		node->next = old_node->next;

		if (old_node == list->tail)
			list->tail = node;
		
		old_node->next = node;		
	}
	else {
		node->next = old_node;
		node->prev = old_node->prev;

		if (old_node == list->head)
			list->head = node;

		old_node->prev = node;
	}

	list->num++;

	return list;
}

/* 根据链表生成链表迭代器 根据给定的方向确定迭代方向 */
struct list_iter *list_iteration(struct listhdr *list, int direction) {
	struct list_iter *iter;

	iter = (struct list_iter *)malloc(sizeof(struct list_iter));
	if (iter == NULL) return NULL;

	if (direction == LIST_START_HEAD)
		iter->next = list->head;
	else iter->next = list->tail;

	iter->direction = direction;

	return iter;
}

/* 返回当前链表迭代器指向的节点 同事更新迭代器的指向节点 */
struct node *next_node(struct list_iter *iter) {
	struct node *node = iter->next;

	if (node != NULL) {
		if (iter->direction == LIST_START_HEAD) iter->next = node->next;
		else iter->next = node->prev;
	}

	return node;
}

/* 链表迭代器指针重新指向链表头 */
void iter_head(struct listhdr *list, struct list_iter *iter) {
    iter->next = list->head;
    li->direction = LIST_START_HEAD;
}

/* 聊表迭代器指针重新指向聊表尾 */
void iter_head(struct listhdr *list, struct list_iter *iter) {
    iter->next = list->tail;
    li->direction = LIST_START_TAIL;
}

/* 释放链表迭代器 */
void release_iter(struct list_iter *iter) {
	free(iter);
}

/* 删除给定链表给定节点，同时更新链表节点数 */
void del_node(struct listhdr *list, struct node *node) {
	if (node->prev) node->prev->next = node->next;
	else list->head = node->next;

	if (node->next) node->next->prev = node->prev;
	else list->tail = node->prev;

	free(node);

	list->num--;
}

/* 根据给定的值搜索链表节点值 如果匹配返回节点指针 */
struct node *search_by_key(struct listhdr *list, void *key) {
	struct list_iter *iter;
	struct node *node;

	iter = list_iteration(list, LIST_START_HEAD);

	while ((node = next_node(iter)) != NULL) {
		if (key == node->value) {
			break;
		}
	}
	release_iter(iter);

	return node;
}


/* 根据索引返回对应的节点指针， 索引如果从表头则是从0开始，如果是从表尾则是从-1开始 */
struct node *index_node(struct listhdr *list, int index) {
	struct node *node;

	if (index < 0) {
		index = (-index) - 1;
		node = list->tail;

		while (index-- && node) node = node->prev;
	}
	else {
		node = list->head;
		while (index-- && node) node = node->next;
	}

	return node;
}
