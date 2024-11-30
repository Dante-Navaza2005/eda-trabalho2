#include <stdio.h>
#include <stdlib.h>

// Estrutura do nó da árvore B+
typedef struct NoBPlus {
    int chave1, chave2;
    struct NoBPlus *ptr1, *ptr2, *ptr3;
    struct NoBPlus *pai;
    int folha; // 1 se for folha, 0 caso contrário
} NoBPlus;

// Funções auxiliares
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

// Função para criar um novo nó
NoBPlus* criarNo(int folha) {
    NoBPlus* no = (NoBPlus*)malloc(sizeof(NoBPlus));
    no->chave1 = -1;
    no->chave2 = -1;
    no->ptr1 = NULL;
    no->ptr2 = NULL;
    no->ptr3 = NULL;
    no->pai = NULL;
    no->folha = folha;
    return no;
}

// Função auxiliar para inserir em um nó folha
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

// Função auxiliar para realizar cisão de folhas
void cisaoFolha(NoBPlus** raiz, NoBPlus* no, int chave) {
    int chaves[3] = {no->chave1, no->chave2, chave};
    for (int i = 0; i < 2; i++) {
        for (int j = i + 1; j < 3; j++) {
            if (chaves[i] > chaves[j]) {
                int temp = chaves[i];
                chaves[i] = chaves[j];
                chaves[j] = temp;
            }
        }
    }

    no->chave1 = chaves[0];
    no->chave2 = -1;

    NoBPlus* novoNo = criarNo(1);
    novoNo->chave1 = chaves[1];
    novoNo->chave2 = chaves[2];
    novoNo->pai = no->pai;

    int chaveMeio = novoNo->chave1;

    if (no->pai == NULL) {
        NoBPlus* novoPai = criarNo(0);
        novoPai->chave1 = chaveMeio;
        novoPai->ptr1 = no;
        novoPai->ptr2 = novoNo;
        no->pai = novoPai;
        novoNo->pai = novoPai;
        *raiz = novoPai;
    } else {
        inserirEmNoInterno(raiz, no->pai, chaveMeio, novoNo);
    }
}

// Função auxiliar para inserir em um nó interno
void inserirEmNoInterno(NoBPlus** raiz, NoBPlus* no, int chave, NoBPlus* filhoDireita) {
    if (no->chave2 == -1) {
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
        cisaoNoInterno(raiz, no, chave, filhoDireita);
    }
}

// Função auxiliar para realizar cisão de nós internos
void cisaoNoInterno(NoBPlus** raiz, NoBPlus* no, int chave, NoBPlus* filhoDireita) {
    int chaves[3] = {no->chave1, no->chave2, chave};
    NoBPlus* filhos[4] = {no->ptr1, no->ptr2, no->ptr3, filhoDireita};

    for (int i = 0; i < 2; i++) {
        for (int j = i + 1; j < 3; j++) {
            if (chaves[i] > chaves[j]) {
                int temp = chaves[i];
                chaves[i] = chaves[j];
                chaves[j] = temp;

                NoBPlus* tempFilho = filhos[i + 1];
                filhos[i + 1] = filhos[j + 1];
                filhos[j + 1] = tempFilho;
            }
        }
    }

    NoBPlus* novoNo = criarNo(0);
    novoNo->chave1 = chaves[2];
    novoNo->chave2 = -1;
    novoNo->ptr1 = filhos[2];
    novoNo->ptr2 = filhos[3];
    novoNo->ptr3 = NULL;

    novoNo->ptr1->pai = novoNo;
    novoNo->ptr2->pai = novoNo;

    no->chave1 = chaves[0];
    no->chave2 = -1;
    no->ptr1 = filhos[0];
    no->ptr2 = filhos[1];
    no->ptr3 = NULL;

    no->ptr1->pai = no;
    no->ptr2->pai = no;

    int chaveMeio = chaves[1];

    novoNo->pai = no->pai;

    if (no->pai == NULL) {
        NoBPlus* novoPai = criarNo(0);
        novoPai->chave1 = chaveMeio;
        novoPai->ptr1 = no;
        novoPai->ptr2 = novoNo;
        no->pai = novoPai;
        novoNo->pai = novoPai;
        *raiz = novoPai;
    } else {
        inserirEmNoInterno(raiz, no->pai, chaveMeio, novoNo);
    }
}

// Função para inserir uma chave na árvore B+
void inserirChave(NoBPlus** raiz, int chave) {
    NoBPlus* no = *raiz;
    while (no->folha == 0) {
        if (chave < no->chave1) {
            no = no->ptr1;
        } else if (no->chave2 == -1 || chave < no->chave2) {
            no = no->ptr2;
        } else {
            no = no->ptr3;
        }
    }

    if (no->chave2 == -1) {
        inserirEmFolha(no, chave);
    } else {
        cisaoFolha(raiz, no, chave);
    }
}

// Função para encontrar a menor chave em um nó
int encontrarMenorChave(NoBPlus* no) {
    while (!no->folha) {
        no = no->ptr1;
    }
    return no->chave1;
}

// Função para atualizar as chaves de um nó pai
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

// Função para identificar o índice do filho no nó pai
int indiceFilho(NoBPlus* pai, NoBPlus* filho) {
    if (pai->ptr1 == filho) {
        return 0;  // Primeiro filho
    } else if (pai->ptr2 == filho) {
        return 1;  // Segundo filho
    } else if (pai->ptr3 == filho) {
        return 2;  // Terceiro filho
    }
    return -1;  // Caso o nó filho não seja encontrado
}

// Função de exclusão (modificada para garantir a exclusão de todas as instâncias da chave)
void excluirChave(NoBPlus** raiz, int chave) {
    NoBPlus* no = *raiz;

    // 1. Buscar a chave nas folhas
    while (no != NULL && !no->folha) {
        if (chave < no->chave1 || no->chave1 == -1) {
            no = no->ptr1;
        } else if (no->chave2 == -1 || chave < no->chave2) {
            no = no->ptr2;
        } else {
            no = no->ptr3;
        }
    }

    // 2. Excluir todas as instâncias da chave nas folhas
    while (no != NULL) {
        if (no->chave1 == chave) {
            no->chave1 = no->chave2;  // Move chave2 para chave1
            no->chave2 = -1;           // Apaga chave2
        } else if (no->chave2 == chave) {
            no->chave2 = -1;           // Apaga chave2
        }

        // Se o nó ficou vazio após a exclusão, redistribuir ou mesclar
        if (no->chave1 == -1) {
            concatenarRedistribuir(raiz, no);  // Chama para redistribuir ou mesclar
        }

        // Continuar a busca no próximo nó pai
        no = buscarChave(no->pai, chave);  // Continua a busca no nó pai
    }

    // 3. Atualiza a raiz caso a chave tenha sido removida da raiz
    if (*raiz != NULL && (*raiz)->chave1 == -1) {
        NoBPlus* oldRaiz = *raiz;
        if ((*raiz)->ptr1 != NULL) {
            *raiz = (*raiz)->ptr1;  // A nova raiz será o primeiro filho
            (*raiz)->pai = NULL;    // A raiz não tem pai
        } else {
            *raiz = NULL;
        }
        free(oldRaiz);
    }

    // Se a chave não for encontrada, apenas imprime uma mensagem de erro
    if (no == NULL) {
        printf("Chave %d não encontrada na árvore.\n", chave);
    }
}


// Função para buscar uma chave no nó (para encontrar a instância no nó pai)
NoBPlus* buscarChave(NoBPlus* no, int chave) {
    if (no == NULL)
        return NULL;

    // Se o nó for folha, verifica se a chave está presente
    if (no->folha) {
        if (no->chave1 == chave || no->chave2 == chave)
            return no;
        else
            return NULL;
    }

    // Se o nó não for folha, percorre os ponteiros internos
    if (chave < no->chave1 || no->chave1 == -1)
        return buscarChave(no->ptr1, chave);
    else if (no->chave2 == -1 || chave < no->chave2)
        return buscarChave(no->ptr2, chave);
    else
        return buscarChave(no->ptr3, chave);
}

// Função para concatenar ou redistribuir nós após a exclusão de uma chave
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


// Função para imprimir a árvore
void imprimirArvore(NoBPlus* no, int nivel) {
    if (no == NULL) return;

    // Imprime o nível da árvore, o endereço do nó e os ponteiros
    for (int i = 0; i < nivel; i++) printf("  ");
    printf("Nó %p: [Chaves: %d, %d]  ptr1: %p  ptr2: %p  ptr3: %p\n", 
           (void*)no, no->chave1, no->chave2, 
           (void*)no->ptr1, (void*)no->ptr2, (void*)no->ptr3);

    // Imprime as subárvores, se houver
    if (!no->folha) {
        imprimirArvore(no->ptr1, nivel + 1);
        imprimirArvore(no->ptr2, nivel + 1);
        if (no->ptr3 != NULL) {
            imprimirArvore(no->ptr3, nivel + 1);
        }
    }
}

// Função para liberar a memória da árvore
void liberarArvore(NoBPlus* no) {
    if (no == NULL) return;

    liberarArvore(no->ptr1);
    liberarArvore(no->ptr2);
    liberarArvore(no->ptr3);

    free(no);
}

// Função principal para teste
int main() {
    NoBPlus* raiz = criarNo(1);

    // Inserir chaves
    inserirChave(&raiz, 10);
    inserirChave(&raiz, 20);
    inserirChave(&raiz, 5);
    inserirChave(&raiz, 15);
    inserirChave(&raiz, 25);
    inserirChave(&raiz, 30);

    // Imprimir a árvore
    printf("Árvore B+ após inserções:\n");
    imprimirArvore(raiz, 0);

    // Excluir uma chave
    excluirChave(&raiz, 15);
    printf("\nÁrvore B+ após exclusão da chave 15:\n");
    imprimirArvore(raiz, 0);

    // Liberar memória
    liberarArvore(raiz);

    return 0;
}
