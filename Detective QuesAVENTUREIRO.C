#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição da struct Sala.
// Representa um cômodo da mansão, com nome, pista opcional e ponteiros para salas adjacentes.
// Forma a árvore binária do mapa da mansão.
typedef struct Sala {
    char nome[50];           // Nome da sala
    char pista[100];         // Pista opcional (string vazia se não houver)
    struct Sala* esquerda;   // Ponteiro para sala à esquerda
    struct Sala* direita;    // Ponteiro para sala à direita
} Sala;

// Definição da struct PistaNode para a árvore BST de pistas.
// Armazena pistas coletadas em ordem alfabética.
typedef struct PistaNode {
    char pista[100];              // Conteúdo da pista
    struct PistaNode* esquerda;   // Ponteiro para subárvore esquerda
    struct PistaNode* direita;    // Ponteiro para subárvore direita
} PistaNode;

// Função para criar uma sala dinamicamente.
// Recebe nome e pista (pista pode ser string vazia "").
// Aloca memória, copia nome e pista, inicializa ponteiros como NULL.
// Retorna ponteiro para a nova sala.
Sala* criarSala(char* nome, char* pista) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro na alocacao de memoria para sala.\n");
        exit(1);
    }
    strcpy(novaSala->nome, nome);
    strcpy(novaSala->pista, pista);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// Função para inserir uma pista na árvore BST.
// Recebe a raiz da BST e a pista a inserir.
// Insere recursivamente baseado na comparação alfabética (strcmp).
// Retorna a nova raiz da BST.
PistaNode* inserirPista(PistaNode* raiz, char* pista) {
    if (raiz == NULL) {
        // Cria novo nó
        PistaNode* novo = (PistaNode*)malloc(sizeof(PistaNode));
        if (novo == NULL) {
            printf("Erro na alocacao de memoria para pista.\n");
            exit(1);
        }
        strcpy(novo->pista, pista);
        novo->esquerda = NULL;
        novo->direita = NULL;
        return novo;
    }
    
    // Comparação alfabética
    if (strcmp(pista, raiz->pista) < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else {
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    return raiz;
}

// Função para exibir pistas em ordem alfabética (in-order traversal).
// Recebe a raiz da BST e imprime as pistas recursivamente.
// Se a BST estiver vazia, informa que não há pistas.
void exibirPistas(PistaNode* raiz) {
    if (raiz == NULL) {
        printf("Nenhuma pista coletada.\n");
        return;
    }
    exibirPistas(raiz->esquerda);
    printf("- %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}

// Função para explorar salas com coleta de pistas.
// Recebe ponteiro para sala atual e ponteiro para raiz da BST de pistas.
// Navega interativamente: 'e' esquerda, 'd' direita, 's' sair.
// Ao entrar em uma sala, coleta pista se existir e insere na BST.
// Exibe nome da sala e pista coletada (se houver).
void explorarSalasComPistas(Sala* atual, PistaNode** raizPistas) {
    char escolha;
    printf("Voce esta na sala: %s\n", atual->nome);
    
    // Coleta pista se existir
    if (strlen(atual->pista) > 0) {
        printf("Pista encontrada: %s\n", atual->pista);
        *raizPistas = inserirPista(*raizPistas, atual->pista);
    } else {
        printf("Nenhuma pista nesta sala.\n");
    }
    
    while (1) {
        // Menu de opções
        printf("\nEscolha uma direcao:\n");
        printf("e - Ir para a esquerda\n");
        printf("d - Ir para a direita\n");
        printf("s - Sair da exploracao\n");
        printf("Sua escolha: ");
        scanf(" %c", &escolha);
        
        if (escolha == 'e') {
            if (atual->esquerda != NULL) {
                atual = atual->esquerda;
                printf("Voce foi para: %s\n", atual->nome);
                // Coleta pista na nova sala
                if (strlen(atual->pista) > 0) {
                    printf("Pista encontrada: %s\n", atual->pista);
                    *raizPistas = inserirPista(*raizPistas, atual->pista);
                } else {
                    printf("Nenhuma pista nesta sala.\n");
                }
            } else {
                printf("Nao ha caminho para a esquerda. Tente outra direcao.\n");
            }
        } else if (escolha == 'd') {
            if (atual->direita != NULL) {
                atual = atual->direita;
                printf("Voce foi para: %s\n", atual->nome);
                // Coleta pista na nova sala
                if (strlen(atual->pista) > 0) {
                    printf("Pista encontrada: %s\n", atual->pista);
                    *raizPistas = inserirPista(*raizPistas, atual->pista);
                } else {
                    printf("Nenhuma pista nesta sala.\n");
                }
            } else {
                printf("Nao ha caminho para a direita. Tente outra direcao.\n");
            }
        } else if (escolha == 's') {
            printf("Exploracao interrompida pelo jogador.\n");
            break;
        } else {
            printf("Escolha invalida. Tente novamente.\n");
        }
    }
}

// Função principal: monta o mapa da mansão com pistas e inicia exploração.
// Cria salas com pistas usando criarSala(), conecta a árvore binária.
// Inicializa BST de pistas como NULL.
// Chama exploração a partir do Hall, depois exibe pistas coletadas em ordem.
// Libera memória das salas (simplificado, sem recursão para árvore completa).
int main() {
    // Criação das salas com pistas (algumas com pistas, outras sem)
    Sala* hall = criarSala("Hall de Entrada", "Chave encontrada no tapete.");
    Sala* salaEstar = criarSala("Sala de Estar", "Bilhete rasgado: 'O culpado esta proximo'.");
    Sala* cozinha = criarSala("Cozinha", "");
    Sala* jardim = criarSala("Jardim", "Pegadas frescas na grama.");
    Sala* escritorio = criarSala("Escritorio", "Documento suspeito na mesa.");
    Sala* biblioteca = criarSala("Biblioteca", "");
    Sala* quarto = criarSala("Quarto", "Diario escondido: 'Plano revelado'.");
    
    // Conexões da árvore
    hall->esquerda = salaEstar;
    hall->direita = escritorio;
    
    salaEstar->esquerda = cozinha;
    salaEstar->direita = jardim;
    
    escritorio->esquerda = biblioteca;
    escritorio->direita = quarto;
    
    // Inicializa BST de pistas
    PistaNode* raizPistas = NULL;
    
    // Exploração
    explorarSalasComPistas(hall, &raizPistas);
    
    // Exibe pistas coletadas em ordem alfabética
    printf("\nPistas coletadas (em ordem alfabetica):\n");
    exibirPistas(raizPistas);
    
    // Liberação simplificada de memória (apenas salas, BST não liberada para brevidade)
    free(hall);
    free(salaEstar);
    free(cozinha);
    free(jardim);
    free(escritorio);
    free(biblioteca);
    free(quarto);
    
    return 0;
}