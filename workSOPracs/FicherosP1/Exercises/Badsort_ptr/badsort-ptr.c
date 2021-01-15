#include <stdio.h>

typedef struct {
    char data[4096];
    int key;
} item;

item array[] = {
    {"bill", 3},
    {"neil", 4},
    {"john", 2},
   
    {"rick", 5},
    {"alex", 1},
};

void sort(item *a, int n) {
    int i = 0, j = 0;
    int s = 1;
    item* p;
	 
    for(i = 0; i < n -1 & s != 0 ; i++) {//cambiado
        s = 0;
       p = a;
      	j = n-1;
	//printf("i = %i: ", i);
        for(j; j > i; j--) {
		//printf(" j %d,",j);
	
            if( p[j].key < p[j-1].key) {//cambiado
                item t = p[j];
                p[j]  = p[j-1];
                p[j-1] = t;
                s++;
            }
	
        } ;
	//printf("\n");
	 
    }
}

int main() {
    int i;
    sort(array,5);
    for(i = 0; i < 5; i++)
        printf("array[%d] = {%s, %d}\n",
                i, array[i].data, array[i].key);
    return 0;
}
