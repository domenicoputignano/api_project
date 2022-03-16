#include <stdio.h>
#include <stdlib.h>
#define MAX 2500
#define INF 21474836
int d,k;

struct nodo{
    int ind;
    int dist;
};
//input functions     
void read_param();
void read_commands();
void aggiungiGrafo();
//main functions
void min_heapify(int);
void buildMinHeap();
void max_heapifyGraph(int);
void buildMaxHeapGraphs();
void cancellaMin();
int dijkstra(int[][400]);
void topK();
//main attributes
int end = 1;
int num_graphs=0;
int num_topK=0;
int ok=1;
struct nodo* topKgraphs;
struct nodo* heap;
int *corr;
int heapsize,heapsize_rank=0;

int main() {
    read_param();
    while(end)
    {
        read_commands();
    }
    return 0;
}
void read_param(){
    char input[22];
    char* str;
    if(fgets(input,22,stdin)!=NULL)
    {
        d = strtol(input,&str,10);
        str[0]=' ';
        k = strtol(str,NULL,10);
    }
    topKgraphs = malloc(k*8);
    heap = malloc(d*8);
    corr = malloc(d*8);
}

void read_commands() {
    char comm[15];
    if(fgets(comm, 15, stdin)==NULL)
        end = 0;
    else
    if (comm[0] == 'A')
        aggiungiGrafo();
    else if (comm[0] == 'T')
        topK();
}

void aggiungiGrafo() {

    int matr[400][400];
    char row[MAX];
    char* str;
    for(int i=0;i<d;i++)
    {
        if(fgets(row, MAX, stdin)!=NULL)
            matr[i][0]=strtoul(row,&str,10);
        if(matr[i][0]==0) matr[i][0]=INF;
        str[0]=' ';
        for(int j=1;j<d;j++)
        {
            matr[i][j]=strtoul (str,&str,10);
            if(matr[i][j]==0) matr[i][j]=INF;
            str[0]=' ';
            if(i==0)
            {
                heap[heapsize].dist = matr[0][j];
    			heap[heapsize].ind = j;
    			heapsize++;
                corr[j]=j-1;
            }
        }
    }
    int c = dijkstra(matr);
    if(heapsize_rank < k)
        {
        	topKgraphs[heapsize_rank].dist = c;
    		topKgraphs[heapsize_rank].ind = num_graphs;
    		heapsize_rank++;
		}
    else{
        if(ok)
        {
            buildMaxHeapGraphs(topKgraphs);
            ok=0;
        }
        if(c < topKgraphs[0].dist)
        {
            topKgraphs[0].dist = c;
            topKgraphs[0].ind = num_graphs;
            max_heapifyGraph(0);
        }
    }
    num_graphs++;
}

void topK()
{
    if(num_topK>0)
        printf("\n");
    num_topK=1;
    for(int i=0 ;i < heapsize_rank ; i++){
        if(i < heapsize_rank-1)
            printf("%d ",topKgraphs[i].ind);
        else  printf("%d",topKgraphs[i].ind);
    }
}

int dijkstra(int g[][400])
{
    buildMinHeap(heap);
    int sum=0;
    while(heapsize > 0)
    {
        struct nodo nodoMin = heap[0];
		cancellaMin(heap);
        if(nodoMin.dist != INF)
            sum+= nodoMin.dist;
        for(int i=1;i<d;i++)
        {
            if(g[nodoMin.ind][i] !=INF && nodoMin.ind != i)
            {
                int ndis = nodoMin.dist + g[nodoMin.ind][i];
                int j=corr[i];

                if(g[0][i]>ndis)
                {
                    g[0][i] = ndis;
                    if(j!=-1)
                    {
                        heap[j].dist = ndis;
                        buildMinHeap(heap);
                    }
                }

            }

        }
    }
    return sum;
}
void min_heapify(int i){
    int left = 2*i+1;
    int right = 2*i+2;
    int min;
    int temp,tempInd;

    if(left <= (heapsize-1) && heap[left].dist < heap[i].dist)
        min = left;
    else min = i;

    if(right <= (heapsize-1) && heap[right].dist < heap[min].dist)
        min = right;
    if(min != i)
    {
        temp = heap[i].dist;
        tempInd = heap[i].ind;

        corr[tempInd] = min;
        corr[heap[min].ind] = i;

        heap[i].dist = heap[min].dist;
        heap[i].ind =  heap[min].ind;

        heap[min].dist = temp;
        heap[min].ind = tempInd;

        min_heapify(min);
    }

}

void max_heapifyGraph(int i){
    int left = 2*i+1;
    int right = 2*i+2;
    int max;
    int temp,tempInd;

    if(left <= (heapsize_rank-1) && topKgraphs[left].dist > topKgraphs[i].dist)
        max = left;
    else max = i;

    if(right <= (heapsize_rank-1) && topKgraphs[right].dist > topKgraphs[max].dist)
        max = right;
    if(max != i)
    {
        temp = topKgraphs[i].dist;
        tempInd = topKgraphs[i].ind;
        
        topKgraphs[i].dist = topKgraphs[max].dist;
        topKgraphs[i].ind =  topKgraphs[max].ind;
        
        topKgraphs[max].dist = temp;
        topKgraphs[max].ind = tempInd;
        
        max_heapifyGraph(max);
    }
}

void buildMinHeap(){
    int l;
    if(heapsize%2==0)
        l= (heapsize-1)/2;
    else l= ((heapsize-1)/2)-1;

    for(int i = l;i>=0;i--)
        min_heapify(i);
}
void buildMaxHeapGraphs(){
    int l;
    if(heapsize_rank%2==0)
        l= (heapsize_rank-1)/2;
    else l= ((heapsize_rank-1)/2)-1;

    for(int i = l;i>=0;i--)
        max_heapifyGraph(i);
}

void cancellaMin(){   
    heap[0].dist = heap[heapsize-1].dist;
    corr[heap[0].ind] = -1;
    heap[0].ind = heap[heapsize-1].ind;
    corr[heap[heapsize-1].ind] = 0;
    heapsize--;
    min_heapify(0);
}

