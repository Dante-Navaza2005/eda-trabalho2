#include <stdio.h>
#include <stdlib.h>

#define MAX_CHAVES 2
#define MAX_PONTEIROS 3

typedef struct NoBMais {
    int chaves[MAX_CHAVES];
    struct NoBMais* ponteiros[MAX_PONTEIROS];
    struct NoBMais* proximo;
    int numChaves;
    int folha;
} NoBMais;

NoBMais* criaNo(int folha) {
    NoBMais* novoNo = (NoBMais*)malloc(sizeof(NoBMais));
    novoNo->numChaves = 0;
    novoNo->folha = folha;
    novoNo->proximo = NULL;
    for (int i = 0; i < MAX_PONTEIROS; i++) {
        novoNo->ponteiros[i] = NULL;
    }
    return novoNo;
}

void imprimeNo(NoBMais* no) {
    printf("(");
    for (int i = 0; i < MAX_CHAVES; i++) {
        if (i < no->numChaves) {
            printf("%d", no->chaves[i]);
        } else {
            printf("_"); // Exibir "_" para chaves vazias
        }
        if (i < MAX_CHAVES - 1) printf(", ");
    }
    for (int i = 0; i < MAX_PONTEIROS; i++) {
        printf(", %p", (void*)no->ponteiros[i]); // Exibe o ponteiro
    }
    printf(") ");
}

void imprimeArvore(NoBMais* no) {
    if (no == NULL) return;
    imprimeNo(no);
    if (!no->folha) {
        for (int i = 0; i <= no->numChaves; i++) {
            imprimeArvore(no->ponteiros[i]);
        }
    }
}

NoBMais* buscaChave(NoBMais* no, int chave) {
    int i = 0;
    while (i < no->numChaves && chave > no->chaves[i]) i++;
    if (i < no->numChaves && chave == no->chaves[i]) return no;
    return no->folha ? NULL : buscaChave(no->ponteiros[i], chave);
}

void insereNaFolha(NoBMais* no, int chave) {
    int i;
    for (i = no->numChaves - 1; i >= 0 && no->chaves[i] > chave; i--) {
        no->chaves[i + 1] = no->chaves[i];
    }
    no->chaves[i + 1] = chave;
    no->numChaves++;
}

NoBMais* insere(NoBMais* raiz, int chave) {
    if (!raiz) {
        raiz = criaNo(1);
        raiz->chaves[0] = chave;
        raiz->numChaves = 1;
        return raiz;
    }
    if (raiz->folha) {
        if (raiz->numChaves < MAX_CHAVES) {
            insereNaFolha(raiz, chave);
            return raiz;
        }
        // Dividir o nó se estiver cheio (simples para fins de exemplo)
        NoBMais* novoNo = criaNo(1);
        novoNo->chaves[0] = chave;
        novoNo->numChaves = 1;
        novoNo->proximo = raiz->proximo;
        raiz->proximo = novoNo;
        return raiz;
    }
    return raiz;
}

NoBMais* removeChave(NoBMais* no, int chave) {
    int i = 0;
    while (i < no->numChaves && chave > no->chaves[i]) i++;
    if (i < no->numChaves && chave == no->chaves[i]) {
        for (int j = i; j < no->numChaves - 1; j++) {
            no->chaves[j] = no->chaves[j + 1];
        }
        no->numChaves--;
    }
    return no;
}

int main() {
    NoBMais* raiz = NULL;

    // Inserção de chaves
    raiz = insere(raiz, 10);
    raiz = insere(raiz, 20);
    raiz = insere(raiz, 5);
    raiz = insere(raiz, 30);

    printf("Estrutura da Árvore B+: ");
    imprimeArvore(raiz);
    printf("\n");

    NoBMais* encontrado = buscaChave(raiz, 20);
    if (encontrado) printf("Chave 20 encontrada!\n");
    else printf("Chave 20 não encontrada!\n");

    raiz = removeChave(raiz, 10);
    printf("Estrutura após remoção de 10: ");
    imprimeArvore(raiz);
    printf("\n");

    return 0;
}
