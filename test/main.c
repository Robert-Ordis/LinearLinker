#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <inttypes.h>

#include <assert.h>


int linear_linker_test();
int linear_linker_sort_test();

int main(int argc, char *argv[]){
	int i;
	
	printf("%s: TEST PROGRAM FOR %s\n", __func__, "LINEAR_LINKER");
	
	printf("%s: start simple LinkedList Test===================================\n", __func__);
	linear_linker_test();
	printf("%s: start simple LinkedList Sorting Test===================================\n", __func__);
	for(i = 0; i < 10; i++){
		linear_linker_sort_test();
	}
	return 0;
}
