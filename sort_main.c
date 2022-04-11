#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>
#include<math.h>
#include<time.h>

int* mrand(int* arr,int num){
	int i,j;
	int n;
	int count = 1;

	for(i = 1; i <= num; i++){
		n = rand() % num;
		for(j = 1; j < count; j++){
			if(n == arr[j])
				break;
		}
		if(j == count){
			arr[count] = n;
			count++;
		}else{
			i--;
		}
	}
	return arr;
}

void exchange_sort(int* arr,int n){
	int i,j;
	int move;

	for(i = 1; i <= n; i++)
		for(j = i + 1; j <= n; j++)
			if(arr[j] < arr[i]){
				move = arr[j];
				arr[j] = arr[i];
				arr[i] = move;
			}
}

void merge(int* U,int* V,int h,int m,int* arr){
	int i,j,k;
	int x;
	i = 1; j = 1; k = 1;
	while(i <= h && j <= m){
		if(U[i] < V[j]){
			arr[k] = U[i];
			i++;
		}else{
			arr[k] = V[j];
			j++;
		}
		k++;
	}
	if(i > h){
		for(x = j; x <= m; x++){
			arr[k] = V[x];
			k++;
		}
	}else{
		for(x = i; x <= h; x++){
			arr[k] = U[x];
			k++;
		}
	}
}

void merge_sort(int* arr,int n){
	int i,j;
	int h,m;
	int *U;
	int *V;

	if(n > 1){
		h = n / 2;
		m = n - h;
		U = (int*)malloc(sizeof(int) * (h + 1));
		V = (int*)malloc(sizeof(int) * (m + 1));
		for(i = 1; i <= h; i++)
			U[i] = arr[i];
		for(i = 1; i <= m; i++)
			V[i] = arr[h + i];
		merge_sort(U,h);
		merge_sort(V,m);
		merge(U,V,h,m,arr);
		
		free(U);
		free(V);
	}
}
int mid(int* arr,int a,int b,int c){
	if(arr[a] >= arr[b]){
		if(arr[b] >= arr[c])
			return b;
		else if(arr[a] <= arr[c])
			return a;
		else
			return c;
	}else if(arr[a] >= arr[c])
		return a;
	else if(arr[b] >= arr[c])
		return c;
	else
		return b;
}

void partition(int* arr,int low,int high,int* pivot){
	int i,j;
	int index;
	int pivot_item;
	int move;
	
	index = mid(arr,low,(low + high) / 2,high);
	pivot_item = arr[index];
	j = low - 1;

	for(i = low; i <= high; i++){
		if(i != index && arr[i] < pivot_item){
			j++;
			if(j == index)
				j++;
			move = arr[i];
			arr[i] = arr[j];
			arr[j] = move;
		}
	}
	if(index > j)
		*pivot = j + 1;
	else
		*pivot = j;
	move = arr[index];
	arr[index] = arr[*pivot];
	arr[*pivot] = move;

}

void quick_sort(int* arr,int low, int high){
	int pivot;
	if(high > low){
		partition(arr,low,high,&pivot);
		quick_sort(arr,low,pivot - 1);
		quick_sort(arr,pivot + 1,high);
	}
}

typedef struct _heap{
	int* S;
	int heap_size;
}heap;

void siftdown(heap* H,int i){
	int parent, largerchild;
	int siftkey;
	bool spotfound;

	siftkey = H->S[i];
	parent = i;
	spotfound = false;
	while(2 * parent <= H->heap_size && !spotfound){
		if(2 * parent < H->heap_size && H->S[2 * parent] < H->S[2 * parent + 1])
			largerchild = 2 * parent + 1;
		else
			largerchild = 2 * parent;
		if(siftkey < H->S[largerchild]){
			H->S[parent] = H->S[largerchild];
			parent = largerchild;
		}
		else
			spotfound = true;
	}
	H->S[parent] = siftkey;
}

int root(heap* H){
	int keyout;

	keyout = H->S[1];
	H->S[1] = H->S[H->heap_size];
	H->heap_size = H->heap_size - 1;
	siftdown(H,1);

	return keyout;
}

void removekeys(heap* H,int *S,int n){
	int i;
	for(i = n; i >= 1; i--)
		S[i] = root(H);
}

void makeheap(heap* H,int n){
	int i;
	H->heap_size = n;
	for(i = n / 2; i >= 1; i--)
		siftdown(H,i);
}

void heap_sort(heap* H,int n){
	makeheap(H,n);
	removekeys(H,H->S,n);
}

typedef struct _node{
	int key;
	struct _node* link;
}node;

typedef struct{
	node* head;
	node* crnt;
}List;

void Initialize(List *list){
	list->head = NULL;
	list->crnt = NULL;
}

node* AllocNode(){
	return (node*)malloc(sizeof(node));
}

void SetNode(node* n,int key,node* link){
	n->key = key;
	n->link = link;
}

void InsertFront(List *list,int key){
	node* ptr = list->head;
	list->head = list->crnt = AllocNode();

	SetNode(list->head,key,ptr);
}

void InsertRear(List* list,int key){
	if(list->head == NULL)
		InsertFront(list,key);
	else{
		node* ptr = list->head;
		while(ptr->link != NULL)
			ptr = ptr->link;
		ptr->link = list->crnt = AllocNode();
		SetNode(ptr->link,key,NULL);
	}
}


void distribute(List* masterlist,List* list,int i){
	int j;
	

	for(j = 0; j <= 9; j++)
		list[j].head = NULL;
	node* ptr = masterlist->head;
	while(ptr != NULL){
		j = (int)((ptr->key) / pow(10, i - 1)) % 10;
		InsertRear(&list[j],ptr->key);
		ptr = ptr->link;
	}
}

void coalesce(List* masterlist,List* list){
	int i;
	node* ptr;

	Initialize(masterlist);
	for(i = 0; i <= 9; i++){
		if(i == 0){
			masterlist->head = list[i].head;
			ptr = masterlist->head;
		}else{
			while(ptr->link != NULL)
				ptr = ptr->link;
			ptr->link = list[i].head;
		}
	}
}

void radix_sort(List* masterlist,int numdights){
	int i;
	List list[10];

	for(i = 1; i <= numdights; i++){
		distribute(masterlist,list,i);
		coalesce(masterlist,list);
	}
}

void Print(int* arr,int num){
	int i;
	for(i = 1; i <= num; i++)
		printf("%d ",arr[i]);
	printf("\n");
}

void Print_list(List* list){
	node* ptr = list->head;
	while(ptr != NULL){
		printf("%d ",ptr->key);
		ptr = ptr->link;
	}
	printf("\n");
}

void Clear(List* list){
	while(list->head != NULL){
		node* ptr = list->head->link;
		free(list->head);
		list->head = list->crnt = ptr;
	}
}

int count_numdigits(int num){
	int i;
	int count = 1;

	while(num >= 10){
		num = num / 10;
		count++;
	}
	return count;
}

int main(){
	int i,j;
	int n;
	int count = 0;
	int data_num, num;
	int* arr;
	heap H;
	List list;
	int numdigits;
	struct timespec tstart = {0,0}, tend = {0,0};
	
	
	srand(time(NULL));
	printf("사용할 data의 개수를 입력하시오 : ");
	scanf("%d",&data_num);
	
	while(1){
		printf("사용할 정렬 방식을 고르시오(1.Exchange 2.Merge 3.Quick 4.Heap 5.Radix : ");
		scanf("%d",&num);
		clock_gettime(CLOCK_MONOTONIC, &tstart);
		switch(num){
			case 1:
				arr = (int*)malloc(sizeof(int) * (data_num + 1));
				arr = mrand(arr,data_num);
				
				clock_gettime(CLOCK_MONOTONIC, &tstart);
				exchange_sort(arr,data_num);
				clock_gettime(CLOCK_MONOTONIC, &tend);

				Print(arr,data_num);
				free(arr);
				break;
			case 2:
				arr = (int*)malloc(sizeof(int) * (data_num + 1));
				arr = mrand(arr,data_num);
				
				clock_gettime(CLOCK_MONOTONIC, &tstart);
				merge_sort(arr,data_num);
				clock_gettime(CLOCK_MONOTONIC, &tend);
				
				Print(arr,data_num);
				free(arr);
				break;
			case 3:
				arr = (int*)malloc(sizeof(int) * (data_num + 1));
				arr = mrand(arr,data_num);

				clock_gettime(CLOCK_MONOTONIC, &tstart);
				quick_sort(arr,1,data_num);
				clock_gettime(CLOCK_MONOTONIC, &tend);
				
				Print(arr,data_num);
				free(arr);
				break;
			case 4:
				H.S = (int*)malloc(sizeof(int) * (data_num + 1));
				H.S = mrand(H.S,data_num);
				
				clock_gettime(CLOCK_MONOTONIC, &tstart);
				heap_sort(&H,data_num);
				clock_gettime(CLOCK_MONOTONIC, &tend);
				
				Print(H.S,data_num);
				free(H.S);
				break;
			case 5:
				arr = (int*)malloc(sizeof(int) * (data_num + 1));
				arr = mrand(arr,data_num);
				Initialize(&list);
				for(i = 1; i <= data_num; i++)
					InsertRear(&list,arr[i]);
				numdigits = count_numdigits(data_num - 1);
				
				clock_gettime(CLOCK_MONOTONIC, &tstart);
				radix_sort(&list,numdigits);
				clock_gettime(CLOCK_MONOTONIC, &tend);
				
				Print_list(&list);
				free(arr);
				Clear(&list);
				break;
			default:
				printf("ERROR : 수가 잘못 입력됨(1~5 사이의 수를 입력하시오)\n");
				continue;
		}
		//end = clock();
		clock_gettime(CLOCK_MONOTONIC, &tend);
		//result = (double)(end - start) / CLOCKS_PER_SEC;
		//printf("소요된 시간 : %.10lf \n",result);
		//printf("수행 시간 %ld clocks (1sec = %ld clocks)\n",end - start,CLOCKS_PER_SEC);
		printf("실행시간 : %.5f seconds\n",((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));
		return 0;
	}	
}
