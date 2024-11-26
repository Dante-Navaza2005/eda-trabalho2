#include <stdio.h>
#include <stdlib.h>

#define MAX_CHAVES 2   // Número máximo de chaves em um nó
#define MIN_CHAVES 1   // Número mínimo de chaves em um nó
#define MAX_PONTEIROS 3 // Número máximo de ponteiros em um nó (MAX_CHAVES + 1)

typedef struct NoArvoreBPlus {
    int chaves[MAX_CHAVES];
    struct NoArvoreBPlus *ponteiros[MAX_PONTEIROS];
    int num_chaves;
    int eh_folha;
} NoArvoreBPlus;

// Função para criar um novo nó
NoArvoreBPlus *criarNo(int eh_folha) {
    NoArvoreBPlus *no = (NoArvoreBPlus *)malloc(sizeof(NoArvoreBPlus));
    no->eh_folha = eh_folha;
    no->num_chaves = 0;
    for (int i = 0; i < MAX_CHAVES; i++)
        no->chaves[i] = -1;
    for (int i = 0; i < MAX_PONTEIROS; i++)
        no->ponteiros[i] = NULL;
    return no;
}

// Função para encontrar o índice da chave
int encontrarIndiceChave(NoArvoreBPlus *no, int chave) {
    int idx = 0;
    while (idx < no->num_chaves && no->chaves[idx] < chave)
        idx++;
    return idx;
}

// Função de busca
NoArvoreBPlus *buscar(NoArvoreBPlus *raiz, int chave) {
    NoArvoreBPlus *atual = raiz;
    while (atual != NULL) {
        int idx = encontrarIndiceChave(atual, chave);
        if (idx < atual->num_chaves && atual->chaves[idx] == chave) {
            return atual;  // Chave encontrada
        }
        if (atual->eh_folha) {
            return NULL;  // Chave não encontrada
        } else {
            atual = atual->ponteiros[idx];
        }
    }
    return NULL;
}

// Função para dividir o nó
void dividirFilho(NoArvoreBPlus *pai, int idx, NoArvoreBPlus *filho) {
    NoArvoreBPlus *novoFilho = criarNo(filho->eh_folha);
    novoFilho->num_chaves = MIN_CHAVES;

    // Copia as últimas chaves para o novo nó
    for (int i = 0; i < MIN_CHAVES; i++) {
        novoFilho->chaves[i] = filho->chaves[i + MIN_CHAVES];
        filho->chaves[i + MIN_CHAVES] = -1;
    }

    // Se não for folha, copia os ponteiros
    if (!filho->eh_folha) {
        for (int i = 0; i <= MIN_CHAVES; i++) {
            novoFilho->ponteiros[i] = filho->ponteiros[i + MIN_CHAVES];
            filho->ponteiros[i + MIN_CHAVES] = NULL;
        }
    }

    filho->num_chaves = MIN_CHAVES;

    // Move os ponteiros no pai
    for (int i = pai->num_chaves; i >= idx + 1; i--) {
        pai->ponteiros[i + 1] = pai->ponteiros[i];
    }
    pai->ponteiros[idx + 1] = novoFilho;

    // Move as chaves no pai
    for (int i = pai->num_chaves - 1; i >= idx; i--) {
        pai->chaves[i + 1] = pai->chaves[i];
    }
    pai->chaves[idx] = novoFilho->chaves[0]; // Promove a menor chave do novo nó
    pai->num_chaves++;
}

// Função para inserir em nó não cheio
void inserirNaoCheio(NoArvoreBPlus *no, int chave) {
    int idx = no->num_chaves - 1;

    if (no->eh_folha) {
        while (idx >= 0 && no->chaves[idx] > chave) {
            no->chaves[idx + 1] = no->chaves[idx];
            idx--;
        }
        no->chaves[idx + 1] = chave;
        no->num_chaves++;
    } else {
        while (idx >= 0 && no->chaves[idx] > chave)
            idx--;
        idx++;
        if (no->ponteiros[idx]->num_chaves == MAX_CHAVES) {
            dividirFilho(no, idx, no->ponteiros[idx]);
            if (no->chaves[idx] < chave)
                idx++;
        }
        inserirNaoCheio(no->ponteiros[idx], chave);
    }
}

// Função principal de inserção
NoArvoreBPlus *inserir(NoArvoreBPlus *raiz, int chave) {
    if (raiz == NULL) {
        raiz = criarNo(1);
        raiz->chaves[0] = chave;
        raiz->num_chaves = 1;
        return raiz;
    }

    if (raiz->num_chaves == MAX_CHAVES) {
        NoArvoreBPlus *novaRaiz = criarNo(0);
        novaRaiz->ponteiros[0] = raiz;
        dividirFilho(novaRaiz, 0, raiz);
        int idx = (novaRaiz->chaves[0] < chave) ? 1 : 0;
        inserirNaoCheio(novaRaiz->ponteiros[idx], chave);
        return novaRaiz;
    } else {
        inserirNaoCheio(raiz, chave);
        return raiz;
    }
}

// Função para imprimir a árvore
void imprimirArvore(NoArvoreBPlus *no, int nivel) {
    if (no != NULL) {
        printf("Nível %d | Ponteiro: %p | ", nivel, no);
        printf("Chaves: ");
        for (int i = 0; i < MAX_CHAVES; i++) {
            printf("%d ", no->chaves[i]);
        }
        printf("| Ponteiros: ");
        for (int i = 0; i < MAX_PONTEIROS; i++) {
            printf("%p ", no->ponteiros[i]);
        }
        printf("\n");

        if (!no->eh_folha) {
            for (int i = 0; i <= no->num_chaves; i++) {
                imprimirArvore(no->ponteiros[i], nivel + 1);
            }
        }
    }
}

// Função principal
int main() {
    NoArvoreBPlus *raiz = NULL;

    // Inserção de chaves
    int valores[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; i++) {
        raiz = inserir(raiz, valores[i]);
    }

    // Impressão da árvore
    printf("Árvore B+ após inserções:\n");
    imprimirArvore(raiz, 0);

    // Busca
    int chaveParaBuscar = 3;
    NoArvoreBPlus *noEncontrado = buscar(raiz, chaveParaBuscar);
    if (noEncontrado != NULL) {
        printf("Chave %d encontrada no nó com ponteiro: %p\n", chaveParaBuscar, noEncontrado);
    } else {
        printf("Chave %d não encontrada na árvore.\n", chaveParaBuscar);
    }

    return 0;
}
