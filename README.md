# Compactador de Dados Huffman em C

Este projeto é uma implementação do **Algoritmo de Compactação de Huffman** desenvolvida do zero em Linguagem C. O objetivo é demonstrar o funcionamento prático da codificação de tamanho variável sem perdas (*lossless data compression*).

## Funcionalidades

- **Tabela de Frequência:** Contagem dinâmica da ocorrência de caracteres ASCII.
- **Fila de Prioridade (Min-Heap):** Organização eficiente dos nós de menor frequência.
- **Árvore de Huffman:** Construção bottom-up da árvore binária de prefixos.
- **Geração de Códigos:** Atribuição de caminhos binários (`0` para esquerda, `1` para direita).
- **Compactação e Descompactação:** Conversão do texto original em string de bits e reconstrução completa via percurso na árvore.
- **Estatísticas de Compressão:** Cálculo percentual do ganho de memória obtido.

##  Estrutura do Algoritmo

1. Contagem de frequência dos caracteres no texto original.
2. Inserção dos nós folha em uma Min-Heap.
3. Combinação iterativa dos dois menores nós para formar a Árvore de Huffman.
4. Traessia recursiva para gerar a tabela de códigos binários.
5. Codificação do texto e decodificação via travessia de folha.

## Como Compilar e Executar

### Pré-requisitos
- Compilador GCC instalado.

### Passos

1. Clone o repositório:
   ```bash
   git clone [https://github.com/DevEduLP/Compactador-de-Dados-Huffman-em-C.git]
   cd huffman-c
   ```

2. Compile o código:
   ```bash
   gcc -o huffman src/huffman.c
   ```

3. Execute o programa:
   ```bash
   ./huffman
   ```

## Exemplo de Saída

```text
Tabela de Codigos de Huffman:
  'a' -> 0
  'b' -> 101
  'r' -> 11
  'c' -> 1000
  'd' -> 1001

=== Estatisticas de Compactacao ===
Texto original: "abracadabra"
Tamanho original: 11 bytes (88 bits)
Texto compactado: 01011101000010010101110
Tamanho compactado: 23 bits
Economia de memoria: 73.86%

Texto Descompactado: abracadabra
```

## 👤 Autor

Desenvolvido por **Seu Nome**.
