#include<stdio.h>
#include<stdio_ext.h>
#include<stdlib.h>
#include<unistd.h>
#include<math.h>

typedef struct cache_entry{
	int updated;
	int* block;
	int* tag;
	int* word;
}ce;

int addressable_bits;

void simulate();
void update();


int total_ref,miss_ref,hit_ref;

int main(){

	int cache_size,mm_size,block_size;
	system("clear");

	printf("\n\n       +*+*+*+*+*+  BASIC CACHE SIMUATOR +*+*+*+*+*+         \n\n\n");

	printf("Enter the size of main memory (let it be the power of two) : ");    scanf("%d",&mm_size);
	printf("\nEnter the size of cache memory (let it be the power of two) : "); scanf("%d",&cache_size);
	printf("\nEnter the size of a block (let it be the power of two) : ");      scanf("%d",&block_size);

	addressable_bits = log(mm_size) / log(2);
	int word_bits =        log(block_size) / log(2);
	int no_blocks = 		cache_size/block_size;
	int block_bits =       log(no_blocks) / log(2);
	int tag_bits  =        addressable_bits - word_bits - block_bits;



	printf("\n\n\n");

	printf("So according to the given data,\n");
	printf("Total addressable bits are : %d\n",addressable_bits);
	printf("Which is divides as %d bits for tag bits,%d bits for block bits and %d bits for word bits.\n\n",tag_bits,block_bits,word_bits);

	__fpurge(stdin);
	printf("\n\npress enter to continue.....");
	char temp; scanf("%c",&temp);
	system("clear");


	ce cache[no_blocks];
	

	for(int i=0;i<no_blocks;i++){
		cache[i].updated = -1;
	}

	printf("Initial cache memory :-- \n\n");
	simulate(cache,no_blocks,tag_bits,block_bits,word_bits);

	return 0;
}

void simulate(ce cache[],int no_blocks,int tag_bits,int block_bits,int word_bits){

	// total_ref = 0;miss_ref=0;hit_ref=0;

	do{
	printf("s.no      tag         word\n");
	for(int i=0;i<no_blocks;i++){
		if(cache[i].updated==-1) printf("%d         ----        ---- \n",i);
		else{
			printf("%d         ",i);
			for(int j=0;j<tag_bits;j++){
				printf("%d",cache[i].tag[j]);
			}
			printf("        ");
			for(int j=0;j<word_bits;j++){
				printf("%d",cache[i].word[j]);
			}	
			printf("\n");
		}
	}

	if(total_ref!=0)
	printf("\nMiss ratio : %.2f             Hit ratio : %.2f \n",(miss_ref/(float)total_ref)*100,(hit_ref/(float)total_ref)*100);

	printf("\n\n");
	printf("Enter the memory block request address to fetch (enter -1 to exit) ");
	printf("\nTo read press 0 and for write press 1 after the request address     :  ");
	int r_w;
	int address; scanf("%d %d",&address,&r_w);

	if(address==-1) return;

	update(cache,address,tag_bits,block_bits,word_bits,no_blocks);


	__fpurge(stdin);
	printf("\n\npress enter to continue.....");
	char temp; scanf("%c",&temp);
	system("clear");

	}while(1);

}

void getBinary(int address,int binAddr[],int n){

	for(int i=n-1;i>=0;i--){
		binAddr[i] = address%2;
		address = address/2;
	}
}

int sameTag(int* binAddr,int tag_bits,int tag_arr[]){
	for(int i=0;i<tag_bits;i++){
		if(binAddr[i]!=tag_arr[i]){
			return 0;
		}
	}
	return 1;
}

int getDec(int binAddr[],int m,int k,int n){
	int val = 0;
	int base = 1;

	for(int i=k+n-1;i>=k;i--){
		val += base * binAddr[i];
		base = base * 2;
	}

	return val;
}

void update(ce cache[],int address,int tag_bits,int block_bits,int word_bits,int no_blocks,int r_w){

	int binAddr[addressable_bits];
	getBinary(address,binAddr,addressable_bits);
	total_ref++;

	printf("\nThe binary representation of your address is : ");
	printf("\nTag bits : ");
	for(int i=0;i<addressable_bits;i++){
		if(i==tag_bits) printf(" block bits : ");
		if(i==addressable_bits-word_bits) printf(" word bits : ");
		printf("%d",binAddr[i]);
	}printf("\n\n");

	int index = getDec(binAddr,addressable_bits,tag_bits,block_bits);

	if(cache[index].updated==-1){

		miss_ref++;
		printf("As there are no prior entries in the cache location indexed at %d It's a cache miss\n",index);
		cache[index].updated=1;
		cache[index].tag = malloc(sizeof(int) * tag_bits);
		cache[index].word = malloc(sizeof(int) * word_bits);
		cache[index].block = malloc(sizeof(int) * block_bits);

		for(int j=0;j<tag_bits;j++){
			cache[index].tag[j] = binAddr[j];
		}
		for(int j=0;j<word_bits;j++){
			cache[index].word[j] = binAddr[addressable_bits-word_bits+j];
		}
	}
	else if(cache[index].updated && sameTag(binAddr,tag_bits,cache[index].tag)){

		hit_ref++;
		printf("As there is already a entry at the index %d in cache whose tag bits are same as the current one,It's a cache Hit.\n",index);
	}
	else{

		miss_ref++;
		printf("Even though there is a entry in %d indexed location its tag bits are different hence its a cache miss\n",index);

		for(int j=0;j<tag_bits;j++){
			cache[index].tag[j] = binAddr[j];
		}
		for(int j=0;j<word_bits;j++){
			cache[index].word[j] = binAddr[addressable_bits-word_bits+j];
		}
	}

}