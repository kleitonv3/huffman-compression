// C program for Huffman Coding
#include <stdio.h>
#include <stdlib.h>

// This constant can be avoided by explicitly
// calculating height of Huffman Tree
#define MAX_TREE_HT 200

// A Huffman tree node
struct MinHeapNode {

	// One of the input characters
	char data;

	// Frequency of the character
	unsigned int freq;

	// Left and right child of this node
	struct MinHeapNode *left, *right;
};

// A Min Heap: Collection of
// min-heap (or Huffman tree) nodes
struct MinHeap {

	// Current size of min heap
	unsigned int size;

	// capacity of min heap
	unsigned int capacity;

	// Array of minheap node pointers
	struct MinHeapNode** array;
};

typedef struct HuffmanArqTree {
	char data;
	unsigned int freq;
	unsigned int posLeft, posRight;
} ArchiveTree;

typedef struct CompressionNode {
	char data; // caractere
	int freq; // frequencia do caractere
	int tamCodes;
	int* codes; // codigo binario
} MatrizCompressao;

typedef struct cabecaArq {
	unsigned int tamArchiveTree; // Tamanho da arvore de Huffman pro arquivo
	unsigned long int bitsText; // Quantidades de bits no texto compresso
} ArchiveHead;

// A utility function allocate a new
// min heap node with given character
// and frequency of the character
struct MinHeapNode* newNode(char data, unsigned int freq)
{
	struct MinHeapNode* temp = (struct MinHeapNode*)malloc(
		sizeof(struct MinHeapNode));

	temp->left = temp->right = NULL;
	temp->data = data;
	temp->freq = freq;
	

	return temp;
}

// A utility function to create
// a min heap of given capacity
struct MinHeap* createMinHeap(unsigned int capacity)

{
	struct MinHeap* minHeap
		= (struct MinHeap*)malloc(sizeof(struct MinHeap));

	// current size is 0
	printf("Antes\n");
	minHeap->size = 0;
	printf("Depos\n");


	minHeap->capacity = capacity;
	
	minHeap->array = (struct MinHeapNode**)malloc((minHeap->capacity)*sizeof(struct MinHeapNode*)); // Estava dando erro sem o 
	
	return minHeap;
}

// A utility function to
// swap two min heap nodes
void swapMinHeapNode(struct MinHeapNode** a,
					struct MinHeapNode** b)

{

	struct MinHeapNode* t = *a;
	*a = *b;
	*b = t;
}

// The standard minHeapify function.
void minHeapify(struct MinHeap* minHeap, int idx)

{

	int smallest = idx;
	int left = 2 * idx + 1;
	int right = 2 * idx + 2;

	if (left < minHeap->size
		&& minHeap->array[left]->freq
			< minHeap->array[smallest]->freq)
		smallest = left;

	if (right < minHeap->size
		&& minHeap->array[right]->freq
			< minHeap->array[smallest]->freq)
		smallest = right;

	if (smallest != idx) {
		swapMinHeapNode(&minHeap->array[smallest],
						&minHeap->array[idx]);
		minHeapify(minHeap, smallest);
	}
}

// A utility function to check
// if size of heap is 1 or not
int isSizeOne(struct MinHeap* minHeap)
{

	return (minHeap->size == 1);
}

// A standard function to extract
// minimum value node from heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap)

{

	struct MinHeapNode* temp = minHeap->array[0];
	minHeap->array[0] = minHeap->array[minHeap->size - 1];

	--minHeap->size;
	minHeapify(minHeap, 0);

	return temp;
}

// A utility function to insert
// a new node to Min Heap
void insertMinHeap(struct MinHeap* minHeap,
				struct MinHeapNode* minHeapNode)

{

	++minHeap->size;
	int i = minHeap->size - 1;

	while (i
		&& minHeapNode->freq
				< minHeap->array[(i - 1) / 2]->freq) {

		minHeap->array[i] = minHeap->array[(i - 1) / 2];
		i = (i - 1) / 2;
	}

	minHeap->array[i] = minHeapNode;
}

// A standard function to build min heap
void buildMinHeap(struct MinHeap* minHeap)

{

	int n = minHeap->size - 1;
	int i;

	for (i = (n - 1) / 2; i >= 0; --i)
		minHeapify(minHeap, i);
}

// A utility function to print an array of size n
void printArr(int* arr, int n)
{
	int i;
	for (i = 0; i < n; ++i)
		printf("%d", arr[i]);

	printf("\n");
}

// Utility function to check if this node is leaf
int isLeaf(struct MinHeapNode* root)

{

	return !(root->left) && !(root->right);
}

// Creates a min heap of capacity
// equal to size and inserts all character of
// data[] in min heap. Initially size of
// min heap is equal to capacity
struct MinHeap* createAndBuildMinHeap(char data[],
									unsigned int freq[], int size)

{
	struct MinHeap* minHeap = createMinHeap(size);
	
	for (int i = 0; i < size; i++) {
		
		minHeap->array[i] = newNode(data[i], freq[i]);
		
	}
	
	minHeap->size = size;
	buildMinHeap(minHeap);

	return minHeap;
}

// The main function that builds Huffman tree
struct MinHeapNode* buildHuffmanTree(char data[], unsigned int freq[], int size) {
	struct MinHeapNode *left, *right, *top;

	// Step 1: Create a min heap of capacity
	// equal to size. Initially, there are
	// modes equal to size.
	
	struct MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);
	
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

// Prints huffman codes from the root of Huffman Tree.
// It uses arr[] to store codes
void printCodes(struct MinHeapNode* root, int arr[],
				int top)

{

	// Assign 0 to left edge and recur
	if (root->left) {

		arr[top] = 0;
		printCodes(root->left, arr, top + 1);
	}

	// Assign 1 to right edge and recur
	if (root->right) {

		arr[top] = 1;
		printCodes(root->right, arr, top + 1);
	}

	// If this is a leaf node, then
	// it contains one of the input
	// characters, print the character
	// and its code from arr[]
	if (isLeaf(root)) {

		printf("[%c : %d] ", root->data, root->freq);
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

void encontraCodigo(struct MinHeapNode* root, int arr[], int top, MatrizCompressao* matriz, int* tam) {
	// Assign 0 to left edge and recur
	if (root->left) {

		arr[top] = 0;
		encontraCodigo(root->left, arr, top + 1, matriz, tam);
	}

	// Assign 1 to right edge and recur
	if (root->right) {

		arr[top] = 1;
		encontraCodigo(root->right, arr, top + 1, matriz, tam);
	}


	// If this is a leaf node, then
	// it contains one of the input
	if (isLeaf(root)) {
		matriz[(*tam)].data = root->data;
		matriz[(*tam)].freq = root->freq;
		// Reescreve arr com o caminho ate aquele char na arvore de Huffman e 
		// retorna o tamanho do codigo binario
		matriz[(*tam)].tamCodes = top;
		
		// Faz malloc do vetor q vai guardar o codigo do char
		matriz[(*tam)].codes = (int*)malloc(top*sizeof(int));
		for (int j=0; j<top; j++) {
			matriz[(*tam)].codes[j] = arr[j];
		}
		(*tam)++;
	}
}

MatrizCompressao* criaMatrizCompressao(struct MinHeapNode* root, char data[], int size) {
	int arr[MAX_TREE_HT], top=0, tam=0;

	MatrizCompressao* matriz = (MatrizCompressao*)malloc(size*sizeof(MatrizCompressao));

	encontraCodigo(root, arr, top, matriz, &tam);

	return matriz;
}

int leArquivoTexto(char* data, int* freq, char* fluxo) {
	FILE* arquivo = fopen("arquivo.txt", "r");
    if(arquivo == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo.txt.");
        return -1;
    }

    int size = 0;
	int size_fluxo = 0;
	int existe = 0;
    int caractere;
    
	while((caractere = fgetc(arquivo)) != EOF ) {
		// Adiciona caractere no fluxo
		if (size_fluxo >= sizeof(fluxo)) {
			fluxo = (char*)realloc(fluxo, sizeof(fluxo)*2);
		}
		fluxo[size_fluxo] = caractere;
		size_fluxo++;

		// Verificar se ja existe esse caractere na estrutura
		existe = 0;
		for (int i=0; i<size; i++) {
			if (data[i] == caractere) {
				freq[i]++;
				existe = 1;
			}
		}
		// Se nao existe adiciona
		if (existe == 0) {
			data[size] = caractere;
			freq[size] = 1;
			size++;
		}

	}

    fclose(arquivo);
    return size;
}

unsigned char* saidaComprimida () {

}

int escreveArquivoBinario (struct MinHeapNode* root, char* data, int size, char* fluxo) {
	ArchiveTree* tree = (ArchiveTree*)malloc(size*sizeof(ArchiveTree));
	int aux = 0;
	formaHuffmanArqTree (root, tree, &aux, 0);

	MatrizCompressao* mat = criaMatrizCompressao(root, data, size);

	ArchiveHead head;
	head.tamArchiveTree = sizeof(tree);
	head.bitsText = 0;
	for (int i=0; i<size; i++) {
		head.bitsText += mat[i].tamCodes*mat[i].freq;
	}

	FILE* arquivo = fopen("saida.bin", "wb");
    if(arquivo == NULL) {
        fprintf(stderr, "Erro ao criar aquivo binario.");
        return -1;
    }
	// Escreve cabeca e Huffman Archive tree
	fwrite(&head, sizeof(ArchiveHead), 1, arquivo);
	fwrite(tree, sizeof(tree), 1, arquivo);
	return 0;
}

// The main function that builds a
// Huffman Tree and print codes by traversing
// the built Huffman Tree
void HuffmanCodes(char data[], int freq[], char* fluxo, int size)

{
	
	// Construct Huffman Tree
	struct MinHeapNode* root = buildHuffmanTree(data, freq, size);
	
	// Print Huffman codes using
	// the Huffman tree built above
	int arr[MAX_TREE_HT], top = 0;

	// escreveHuffmanTree(root, MAX_TREE_HT);
	MatrizCompressao* mat = criaMatrizCompressao(root, data, size);
	printf("Informacoes na estrutura: \n");
	for (int i=0; i<size; i++) {
		printf("Caracter: %c, Codigo: ", mat[i].data);
		printArr(mat[i].codes, mat[i].tamCodes);
	}

	printCodes(root, arr, top);

	escreveArquivoBinario(root, data, size, fluxo);
}

// Driver code
int main()
{

	char arr[MAX_TREE_HT];
	int freq[MAX_TREE_HT];
	char *fluxo = (char*)malloc(MAX_TREE_HT*sizeof(char));

	int size = leArquivoTexto(arr, freq, fluxo);

	HuffmanCodes(arr, freq, fluxo, size);

	return 0;
}
