#include "gc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct elemento
{
    short int num;
    int quant;
    struct elemento *esq;
    struct elemento *dir;
} Elemento;

Elemento *cria()
{
    return NULL;
}

Elemento *insereElemento(Elemento *a, short int num)
{
    if (a == NULL)
    {
        a = (Elemento *) malloc(sizeof(Elemento));
        a->num = num;
        a->quant = 1;
        a->esq = NULL;
        a->dir = NULL;
    }

    else if (num < a->num)
        a->esq = insereElemento(a->esq, num);

    else
        a->dir = insereElemento(a->dir, num);

    return a;
}

Elemento *existe(Elemento *a, int num)
{
    if (a == NULL || a->num == num)
        return a;

    if (a->num > num)
        return existe(a->esq, num);

    else
        return existe(a->dir, num);
}

void emOrdem(FILE *saida, Elemento *a)
{
    if (a != NULL)
    {
        emOrdem(saida, a->esq);
        fprintf(saida, "%hu,%d\n", a->num, a->quant);
        emOrdem(saida, a->dir);
    }
}

int main(void)
{
    //GC_INIT();
    char arqEntrada[100], arqSaida[100], plot[100];

    printf("Digite o nome do arquivo de entrada:\n");
    scanf("%s", arqEntrada);

    int i;

    FILE *entrada;
    Elemento *lista;
    Elemento *aux;
    FILE *saida;

    short int numero;

    printf("\n\n%s\n\n", arqEntrada);

    for (i = 0; i < 2; i++)
    {
        if (i == 0)
        {
            entrada = fopen(arqEntrada, "rb");

            if (entrada == NULL)
            {
                perror("Nao foi possivel abrir o arquivo");
                return -1;
            }
        }
        else
        {
            strcat(arqEntrada, ".zip");
            entrada = fopen(arqEntrada, "rb");
            if (entrada == NULL)
            {
                perror("Nao foi possivel abrir o arquivo\n");
                return -1;
            }
        }

        lista = cria();
        aux = NULL;

        while (!feof(entrada))
        {
            fread(&numero, 1, sizeof(short int), entrada);

            aux = existe(lista, numero);

            if (aux!=NULL)
            {
                aux->quant++;
            }
            else
            {
                lista = insereElemento(lista, numero);
            }

        }

        fclose(entrada);

        strcpy(arqSaida, arqEntrada);

        saida = fopen(strcat(arqSaida, ".csv"), "w");

        fprintf(saida, "bits,frequencia\n");
        emOrdem(saida, lista);

        fclose(saida);

        printf("Arquivo gerado: %s\n", arqSaida);



        strcpy(plot, "./plot ");
        strcat(plot, arqSaida);

        system(plot);
    }

    return 0;
}
