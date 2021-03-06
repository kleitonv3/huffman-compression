#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "huffman.h"

// Funcao main para rodar as funcoes
int main()
{
	int size, op = 0;
	char arq[50] = "NULL";
	char arq2[50] = "NULL";
	MatrizCompressao* mat = NULL;
	struct MinHeapNode* root = NULL;
	
	char arr[MAX_TREE_HT];
	int freq[MAX_TREE_HT];
	char *fluxo = (char*)malloc(MAX_SIZE_FLUXO*sizeof(char));	

	// HuffmanCodes(arr, freq, fluxo, size);

	do {
		printf("\n\n=-=-=-=- MENU -=-=-=-=\n");
		printf("[0] - Sair\n");
		printf("[1] - Comprimir arquivo\n");
		printf("[2] - Descomprimir arquivo\n");
		printf("[3] - Frequencia dos elementos\n");
		printf("[4] - Imprimir codigo de Huffman\n");
		
		printf("\nDigite sua escolha: ");
		scanf("%d", &op);

		switch (op)
		{
		case 0:
			printf("\nSaindo...\n");
			exit(1);
			break;

		case 1:
			printf("\nDigite o nome do arquivo de text (sem extensao): ");
			scanf("%s", arq);
			size = leArquivoTexto(arr, freq, fluxo, arq);

			if (size == -1) {
				exit(1);
			}

			root = buildHuffmanTree(arr, freq, size);
			mat = criaMatrizCompressao(root, size);

			printf("\nDigite o nome do arquivo de saida (sem extensao): ");
			scanf("%s", arq);
			escreveArquivoBinario(root, size, fluxo, arq);

			break;

		case 2:
			printf("\nDigite o nome do arquivo que vai ser descomprimido (sem extensao): ");
			scanf("%s", arq);
			printf("\nDigite o nome do arquivo de saida (sem extensao): ");
			scanf("%s", arq2);

			escreveArquivoTexto(arq, arq2);
			break;

		case 3:
			printf("\nA frequencia e o codigo de huffman sao mostrados nas funcoes de compressao e descompressao\n");
			break;

		case 4:
			printf("\nA frequencia e o codigo de huffman sao mostrados nas funcoes de compressao e descompressao\n");
			break;
		
		default:
			printf("\n\nOpcao nao reconhecida, tente novamente.\n\n");
			break;
		}
	} while (op != 0);

	return 0;
}

// Funcao para alocar um novo min heap node
struct MinHeapNode* newNode(char data, unsigned int freq)
{
	struct MinHeapNode* temp =
		(struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));

	temp->left = temp->right = NULL;
	temp->data = data;
	temp->freq = freq;
	

	return temp;
}

// Funcao para alocar um novo min heap (Collection de min heap node)
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

// Funcao para trocar dois node
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b)
{
	struct MinHeapNode* t = *a;
	*a = *b;
	*b = t;
}

// Funcao base para minHeapify (min heap)
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

// Funcao para checar se tem tamanho 1 ou nao
int isSizeOne(struct MinHeap* minHeap)
{
	return (minHeap->size == 1);
}

// Funcao base para extrair o noh de menor valor do heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap)
{
	struct MinHeapNode* temp = minHeap->array[0];
	minHeap->array[0] = minHeap->array[minHeap->size - 1];

	--minHeap->size;
	minHeapify(minHeap, 0);

	return temp;
}

// Funcao para inserir um novo noh no min heap
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

// Funcao base para criar um min heap
void buildMinHeap(struct MinHeap* minHeap)
{
	int n = minHeap->size - 1;
	int i;

	for (i = (n - 1) / 2; i >= 0; --i)
		minHeapify(minHeap, i);
}

// Funcao para printar um array de tamanho n
void printArr(int* arr, int n)
{
	int i;
	for (i = 0; i < n; ++i)
		printf("%d", arr[i]);

	printf("\n");
}

// Funcao para checar se o noh eh uma folha
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

// Funcao principal para criar a Huffman tree
struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size)
{
	struct MinHeapNode *left, *right, *top;

	//Cria um min heap de capacidade para receber o tamanho m??ximo
	
	struct MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);
	
	// Loop enquanto o tamanho nao eh 1
	while (!isSizeOne(minHeap)) {

		// Extrai os dois menores do heap
		left = extractMin(minHeap);
		right = extractMin(minHeap);

		// Cria um no interno que recebe a frequencia dos dois.
		// Faz os dois nos serem filhos a esquerda e direita do novo no.
		// Adiciona o novo no ao min heap.
		// '\\' eh um valor especial para os nos internos - nao usados
		top = newNode('\\', left->freq + right->freq);

		top->left = left;
		top->right = right;

		insertMinHeap(minHeap, top);
	}

	// O no que sobrou se torna a raiz e a arvore esta completa
	return extractMin(minHeap);
}

// Funcao para imprimir Huffman codes da raiz da arvore
void printCodes(struct MinHeapNode* root, int arr[], int top)
{
	// Atribui 0 para a esquerda e volta com recursao
	if (root->left) 
	{
		arr[top] = 0;
		printCodes(root->left, arr, top + 1);
	}

	// Atribui 1 para a esquerda e volta com recursao
	if (root->right) 
	{
		arr[top] = 1;
		printCodes(root->right, arr, top + 1);
	}

	// Se eh folha, deve imprimir o caracter e seu codigo do arr[]
	if (isLeaf(root)) {

		printf("%c : [%d]  \t", root->data, root->freq);
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

MatrizCompressao* criaMatrizCompressao(struct MinHeapNode* root, int size) {
	int arr[MAX_TREE_HT], top=0, tam=0;

	MatrizCompressao* matriz = (MatrizCompressao*)malloc(size*sizeof(MatrizCompressao));

	encontraCodigo(root, arr, top, matriz, &tam);

	return matriz;
}

int leArquivoTexto(char* data, int* freq, char* fluxo, char arq[]) {
	strcat(arq, ".txt");
	FILE* arquivo = fopen(arq, "r");
    if(arquivo == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo de texto.");
        return -1;
    }

    int size = 0;
	int size_fluxo = 0;
	int existe = 0;
    int caractere;
    
	while((caractere = fgetc(arquivo)) != EOF ) {
		// Adiciona caractere no fluxo
		if (size_fluxo >= MAX_SIZE_FLUXO) {
			fluxo = (char*)realloc(fluxo, sizeof(char)*MAX_SIZE_FLUXO*2);
			MAX_SIZE_FLUXO *= 2;
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

int escreveArquivoBinario (struct MinHeapNode* root, int size, char* fluxo, char arq[]) {
	int quant, quant_bytesText, posicao, posbyte, posbit, desloc, i, j, temp = 0;
	MatrizCompressao* mat;
	unsigned char* saida;
	unsigned char aux;
	ArchiveTree* tree;
	ArchiveHead head;
	int* codigoBin;
	FILE* arquivo;
	
	// Arvore de Huffman alterada para arquivo
	quant = quantidadeDeElementos(root);
	tree = (ArchiveTree*)malloc(quant*sizeof(ArchiveTree));
	formaHuffmanArqTree (root, tree, &temp, 0);

	// Matriz de compressao
	mat = criaMatrizCompressao(root, size);

	int arr[MAX_TREE_HT], top = 0;
	printf("\nFrequencia e codigo: \n");
	printCodes(root, arr, top);
	
	// Cabeca de informacoes
	head.tamArchiveTree = quant;
	head.bitsText = 0;
	head.quantCaracters = 0;
	for (int i=0; i<size; i++) {
		head.bitsText += mat[i].tamCodes*mat[i].freq;
		head.quantCaracters += mat[i].freq;
	}
    strcat(arq, ".bin");
	arquivo = fopen(arq, "wb");
    if(arquivo == NULL) {
        fprintf(stderr, "Erro ao criar aquivo binario.");
        return -1;
    }
	// Escreve cabeca e Huffman Archive tree
	fwrite(&head, sizeof(ArchiveHead), 1, arquivo);
	fwrite(tree, sizeof(ArchiveTree), quant, arquivo);
    
	// Gera e escreve o texto comprimido
	quant_bytesText = (int)ceil(head.bitsText/8.0); // Quantidade de bytes da saida comprimida
	saida = (unsigned char*)malloc(quant_bytesText*sizeof(unsigned char));
	
	memset(saida,0,quant_bytesText); // zera todos os bits do vetor saida

    // inicializa????o do contador de bit de gravacao o vetor saida
    posicao = 0;
    
    // inserir os valores de fluxo comprimidos no vetor saida
    for (i=0; i<head.quantCaracters; i++) {
        
        // Encontrando o codigo do elemento
		for (j=0; j<size; j++) {
			if (fluxo[i] == mat[j].data) {
				codigoBin = mat[j].codes;
				temp = mat[j].tamCodes;
				break;
			}
		}
		

		// Gravar no vetor de saida
		for (j=0; j<temp; j++) {
			posbyte = posicao/8;
			posbit = posicao%8;
			aux = codigoBin[j];
			aux = aux << 8 - posbit - 1;
			saida[posbyte] = saida[posbyte] | aux;
			posicao++;
			
		}
    }
    
	// Escreve vetor de saida
	fwrite(saida, sizeof(unsigned char), quant_bytesText, arquivo);

	fclose(arquivo);

	return 0;
}
void printTree (struct MinHeapNode* root, int level)
{
	int i;

	if (root != NULL) {
		printTree(root->right, level+1);

		for(i=0; i<level; i++)
			printf("\t");

		printf("[%c : %d]", root->data, root->freq);

		printTree(root->left, level+1);
	}
}
int escreveArquivoTexto (char arq_entrada[], char arq_saida[]) {
	int padrao, cont, size, quant_bytesText, posicao, posbyte, posbit, desloc, i, j, temp = 0;
	struct MinHeapNode *root, *ptr_aux;
	MatrizCompressao* mat;
	unsigned char* saida, *fluxo;
	unsigned char aux, data;
	ArchiveTree* tree;
	ArchiveHead head;
	int codigoBin[20];
	FILE* arquivo;

	strcat(arq_entrada, ".bin");
	arquivo = fopen(arq_entrada, "rb");
    if(arquivo == NULL) {
        fprintf(stderr, "Erro ao acessar aquivo comprimido.");
        return -1;
    }

	// Lendo a cabeca
	fread(&head, sizeof(head), 1, arquivo);

	
	
	// Lendo a Archive Tree e gerando os Nodes da arvore de Huffman
	tree = (ArchiveTree*)malloc(sizeof(ArchiveTree)*head.tamArchiveTree);
	fread(tree, sizeof(ArchiveTree), head.tamArchiveTree, arquivo);
	
	root = leHuffmanArqTree(root, tree, 0);
	
	// Lendo string comprimida
	quant_bytesText = (int)ceil(head.bitsText/8.0); // Quantidade de bytes da saida comprimida
	fluxo = (unsigned char*)malloc(sizeof(unsigned char)*quant_bytesText);
	fread(fluxo, sizeof(fluxo), quant_bytesText, arquivo);
	
	fclose(arquivo);

	int arr[MAX_TREE_HT], top = 0;
	printf("\nFrequencia e codigo: \n");
	printCodes(root, arr, top);
	
	// Gerando vetor de saida
	saida = (unsigned char*)malloc(head.quantCaracters*sizeof(unsigned char));
	
	//memset(saida,0,sizeof(unsigned char)*head.quantCaracters); // zera todos os bits do vetor saida
	
	posicao = 0;
	ptr_aux = root;
	cont = 0;
	padrao = 0;

	for (i=0; i<head.bitsText; i++) {
        // Encontrando o codigo do elemento	

		posbyte = posicao/8;
		posbit = posicao%8;
		aux = 1;
		aux = aux << 8 - posbit - 1;
		aux = fluxo[posbyte] & aux;
		aux = aux >> 8 - posbit - 1;
		posicao += 1;
		
		
		if (aux == 1) {
			ptr_aux = ptr_aux->right;
		} else {
			ptr_aux = ptr_aux->left;
		}

		if (isLeaf(ptr_aux)) {
			saida[cont] = ptr_aux->data;
			ptr_aux = root; // reinicia
			cont++;
		}
    }

	// Escrevendo no arquivo
	strcat(arq_saida, ".txt");
	arquivo = fopen(arq_saida, "w");
    if(arquivo == NULL) {
        fprintf(stderr, "Erro ao criar aquivo descomprimido.");
        return -1;
    }
	fwrite(saida, sizeof(unsigned char), head.quantCaracters, arquivo);
	fclose(arquivo);

	return 0;
}

