#include <stdio.h>
#include <stdlib.h>

// This constant can be avoided by explicitly
// calculating height of Huffman Tree
#define MAX_TREE_HT 200

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
	
	minHeap->array = (struct MinHeapNode**)malloc((minHeap->capacity)*sizeof(struct MinHeapNode*)); // Estava dando erro sem o 
	
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
void printArr(int* arr, int n)
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
	
	for (int i = 0; i < size; i++) {
		
		minHeap->array[i] = newNode(data[i], freq[i]);
		
	}
	
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
	if (isLeaf(root)) {

		printf("[%c : %d] ", root->data, root->freq);
		printArr(arr, top);
	}
}

// Calcula quantidade de Nodes na arvore
int quantidadeDeElementos (struct MinHeapNode* root) {
	if (root == NULL) return 0;
	else return (1 + quantidadeDeElementos(root->left) + quantidadeDeElementos(root->right));
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
			fluxo = (char*)realloc(fluxo, sizeof(char)*size_fluxo*2);
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
	int quant, aux = 0;
	quant = quantidadeDeElementos(root);
	ArchiveTree* tree = (ArchiveTree*)malloc(quant*sizeof(ArchiveTree));
	formaHuffmanArqTree (root, tree, &aux, 0);

	MatrizCompressao* mat = criaMatrizCompressao(root, data, size);
	ArchiveHead head;
	head.tamArchiveTree = quant;
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
	fwrite(tree, sizeof(tree), quant, arquivo);

	rewind(arquivo);
	fread(&head, sizeof(head), 1, arquivo);
	fread(tree, sizeof(tree), quant, arquivo);
	return 0;
}

// Fun��o principal para criar a Huffman Tree e imprimir os c�digos
void HuffmanCodes(char data[], int freq[], char* fluxo, int size)
{
	// Contruir a �rvore Huffman 
	struct MinHeapNode* root = buildHuffmanTree(data, freq, size);

	// Imprime a �rvore
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

// Fun��o main para rodar as fun��es
int main()
{

	char arr[MAX_TREE_HT];
	int freq[MAX_TREE_HT];
	char *fluxo = (char*)malloc(MAX_TREE_HT*sizeof(char));

	int size = leArquivoTexto(arr, freq, fluxo);

	HuffmanCodes(arr, freq, fluxo, size);

	return 0;
}
