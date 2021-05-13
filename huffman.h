#ifndef __HUFFMAN_H__
#define __HUFFMAN_H__

// This constant can be avoided by explicitly
// calculating height of Huffman Tree
#define MAX_TREE_HT 5000
int MAX_SIZE_FLUXO = MAX_TREE_HT;
// Huffman tree node
struct MinHeapNode {
	unsigned char data;
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

typedef struct CompressionNode {
	char data; // caractere
	int freq; // frequencia do caractere
	int tamCodes;
	int* codes; // codigo binario
} MatrizCompressao;

typedef struct cabecaArq {
	// Tamanho da arvore de Huffman pro arquivo
	unsigned int tamArchiveTree;
	// Quantidades de bits no texto compresso
	unsigned long int bitsText;
	// Quantidade de caracteres gravados
	unsigned int quantCaracters;
} ArchiveHead;

// Fun��o para alocar um novo min heap node
struct MinHeapNode* newNode(char data, unsigned int freq);

// Fun��o para alocar um novo min heap (Collection de min heap node)
struct MinHeap* createMinHeap(unsigned int capacity);

// Fun��o para trocar dois node
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b);

// Fun��o base para minHeapify (min heap)
void minHeapify(struct MinHeap* minHeap, int idx);

// Fun��o para checar se tem tamanho 1 ou n�o
int isSizeOne(struct MinHeap* minHeap);

// Fun��o base para extrair o n� de menor valor do heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap);

// Fun��o para inserir um novo n� no min heap
void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode);

// Fun��o base para criar um min heap
void buildMinHeap(struct MinHeap* minHeap);

// Fun��o para printar um array de tamanho n
void printArr(int* arr, int n);

// Fun��o para checar se o n� � uma folha
int isLeaf(struct MinHeapNode* root);

// Cria um min heap de capacidade de tamanho de todos os char em data[]
struct MinHeap* createAndBuildMinHeap(char data[], int freq[], int size);

// Fun��o principal para criar a Huffman tree
struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size);

// Fun��o para imprimir Huffman codes da raiz da �rvore
void printCodes(struct MinHeapNode* root, int arr[], int top);

// Calcula quantidade de Nodes na arvore
int quantidadeDeElementos (struct MinHeapNode* root);

unsigned int formaHuffmanArqTree (struct MinHeapNode* root, ArchiveTree* tree, int *pos, int aux);

struct MinHeapNode* leHuffmanArqTree(struct MinHeapNode* root, ArchiveTree* tree, int aux);

void encontraCodigo(struct MinHeapNode* root, int arr[], int top, MatrizCompressao* matriz, int* tam);

MatrizCompressao* criaMatrizCompressao(struct MinHeapNode* root, int size);

int leArquivoTexto(char* data, int* freq, char* fluxo, char arq[]);

int escreveArquivoBinario (struct MinHeapNode* root, int size, char* fluxo, char arq[]);

void printTree (struct MinHeapNode* root, int level);

int escreveArquivoTexto (char arq_entrada[], char arq_saida[]);

#endif // __HUFFMAN_H__