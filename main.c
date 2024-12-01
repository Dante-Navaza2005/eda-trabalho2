// Dante Honorato Navaza 2321406 3WC
// Rafael Soares 2320470  3WA
// Guilherme Cunha 2320951 3WA

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
} NoBPlus;


NoBPlus* criarNo(int folha);
void inserir_chave(NoBPlus** raiz, int chave);
void excluir_chave(NoBPlus** raiz, int chave);
NoBPlus* buscar_chave(NoBPlus* no, int chave);
void imprimir_arvore(NoBPlus* no, int nivel);
void inserir_em_folha(NoBPlus* no, int chave);
void cisao_folha(NoBPlus** raiz, NoBPlus* no, int chave);
void inserir_em_noInterno(NoBPlus** raiz, NoBPlus* no, int chave, NoBPlus* filhoDireita);
void cisao_no_interno(NoBPlus** raiz, NoBPlus* no, int chave, NoBPlus* filhoDireita);
int indice_filho(NoBPlus* pai, NoBPlus* filho);
void concatenar_redistribuir(NoBPlus** raiz, NoBPlus* no);
void atualizar_chaves(NoBPlus* no);
int encontrar_menor(NoBPlus* no);


NoBPlus* criarNo(int folha) 
{
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

void inserir_em_folha(NoBPlus* no, int chave) 
{
    if (no->chave1 == -1) 
    {
        no->chave1 = chave;
    } 
    else if (no->chave2 == -1) 
    {
        if (chave < no->chave1)
        {
            no->chave2 = no->chave1;
            no->chave1 = chave;
        } 
        else 
        {
            no->chave2 = chave;
        }
    }
}

void cisao_folha(NoBPlus** raiz, NoBPlus* no, int chave) 
{
    int chaves[3] = { no->chave1, no->chave2, chave };
    for (int i = 0; i < 2; i++) //ordem é 2
    {
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

    if (no->pai == NULL) 
    {
        NoBPlus* novoPai = criarNo(0);
        novoPai->chave1 = chaveMeio;
        novoPai->ptr1 = no;
        novoPai->ptr2 = novoNo;
        no->pai = novoPai;
        novoNo->pai = novoPai;
        *raiz = novoPai;
    } 
    else 
    {
        inserir_em_noInterno(raiz, no->pai, chaveMeio, novoNo);
    }
}

void inserir_em_noInterno(NoBPlus** raiz, NoBPlus* no, int chave, NoBPlus* filhoDireita) {
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
    } 
    else 
    {
        cisao_no_interno(raiz, no, chave, filhoDireita);
    }
}


void cisao_no_interno(NoBPlus** raiz, NoBPlus* no, int chave, NoBPlus* filhoDireita) {
    int chaves[3] = { no->chave1, no->chave2, chave };
    NoBPlus* filhos[4] = { no->ptr1, no->ptr2, no->ptr3, filhoDireita };

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

    
    NoBPlus* novoNo = criarNo(0); // criando o no novo
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
    } 
    else 
    {
        inserir_em_noInterno(raiz, no->pai, chaveMeio, novoNo);
    }
}

void inserir_chave(NoBPlus** raiz, int chave) {
    NoBPlus* no = *raiz;
    while (!no->folha) {
        if (chave < no->chave1 || no->chave1 == -1)
            no = no->ptr1;
        else if (no->chave2 == -1 || chave < no->chave2)
            no = no->ptr2;
        else
            no = no->ptr3;
    }
    if (no->chave2 == -1) 
    {
        inserir_em_folha(no, chave);
    } 
    else 
    {
        cisao_folha(raiz, no, chave);
    }
}

NoBPlus* buscar_chave(NoBPlus* no, int chave) {
    if (no == NULL)
        return NULL;
    if (no->folha) {
        if (no->chave1 == chave || no->chave2 == chave)
            return no;
        else
            return NULL;
    }
    if (chave < no->chave1 || no->chave1 == -1)
        return buscar_chave(no->ptr1, chave);
    else if (no->chave2 == -1 || chave < no->chave2)
        return buscar_chave(no->ptr2, chave);
    else
        return buscar_chave(no->ptr3, chave);
}

int indice_filho(NoBPlus* pai, NoBPlus* filho) {
    if (pai->ptr1 == filho)
        return 0;
    else if (pai->ptr2 == filho)
        return 1;
    else
        return 2;
}

int encontrar_menorChave(NoBPlus* no) {
    while (!no->folha) 
    {
        no = no->ptr1;
    }
    return no->chave1;
}

void atualizar_chaves(NoBPlus* no) {
    if (no == NULL || no->folha)
        return;

    if (no->ptr2 != NULL) {
        no->chave1 = encontrar_menor(no->ptr2);
    } 
    else 
    {
        no->chave1 = -1;
    }

    if (no->ptr3 != NULL) {
        no->chave2 = encontrar_menor(no->ptr3);
    } 
    else 
    {
        no->chave2 = -1;
    }
}

int encontrar_menor(NoBPlus* no) 
{
    while (!no->folha) 
    {
        no = no->ptr1;
    }
    return no->chave1;
}




void excluir_chave(NoBPlus** raiz, int chave) 
{
    NoBPlus* no = *raiz;

    while (no != NULL && !no->folha) {
        if (chave < no->chave1 || no->chave1 == -1) {
            no = no->ptr1;
        } 
        else if (no->chave2 == -1 || chave < no->chave2) {
            no = no->ptr2;
        } 
        else {
            no = no->ptr3;
        }
    }

    if (no == NULL) 
    {
        printf("Chave %d não encontrada na árvore.\n", chave);
        return;
    }

    if (no->chave1 == chave) 
    {
        no->chave1 = no->chave2; 
        no->chave2 = -1;         
    } 
    else if (no->chave2 == chave) 
    {
        no->chave2 = -1;         
    } 
    else 
    {
        printf("Chave %d não encontrada na folha.\n", chave);
        return;
    }

    if (no->chave1 == -1 && no->chave2 == -1) 
    {
        if (no->pai == NULL) 
        { 
            *raiz = NULL;
        } 
        else 
        {
            NoBPlus* pai = no->pai;

            if (pai->ptr1 == no) 
            {
                pai->ptr1 = NULL;
            } 
            else if (pai->ptr2 == no) 
            {
                pai->ptr2 = NULL;
            } 
            else if (pai->ptr3 == no) 
            {
                pai->ptr3 = NULL;
            }

            if (pai->chave1 == -1 && pai->ptr1 == NULL && pai->ptr2 == NULL && pai->ptr3 == NULL) 
            {
                concatenar_redistribuir(raiz, pai);
            }
        }
        free(no);
        printf("Nó folha vazio removido.\n");
    } 
    else 
    {
        printf("Chave %d removida da folha.\n", chave);
    }
}

void concatenar_redistribuir(NoBPlus** raiz, NoBPlus* no) {
    NoBPlus* pai = no->pai;
    if (pai == NULL) {
        *raiz = NULL;
        free(no);
        return;
    }

    int indice = indice_filho(pai, no);
    NoBPlus* irmao = NULL;

    if (indice > 0) 
    {
        if (indice == 1) 
        {
            irmao = pai->ptr1;
        } 
        else 
        {
            irmao = pai->ptr2;
        }
    }

    if (irmao == NULL || (irmao->chave2 == -1)) 
    {
        if (indice < 2) 
        {
            if (indice == 0 && pai->ptr2 != NULL) 
            {
                irmao = pai->ptr2;
            } 
            else if (indice == 1 && pai->ptr3 != NULL) {
                irmao = pai->ptr3;
            }
        }
    }

    if (irmao != NULL && irmao->chave2 != -1) 
    {
        if (indice < indice_filho(pai, irmao)) 
        {
            no->chave1 = irmao->chave1;
            irmao->chave1 = irmao->chave2;
            irmao->chave2 = -1;
        } 
        else 
        {
            no->chave1 = irmao->chave2;
            irmao->chave2 = -1;
        }
        atualizar_chaves(pai);
    } 
    else 
    {
        if (irmao != NULL) 
        {
            if (indice < indice_filho(pai, irmao)) 
            { //concatenando com o irmao
                irmao->chave2 = irmao->chave1;
                irmao->chave1 = no->chave1;
            } 
            else 
            {
                irmao->chave2 = no->chave1;
            }
            if (indice == 0) 
            {
                pai->ptr1 = irmao;
                pai->ptr2 = pai->ptr3;
                pai->ptr3 = NULL;
            } 
            else if (indice == 1) 
            {
                pai->ptr2 = pai->ptr3;
                pai->ptr3 = NULL;
            }
            atualizar_chaves(pai);
            free(no);
        }

        if (pai->chave1 == -1) 
        {
            concatenar_redistribuir(raiz, pai);
        }
    }
}


void imprimir_arvore(NoBPlus* no, int nivel) {
    if (no == NULL)
        return;
    for (int i = 0; i < nivel; i++)
        printf("    ");
    printf("Nó %p: chave1=%d, chave2=%d, ptr1=%p, ptr2=%p, ptr3=%p\n",
           (void*)no, no->chave1, no->chave2, (void*)no->ptr1, (void*)no->ptr2, (void*)no->ptr3);
    if (!no->folha) {
        imprimir_arvore(no->ptr1, nivel + 1);
        imprimir_arvore(no->ptr2, nivel + 1);
        if (no->ptr3 != NULL)
            imprimir_arvore(no->ptr3, nivel + 1);
    }
}

int main() {
    NoBPlus *raiz = criarNo(1); 



    inserir_chave(&raiz, 10);
    inserir_chave(&raiz, 20);
    inserir_chave(&raiz, 5);
    inserir_chave(&raiz, 15);
    inserir_chave(&raiz, 25);
    inserir_chave(&raiz, 30);

    
    printf("Árvore B+ após inserções:\n");
    imprimir_arvore(raiz, 0);
    printf("\n\n");

    int chaveBusca = 30;
    NoBPlus* resultado = buscar_chave(raiz, chaveBusca);
    if (resultado != NULL)
        printf("Chave %d encontrada no nó %p\n", chaveBusca, (void*)resultado);
    else
        printf("Chave %d não encontrada na árvore\n", chaveBusca);

    excluir_chave(&raiz, 20);
    printf("Árvore B+ após exclusão da chave 20:\n");
    imprimir_arvore(raiz, 0);

    return 0;
}