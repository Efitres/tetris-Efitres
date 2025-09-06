// ============================================================================
//         PROJETO TETRIS STACK - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS: 
// (NIVEL NOVATO)  -   implementar um programa em C que simula a fila de peças futuras do Tetris Stack.
                    // As peças têm um tipo (como 'I', 'O', 'T', 'L’), que representa suas formas e um identificador
                    // numérico (id) exclusivo. O programa oferecerá ao jogador a opção de visualizar a fila, jogar
                    // uma peça (remoção da frente) ou adicionar uma nova peça ao final da fila.

// (NIVEL AVENTUREIRO) - Implementar um programa em C que simula o gerenciamento de peças no jogo Tetris Stack, utilizando
                    // uma fila circular de peças futuras e uma pilha de reserva. Cada peça tem um tipo (como 'I', 'O', 'T', 'L')
                    // e um identificador numérico único (id). O jogador poderá visualizar a fila e a pilha, jogar peças,
                    // reservar peças para uso posterior e remover peças da reserva.


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ================================
// Constantes
// ================================
#define TAM_FILA 5   // Tamanho fixo da fila
#define TAM_PILHA 3  // Capacidade máxima da pilha

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

typedef struct {
    Peca pilha[TAM_PILHA];
    int topo;
} Pilha;

// ================================
// Protótipos de funções
// ================================
// Fila
void inicializarFila(Fila *f);
int filaCheia(Fila *f);
int filaVazia(Fila *f);
void enqueue(Fila *f, Peca p);
Peca dequeue(Fila *f);
void exibirFila(Fila *f);

// Pilha
void inicializarPilha(Pilha *p);
int pilhaCheia(Pilha *p);
int pilhaVazia(Pilha *p);
void push(Pilha *p, Peca x);
Peca pop(Pilha *p);
void exibirPilha(Pilha *p);

// Peças
Peca gerarPeca(int id);

// ================================
// Função principal
// ================================
int main() {
    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    int opcao;
    int idGlobal = 0;

    // Inicializa fila com 5 peças
    for (int i = 0; i < TAM_FILA; i++) {
        enqueue(&fila, gerarPeca(idGlobal++));
    }

    // Exibe o estado inicial
    exibirFila(&fila);
    exibirPilha(&pilha);

    do {
        printf("\n===== MENU =====\n");
        printf("1 - Jogar peça (dequeue)\n");
        printf("2 - Reservar peça (push)\n");
        printf("3 - Usar peça reservada (pop)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: { // Jogar peça
                if (!filaVazia(&fila)) {
                    Peca jogada = dequeue(&fila);
                    printf("\nPeça jogada: [%c %d]\n", jogada.nome, jogada.id);
                    enqueue(&fila, gerarPeca(idGlobal++)); // sempre repõe na fila
                } else {
                    printf("\nFila vazia!\n");
                }
                break;
            }
            case 2: { // Reservar peça
                if (!filaVazia(&fila) && !pilhaCheia(&pilha)) {
                    Peca reservada = dequeue(&fila);
                    push(&pilha, reservada);
                    printf("\nPeça [%c %d] movida para reserva.\n", reservada.nome, reservada.id);
                    enqueue(&fila, gerarPeca(idGlobal++));
                } else if (pilhaCheia(&pilha)) {
                    printf("\nPilha cheia! Não é possível reservar.\n");
                }
                break;
            }
            case 3: { // Usar peça reservada
                if (!pilhaVazia(&pilha)) {
                    Peca usada = pop(&pilha);
                    printf("\nPeça reservada usada: [%c %d]\n", usada.nome, usada.id);
                    enqueue(&fila, gerarPeca(idGlobal++));
                } else {
                    printf("\nPilha vazia! Não há peça reservada para usar.\n");
                }
                break;
            }
            case 0:
                printf("\nSaindo do jogo...\n");
                break;
            default:
                printf("\nOpção inválida! Tente novamente.\n");
        }

        // Mostrar estado atual sempre após uma ação
        exibirFila(&fila);
        exibirPilha(&pilha);

    } while (opcao != 0);

    return 0;
}

// ================================
// Implementação das funções
// ================================

// -------- Fila --------
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = -1;
    f->qtd = 0;
    srand(time(NULL));
}

int filaCheia(Fila *f) {
    return (f->qtd == TAM_FILA);
}

int filaVazia(Fila *f) {
    return (f->qtd == 0);
}

void enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) return;
    f->fim = (f->fim + 1) % TAM_FILA;
    f->fila[f->fim] = p;
    f->qtd++;
}

Peca dequeue(Fila *f) {
    Peca p = {'-', -1};
    if (filaVazia(f)) return p;
    p = f->fila[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_FILA;
    f->qtd--;
    return p;
}

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

// -------- Pilha --------
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaCheia(Pilha *p) {
    return (p->topo == TAM_PILHA - 1);
}

int pilhaVazia(Pilha *p) {
    return (p->topo == -1);
}

void push(Pilha *p, Peca x) {
    if (pilhaCheia(p)) return;
    p->pilha[++(p->topo)] = x;
}

Peca pop(Pilha *p) {
    Peca x = {'-', -1};
    if (pilhaVazia(p)) return x;
    return p->pilha[(p->topo)--];
}

void exibirPilha(Pilha *p) {
    printf("\nPilha de reserva (Topo -> Base):\n");
    if (pilhaVazia(p)) {
        printf("[vazia]\n");
        return;
    }
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->pilha[i].nome, p->pilha[i].id);
    }
    printf("\n");
}

// -------- Peças --------
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    int r = rand() % 4;
    Peca p;
    p.nome = tipos[r];
    p.id = id;
    return p;
}
