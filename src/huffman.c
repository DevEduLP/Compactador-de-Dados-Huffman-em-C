#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição da estrutura de cada nó da árvore de Huffman
typedef struct Node {
    unsigned char caracter;
    int frequencia;
    struct Node *esquerda;
    struct Node *direita;
} Node;

// Estrutura que representa a fila de prioridade (min-heap)
typedef struct MinHeap {
    int tamanho;
    int capacidade;
    Node** array;
} MinHeap;

// Protótipos das funções
Node* criar_no(unsigned char caracter, int frequencia);
MinHeap* criar_min_heap(int capacidade);
void trocar_nos(Node** a, Node** b);
void min_heapify(MinHeap* heap, int idx);
void inserir_min_heap(MinHeap* heap, Node* no);
Node* remover_min(MinHeap* heap);
MinHeap* criar_e_preencher_heap(int frequencias[]);
Node* construir_arvore_huffman(int frequencias[]);
void gerar_codigos(Node* raiz, int caminho[], int topo, char codigos[256][256]);
void imprimir_arvore(Node* raiz, int nivel);
void calcular_economia(char texto[], char texto_compactado[]);
void compactar_texto(char texto[], char codigos[256][256], char texto_compactado[]);
void descompactar_texto(char texto_compactado[], Node* raiz);
void liberar_arvore(Node* raiz);

int main() {
    char texto[] = "abracadabra";
    int frequencias[256] = {0};

    // 1. Contagem de frequências
    for (int i = 0; texto[i] != '\0'; i++) {
        frequencias[(unsigned char)texto[i]]++;
    }

    // 2. Construção da Árvore de Huffman
    Node* raiz = construir_arvore_huffman(frequencias);

    // 3. Matriz para guardar os códigos e vetor temporário de caminho
    char codigos[256][256] = {{0}};
    int caminho[256];

    // 4. Gera a tabela de códigos
    gerar_codigos(raiz, caminho, 0, codigos);

    // Exibe os códigos gerados para cada caractere
    printf("Tabela de Codigos de Huffman:\n");
    for (int i = 0; i < 256; i++) {
        if (frequencias[i] > 0) {
            printf("  '%c' -> %s\n", i, codigos[i]);
        }
    }

    // 5. Compacta o texto e calcula a economia
    char texto_compactado[1024];
    compactar_texto(texto, codigos, texto_compactado);
    calcular_economia(texto, texto_compactado);

    // 6. Descompacta o texto para validar o algoritmo
    descompactar_texto(texto_compactado, raiz);

    // 7. Libera a memória alocada para a árvore
    liberar_arvore(raiz);

    return 0;
}

// Criar um novo nó na memória
Node* criar_no(unsigned char caracter, int frequencia) {
    Node* novo = (Node*) malloc(sizeof(Node));
    novo->caracter = caracter;
    novo->frequencia = frequencia;
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

// Alocar e inicializar a Min-Heap
MinHeap* criar_min_heap(int capacidade) {
    MinHeap* heap = (MinHeap*) malloc(sizeof(MinHeap));
    heap->tamanho = 0;
    heap->capacidade = capacidade;
    heap->array = (Node**) malloc(heap->capacidade * sizeof(Node*));
    return heap;
}

// Trocar dois ponteiros de nó de posição
void trocar_nos(Node** a, Node** b) {
    Node* temp = *a;
    *a = *b;
    *b = temp;
}

// Reorganizar o heap para baixo
void min_heapify(MinHeap* heap, int idx) {
    int menor = idx;
    int esquerda = 2 * idx + 1;
    int direita = 2 * idx + 2;

    if (esquerda < heap->tamanho &&
        heap->array[esquerda]->frequencia < heap->array[menor]->frequencia) {
        menor = esquerda;
    }
    if (direita < heap->tamanho &&
        heap->array[direita]->frequencia < heap->array[menor]->frequencia) {
        menor = direita;
    }
    if (menor != idx) {
        trocar_nos(&heap->array[menor], &heap->array[idx]);
        min_heapify(heap, menor);
    }
}

// Inserir nó no heap
void inserir_min_heap(MinHeap* heap, Node* no) {
    heap->tamanho++;
    int i = heap->tamanho - 1;

    while (i > 0 && no->frequencia < heap->array[(i - 1) / 2]->frequencia) {
        heap->array[i] = heap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }

    heap->array[i] = no;
}

// Extrair o menor nó do heap
Node* remover_min(MinHeap* heap) {
    if (heap->tamanho == 0) return NULL;

    Node* raiz = heap->array[0];
    heap->array[0] = heap->array[heap->tamanho - 1];
    heap->tamanho--;

    min_heapify(heap, 0);

    return raiz;
}

// Preencher heap com os caracteres do texto
MinHeap* criar_e_preencher_heap(int frequencias[]) {
    MinHeap* heap = criar_min_heap(256);

    for (int i = 0; i < 256; i++) {
        if (frequencias[i] > 0) {
            Node* novo = criar_no((unsigned char)i, frequencias[i]);
            inserir_min_heap(heap, novo);
        }
    }

    return heap;
}

// Construir a árvore completa
Node* construir_arvore_huffman(int frequencias[]) {
    MinHeap* heap = criar_e_preencher_heap(frequencias);

    while (heap->tamanho > 1) {
        Node* esquerda = remover_min(heap);
        Node* direita = remover_min(heap);

        Node* pai = criar_no('$', esquerda->frequencia + direita->frequencia);
        pai->esquerda = esquerda;
        pai->direita = direita;

        inserir_min_heap(heap, pai);
    }

    Node* raiz = remover_min(heap);

    free(heap->array);
    free(heap);

    return raiz;
}

// Percorrer a árvore gerando os códigos de bits
void gerar_codigos(Node* raiz, int caminho[], int topo, char codigos[256][256]) {
    if (raiz == NULL) return;

    if (raiz->esquerda != NULL) {
        caminho[topo] = 0;
        gerar_codigos(raiz->esquerda, caminho, topo + 1, codigos);
    }

    if (raiz->direita != NULL) {
        caminho[topo] = 1;
        gerar_codigos(raiz->direita, caminho, topo + 1, codigos);
    }

    if (raiz->esquerda == NULL && raiz->direita == NULL) {
        for (int i = 0; i < topo; i++) {
            codigos[raiz->caracter][i] = caminho[i] + '0';
        }
        codigos[raiz->caracter][topo] = '\0';
    }
}

void calcular_economia(char texto[], char texto_compactado[]) {
    int tamanho_original_caracteres = strlen(texto);
    int bits_originais = tamanho_original_caracteres * 8;
    int bits_compactados = strlen(texto_compactado);

    float economia = (1.0f - ((float)bits_compactados / (float)bits_originais)) * 100.0f;

    printf("\n=== Estatisticas de Compactacao ===\n");
    printf("Texto original: \"%s\"\n", texto);
    printf("Tamanho original: %d bytes (%d bits)\n", tamanho_original_caracteres, bits_originais);
    printf("Texto compactado: %s\n", texto_compactado);
    printf("Tamanho compactado: %d bits\n", bits_compactados);
    printf("Economia de memoria: %.2f%%\n", economia);
}

void compactar_texto(char texto[], char codigos[256][256], char texto_compactado[]) {
    texto_compactado[0] = '\0';
    for (int i = 0; texto[i] != '\0'; i++) {
        unsigned char c = (unsigned char)texto[i];
        strcat(texto_compactado, codigos[c]);
    }
}

void descompactar_texto(char texto_compactado[], Node* raiz) {
    Node* atual = raiz;
    printf("\nTexto Descompactado: ");

    for (int i = 0; texto_compactado[i] != '\0'; i++) {
        if (texto_compactado[i] == '0') {
            atual = atual->esquerda;
        } else if (texto_compactado[i] == '1') {
            atual = atual->direita;
        }

        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("%c", atual->caracter);
            atual = raiz;
        }
    }
    printf("\n");
}

void liberar_arvore(Node* raiz) {
    if (raiz == NULL) return;
    liberar_arvore(raiz->esquerda);
    liberar_arvore(raiz->direita);
    free(raiz);
}
