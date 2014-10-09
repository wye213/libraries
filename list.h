/* Author:Tony yin
 * Date:2014-9-22
 * Description:参考了Redis的双端链表的实现，在链表基础上实现一个迭代器比较新颖，其实在面向对象语言java和C++上都有，只是自己没有想到，继续努力。
 */
 
#ifndef _LIST_H
#define _LIST_H

#define LIST_START_HEAD 1  // 链表迭代器方向
#define LIST_START_TAIL 0

struct node {
	struct node *prev;
	struct node *next;
	void *value;
};

struct listhdr {
	struct node *head;
	struct node *tail;
	unsigned long num;
};

struct list_iter {
	struct node *next;
	int direction;
};

/* 分别 返回链表节点数，链表头节点，链表尾结点 */
#define list_num(l) ((l)->num);
#define list_head(l) ((l)->head);
#define list_tail(l) ((l)->tail);

/* 分别返回给定的链表结点的前驱结点，后继结点，结点值 */
#define node_prev(n) ((n)->prev);
#define node_next(n) ((n)->next);
#define node_value(n) ((n)->value);


struct listhdr *list_create();
struct listhdr *add_head_node(struct listhdr *list, void *value);
struct listhdr *add_tail_node(struct listhdr *list, void *value);
struct listhdr *insert_node(struct listhdr *list, void *value);

struct list_iter *list_iteration(struct listhdr *list, int direction);
struct node *next_node(struct list_iter *iter);
void iter_head(struct listhdr *list, struct list_iter *iter);
void iter_tail(struct listhdr *list, struct list_iter *iter);
void release_iter(struct list_iter *iter);

void del_node(struct listhdr *list, struct node *node);

struct node *search_by_key(struct listhdr *list, void *value);
struct node *index_node(struct listhdr *list, int index);

#endif