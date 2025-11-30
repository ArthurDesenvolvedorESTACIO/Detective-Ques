#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para representar um cômodo na árvore binária da mansão
typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Estrutura para um nó da árvore BST de pistas coletadas
typedef struct NoPista {
    char pista[100];
    struct NoPista *esquerda;
    struct NoPista *direita;
} NoPista;

// Estrutura para um nó da lista ligada na tabela hash (para colisões)
typedef struct NoHash {
    char pista[100];
    char suspeito[50];
    struct NoHash *proximo;
} NoHash;

// Tabela hash com 10 buckets (usando hash simples baseado no primeiro char da pista)
#define TAM_HASH 10
NoHash *tabelaHash[TAM_HASH] = {NULL};

// Lista de suspeitos possíveis (fixa para simplificação)
char suspeitos[5][50] = {"Jardineiro", "Cozinheiro", "Mordomo", "Dona da Casa", "Vizinho"};

// Função hash simples: soma dos caracteres da pista módulo TAM_HASH
int hash(char *pista) {
    int soma = 0;
    for (int i = 0; pista[i] != '\0'; i++) {
        soma += pista[i];
    }
    return soma % TAM_HASH;
}

// Função para criar um cômodo dinamicamente
Sala *criarSala(char *nome, char *pista) {
    // Aloca memória para o novo cômodo
    Sala *novaSala = (Sala *)malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro ao alocar memória para a sala.\n");
        exit(1);
    }
    // Copia o nome e a pista para a estrutura
    strcpy(novaSala->nome, nome);
    strcpy(novaSala->pista, pista);
    // Inicializa ponteiros para NULL
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// Função para inserir uma pista na árvore BST de pistas coletadas
NoPista *inserirPista(NoPista *raiz, char *pista) {
    // Se a raiz for NULL, cria um novo nó
    if (raiz == NULL) {
        NoPista *novo = (NoPista *)malloc(sizeof(NoPista));
        if (novo == NULL) {
            printf("Erro ao alocar memória para a pista.\n");
            exit(1);
        }
        strcpy(novo->pista, pista);
        novo->esquerda = NULL;
        novo->direita = NULL;
        return novo;
    }
    // Insere recursivamente na esquerda se a pista for menor, direita se maior
    if (strcmp(pista, raiz->pista) < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (strcmp(pista, raiz->pista) > 0) {
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    // Se igual, não insere (evita duplicatas)
    return raiz;
}

// Função para inserir uma associação pista-suspeito na tabela hash
void inserirNaHash(char *pista, char *suspeito) {
    // Calcula o índice hash
    int indice = hash(pista);
    // Cria um novo nó para a lista ligada
    NoHash *novo = (NoHash *)malloc(sizeof(NoHash));
    if (novo == NULL) {
        printf("Erro ao alocar memória para a hash.\n");
        exit(1);
    }
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->proximo = NULL;
    // Insere no início da lista do bucket
    novo->proximo = tabelaHash[indice];
    tabelaHash[indice] = novo;
}

// Função para encontrar o suspeito associado a uma pista na tabela hash
char *encontrarSuspeito(char *pista) {
    // Calcula o índice hash
    int indice = hash(pista);
    // Percorre a lista ligada no bucket
    NoHash *atual = tabelaHash[indice];
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito;
        }
        atual = atual->proximo;
    }
    return NULL; // Não encontrado
}

// Função recursiva para explorar as salas
void explorarSalas(Sala *salaAtual, NoPista **raizPistas) {
    if (salaAtual == NULL) {
        printf("Sala inválida.\n");
        return;
    }
    // Exibe o nome da sala e a pista
    printf("Você entrou na %s.\n", salaAtual->nome);
    printf("Pista encontrada: %s\n", salaAtual->pista);
    // Insere a pista na BST de pistas coletadas
    *raizPistas = inserirPista(*raizPistas, salaAtual->pista);
    // Associa a pista ao suspeito na hash (associações fixas no código)
    if (strcmp(salaAtual->pista, "Pegada de lama") == 0) {
        inserirNaHash(salaAtual->pista, "Jardineiro");
    } else if (strcmp(salaAtual->pista, "Faca suja") == 0) {
        inserirNaHash(salaAtual->pista, "Cozinheiro");
    } else if (strcmp(salaAtual->pista, "Luva rasgada") == 0) {
        inserirNaHash(salaAtual->pista, "Mordomo");
    } else if (strcmp(salaAtual->pista, "Joia roubada") == 0) {
        inserirNaHash(salaAtual->pista, "Dona da Casa");
    } else if (strcmp(salaAtual->pista, "Cigarro estranho") == 0) {
        inserirNaHash(salaAtual->pista, "Vizinho");
    } else if (strcmp(salaAtual->pista, "Terra no tapete") == 0) {
        inserirNaHash(salaAtual->pista, "Jardineiro");
    } else if (strcmp(salaAtual->pista, "Restos de comida") == 0) {
        inserirNaHash(salaAtual->pista, "Cozinheiro");
    } else if (strcmp(salaAtual->pista, "Botão de uniforme") == 0) {
        inserirNaHash(salaAtual->pista, "Mordomo");
    } else if (strcmp(salaAtual->pista, "Carta suspeita") == 0) {
        inserirNaHash(salaAtual->pista, "Dona da Casa");
    } else if (strcmp(salaAtual->pista, "Pegada de sapato") == 0) {
        inserirNaHash(salaAtual->pista, "Vizinho");
    }
    // Loop para navegação
    char escolha;
    while (1) {
        printf("Escolha: e (esquerda), d (direita), s (sair): ");
        scanf(" %c", &escolha);
        if (escolha == 'e') {
            if (salaAtual->esquerda != NULL) {
                explorarSalas(salaAtual->esquerda, raizPistas);
            } else {
                printf("Não há sala à esquerda.\n");
            }
        } else if (escolha == 'd') {
            if (salaAtual->direita != NULL) {
                explorarSalas(salaAtual->direita, raizPistas);
            } else {
                printf("Não há sala à direita.\n");
            }
        } else if (escolha == 's') {
            return; // Sai da exploração
        } else {
            printf("Escolha inválida.\n");
        }
    }
}

// Função para listar pistas em ordem (in-order traversal da BST)
void listarPistas(NoPista *raiz) {
    if (raiz != NULL) {
        listarPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        listarPistas(raiz->direita);
    }
}

// Função recursiva para contar pistas por suspeito
int contarPistasPorSuspeito(NoPista *raiz, char *suspeito) {
    if (raiz == NULL) return 0;
    char *suspeitoPista = encontrarSuspeito(raiz->pista);
    int count = (suspeitoPista != NULL && strcmp(suspeitoPista, suspeito) == 0) ? 1 : 0;
    return count + contarPistasPorSuspeito(raiz->esquerda, suspeito) + contarPistasPorSuspeito(raiz->direita, suspeito);
}

// Função para verificar o suspeito final e conduzir o julgamento
void verificarSuspeitoFinal(NoPista *raizPistas) {
    // Lista as pistas coletadas
    printf("\nPistas coletadas:\n");
    listarPistas(raizPistas);
    // Solicita a acusação
    printf("\nSuspeitos possíveis: Jardineiro, Cozinheiro, Mordomo, Dona da Casa, Vizinho\n");
    printf("Quem você acusa? ");
    char acusacao[50];
    scanf("%s", acusacao);
    // Verifica se há pelo menos duas pistas apontando para o suspeito
    int count = contarPistasPorSuspeito(raizPistas, acusacao);
    if (count >= 2) {
        printf("Parabéns! Você acusou corretamente com %d pistas. O culpado é %s!\n", count, acusacao);
    } else {
        printf("Acusação incorreta. Você só tem %d pistas contra %s. Tente novamente!\n", count, acusacao);
    }
}

int main() {
    // Monta a árvore da mansão manualmente (fixa)
    Sala *raiz = criarSala("Hall de Entrada", "Pegada de lama");
    raiz->esquerda = criarSala("Sala de Estar", "Faca suja");
    raiz->direita = criarSala("Biblioteca", "Luva rasgada");
    raiz->esquerda->esquerda = criarSala("Jardim", "Joia roubada");
    raiz->esquerda->direita = criarSala("Cozinha", "Cigarro estranho");
    raiz->direita->esquerda = criarSala("Quarto Principal", "Terra no tapete");
    raiz->direita->direita = criarSala("Escritório", "Restos de comida");
    raiz->esquerda->esquerda->esquerda = criarSala("Garagem", "Botão de uniforme");
    raiz->esquerda->esquerda->direita = criarSala("Piscina", "Carta suspeita");
    raiz->esquerda->direita->esquerda = criarSala("Despensa", "Pegada de sapato");

    // Inicializa a raiz da BST de pistas
    NoPista *raizPistas = NULL;

    // Inicia a exploração
    printf("Bem-vindo ao Detective Quest! Explore a mansão para coletar pistas.\n");
    explorarSalas(raiz, &raizPistas);

    // Conduz o julgamento final
    verificarSuspeitoFinal(raizPistas);

    // Libera memória (simplificado, não implementado completamente para brevidade)
    // Em um código real, seria necessário liberar todas as alocações dinâmicas.

    return 0;
}