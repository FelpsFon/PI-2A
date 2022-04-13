#include <stdio.h>
#include <stdlib.h>

typedef struct No {
    int elemento;
    struct No *proximo;
} t_no;

typedef struct {
    t_no *inicio;
    t_no *fim;
} t_lista;

t_lista *inicializar_lista() {
    t_lista *lista = malloc(sizeof(t_lista));
    lista->inicio = NULL;
    lista->fim = NULL;

    return lista;
}

int i = 1;
void inserir_elemento(t_lista *lista) {
    // inserir elemento na lista
    // passo 1: Criar novo elemento.
    t_no *no = NULL;
    no = malloc(sizeof(t_no));
    //printf("Insira o novo número: ");
    //scanf("%d", &no->elemento);
    no->elemento = i++;
    no->proximo = NULL;
    // passo 2: Inserir no fim da lista.
    if(lista->inicio == NULL) {
        // Se a lista estiver vazia
        lista->inicio = no;
        lista->fim = no;
    } else {
        // inserção no fim
        lista->fim->proximo = no;
        lista->fim = no;

    }
}

void listar_elementos(t_lista *lista) {
    if(lista->inicio == NULL) {
        printf("Lista vazia!\n");
    } else {
        t_no *no = lista->inicio;
        while(no != NULL) {
            printf("%d\n", no->elemento);
            no = no->proximo;
        }
    }
}

int main () {

    // inicializar lista
    t_lista *lista = inicializar_lista();
    inserir_elemento(lista);
    inserir_elemento(lista);
    inserir_elemento(lista);
    listar_elementos(lista);

    return 0;
}