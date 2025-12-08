#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_NOME 50

// Estrutura para Sala
typedef struct Sala {
    char nome[MAX_NOME];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Cria nova sala, com alocação dinâmica
Sala* criarSala(const char* nome) {
    Sala* nova = (Sala*)malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro ao alocar mamória!\n");
        exit(1);
    }

    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// Função para limpar o buffer do teclado
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função para explorar salas interativamente
void explorarSalas(Sala *salaAtual) {
    char opcao;

    while (salaAtual != NULL) {
        printf("\n---------------------------------------------\n");
        printf("Você está em: %s\n", salaAtual->nome);
        printf("\n---------------------------------------------\n");

        // Verifica se o nó atual é uma folha
        if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
            printf("Não há mais salas para explorar aqui.\n");
            break;
        }

        // Mostar as opções disponíveis
        printf("Para onde você quer ir?\n");
        if (salaAtual->esquerda != NULL) {
            printf("   [e] Esquerda -> %s\n", salaAtual->esquerda->nome);
        }
        if (salaAtual->direita != NULL) {
            printf("   [d] Direita -> %s\n", salaAtual->direita->nome);
        }

        printf("   [s] Sair do jogo\n");
        
        printf("\nEscolha: ");
        scanf("%c", &opcao);

        // Limpa o buffer de entrada
        while (getchar() != '\n');

        // Processa a escolha do usuário
        switch (opcao) {
            case 'e':
                if (salaAtual->esquerda != NULL) {
                    salaAtual = salaAtual->esquerda;
                } else {
                    printf("Não há caminho à esquerda!\n");
                }
                break;
            case 'd':
                if (salaAtual->direita != NULL) {
                    salaAtual = salaAtual->direita;
                } else {
                    printf("Não há caminho à direita!\n");
                }
                break;
            case 's':
                printf("Saindo do jogo... Até logo!\n");
                return;
            default:
                printf("Opção inválida! Use 'e', 'd' ou 's'.\n");
        }
    }

    printf("\nJogo finalizado!\n");
}

void liberarArvore(Sala *raiz) {
    if (raiz != NULL) {
        liberarArvore(raiz->esquerda);
        liberarArvore(raiz->direita);
        free(raiz);
    }
}



int main() {

    // 🌱 Nível Novato: Mapa da Mansão com Árvore Binária
    //
    // - Crie uma struct Sala com nome, e dois ponteiros: esquerda e direita. - FEITO
    // - Use funções como criarSala(), conectarSalas() e explorarSalas(). - FEITO
    // - A árvore pode ser fixa: Hall de Entrada, Biblioteca, Cozinha, Sótão etc.
    // - O jogador deve poder explorar indo à esquerda (e) ou à direita (d).
    // - Finalize a exploração com uma opção de saída (s).
    // - Exiba o nome da sala a cada movimento.
    // - Use recursão ou laços para caminhar pela árvore.
    // - Nenhuma inserção dinâmica é necessária neste nível.

    // Construíndo a ávore binária da mansão
    Sala *hall = criarSala("Hall de Entrada");

    // Nível 1
    hall->esquerda = criarSala("Biblioteca");
    hall->direita = criarSala("Sala de Estar");

    // Nível 2
    hall->esquerda->esquerda = criarSala("Sala de Leitura");
    hall->esquerda->direita = criarSala("Jardim de Inverno");
    hall->direita->esquerda = criarSala("Cozinha");
    hall->direita->direita = criarSala("Banheiro");

    // Nível 3
    hall->esquerda->esquerda->esquerda = criarSala("Sala de Música");
    hall->esquerda->esquerda->direita = criarSala("Estufa");
    hall->direita->esquerda->esquerda = criarSala("Varanda");
    hall->direita->esquerda->direita = criarSala("Quarto");

    printf("\nExplore os cômodos escolhendo ir para à esquerda ou para a direita!\n");

    // Inicia a exploração
    explorarSalas(hall);

    // Libera a memória alocada
    liberarArvore(hall);

    return 0;
}

