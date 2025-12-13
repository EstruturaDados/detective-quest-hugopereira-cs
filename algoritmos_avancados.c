#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_NOME 50
#define MAX_PISTA 50
#define MAX_SUSPEITO 25
#define TAMANHO_HASH 20

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

// Estrutura para nó da lista encadeada (trata colisões)
typedef struct NoHash {
    char pista[MAX_PISTA];
    char suspeito[MAX_SUSPEITO];
    struct NoHash* proximo;
} NoHash;

// Estrutura da tabela hash
typedef struct {
    NoHash* tabela[TAMANHO_HASH];
} TabelaHash;

// ======== PROTÓTIPOS DAS FUNÇÕES ========
// Funções da Tabela Hash
int funcaoHash(const char* chave);
TabelaHash* criarTabelaHash();
void inserirHash(TabelaHash* tabela, const char* pista, const char* suspeito);
char* buscarSupeitoHash(TabelaHash* tabela, const char* pista);
void exibirAssociacoes(TabelaHash* tabela, NoPista* bstPistas);
char* analisarSuspeitos(TabelaHash* tabela, NoPista* bstPistas);
void liberarTabelaHash(TabelaHash* tabela);

// Funções da BST
NoPista* criarNoPista(const char* pista);
NoPista* inserirPista(NoPista* raiz, const char* pista);
int buscarPista(NoPista* raiz, const char* pista);
void exibirPistaEmOrdem(NoPista* raiz);
int contarPistas(NoPista* raiz);
void liberarMemoriaBST(NoPista* raiz);

// Funções das Salas
Sala* criarSala(const char* nome, const char* pista);
void explorarSalas(Sala *salaAtual, NoPista** bstPistas, TabelaHash* tabelaSuspeitos);
void liberarArvore(Sala *raiz);

int main() {
    setlocale(LC_ALL, "Portuguese");

    // Construindo a árvore BST
    NoPista* bstPistas = NULL;

    // Criando a tabela hash para suspeitos
    TabelaHash* tabelaSuspeitos = criarTabelaHash();

    // Define quais pistas pertencem a cada suspeito
    inserirHash(tabelaSuspeitos, "Livro aberto na pagina 214", "Professor Marcus");
    inserirHash(tabelaSuspeitos, "Foto rasgada no chão", "Condessa Helena");
    inserirHash(tabelaSuspeitos, "Carta antiga escondida", "Professor Marcus");
    inserirHash(tabelaSuspeitos, "Colher de jardinagem caida", "Jardineiro Thomas");
    inserirHash(tabelaSuspeitos, "Faca enferrujada na mesa", "Chef Antoine");

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

    printf("\nHISTÒRIA:\n");
    printf("Um crime ocorreu nesta mansão!\n");
    printf("\nExplore os cômodos, colete pistas e descubra quem é o culpado!\n");
    printf("\nEscolha se quer ir para a esquerda ou para a direita.\n");
    printf("\nPressione enter para continuar...");
    getchar();

    // Inicia a exploração
    explorarSalas(hall, &bstPistas, tabelaSuspeitos);

    // Resumo da investigação
    printf("\n------- Resumo da Investigação -------\n");
    printf("Pistas coletadas: %d\n", contarPistas(bstPistas));
    if (bstPistas != NULL) {
        printf("\nSuas pistas (em ordem alfabética):\n");
        exibirPistaEmOrdem(bstPistas);
    }
    printf("\n-------------------------------------------\n\n");

    // Exibe associações
    exibirAssociacoes(tabelaSuspeitos, bstPistas);

    char* culpadoReal = analisarSuspeitos(tabelaSuspeitos, bstPistas);

    // Pede ao jogador para adivinhar o culpado
    if (culpadoReal != NULL && contarPistas(bstPistas) > 0) {
        char palpite [MAX_SUSPEITO];

        printf("\n======== JULGAMENTO =======\n");
        printf("\nBaseado nas pistas coletadas, quem você acha que é o culpado?\n");
        printf("\nDigite o nome do supeito: ");
        fgets(palpite, MAX_SUSPEITO, stdin);
        // Remove o \n do final
        palpite[strcspn(palpite, "\n")] = '\0';
        
        printf("\n-------------------------------------------\n");
        
        // Compara com o culpado real (ignora maiúsculas/minúsculas)
        if (strcasecmp(palpite, culpadoReal) == 0) {
            printf("\nPARABÉNS! VOCÊ ACERTOU!\n\n");
            printf("O culpado era mesmo %s!\n", culpadoReal);
        } else {
            printf("\nQue pena! Você errou!\n\n");
            printf("Seu palpite: %s\n", palpite);
            printf("O verdadeiro culpado: %s\n\n", culpadoReal);
            printf("Analise as pistas com mais atenção da próxima vez!\n");
        }
        printf("\n-------------------------------------------\n\n");
    }

    printf("\nObrigado por auxiliar nesta investigação!\n");

    // Libera a memória alocada
    liberarArvore(hall);
    liberarMemoriaBST(bstPistas);

    printf("\nAté a próxima investigação!\n\n");

    return 0;
}

// ========= FUNÇOES DA TABELA HASH =========
// Soma ASCII % tamanho
int funcaoHash( const char* chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++) {
        soma += chave[i];
    }
    return soma % TAMANHO_HASH;
}

// Inicializa a tabela hash
TabelaHash* criarTabelaHash() {
    TabelaHash* tabela = (TabelaHash*)malloc(sizeof(TabelaHash));
    if (tabela == NULL) {
        printf("Erro ao alocar memória para a tabela hash!\n");
        exit(1);
    }

    for (int i = 0; i < TAMANHO_HASH; i++) {
        tabela->tabela[i] = NULL;
    }

    return tabela;
}

// Insere pista e suspeito na tabela hash
void inserirHash(TabelaHash* tabela, const char* pista, const char* suspeito) {
    int indice = funcaoHash(pista);

    // Cria novo nó
    NoHash* novo = (NoHash*)malloc(sizeof(NoHash));
    if (novo == NULL) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }

    strcpy(novo-> pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->proximo = NULL;

    // Insere no início da lista (tratamento de colisão)
    if (tabela->tabela[indice] == NULL) {
        tabela->tabela[indice] = novo;
    } else {
        novo->proximo = tabela->tabela[indice];
        tabela->tabela[indice] = novo;
    }
}

// Busca suspeito associado a uma pista
char* buscarSupeitoHash(TabelaHash* tabela, const char* pista) {
    int indice = funcaoHash(pista);
    NoHash* atual = tabela->tabela[indice];

    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito;
        }
        atual = atual->proximo;
    }
    return NULL;
}

// Exibe todas as associações pista -> suspeita
void exibirAssociacoes(TabelaHash* tabela, NoPista* bstPistas) {
    printf("\n======= ASSOCIAÇÕES PISTA -> SUSPEITO =======\n");
    int encontrouAlguma = 0;

    for (int i = 0; i < TAMANHO_HASH; i++) {
        NoHash* atual = tabela->tabela[i];
        while (atual != NULL) {
            // Verifica se a pista foi coletada (está na BST)
            if (buscarPista(bstPistas, atual->pista)) {
                printf("    \"%s\" -> %s\n", atual->pista, atual->suspeito);
                encontrouAlguma = 1;
            }
            atual = atual->proximo;
        }
    }

    if (!encontrouAlguma) {
        printf("Nenhuma associação registrada ainda.\n");
    }
    printf("\n---------------------------------------------\n");
}

// Conta quantas vezes cada suspeito aparece (apenas pistas coletadas)
char* analisarSuspeitos(TabelaHash* tabela, NoPista* bstPistas) {
    // Array para contar suspeitos (máximo de 5 suspeitos diferentes)
    static char suspeitos[5][MAX_SUSPEITO];
    int contagens[5] = {0};
    int numSuspeitos = 0;

    // Percorre toda a tabela hash
    for (int i = 0; i < TAMANHO_HASH; i++) {
        NoHash* atual = tabela->tabela[i];
        while (atual != NULL) {
            // Verifica se a pista foi coletada
            if (buscarPista(bstPistas, atual->pista)) {
                int encontrado = 0;
                for (int j = 0; j < numSuspeitos; j++) {
                    if (strcmp(suspeitos[j], atual->suspeito) == 0) {
                        contagens[j]++;
                        encontrado = 1;
                        break;
                    }
                }
                // Adiciona novo suspeito, caso não tenha encontrado
                if (!encontrado && numSuspeitos < 5) {
                    strcpy(suspeitos[numSuspeitos], atual->suspeito);
                    contagens[numSuspeitos] = 1;
                    numSuspeitos++;
                }
            }
            atual = atual->proximo;
        }
    }

    // Exibe a análise
    printf("\n======= Análise de Suspeitos =======\n");
    if (numSuspeitos == 0) {
        printf("Nenhum suspeito identificado ainda.\n");
        return NULL;
    }
    printf("Número de pistas por suspeito:\n\n");

    int maxContagem = 0;
    int indiceMaiorSuspeito = 0;

    for (int i = 0; i < numSuspeitos; i++) {
        printf("\n    %s: %d pista(s)\n", suspeitos[i], contagens[i]);
        if (contagens[i] > maxContagem) {
            maxContagem = contagens[i];
            indiceMaiorSuspeito = i;
        }
    }

    printf("\nSUSPEITO PRINCIPAL: %s (%d pista(s))\n", suspeitos[indiceMaiorSuspeito], contagens[indiceMaiorSuspeito]);
    printf("\n---------------------------------------------\n");
    return suspeitos[indiceMaiorSuspeito];
}

// Libera memória da tabela hash
void liberarTabelaHash(TabelaHash* tabela) {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        NoHash* atual = tabela->tabela[i];
        while (atual != NULL) {
            NoHash* temp = atual;
            atual = atual->proximo;
            free(temp);
        }
    }
    free(tabela);
}

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

// ======= FUNÇÃO PRINCIPAL DO JOGO ========
// Função para explorar salas interativamente
void explorarSalas(Sala *salaAtual, NoPista** bstPistas, TabelaHash* tabelaSuspeitos) { // ** ---> Um ponteiro para um ponteiro de nó de pista
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
        printf("\nPara onde você quer ir?\n");
        if (salaAtual->esquerda != NULL) {
            printf("   [e] Esquerda <- %s\n", salaAtual->esquerda->nome);
        }
        if (salaAtual->direita != NULL) {
            printf("   [d] Direita -> %s\n", salaAtual->direita->nome);
        }

        printf("   [p] Ver pistas coletadas\n");
        printf("   [a] Ver associações pista -> suspeito\n");
        printf("   [s] Sair e finalizar a investigação\n");
        
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
                    printf("\nNão há caminho à esquerda!\n");
                    printf("Pressione enter para continuar...");
                    getchar();
                }
                break;
            case 'd':
                if (salaAtual->direita != NULL) {
                    salaAtual = salaAtual->direita;
                } else {
                    printf("Não há caminho à direita!\n");
                    printf("Pressione enter para continuar...");
                    getchar();
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
            case 'a':
                exibirAssociacoes(tabelaSuspeitos, *bstPistas);
                printf("\nPressione enter para continuar...");
                getchar();
                break;
            case 's':
                printf("Saindo do jogo... Até logo!\n");
                return;
            default:
                printf("\nOpção inválida! Use 'e', 'd', 'p', 'a' ou 's'.\n");
                printf("Pressione enter para continuar...");
                getchar();
        }
    }

    printf("\nExploração finalizada!\n");
}

void liberarArvore(Sala *raiz) {
    if (raiz != NULL) {
        liberarArvore(raiz->esquerda);
        liberarArvore(raiz->direita);
        free(raiz);
    }
}
