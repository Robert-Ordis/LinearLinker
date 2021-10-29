#include <linear_linker/linear_linker.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define	LL_TEST_NUM 30000

typedef struct lltest_s {
	int num;
	LINEAR_LINKER_S_(struct lltest_s) link1;
}lltest_t;

static int compare_count;
static inline int lltest_compare(lltest_t *a, lltest_t *b){
	LL_DBG_PRINTF_VERBOSE_("%s: %p and %p\n", __func__, a, b);
	if(a == NULL && b != NULL){
		LL_DBG_PRINTF_VERBOSE_("%s: NULL vs %d\n", __func__, b->num);
	}
	else if(a != NULL && b == NULL){
		LL_DBG_PRINTF_VERBOSE_("%s: %d vs NULL\n", __func__, a->num);
	}
	else if(a == NULL && b == NULL){
		LL_DBG_PRINTF_VERBOSE_("%s: NULL vs NULL\n", __func__);
	}
	else{
		compare_count++;
		LL_DBG_PRINTF_VERBOSE_("%s: %d vs %d\n", __func__, a->num, b->num);
		return a->num - b->num;
		//return b->num - a->num;
	}
	return 0;
}

static inline int lltest_compare_inv(lltest_t *a, lltest_t *b){
	LL_DBG_PRINTF_VERBOSE_("%s: %d vs %d\n", __func__, a->num, b->num);
	compare_count++;
	return b->num - a->num;
}

int linear_linker_test(){
	lltest_t testers[LL_TEST_NUM];
	lltest_t *head = NULL;
	
	lltest_t *odds = NULL;
	
	lltest_t *h;
	int i;
	srand(time(NULL));
	LL_DBG_PRINTF_INFO_("%s: INITIATING LINKED LIST...\n", __func__);
	for(i = 0; i < LL_TEST_NUM; i++){
		testers[i].num = i;
		LINEAR_LINKER_ADD_(&head, &testers[i], lltest_t, link1);
		LL_DBG_PRINTF_DEBUG_("%s[%p]: num=%d\n", __func__, &testers[i], i);
	}
	
	LL_DBG_PRINTF_DEBUG_("%s: show from %p\n", __func__, &head);
	for(h = head; h != NULL; h = LINEAR_LINKER_NEXT_(h, lltest_t, link1)){
		LL_DBG_PRINTF_VERBOSE_("%s[%p]: num=%d\n", __func__, h, h->num);
	}
	
	LL_DBG_PRINTF_INFO_("%s: delete and put tail of list(%p)\n", __func__, &head);
	for(h = head; h != NULL; h = LINEAR_LINKER_NEXT_(h, lltest_t, link1)){
		if(h->num > (LL_TEST_NUM / 2)){
			LINEAR_LINKER_DEL_(&head, h, lltest_t, link1);
			LL_DBG_PRINTF_DEBUG_("%s[%p]... deleted. number is %d\n", __func__, h, h->num);
			LL_DBG_PRINTF_DEBUG_("%s[%p]... and put it in tail.\n", __func__, h);
			LINEAR_LINKER_ADD_(&head, h, lltest_t, link1);
			break;
		}
	}
	
	LL_DBG_PRINTF_DEBUG_("%s: show from %p\n", __func__, &head);
	for(h = head; h != NULL; h = LINEAR_LINKER_NEXT_(h, lltest_t, link1)){
		LL_DBG_PRINTF_VERBOSE_("%s[%p]: num=%d\n", __func__, h, h->num);
	}
	
	LL_DBG_PRINTF_INFO_("%s: move to another list(%p->%p)\n", __func__, &head, &odds);
	h = head;
	while(h != NULL){
		lltest_t *h_next = LINEAR_LINKER_NEXT_(h, lltest_t, link1);
		if(h->num & 0x01){
			LINEAR_LINKER_DEL_(&head, h, lltest_t, link1);
			LL_DBG_PRINTF_INFO_("%s[%p]... deleted. number is %d\n", __func__, h, h->num);
			LINEAR_LINKER_ADD_(&odds, h, lltest_t, link1);
		}
		h = h_next;
	}
	
	lltest_t *p = NULL;
	LL_DBG_PRINTF_DEBUG_("%s: show the first list(%p)\n", __func__, &head);
	for(h = head; h != NULL; h = LINEAR_LINKER_NEXT_(h, lltest_t, link1)){
		LL_DBG_PRINTF_VERBOSE_("%s[%p]: num=%d\n", __func__, h, h->num);
		if(p == NULL && h->num > (LL_TEST_NUM / 2)){
			p = h;
			LL_DBG_PRINTF_VERBOSE_("%s[%p]:->ODD WILL BE INSERTED AFTER THIS\n", __func__, h);
		}
	}
	
	LL_DBG_PRINTF_DEBUG_("%s: show the another list(%p)\n", __func__, &odds);
	for(h = odds; h != NULL; h = LINEAR_LINKER_NEXT_(h, lltest_t, link1)){
		LL_DBG_PRINTF_VERBOSE_("%s[%p]: num=%d\n", __func__, h, h->num);
		assert(h->num & 0x01);
	}
	
	LL_DBG_PRINTF_INFO_("%s: move to first list again(%p->%p)\n", __func__, &odds, &head);
	
	p = NULL;
	while(1){
		LINEAR_LINKER_POP_(&odds, &h, lltest_t, link1);
		if(h == NULL){
			break;
		}
		LL_DBG_PRINTF_DEBUG_("%s[%p] popped->num is %d\n", __func__, h, h->num);
		LL_DBG_PRINTF_DEBUG_("%s[%p] insert after %p\n", __func__, h, p);
		LINEAR_LINKER_INSERT_AFTER_(&head, h, p, lltest_t, link1);
		p = h;	//入れたところの後ろに入れる。
	}
	
	LL_DBG_PRINTF_DEBUG_("%s: show the another list(%p)\n", __func__, &odds);
	for(h = odds; h != NULL; h = LINEAR_LINKER_NEXT_(h, lltest_t, link1)){
		LL_DBG_PRINTF_DEBUG_("%s[%p]: num=%d\n", __func__, h, h->num);
	}
	
	LL_DBG_PRINTF_DEBUG_("%s: show the first list(%p)\n", __func__, &head);
	for(h = head; h != NULL; h = LINEAR_LINKER_NEXT_(h, lltest_t, link1)){
		LL_DBG_PRINTF_DEBUG_("%s[%p]: num=%d\n", __func__, h, h->num);
	}
	
	LL_DBG_PRINTF_DEBUG_("%s: test for comparison insertion\n", __func__);
	LL_DBG_PRINTF_DEBUG_("%s: move to another list(%p->%p)\n", __func__, &head, &odds);
	h = head;
	while(h != NULL){
		lltest_t *h_next = LINEAR_LINKER_NEXT_(h, lltest_t, link1);
		if(h->num & 0x01){
			LINEAR_LINKER_DEL_(&head, h, lltest_t, link1);
			LL_DBG_PRINTF_DEBUG_("%s[%p]... deleted. number is %d\n", __func__, h, h->num);
			LINEAR_LINKER_ADD_(&odds, h, lltest_t, link1);
		}
		h = h_next;
	}
	
	LL_DBG_PRINTF_DEBUG_("%s: show the first list(%p)\n", __func__, &head);
	for(h = head; h != NULL; h = LINEAR_LINKER_NEXT_(h, lltest_t, link1)){
		LL_DBG_PRINTF_DEBUG_("%s[%p]: num=%d\n", __func__, h, h->num);
	}
#if 0
	LL_DBG_PRINTF_DEBUG_("%s: then insert with comparison\n", __func__);
	while(1){
		LINEAR_LINKER_POP_(&odds, &h, lltest_t, link1);
		if(h == NULL){
			break;
		}
		LL_DBG_PRINTF_DEBUG_("%s[%p] popped->num is %d\n", __func__, h, h->num);
		LINEAR_LINKER_INSERT_INORDER_(&head, h, lltest_compare, lltest_t, link1);
	}
	
	LL_DBG_PRINTF_DEBUG_("%s: show the another list(%p)\n", __func__, &odds);
	for(h = odds; h != NULL; h = LINEAR_LINKER_NEXT_(h, lltest_t, link1)){
		LL_DBG_PRINTF_DEBUG_("%s[%p]: num=%d\n", __func__, h, h->num);
	}
	
	
#else
	while(1){
		LINEAR_LINKER_POP_(&odds, &h, lltest_t, link1);
		if(h == NULL){
			break;
		}
		LL_DBG_PRINTF_DEBUG_("%s[%p] popped->num is %d\n", __func__, h, h->num);
		LINEAR_LINKER_ADD_(&head, h, lltest_t, link1);
	}
#endif
	
	LL_DBG_PRINTF_DEBUG_("%s: show the first list(%p)\n", __func__, &head);
	for(h = head; h != NULL; h = LINEAR_LINKER_NEXT_(h, lltest_t, link1)){
		LL_DBG_PRINTF_DEBUG_("%s[%p]: num=%d\n", __func__, h, h->num);
	}
	
	LINEAR_LINKER_WHOLE_ASSERT_(&head, lltest_t, link1);
	int nth;
	
	nth = 3;
	LL_DBG_PRINTF_DEBUG_("%s: test for nth getting\n", __func__);
	LINEAR_LINKER_NTH_NEXT_(head, &h, nth, lltest_t, link1);
	LL_DBG_PRINTF_DEBUG_("%s[%p]: %dth is num=%d\n", __func__, h, nth, h->num);
	
	nth = 1;
	LINEAR_LINKER_NTH_NEXT_(h, &h, nth, lltest_t, link1);
	LL_DBG_PRINTF_DEBUG_("%s[%p]: %dth next is num=%d\n", __func__, h, nth, h->num);
	
	nth = LL_TEST_NUM;
	LINEAR_LINKER_NTH_NEXT_(h, &h, nth, lltest_t, link1);
	LL_DBG_PRINTF_DEBUG_("%s[%p]: %dth next \n", __func__, h, nth);
	
	LINEAR_LINKER_SORT_(&head, lltest_compare, lltest_t, link1);
	LL_DBG_PRINTF_DEBUG_("%s: show the first list(%p)\n", __func__, &head);
	for(h = head; h != NULL; h = LINEAR_LINKER_NEXT_(h, lltest_t, link1)){
		LL_DBG_PRINTF_DEBUG_("%s[%p]: num=%d\n", __func__, h, h->num);
	}
	
	
	sleep(1);
	
	
	
	return 0;
}



int linear_linker_sort_test(){
	lltest_t testers[LL_TEST_NUM];
	lltest_t *head = NULL;
	
	lltest_t *h_p = NULL;
	
	lltest_t *h;
	int i;
	srand(time(NULL));
	LL_DBG_PRINTF_DEBUG_("%s: INITIATING LINKED LIST...\n", __func__);
	for(i = 0; i < LL_TEST_NUM; i++){
		testers[i].num = rand();
		LINEAR_LINKER_ADD_(&head, &testers[i], lltest_t, link1);
		LL_DBG_PRINTF_DEBUG_("%s[%p]: num=%d\n", __func__, &testers[i], i);
	}
	
	LL_DBG_PRINTF_DEBUG_("%s: show from %p\n", __func__, &head);
	for(h = head; h != NULL; h = LINEAR_LINKER_NEXT_(h, lltest_t, link1)){
		LL_DBG_PRINTF_DEBUG_("%s[%p]: num=%d\n", __func__, h, h->num);
	}
	
	compare_count = 0;
	LINEAR_LINKER_SORT_(&head, lltest_compare, lltest_t, link1);
	
	LL_DBG_PRINTF_INFO_("%s: compare_count[random->ordered]: %d\n", __func__, compare_count);
	
	LL_DBG_PRINTF_DEBUG_("%s: show the first list(%p)\n", __func__, &head);
	for(h = head; h != NULL; h = LINEAR_LINKER_NEXT_(h, lltest_t, link1)){
		LL_DBG_PRINTF_DEBUG_("%s[%p]: num=%d\n", __func__, h, h->num);
	}
	
	LINEAR_LINKER_WHOLE_ASSERT_(&head, lltest_t, link1);
	
	h_p = head;
	h = head;
	LL_DBG_PRINTF_DEBUG_("%s: whole checking if sorted correctly\n", __func__);
	i = 0;
	while((h = LINEAR_LINKER_NEXT_(h, lltest_t, link1)) != NULL){
		i++;
		assert(lltest_compare(h_p, h) <= 0);
		h_p = h;
	}
	assert(i == (LL_TEST_NUM - 1));
	
	LL_DBG_PRINTF_DEBUG_("%s:==========sort to same ordered list====\n", __func__);
	compare_count = 0;
	LINEAR_LINKER_SORT_(&head, lltest_compare, lltest_t, link1);
	
	assert(i == (LL_TEST_NUM - 1));
	LL_DBG_PRINTF_INFO_("%s: compare_count[ordered->ordered]: %d\n", __func__, compare_count);
	
	LL_DBG_PRINTF_DEBUG_("%s: show the first list(%p)\n", __func__, &head);
	for(h = head; h != NULL; h = LINEAR_LINKER_NEXT_(h, lltest_t, link1)){
		LL_DBG_PRINTF_DEBUG_("%s[%p]: num=%d\n", __func__, h, h->num);
	}
	
	LINEAR_LINKER_WHOLE_ASSERT_(&head, lltest_t, link1);
	
	h_p = head;
	h = head;
	LL_DBG_PRINTF_DEBUG_("%s: whole checking if sorted correctly\n", __func__);
	i = 0;
	while((h = LINEAR_LINKER_NEXT_(h, lltest_t, link1)) != NULL){
		i++;
		assert(lltest_compare(h_p, h) <= 0);
		h_p = h;
	}
	assert(i == (LL_TEST_NUM - 1));
	
	
	LL_DBG_PRINTF_DEBUG_("%s:==========sort to inversion list====\n", __func__);
	compare_count = 0;
	LINEAR_LINKER_SORT_(&head, lltest_compare_inv, lltest_t, link1);
	
	assert(i == (LL_TEST_NUM - 1));
	LL_DBG_PRINTF_INFO_("%s: compare_count[ordered->reverse]: %d\n", __func__, compare_count);
	
	LL_DBG_PRINTF_DEBUG_("%s: show the first list(%p)\n", __func__, &head);
	for(h = head; h != NULL; h = LINEAR_LINKER_NEXT_(h, lltest_t, link1)){
		LL_DBG_PRINTF_DEBUG_("%s[%p]: num=%d\n", __func__, h, h->num);
	}
	
	LINEAR_LINKER_WHOLE_ASSERT_(&head, lltest_t, link1);
	
	
	
	h_p = head;
	h = head;
	LL_DBG_PRINTF_DEBUG_("%s: whole checking if sorted correctly\n", __func__);
	i = 0;
	while((h = LINEAR_LINKER_NEXT_(h, lltest_t, link1)) != NULL){
		i++;
		assert(lltest_compare_inv(h_p, h) <= 0);
		h_p = h;
	}
	sleep(1);
	
	
	return 0;
}

