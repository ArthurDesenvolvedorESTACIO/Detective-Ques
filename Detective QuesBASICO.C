#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição da struct Sala.
// Esta struct representa um cômodo da mansão, com um nome e ponteiros para salas à esquerda e direita.
// Forma a base da árvore binária que representa o mapa da mansão.
typedef struct Sala {
    char nome[50];           // Nome da sala (ex: "Hall de Entrada")
    struct Sala* esquerda;   // Ponteiro para a sala à esquerda
    struct Sala* direita;    // Ponteiro para a sala à direita
} Sala;

// Função para criar uma sala dinamicamente.
// Recebe o nome da sala como parâmetro.
// Aloca memória usando malloc, copia o nome e inicializa ponteiros como NULL.
// Retorna o ponteiro para a nova sala criada.
Sala* criarSala(char* nome) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));  // Alocação dinâmica
    if (novaSala == NULL) {
        printf("Erro na alocacao de memoria.\n");
        exit(1);
    }
    strcpy(novaSala->nome, nome);  // Copia o nome
    novaSala->esquerda = NULL;     // Inicializa ponteiros como NULL
    novaSala->direita = NULL;
    return novaSala;
}

// Função para explorar as salas interativamente.
// Recebe o ponteiro para a sala atual (inicialmente o Hall de Entrada).
// Permite ao jogador navegar pela árvore escolhendo 'e' (esquerda), 'd' (direita) ou 's' (sair).
// Continua até chegar a um nó-folha (sem filhos) ou o jogador sair.
// Exibe o nome de cada sala visitada durante a exploração.
void explorarSalas(Sala* atual) {
    char escolha;
    printf("Voce esta na sala: %s\n", atual->nome);  // Exibe sala atual
    
    while (1) {
        // Verifica se é um nó-folha (sem caminhos)
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Voce chegou a um comodo sem saidas: %s\n", atual->nome);
            printf("Exploracao encerrada.\n");
            break;
        }
        
        // Menu de opções
        printf("Escolha uma direcao:\n");
        printf("e - Ir para a esquerda\n");
        printf("d - Ir para a direita\n");
        printf("s - Sair da exploracao\n");
        printf("Sua escolha: ");
        scanf(" %c", &escolha);  // Espaço antes de %c para ignorar newline
        
        if (escolha == 'e') {
            if (atual->esquerda != NULL) {
                atual = atual->esquerda;  // Move para esquerda
                printf("Voce foi para: %s\n", atual->nome);
            } else {
                printf("Nao ha caminho para a esquerda. Tente outra direcao.\n");
            }
        } else if (escolha == 'd') {
            if (atual->direita != NULL) {
                atual = atual->direita;  // Move para direita
                printf("Voce foi para: %s\n", atual->nome);
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

// Função principal: monta o mapa da mansão e inicia a exploração.
// Cria as salas usando criarSala() e conecta-as para formar a árvore binária.
// A estrutura é fixa e definida no código.
// Após montar, chama explorarSalas() a partir do Hall de Entrada.
int main() {
    // Criação das salas (árvore binária fixa)
    Sala* hall = criarSala("Hall de Entrada");
    Sala* salaEstar = criarSala("Sala de Estar");
    Sala* cozinha = criarSala("Cozinha");
    Sala* jardim = criarSala("Jardim");
    Sala* escritorio = criarSala("Escritorio");
    Sala* biblioteca = criarSala("Biblioteca");
    Sala* quarto = criarSala("Quarto");
    
    // Conexões da árvore
    hall->esquerda = salaEstar;
    hall->direita = escritorio;
    
    salaEstar->esquerda = cozinha;
    salaEstar->direita = jardim;
    
    escritorio->esquerda = biblioteca;
    escritorio->direita = quarto;
    
    // Inicia exploração a partir do Hall
    explorarSalas(hall);
    
    // Liberação de memória (opcional, mas boa prática)
    // Em um programa real, implementar uma função recursiva para liberar toda a árvore
    free(hall);
    free(salaEstar);
    free(cozinha);
    free(jardim);
    free(escritorio);
    free(biblioteca);
    free(quarto);
    
    return 0;
}