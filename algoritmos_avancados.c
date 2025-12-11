#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_NOME 50
#define MAX_PISTA 50


// ======== ESTRUTURAS ============
// Estrutura da Árvore BST
typedef struct NoPista {
    char pista[MAX_PISTA];
    struct NoPista* esquerda;
    struct NoPista* direita;
} NoPista;


// Estrutura para Sala
typedef struct Sala {
    char nome[MAX_NOME];
    char pista[MAX_PISTA];
    int pistaColetada;
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// ========= FUNÇOES DA BST =========
// Cria nó de pista
NoPista* criarNoPista(const char* pista) {
    NoPista* novo = (NoPista*)malloc(sizeof(NoPista));
    if (novo == NULL) {
        printf("Erro ao alocar memória.");
        exit(1);
    }
    strcpy(novo->pista, pista);
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

// Insere pista na BST (ordenada alfabeticamente)
NoPista* inserirPista(NoPista* raiz, const char* pista) {
    if (raiz == NULL) {
        return criarNoPista(pista);
    }

    int comparacao = strcmp(pista, raiz->pista);

    if (comparacao < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (comparacao > 0) {
        raiz->direita = inserirPista(raiz->direita, pista);
    }

    // Se for igual, não insere

    return raiz;
}

// Busca pista na árvore
int buscarPista(NoPista* raiz, const char* pista) {
    if (raiz == NULL) {
        return 0;
    }

    int comparacao = strcmp(pista, raiz->pista);

    if (comparacao == 0) {
        return 1;
    } else if (comparacao < 0) {
        return buscarPista(raiz->esquerda, pista);
    } else {
        return buscarPista(raiz->direita, pista);
    }
}

// Exibe pistas em ordem alfabética
void exibirPistaEmOrdem(NoPista* raiz) {
    if (raiz != NULL) {
        exibirPistaEmOrdem(raiz->esquerda);
        printf("%s ", raiz->pista);
        exibirPistaEmOrdem(raiz->direita);
    }
}

// Conta o número de pistas (adicionado por mim, para melhorar a experiência do usuário)
int contarPistas(NoPista* raiz) {
    if (raiz == NULL) {
        return 0;
    } else {
        return 1 + contarPistas(raiz->esquerda) + contarPistas(raiz->direita);
    }
}

// Libera memória da BST
void liberarMemoriaBST(NoPista* raiz) {
    if (raiz != NULL) {
        liberarMemoriaBST(raiz->esquerda);
        liberarMemoriaBST(raiz->direita);
        free(raiz);
    }
}

// ======= FUNÇÕES DAS SALAS ========
// Cria nova sala, com alocação dinâmica
Sala* criarSala(const char* nome, const char* pista) {
    Sala* nova = (Sala*)malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro ao alocar mamória!\n");
        exit(1);
    }

    strcpy(nova->nome, nome);
    if (pista != NULL && strlen(pista) > 0) {
        strcpy(nova->pista, pista);
        nova->pistaColetada = 0; // Pista existe mas NÃO foi coletada
    } else {
        nova->pista[0] = '\0';
        nova->pistaColetada = 1; // Pista já foi coletada
    }

    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// Função para limpar o buffer do teclado
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// ======= FUNÇÃO PRINCIPAL DO JOGO ========
// Função para explorar salas interativamente
void explorarSalas(Sala *salaAtual, NoPista** bstPistas) { // ** ---> Um ponteiro para um ponteiro de nó de pista
    char opcao;

    while (salaAtual != NULL) {
        printf("\n---------------------------------------------\n");
        printf("Você está em: %s\n", salaAtual->nome);
        printf("\n---------------------------------------------\n");

        // Verifica se há pista nesta sala
        if (strlen(salaAtual->pista) > 0 && !salaAtual->pistaColetada) {
            printf("\nPISTA ENCONTRADA!\n");
            printf("    \"%s\"\n", salaAtual->pista);
            *bstPistas = inserirPista(*bstPistas, salaAtual->pista);
            salaAtual->pistaColetada = 1;
            printf("\n  Pista adicionada ao seu caderno de investigação.\n");
        }

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

        printf("   [p] Ver pistas coletadas\n");
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
            case 'p':
                printf("\n-------- Caderno de pistas --------\n");
                if (*bstPistas == NULL) {
                    printf("Caderno de pistas está vazio! Nenhuma pista coletada.\n");
                } else {
                    printf("Total de pistas : %d\n\n", contarPistas(*bstPistas));
                    exibirPistaEmOrdem(*bstPistas);
                }
                printf("-------------------------------------------\n");
                printf("\nPressione enter para continuar...");
                getchar();
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
    setlocale(LC_ALL, "Portuguese");

    // Construindo a árvore BST
    NoPista* bstPistas = NULL;

    // Construíndo a ávore binária da mansão
    Sala *hall = criarSala("Hall de Entrada", "");

    // Nível 1
    hall->esquerda = criarSala("Biblioteca", "Livro aberto na pagina 214");
    hall->direita = criarSala("Sala de Estar", "Foto rasgada no chão");

    // Nível 2
    hall->esquerda->esquerda = criarSala("Sala de Leitura", "Carta antiga escondida");
    hall->esquerda->direita = criarSala("Jardim de Inverno", "Colher de jardinagem caida");
    hall->direita->esquerda = criarSala("Cozinha", "Faca enferrujada na mesa");
    hall->direita->direita = criarSala("Banheiro", "");

    // Nível 3
    hall->esquerda->esquerda->esquerda = criarSala("Sala de Música", "Piano desafinado tocando sozinho");
    hall->esquerda->esquerda->direita = criarSala("Estufa", "Saco de adubo ragado");
    hall->direita->esquerda->esquerda = criarSala("Varanda", "Cadeira de balanco tombada");
    hall->direita->esquerda->direita = criarSala("Quarto", "Diario com paginas arrancadas");

    printf("\nExplore os cômodos escolhendo ir para à esquerda ou para a direita!\n");

    // Inicia a exploração
    explorarSalas(hall, &bstPistas);

    // Resumo da investigação
    printf("\n------- Resumo da Investigação -------");
    printf("Pistas coletadas: %d\n", contarPistas(bstPistas));
    if (bstPistas != NULL) {
        printf("\nSuas pistas (em ordem alfabética):\n");
        exibirPistaEmOrdem(bstPistas);
    }
    printf("\n-------------------------------------------\n\n");

    // Libera a memória alocada
    liberarArvore(hall);
    liberarMemoriaBST(bstPistas);

    return 0;
}

