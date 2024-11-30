#include <stdio.h>
#include <stdlib.h>

typedef struct NoBPlus {
    int chave1;
    int chave2;
    struct NoBPlus *ptr1;
    struct NoBPlus *ptr2;
    struct NoBPlus *ptr3;
    struct NoBPlus *pai;
    int folha; // 1 se for folha, 0 se for nó interno
    int ref_count; // Contagem de referências
} NoBPlus;

// Protótipos das funções
NoBPlus* criarNo(int folha);
void inserirChave(NoBPlus** raiz, int chave);
void excluirChave(NoBPlus** raiz, int chave);
NoBPlus* buscarChave(NoBPlus* no, int chave);
void imprimirArvore(NoBPlus* no, int nivel);
void liberarArvore(NoBPlus* no);
void inserirEmFolha(NoBPlus* no, int chave);
void cisaoFolha(NoBPlus** raiz, NoBPlus* no, int chave);
void inserirEmNoInterno(NoBPlus** raiz, NoBPlus* no, int chave, NoBPlus* filhoDireita);
void cisaoNoInterno(NoBPlus** raiz, NoBPlus* no, int chave, NoBPlus* filhoDireita);
int indiceFilho(NoBPlus* pai, NoBPlus* filho);
void concatenarRedistribuir(NoBPlus** raiz, NoBPlus* no);
void atualizarChaves(NoBPlus* no);

// Função para criar um novo nó
NoBPlus* criarNo(int folha) {
    NoBPlus* no = (NoBPlus*)malloc(sizeof(NoBPlus));
    if (no == NULL) {
        fprintf(stderr, "Erro ao alocar memória para nó.\n");
        exit(1);  // Ou outro tratamento de erro adequado
    }
    no->chave1 = -1;
    no->chave2 = -1;
    no->ptr1 = no->ptr2 = no->ptr3 = NULL;
    no->pai = NULL;
    no->folha = folha;
    no->ref_count = 1; // Inicializa com uma referência
    return no;
}

void adicionarReferencia(NoBPlus* no) {
    if (no != NULL) {
        no->ref_count++;
    }
}

// Função auxiliar para inserir chave em um nó folha
void inserirEmFolha(NoBPlus* no, int chave) {
    if (no->chave1 == -1) {
        no->chave1 = chave;
    } else if (no->chave2 == -1) {
        if (chave < no->chave1) {
            no->chave2 = no->chave1;
            no->chave1 = chave;
        } else {
            no->chave2 = chave;
        }
    }
}

void inserirEmNoInterno(NoBPlus** raiz, NoBPlus* no, int chave, NoBPlus* filhoDireita) {
    if (no->chave2 == -1) {
        // Tem espaço no nó interno
        if (chave < no->chave1) {
            no->chave2 = no->chave1;
            no->chave1 = chave;
            no->ptr3 = no->ptr2;
            no->ptr2 = filhoDireita;
        } else {
            no->chave2 = chave;
            no->ptr3 = filhoDireita;
        }
        filhoDireita->pai = no;
    } else {
        // Nó interno cheio, precisa cisão
        cisaoNoInterno(raiz, no, chave, filhoDireita);
    }
}

void cisaoFolha(NoBPlus** raiz, NoBPlus* no, int chave) {
    int chaves[3] = { no->chave1, no->chave2, chave };
    // Ordena as chaves
    for (int i = 0; i < 2; i++) {
        for (int j = i + 1; j < 3; j++) {
            if (chaves[i] > chaves[j]) {
                int temp = chaves[i];
                chaves[i] = chaves[j];
                chaves[j] = temp;
            }
        }
    }

    // Atualiza o nó atual com as duas primeiras chaves
    no->chave1 = chaves[0];
    no->chave2 = -1;

    // Cria um novo nó folha
    NoBPlus* novoNo = criarNo(1);
    novoNo->chave1 = chaves[1];
    novoNo->chave2 = chaves[2];
    novoNo->pai = no->pai;

    // Sobe a chave para o pai
    int chaveSubir = novoNo->chave1;

    if (no->pai == NULL) {
        // Cria novo nó raiz
        NoBPlus* novoPai = criarNo(0);
        novoPai->chave1 = chaveSubir;
        novoPai->ptr1 = no;
        novoPai->ptr2 = novoNo;
        no->pai = novoPai;
        novoNo->pai = novoPai;
        *raiz = novoPai;
    } else {
        // Insere a chave no nó pai
        inserirEmNoInterno(raiz, no->pai, chaveSubir, novoNo);
    }
}

void cisaoNoInterno(NoBPlus** raiz, NoBPlus* no, int chave, NoBPlus* filhoDireita) {
    int chaves[3] = { no->chave1, no->chave2, chave };
    NoBPlus* filhos[4] = { no->ptr1, no->ptr2, no->ptr3, filhoDireita };

    // Ordena as chaves e ajusta os filhos associados
    for (int i = 0; i < 2; i++) {
        for (int j = i + 1; j < 3; j++) {
            if (chaves[i] > chaves[j]) {
                int tempChave = chaves[i];
                chaves[i] = chaves[j];
                chaves[j] = tempChave;
                NoBPlus* tempFilho = filhos[i + 1];
                filhos[i + 1] = filhos[j + 1];
                filhos[j + 1] = tempFilho;
            }
        }
    }

    // Chave do meio que irá subir para o pai
    int chaveMeio = chaves[1];

    // Cria novo nó à direita
    NoBPlus* novoNoDireita = criarNo(0);
    novoNoDireita->chave1 = chaves[2];
    novoNoDireita->chave2 = -1;
    novoNoDireita->ptr1 = filhos[2];
    novoNoDireita->ptr2 = filhos[3];
    novoNoDireita->ptr3 = NULL;

    // Atualiza os pais dos filhos do novo nó direito
    if (novoNoDireita->ptr1) novoNoDireita->ptr1->pai = novoNoDireita;
    if (novoNoDireita->ptr2) novoNoDireita->ptr2->pai = novoNoDireita;

    // Atualiza o nó atual para ser o nó esquerdo
    no->chave1 = chaves[0];
    no->chave2 = -1;
    no->ptr1 = filhos[0];
    no->ptr2 = filhos[1];
    no->ptr3 = NULL;

    // Atualiza os pais dos filhos do nó esquerdo
    if (no->ptr1) no->ptr1->pai = no;
    if (no->ptr2) no->ptr2->pai = no;

    // Ajusta o pai
    novoNoDireita->pai = no->pai;

    if (no->pai == NULL) {
        // Cria um novo nó raiz
        NoBPlus* novaRaiz = criarNo(0);
        novaRaiz->chave1 = chaveMeio;
        novaRaiz->chave2 = -1;
        novaRaiz->ptr1 = no;
        novaRaiz->ptr2 = novoNoDireita;
        novaRaiz->ptr3 = NULL;
        no->pai = novaRaiz;
        novoNoDireita->pai = novaRaiz;
        *raiz = novaRaiz;
    } else {
        // Insere a chave do meio no nó pai
        inserirEmNoInterno(raiz, no->pai, chaveMeio, novoNoDireita);
    }
}

void inserirChave(NoBPlus** raiz, int chave) {
    NoBPlus* no = *raiz;
    // Percorre a árvore até a folha apropriada
    while (!no->folha) {
        if (chave < no->chave1)
            no = no->ptr1;
        else if (no->chave2 == -1 || chave < no->chave2)
            no = no->ptr2;
        else
            no = no->ptr3;
    }
    // Insere a chave na folha
    if (no->chave2 == -1) {
        inserirEmFolha(no, chave);
    } else {
        // Nó folha cheio, precisa de cisão
        cisaoFolha(raiz, no, chave);
    }
}

NoBPlus* buscarChave(NoBPlus* no, int chave) {
    if (no == NULL)
        return NULL;
    if (no->folha) {
        if (no->chave1 == chave || no->chave2 == chave)
            return no;
        else
            return NULL;
    }
    if (chave < no->chave1 || no->chave1 == -1)
        return buscarChave(no->ptr1, chave);
    else if (no->chave2 == -1 || chave < no->chave2)
        return buscarChave(no->ptr2, chave);
    else
        return buscarChave(no->ptr3, chave);
}

int indiceFilho(NoBPlus* pai, NoBPlus* filho) {
    if (pai->ptr1 == filho)
        return 0;
    else if (pai->ptr2 == filho)
        return 1;
    else
        return 2;
}

int encontrarMenorChave(NoBPlus* no) {
    while (!no->folha) {
        no = no->ptr1;
    }
    return no->chave1;
}

void atualizarChaves(NoBPlus* no) {
    if (no == NULL || no->folha)
        return;

    // Atualiza chave1 para refletir a menor chave do ptr2
    if (no->ptr2 != NULL) {
        no->chave1 = encontrarMenorChave(no->ptr2);
    } else {
        no->chave1 = -1;
    }

    // Atualiza chave2 para refletir a menor chave do ptr3
    if (no->ptr3 != NULL) {
        no->chave2 = encontrarMenorChave(no->ptr3);
    } else {
        no->chave2 = -1;
    }
}

// Implementação da função excluirChave
void excluirChave(NoBPlus** raiz, int chave) {
    NoBPlus* no = *raiz;

    // Busca a folha que contém a chave
    while (!no->folha) {
        if (chave < no->chave1 || no->chave1 == -1)
            no = no->ptr1;
        else if (no->chave2 == -1 || chave < no->chave2)
            no = no->ptr2;
        else
            no = no->ptr3;
    }

    // Remove a chave do nó folha
    if (no->chave1 == chave) {
        no->chave1 = no->chave2;
        no->chave2 = -1;
    } else if (no->chave2 == chave) {
        no->chave2 = -1;
    } else {
        printf("Chave %d não encontrada na árvore.\n", chave);
        return;
    }

    // Se o nó ainda tem uma chave, atualiza os nós internos
    if (no->chave1 != -1) {
        atualizarChaves(no->pai);
        return;
    }

    // Ajusta a árvore se necessário
    concatenarRedistribuir(raiz, no);
}

void concatenarRedistribuir(NoBPlus** raiz, NoBPlus* no) {
    NoBPlus* pai = no->pai;
    if (pai == NULL) {
        *raiz = NULL;
        free(no);
        return;
    }
    int indice = indiceFilho(pai, no);

    NoBPlus* irmao = NULL;
    int indiceIrmao = -1;

    // Tenta pegar emprestado do irmão esquerdo
    if (indice > 0) {
        irmao = (indice == 1) ? pai->ptr1 : pai->ptr2;
        indiceIrmao = indice - 1;
    }
    // Se não conseguir, tenta pegar emprestado do irmão direito
    if (irmao == NULL || irmao->chave2 == -1) {
        if (indice < 2 && ((indice == 0 && pai->ptr2 != NULL) || (indice == 1 && pai->ptr3 != NULL))) {
            irmao = (indice == 0) ? pai->ptr2 : pai->ptr3;
            indiceIrmao = indice + 1;
        }
    }

    // Tenta redistribuir
    if (irmao != NULL && irmao->chave2 != -1) {
        if (indice < indiceIrmao) {
            // Pega emprestado do irmão direito
            no->chave1 = irmao->chave1;
            irmao->chave1 = irmao->chave2;
            irmao->chave2 = -1;
            // Atualiza as chaves no pai
            atualizarChaves(pai);
        } else {
            // Pega emprestado do irmão esquerdo
            no->chave1 = irmao->chave2;
            irmao->chave2 = -1;
            // Atualiza as chaves no pai
            atualizarChaves(pai);
        }
    } else {
        // Concatena com o irmão
        if (irmao != NULL) {
            if (indice < indiceIrmao) {
                // Concatena no com irmão direito
                if (irmao->chave2 == -1) {
                    irmao->chave2 = irmao->chave1;
                    irmao->chave1 = no->chave1;
                }
                free(no);
                if (indice == 0) {
                    pai->ptr1 = irmao;
                    pai->ptr2 = pai->ptr3;
                    pai->ptr3 = NULL;
                    pai->chave1 = pai->chave2;
                    pai->chave2 = -1;
                } else {
                    pai->ptr2 = pai->ptr3;
                    pai->ptr3 = NULL;
                    pai->chave2 = -1;
                }
            } else {
                // Concatena no com irmão esquerdo
                if (irmao->chave2 == -1) {
                    irmao->chave2 = no->chave1;
                }
                free(no);
                if (indice == 1) {
                    pai->ptr2 = pai->ptr3;
                    pai->ptr3 = NULL;
                    pai->chave1 = pai->chave2;
                    pai->chave2 = -1;
                } else {
                    pai->ptr3 = NULL;
                    pai->chave2 = -1;
                }
            }
            // Atualiza as chaves no pai
            atualizarChaves(pai);
        }

        // Se o pai ficou sem chaves, ajustar
        if (pai->chave1 == -1 && pai->pai == NULL) {
            *raiz = pai->ptr1;
            (*raiz)->pai = NULL;
            free(pai);
        } else if (pai->chave1 == -1) {
            concatenarRedistribuir(raiz, pai);
        }
    }
}

// Função para imprimir a árvore completa
void imprimirArvore(NoBPlus* no, int nivel) {
    if (no == NULL)
        return;
    for (int i = 0; i < nivel; i++)
        printf("    ");
    printf("Nó %p: chave1=%d, chave2=%d, ptr1=%p, ptr2=%p, ptr3=%p\n",
           (void*)no, no->chave1, no->chave2, (void*)no->ptr1, (void*)no->ptr2, (void*)no->ptr3);
    if (!no->folha) {
        imprimirArvore(no->ptr1, nivel + 1);
        imprimirArvore(no->ptr2, nivel + 1);
        if (no->ptr3 != NULL)
            imprimirArvore(no->ptr3, nivel + 1);
    }
}

// Função para liberar a árvore
void liberarArvore(NoBPlus* no) {
    if (no == NULL) return;

    // Se o nó for folha e tiver referências, diminua a contagem
    if (no->ref_count > 0) {
        no->ref_count--;
        return; // Não libera ainda, pois ainda há referências ao nó
    }

    // Liberar filhos (recursão) antes de liberar o próprio nó
    liberarArvore(no->ptr1);
    liberarArvore(no->ptr2);
    liberarArvore(no->ptr3);

    // Liberar o nó depois de seus filhos
    free(no);
}

// Função principal para testar
int main() {
    NoBPlus *raiz = criarNo(1); // Começa com um nó folha vazio

    // Inserir algumas chaves na árvore
    inserirChave(&raiz, 10);
    inserirChave(&raiz, 20);
    inserirChave(&raiz, 5);
    inserirChave(&raiz, 15);
    inserirChave(&raiz, 25);
    inserirChave(&raiz, 30);

    // Imprimir a árvore
    printf("Árvore B+ após inserções:\n");
    imprimirArvore(raiz, 0);

    // Buscar uma chave
    int chaveBusca = 15;
    NoBPlus* resultado = buscarChave(raiz, chaveBusca);
    if (resultado != NULL)
        printf("Chave %d encontrada no nó %p\n", chaveBusca, (void*)resultado);
    else
        printf("Chave %d não encontrada na árvore\n", chaveBusca);

    // Excluir uma chave
    excluirChave(&raiz, 20);
    printf("Árvore B+ após exclusão da chave 20:\n");
    imprimirArvore(raiz, 0);

    // Liberar a árvore
    liberarArvore(raiz);

    return 0;
}