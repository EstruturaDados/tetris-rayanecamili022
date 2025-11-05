#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// =======================
// Estrutura da peça
// =======================
typedef struct {
    char tipo;
    int id;
} Peca;

// =======================
// Estruturas da fila e pilha
// =======================
typedef struct {
    Peca itens[TAM_FILA];
    int inicio, fim, qtd;
} Fila;

typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} Pilha;

// =======================
// Funções da Fila
// =======================
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = -1;
    f->qtd = 0;
}

int filaCheia(Fila *f) {
    return f->qtd == TAM_FILA;
}

int filaVazia(Fila *f) {
    return f->qtd == 0;
}

void enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) return;
    f->fim = (f->fim + 1) % TAM_FILA;
    f->itens[f->fim] = p;
    f->qtd++;
}

Peca dequeue(Fila *f) {
    Peca p = {'-', -1};
    if (!filaVazia(f)) {
        p = f->itens[f->inicio];
        f->inicio = (f->inicio + 1) % TAM_FILA;
        f->qtd--;
    }
    return p;
}

void mostrarFila(Fila *f) {
    printf("\n🧱 Fila de Peças Futuras:\n");
    if (filaVazia(f)) {
        printf("(vazia)\n");
        return;
    }
    int i, idx;
    for (i = 0; i < f->qtd; i++) {
        idx = (f->inicio + i) % TAM_FILA;
        printf("[%d] Tipo: %c | ID: %d\n", i + 1, f->itens[idx].tipo, f->itens[idx].id);
    }
}

// =======================
// Funções da Pilha
// =======================
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == TAM_PILHA - 1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

void push(Pilha *p, Peca x) {
    if (pilhaCheia(p)) {
        printf("❌ Pilha cheia!\n");
        return;
    }
    p->topo++;
    p->itens[p->topo] = x;
}

Peca pop(Pilha *p) {
    Peca x = {'-', -1};
    if (!pilhaVazia(p)) {
        x = p->itens[p->topo];
        p->topo--;
    }
    return x;
}

void mostrarPilha(Pilha *p) {
    printf("\n📦 Pilha de Reserva:\n");
    if (pilhaVazia(p)) {
        printf("(vazia)\n");
        return;
    }
    for (int i = p->topo; i >= 0; i--) {
        printf("[%d] Tipo: %c | ID: %d\n", i + 1, p->itens[i].tipo, p->itens[i].id);
    }
}

// =======================
// Geração de peças
// =======================
char tipos[] = {'I', 'O', 'T', 'S', 'Z', 'J', 'L'};
int contadorID = 1;

Peca gerarPeca() {
    Peca nova;
    nova.tipo = tipos[rand() % 7];
    nova.id = contadorID++;
    return nova;
}

// =======================
// Trocas (Nível Mestre)
// =======================
void trocarFrenteComTopo(Fila *f, Pilha *p) {
    if (filaVazia(f) || pilhaVazia(p)) {
        printf("⚠️ Não é possível trocar! Fila ou pilha vazia.\n");
        return;
    }
    Peca temp = f->itens[f->inicio];
    f->itens[f->inicio] = p->itens[p->topo];
    p->itens[p->topo] = temp;
    printf("🔄 Peça da frente trocada com o topo da pilha!\n");
}

void trocarTres(Fila *f, Pilha *p) {
    if (p->topo != 2 || f->qtd < 3) {
        printf("⚠️ É preciso ter 3 peças na pilha e pelo menos 3 na fila!\n");
        return;
    }
    for (int i = 0; i < 3; i++) {
        int idxFila = (f->inicio + i) % TAM_FILA;
        Peca temp = f->itens[idxFila];
        f->itens[idxFila] = p->itens[i];
        p->itens[i] = temp;
    }
    printf("🔁 As 3 primeiras peças da fila foram trocadas com as da pilha!\n");
}

// =======================
// Função principal
// =======================
int main() {
    srand(time(NULL));

    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche fila inicial
    for (int i = 0; i < TAM_FILA; i++) {
        enqueue(&fila, gerarPeca());
    }

    int opcao;
    do {
        printf("\n========== MENU ==========\n");
        printf("1 - Jogar peça (remover da fila)\n");
        printf("2 - Enviar peça da fila para reserva (pilha)\n");
        printf("3 - Usar peça da reserva (remover da pilha)\n");
        printf("4 - Trocar frente da fila com topo da pilha\n");
        printf("5 - Trocar 3 primeiras da fila com as 3 da pilha\n");
        printf("0 - Sair\n");
        printf("==========================\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                if (filaVazia(&fila)) {
                    printf("⚠️ Fila vazia!\n");
                } else {
                    Peca jogada = dequeue(&fila);
                    printf("🎮 Jogou peça Tipo %c | ID %d\n", jogada.tipo, jogada.id);
                    enqueue(&fila, gerarPeca());
                }
                break;
            }
            case 2: {
                if (filaVazia(&fila)) {
                    printf("⚠️ Fila vazia!\n");
                } else if (pilhaCheia(&pilha)) {
                    printf("⚠️ Pilha cheia!\n");
                } else {
                    Peca p = dequeue(&fila);
                    push(&pilha, p);
                    printf("📦 Peça Tipo %c | ID %d enviada para reserva.\n", p.tipo, p.id);
                    enqueue(&fila, gerarPeca());
                }
                break;
            }
            case 3: {
                if (pilhaVazia(&pilha)) {
                    printf("⚠️ Pilha vazia!\n");
                } else {
                    Peca usada = pop(&pilha);
                    printf("🎯 Usou peça Tipo %c | ID %d da reserva.\n", usada.tipo, usada.id);
                }
                break;
            }
            case 4:
                trocarFrenteComTopo(&fila, &pilha);
                break;
            case 5:
                trocarTres(&fila, &pilha);
                break;
            case 0:
                printf("👋 Saindo...\n");
                break;
            default:
                printf("❌ Opção inválida!\n");
        }

        mostrarFila(&fila);
        mostrarPilha(&pilha);

    } while (opcao != 0);

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// =======================
// Estrutura da peça
// =======================
typedef struct {
    char tipo;
    int id;
} Peca;

// =======================
// Estruturas da fila e pilha
// =======================
typedef struct {
    Peca itens[TAM_FILA];
    int inicio, fim, qtd;
} Fila;

typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} Pilha;

// =======================
// Funções da Fila
// =======================
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = -1;
    f->qtd = 0;
}

int filaCheia(Fila *f) {
    return f->qtd == TAM_FILA;
}

int filaVazia(Fila *f) {
    return f->qtd == 0;
}

void enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) return;
    f->fim = (f->fim + 1) % TAM_FILA;
    f->itens[f->fim] = p;
    f->qtd++;
}

Peca dequeue(Fila *f) {
    Peca p = {'-', -1};
    if (!filaVazia(f)) {
        p = f->itens[f->inicio];
        f->inicio = (f->inicio + 1) % TAM_FILA;
        f->qtd--;
    }
    return p;
}

void mostrarFila(Fila *f) {
    printf("\n🧱 Fila de Peças Futuras:\n");
    if (filaVazia(f)) {
        printf("(vazia)\n");
        return;
    }
    int i, idx;
    for (i = 0; i < f->qtd; i++) {
        idx = (f->inicio + i) % TAM_FILA;
        printf("[%d] Tipo: %c | ID: %d\n", i + 1, f->itens[idx].tipo, f->itens[idx].id);
    }
}

// =======================
// Funções da Pilha
// =======================
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == TAM_PILHA - 1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

void push(Pilha *p, Peca x) {
    if (pilhaCheia(p)) {
        printf("❌ Pilha cheia!\n");
        return;
    }
    p->topo++;
    p->itens[p->topo] = x;
}

Peca pop(Pilha *p) {
    Peca x = {'-', -1};
    if (!pilhaVazia(p)) {
        x = p->itens[p->topo];
        p->topo--;
    }
    return x;
}

void mostrarPilha(Pilha *p) {
    printf("\n📦 Pilha de Reserva:\n");
    if (pilhaVazia(p)) {
        printf("(vazia)\n");
        return;
    }
    for (int i = p->topo; i >= 0; i--) {
        printf("[%d] Tipo: %c | ID: %d\n", i + 1, p->itens[i].tipo, p->itens[i].id);
    }
}

// =======================
// Geração de peças
// =======================
char tipos[] = {'I', 'O', 'T', 'S', 'Z', 'J', 'L'};
int contadorID = 1;

Peca gerarPeca() {
    Peca nova;
    nova.tipo = tipos[rand() % 7];
    nova.id = contadorID++;
    return nova;
}

// =======================
// Trocas (Nível Mestre)
// =======================
void trocarFrenteComTopo(Fila *f, Pilha *p) {
    if (filaVazia(f) || pilhaVazia(p)) {
        printf("⚠️ Não é possível trocar! Fila ou pilha vazia.\n");
        return;
    }
    Peca temp = f->itens[f->inicio];
    f->itens[f->inicio] = p->itens[p->topo];
    p->itens[p->topo] = temp;
    printf("🔄 Peça da frente trocada com o topo da pilha!\n");
}

void trocarTres(Fila *f, Pilha *p) {
    if (p->topo != 2 || f->qtd < 3) {
        printf("⚠️ É preciso ter 3 peças na pilha e pelo menos 3 na fila!\n");
        return;
    }
    for (int i = 0; i < 3; i++) {
        int idxFila = (f->inicio + i) % TAM_FILA;
        Peca temp = f->itens[idxFila];
        f->itens[idxFila] = p->itens[i];
        p->itens[i] = temp;
    }
    printf("🔁 As 3 primeiras peças da fila foram trocadas com as da pilha!\n");
}

// =======================
// Função principal
// =======================
int main() {
    srand(time(NULL));

    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche fila inicial
    for (int i = 0; i < TAM_FILA; i++) {
        enqueue(&fila, gerarPeca());
    }

    int opcao;
    do {
        printf("\n========== MENU ==========\n");
        printf("1 - Jogar peça (remover da fila)\n");
        printf("2 - Enviar peça da fila para reserva (pilha)\n");
        printf("3 - Usar peça da reserva (remover da pilha)\n");
        printf("4 - Trocar frente da fila com topo da pilha\n");
        printf("5 - Trocar 3 primeiras da fila com as 3 da pilha\n");
        printf("0 - Sair\n");
        printf("==========================\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                if (filaVazia(&fila)) {
                    printf("⚠️ Fila vazia!\n");
                } else {
                    Peca jogada = dequeue(&fila);
                    printf("🎮 Jogou peça Tipo %c | ID %d\n", jogada.tipo, jogada.id);
                    enqueue(&fila, gerarPeca());
                }
                break;
            }
            case 2: {
                if (filaVazia(&fila)) {
                    printf("⚠️ Fila vazia!\n");
                } else if (pilhaCheia(&pilha)) {
                    printf("⚠️ Pilha cheia!\n");
                } else {
                    Peca p = dequeue(&fila);
                    push(&pilha, p);
                    printf("📦 Peça Tipo %c | ID %d enviada para reserva.\n", p.tipo, p.id);
                    enqueue(&fila, gerarPeca());
                }
                break;
            }
            case 3: {
                if (pilhaVazia(&pilha)) {
                    printf("⚠️ Pilha vazia!\n");
                } else {
                    Peca usada = pop(&pilha);
                    printf("🎯 Usou peça Tipo %c | ID %d da reserva.\n", usada.tipo, usada.id);
                }
                break;
            }
            case 4:
                trocarFrenteComTopo(&fila, &pilha);
                break;
            case 5:
                trocarTres(&fila, &pilha);
                break;
            case 0:
                printf("👋 Saindo...\n");
                break;
            default:
                printf("❌ Opção inválida!\n");
        }

        mostrarFila(&fila);
        mostrarPilha(&pilha);

    } while (opcao != 0);

    return 0;
}
