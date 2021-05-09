#include <stdio.h>
#include <stdlib.h>

// This constant can be avoided by explicitly
// calculating height of Huffman Tree
#define MAX_TREE_HT 100

// Huffman tree node
struct MinHeapNode {
	char data;
	unsigned int freq;
	struct MinHeapNode *left, *right;
};

// A Min Heap: Collection of
// min-heap (or Huffman tree) nodes
struct MinHeap {

	// Tamanho atual do min heap
	unsigned int size;

	// capacity of min heap
	unsigned int capacity;

	// Array de min heap node pointers
	struct MinHeapNode** array;
};

typedef struct HuffmanArqTree {
	char data;
	unsigned int freq;
	unsigned int posLeft, posRight;
} ArchiveTree;

// Fun��o para alocar um novo min heap node
struct MinHeapNode* newNode(char data, unsigned int freq)
{
	struct MinHeapNode* temp =
		(struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));

	temp->left = temp->right = NULL;
	temp->data = data;
	temp->freq = freq;

	return temp;
}

// Fun��o para alocar um novo min heap (Collection de min heap node)
struct MinHeap* createMinHeap(unsigned int capacity)
{
	struct MinHeap* minHeap
		= (struct MinHeap*)malloc(sizeof(struct MinHeap));

	// current size is 0
	minHeap->size = 0;

	minHeap->capacity = capacity;

	minHeap->array = (struct MinHeapNode**)malloc(
		minHeap->capacity * sizeof(struct MinHeapNode*));
	return minHeap;
}

// Fun��o para trocar dois node
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b)
{
	struct MinHeapNode* t = *a;
	*a = *b;
	*b = t;
}

// Fun��o base para minHeapify (min heap)
void minHeapify(struct MinHeap* minHeap, int idx)
{
	int smallest = idx;
	int left = 2 * idx + 1;
	int right = 2 * idx + 2;

	if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
		smallest = left;

	if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
		smallest = right;

	if (smallest != idx) 
	{
		swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
		minHeapify(minHeap, smallest);
	}
}

// Fun��o para checar se tem tamanho 1 ou n�o
int isSizeOne(struct MinHeap* minHeap)
{
	return (minHeap->size == 1);
}

// Fun��o base para extrair o n� de menor valor do heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap)
{
	struct MinHeapNode* temp = minHeap->array[0];
	minHeap->array[0] = minHeap->array[minHeap->size - 1];

	--minHeap->size;
	minHeapify(minHeap, 0);

	return temp;
}

// Fun��o para inserir um novo n� no min heap
void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode)
{
	++minHeap->size;
	int i = minHeap->size - 1;

	while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) 
	{
		minHeap->array[i] = minHeap->array[(i - 1) / 2];
		i = (i - 1) / 2;
	}

	minHeap->array[i] = minHeapNode;
}

// Fun��o base para criar um min heap
void buildMinHeap(struct MinHeap* minHeap)
{
	int n = minHeap->size - 1;
	int i;

	for (i = (n - 1) / 2; i >= 0; --i)
		minHeapify(minHeap, i);
}

// Fun��o para printar um array de tamanho n
void printArr(int arr[], int n)
{
	int i;
	for (i = 0; i < n; ++i)
		printf("%d", arr[i]);

	printf("\n");
}

// Fun��o para checar se o n� � uma folha
int isLeaf(struct MinHeapNode* root)
{
	return !(root->left) && !(root->right);
}

// Cria um min heap de capacidade de tamanho de todos os char em data[]
struct MinHeap* createAndBuildMinHeap(char data[], int freq[], int size)
{
	struct MinHeap* minHeap = createMinHeap(size);

	for (int i = 0; i < size; ++i)
		minHeap->array[i] = newNode(data[i], freq[i]);

	minHeap->size = size;
	buildMinHeap(minHeap);

	return minHeap;
}

// Fun��o principal para criar a Huffman tree
struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size)
{
	struct MinHeapNode *left, *right, *top;

	// Step 1: Create a min heap of capacity
	// equal to size. Initially, there are
	// modes equal to size.
	struct MinHeap* minHeap
		= createAndBuildMinHeap(data, freq, size);

	// Iterate while size of heap doesn't become 1
	while (!isSizeOne(minHeap)) {

		// Step 2: Extract the two minimum
		// freq items from min heap
		left = extractMin(minHeap);
		right = extractMin(minHeap);

		// Step 3: Create a new internal
		// node with frequency equal to the
		// sum of the two nodes frequencies.
		// Make the two extracted node as
		// left and right children of this new node.
		// Add this node to the min heap
		// '\' is a special value for internal nodes, not
		// used
		top = newNode('\\', left->freq + right->freq);

		top->left = left;
		top->right = right;

		insertMinHeap(minHeap, top);
	}

	// Step 4: The remaining node is the
	// root node and the tree is complete.
	return extractMin(minHeap);
}

// Fun��o para imprimir Huffman codes da raiz da �rvore
void printCodes(struct MinHeapNode* root, int arr[], int top)
{
	// Atribui 0 para a esquerda e volta com recurs�o
	if (root->left) 
	{
		arr[top] = 0;
		printCodes(root->left, arr, top + 1);
	}

	// Atribui 1 para a esquerda e volta com recurs�o
	if (root->right) 
	{
		arr[top] = 1;
		printCodes(root->right, arr, top + 1);
	}

	// Se � folha, deve imprimir o caracter e seu codigo do arr[]
	if (isLeaf(root)) 
	{
		printf("%c: ", root->data);
		printArr(arr, top);
	}
}

unsigned int formaHuffmanArqTree (struct MinHeapNode* root, ArchiveTree* tree, int *pos, int aux) {
	// Primeiro formamos a arvore que vai ser escrita a partir da HuffmanTree
	// Escreve raiz -> escreve sub-arvore esquerda -> escreve sub-arvore direita
	// pos eh a proxima posicao de escrita
	// aux eh a posicao atual de acesso no vetor

	tree[(*pos)].data = root->data;
	tree[(*pos)].freq = root->freq;
	(*pos) += 1;

	if (root->left) {
		tree[aux].posLeft = formaHuffmanArqTree(root->left, tree, pos, (*pos));
	} else {
		tree[aux].posLeft = 0; // Indica um ponteiro para NULL
	}

	if (root->right) {
		tree[aux].posRight = formaHuffmanArqTree(root->right, tree, pos, (*pos));
	} else {
		tree[aux].posRight = 0; // Indica um ponteiro para NULL
	}

	return aux;
}

void escreveHuffmanTree(struct MinHeapNode* root, int size) {
	ArchiveTree* tree = (ArchiveTree*)malloc(size*sizeof(ArchiveTree));
	int aux = 0;
	formaHuffmanArqTree (root, tree, &aux, 0);
	// printf("\nPrimeiro node [%c : %d] %d %d\n", tree[0].data, tree[0].freq, tree[0].posLeft, tree[0].posRight);
}

struct MinHeapNode* leHuffmanArqTree(struct MinHeapNode* root, ArchiveTree* tree, int aux) {
	
	root = newNode(tree[aux].data, tree[aux].freq);

	if (tree[aux].posLeft != 0) { // Se aponta para a raiz (tree[0]) entao e NULL
		root->left = leHuffmanArqTree(root, tree, tree[aux].posLeft);
	} else {
		root->left = NULL;
	}

	if (tree[aux].posRight != 0) { // Se aponta para a raiz (tree[0]) entao e NULL
		root->right = leHuffmanArqTree(root, tree, tree[aux].posRight);
	} else {
		root->right = NULL;
	}

	return root;
}

// Fun��o principal para criar a Huffman Tree e imprimir os c�digos
void HuffmanCodes(char data[], int freq[], int size)
{
	// Contruir a �rvore Huffman 
	struct MinHeapNode* root = buildHuffmanTree(data, freq, size);

	// Imprime a �rvore
	int arr[MAX_TREE_HT], top = 0;

	escreveHuffmanTree(root, MAX_TREE_HT);

	printCodes(root, arr, top);
}

// Fun��o main para rodar as fun��es
int main()
{
	char arr[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g' };
	int freq[] = { 5, 9, 12, 13, 16, 45, 50 };

	int size = sizeof(arr) / sizeof(arr[0]);

	HuffmanCodes(arr, freq, size);

	return 0;
}
