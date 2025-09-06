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

// (NIVEL MESTRE) - // Desenvolver um gerenciador de peças que usa duas estruturas de dados: uma fila circular com capacidade
                    // para 5 peças e uma pilha com capacidade para 3 peças. O sistema irá executar ações estratégicas com elas,
                    // como jogar, reservar, recuperar e realizar uma troca em bloco entre as peças da fila e as da pilha.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ================================
// Constantes
// ================================
#define TAM_FILA  5    // capacidade fixa da fila
#define TAM_PILHA 3    // capacidade fixa da pilha

// ================================
// Estruturas
// ================================
typedef struct {
    char nome; // 'I', 'O', 'T', 'L'
    int  id;   // identificador único
} Peca;

typedef struct {
    Peca fila[TAM_FILA];
    int inicio; // índice do primeiro elemento
    int fim;    // índice do último elemento
    int qtd;    // quantidade atual
} Fila;

typedef struct {
    Peca pilha[TAM_PILHA];
    int topo; // índice do topo (inicia em -1)
} Pilha;

// ================================
// Protótipos
// ================================
// Peças
Peca gerarPeca(int id);

// Fila
void inicializarFila(Fila *f);
int  filaCheia(Fila *f);
int  filaVazia(Fila *f);
void enqueue(Fila *f, Peca p);
Peca dequeue(Fila *f);       // remove da frente
Peca* frontRef(Fila *f);     // referência à peça da frente (sem remover)
void exibirFila(Fila *f);

// Pilha
void inicializarPilha(Pilha *p);
int  pilhaCheia(Pilha *p);
int  pilhaVazia(Pilha *p);
void push(Pilha *p, Peca x);
Peca pop(Pilha *p);          // remove do topo
Peca* topRef(Pilha *p);      // referência ao topo (sem remover)
void exibirPilha(Pilha *p);

// Estado
void exibirEstado(Fila *f, Pilha *p);

// Trocas (V3)
int trocarPeçaAtual(Fila *f, Pilha *p);    // troca frente da fila <-> topo da pilha
int trocarBloco3(Fila *f, Pilha *p);       // troca 3 primeiros da fila <-> 3 da pilha

// ================================
// main
// ================================
int main() {
    Fila  fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    int opcao;
    int idGlobal = 0;

    // Inicializa a fila com 5 peças
    for (int i = 0; i < TAM_FILA; i++) {
        enqueue(&fila, gerarPeca(idGlobal++));
    }


    do {
        printf("\n========================\n");
        printf("\n===== TETRIS STACK =====\n");
        printf("\n========================\n\n");
        // Exibe estado inicial
        exibirEstado(&fila, &pilha);
        printf("\n1 - Jogar peça da frente da fila (dequeue)\n");
        printf("2 - Reservar peça (mover frente da fila -> topo da pilha)\n");
        printf("3 - Usar peça reservada (pop da pilha)\n");
        printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
        printf("5 - Trocar os 3 primeiros da fila com as 3 peças da pilha\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        if (scanf("%d", &opcao) != 1) {
            // limpeza simples se entrada for inválida
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {}
            opcao = -1;
        }

        switch (opcao) {
            case 1: { // Jogar peça
                if (!filaVazia(&fila)) {
                    Peca jogada = dequeue(&fila);
                    printf("\nAção: peça jogada: [%c %d]\n", jogada.nome, jogada.id);
                    // repor a fila
                    enqueue(&fila, gerarPeca(idGlobal++));
                } else {
                    printf("\nFila vazia! Nada para jogar.\n");
                }
                break;
            }
            case 2: { // Reservar peça (fila -> pilha)
                if (filaVazia(&fila)) {
                    printf("\nFila vazia! Nada para reservar.\n");
                } else if (pilhaCheia(&pilha)) {
                    printf("\nPilha cheia! Não é possível reservar.\n");
                } else {
                    Peca movida = dequeue(&fila);
                    push(&pilha, movida);
                    printf("\nAção: peça movida para reserva: [%c %d]\n", movida.nome, movida.id);
                    // repor a fila
                    enqueue(&fila, gerarPeca(idGlobal++));
                }
                break;
            }
            case 3: { // Usar peça reservada (pop)
                if (!pilhaVazia(&pilha)) {
                    Peca usada = pop(&pilha);
                    printf("\nAção: peça reservada usada: [%c %d]\n", usada.nome, usada.id);
                    // repor a fila
                    if (!filaCheia(&fila)) {
                        enqueue(&fila, gerarPeca(idGlobal++));
                    }
                } else {
                    printf("\nPilha vazia! Não há peça reservada para usar.\n");
                }
                break;
            }
            case 4: { // Troca simples: frente da fila <-> topo da pilha
                int ok = trocarPeçaAtual(&fila, &pilha);
                if (ok) {
                    printf("\nAção: troca realizada entre frente da fila e topo da pilha.\n");
                }
                break;
            }
            case 5: { // Troca 3x3: primeiros da fila <-> peças da pilha
                int ok = trocarBloco3(&fila, &pilha);
                if (ok) {
                    printf("\nAção: troca realizada entre os 3 primeiros da fila e as 3 peças da pilha.\n");
                }
                break;
            }
            case 0:
                printf("\nSaindo...\n");
                break;
            default:
                printf("\nOpção inválida! Tente novamente.\n");
        }

        // Exibir estado após cada ação (inclusive entradas inválidas, para manter padrão visual)
        exibirEstado(&fila, &pilha);

    } while (opcao != 0);

    return 0;
}

// ================================
// Implementações
// ================================

// -------- Peças --------
Peca gerarPeca(int id) {
    static const char tipos[] = {'I', 'O', 'T', 'L'};
    int r = rand() % 4;
    Peca p;
    p.nome = tipos[r];
    p.id   = id;
    return p;
}

// -------- Fila --------
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim    = -1;
    f->qtd    = 0;
    // semente aleatória uma vez por execução (colocado aqui para já existir antes do primeiro gerarPeca)
    static int seeded = 0;
    if (!seeded) {
        seeded = 1;
        srand((unsigned)time(NULL));
    }
}

int filaCheia(Fila *f)  { return f->qtd == TAM_FILA; }
int filaVazia(Fila *f)  { return f->qtd == 0; }

void enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) return;               // proteção
    f->fim = (f->fim + 1) % TAM_FILA;
    f->fila[f->fim] = p;
    f->qtd++;
}

Peca dequeue(Fila *f) {
    Peca vazio = {'-', -1};
    if (filaVazia(f)) return vazio;
    Peca p = f->fila[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_FILA;
    f->qtd--;
    return p;
}

Peca* frontRef(Fila *f) {
    if (filaVazia(f)) return NULL;
    return &f->fila[f->inicio];
}

void exibirFila(Fila *f) {
    printf("Fila de Peças: ");
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

int pilhaCheia(Pilha *p) { return p->topo == TAM_PILHA - 1; }
int pilhaVazia(Pilha *p) { return p->topo == -1; }

void push(Pilha *p, Peca x) {
    if (pilhaCheia(p)) return;              // proteção
    p->pilha[++(p->topo)] = x;
}

Peca pop(Pilha *p) {
    Peca vazio = {'-', -1};
    if (pilhaVazia(p)) return vazio;
    return p->pilha[(p->topo)--];
}

Peca* topRef(Pilha *p) {
    if (pilhaVazia(p)) return NULL;
    return &p->pilha[p->topo];
}

void exibirPilha(Pilha *p) {
    printf("Pilha de reserva(Topo -> base): ");
    if (pilhaVazia(p)) {
        printf("[vazia]\n");
        return;
    }
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->pilha[i].nome, p->pilha[i].id);
    }
    printf("\n");
}

// -------- Estado --------
void exibirEstado(Fila *f, Pilha *p) {
    printf("\n");
    exibirFila(f);
    printf("\n");
    exibirPilha(p);
}

// -------- Trocas (NIVEL MESTRE) --------
// Troca simples: frente da fila <-> topo da pilha (sem gerar/descartar)
int trocarPeçaAtual(Fila *f, Pilha *p) {
    Peca *frente = frontRef(f);
    Peca *top    = topRef(p);
    if (!frente) {
        printf("\nNão há peça na frente da fila para trocar.\n");
        return 0;
    }
    if (!top) {
        printf("\nPilha vazia! Não há topo para trocar.\n");
        return 0;
    }
    Peca tmp = *frente;
    *frente  = *top;
    *top     = tmp;
    return 1;
}

// Troca em bloco 3x3: primeiros 3 da fila <-> 3 da pilha
// Regra de ordem (seguindo exemplo do enunciado):
// - Os 3 elementos da pilha (Topo->Base) vão para a fila na ordem [topo, prox, prox].
// - Os 3 primeiros da fila (frente->...) vão para a pilha em ordem LIFO: topo recebe o 3º da fila,
//   depois o 2º, e depois o 1º, resultando (Topo->Base) = [q2, q1, q0].
int trocarBloco3(Fila *f, Pilha *p) {
    if (f->qtd < 3) {
        printf("\nA fila não possui 3 peças para trocar.\n");
        return 0;
    }
    if (p->topo < 2) {
        printf("\nA pilha não possui 3 peças para trocar.\n");
        return 0;
    }

    // Índices dos 3 primeiros da fila
    int i0 = f->inicio;
    int i1 = (f->inicio + 1) % TAM_FILA;
    int i2 = (f->inicio + 2) % TAM_FILA;

    // Copiar peças atuais
    Peca q0 = f->fila[i0];
    Peca q1 = f->fila[i1];
    Peca q2 = f->fila[i2];

    Peca s0 = p->pilha[p->topo];     // topo
    Peca s1 = p->pilha[p->topo - 1];
    Peca s2 = p->pilha[p->topo - 2]; // mais abaixo

    // Colocar na fila [s0,s1,s2]
    f->fila[i0] = s0;
    f->fila[i1] = s1;
    f->fila[i2] = s2;

    // Colocar na pilha (Topo->Base) = [q2, q1, q0]
    p->pilha[p->topo]     = q2;
    p->pilha[p->topo - 1] = q1;
    p->pilha[p->topo - 2] = q0;

    return 1;
}
