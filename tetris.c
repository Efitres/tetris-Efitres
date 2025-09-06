// ============================================================================
//         PROJETO TETRIS STACK - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS: 
// (NIVEL NOVATO) 1 - implementar um programa em C que simula a fila de peças futuras do Tetris Stack.
                    // As peças têm um tipo (como 'I', 'O', 'T', 'L’), que representa suas formas e um identificador
                    // numérico (id) exclusivo. O programa oferecerá ao jogador a opção de visualizar a fila, jogar
                    // uma peça (remoção da frente) ou adicionar uma nova peça ao final da fila.


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ================================
// Constantes
// ================================
#define TAM_FILA 5   // Tamanho fixo da fila de peças

// ================================
// Estruturas
// ================================
typedef struct {
    char nome;  // Tipo da peça ('I', 'O', 'T', 'L')
    int id;     // Identificador único
} Peca;

typedef struct {
    Peca fila[TAM_FILA];
    int inicio;
    int fim;
    int qtd;
} Fila;

// ================================
// Protótipos de funções
// ================================
void inicializarFila(Fila *f);
int filaCheia(Fila *f);
int filaVazia(Fila *f);
void enqueue(Fila *f, Peca p);
Peca dequeue(Fila *f);
void exibirFila(Fila *f);
Peca gerarPeca(int id);

// ================================
// Função principal
// ================================
int main() {
    Fila f;
    inicializarFila(&f);

    int opcao;
    int idGlobal = 0; // contador global de ids

    // Inicializa a fila com 5 peças
    for (int i = 0; i < TAM_FILA; i++) {
        enqueue(&f, gerarPeca(idGlobal++));
    }

    do {
        printf("\n===== MENU =====\n");
        printf("1 - Jogar peça (dequeue)\n");
        printf("2 - Inserir nova peça (enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                if (!filaVazia(&f)) {
                    Peca removida = dequeue(&f);
                    printf("\nPeça jogada: [%c %d]\n", removida.nome, removida.id);
                } else {
                    printf("\nFila vazia! Não há peças para jogar.\n");
                }
                exibirFila(&f);
                break;

            case 2:
                if (!filaCheia(&f)) {
                    enqueue(&f, gerarPeca(idGlobal++));
                } else {
                    printf("\nFila cheia! Não é possível inserir nova peça.\n");
                }
                exibirFila(&f);
                break;

            case 0:
                printf("\nSaindo do jogo...\n");
                break;

            default:
                printf("\nOpção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}

// ================================
// Implementação das funções
// ================================

// Inicializa os índices da fila
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = -1;
    f->qtd = 0;
    srand(time(NULL)); // semente para gerar peças aleatórias
}

// Verifica se a fila está cheia
int filaCheia(Fila *f) {
    return (f->qtd == TAM_FILA);
}

// Verifica se a fila está vazia
int filaVazia(Fila *f) {
    return (f->qtd == 0);
}

// Insere uma peça no final da fila
void enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) return;
    f->fim = (f->fim + 1) % TAM_FILA;
    f->fila[f->fim] = p;
    f->qtd++;
}

// Remove uma peça da frente da fila
Peca dequeue(Fila *f) {
    Peca p = {'-', -1}; // peça "vazia" caso dê erro
    if (filaVazia(f)) return p;
    p = f->fila[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_FILA;
    f->qtd--;
    return p;
}

// Exibe a fila atual de peças
void exibirFila(Fila *f) {
    printf("\nFila de peças:\n");
    if (filaVazia(f)) {
        printf("[vazia]\n");
        return;
    }
    int i = f->inicio;
    for (int c = 0; c < f->qtd; c++) {
        printf("[%c %d] ", f->fila[i].nome, f->fila[i].id);
        i = (i + 1) % TAM_FILA;
    }
    printf("\n");
}

// Gera uma peça aleatória
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    int r = rand() % 4;
    Peca p;
    p.nome = tipos[r];
    p.id = id;
    return p;
}
