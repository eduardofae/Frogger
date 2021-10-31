#include<stdio.h>
#include<string.h>
#include<time.h>
#include<conio2.h>
#include"arquivos.h"

int salvaEstadoDoJogo(ESTADO estado)
{
    FILE *arq = fopen(strcat(estado.jogador.nome, ".bin"), "wb");
    int erro = 0;
    if(!arq)
    {
        erro++;
    }
    else
    {
        fwrite(&estado, sizeof(ESTADO), 1, arq);
        if(ferror(arq))
        {
            erro++;
        }
        fclose(arq);
    }
    return erro;
}

int leJogoSalvo(ESTADO *estado, char nome[])
{
    FILE *arq = fopen(nome, "rb");
    int erro = 0;
    if(!arq)
    {
        erro++;
    }
    else
    {
        fread(estado, sizeof(ESTADO), 1, arq);
        if(ferror(arq))
        {
            erro++;
        }
        fclose(arq);
    }
    return erro;
}

int AbreArqJog(FILE *arqtxt, char nome[])
{
    arqtxt = fopen(nome, "r+");
    if(!arqtxt)
    {
        return 0;
    }
    return 1;
}

int LeArqTexto(FILE *arqtxt, ESTADO estado, JOGADOR jogadores[])
{
    int i = 0;
    while(!feof(arqtxt))
    {
        fscanf(arqtxt, "%s", jogadores[i].nome);
        fscanf(arqtxt, "%d", &jogadores[i].saposEspera);
        fscanf(arqtxt, "%d", &jogadores[i].saposSalvos);
        fscanf(arqtxt, "%d", &jogadores[i].tempoJogo);
        fscanf(arqtxt, "%ld", &jogadores[i].score);
        i++;
    }
    return i;
}

void OrdenaListaJogadores(JOGADOR jogadores[], int tamArray)
{
    int i, j;
    for(i = 0; i < tamArray - 1; i++)
    {
        for(j = i + 1; j < tamArray; j++)
        {
            if(jogadores[i].score > jogadores[j].score)
            {
                troca(&jogadores[i], &jogadores[j]);
            }
        }
    }
}

void InsereJogador(JOGADOR jogadores[], JOGADOR jog, int *tamArray, int maxArray)
{
    int i = 0, encontrou = 0;
    while(!encontrou && i < *tamArray)
    {
        if(strcmp(jog.nome, jogadores[i].nome) == 0)
        {
            if(jog.score > jogadores[i].score)
            {
                jogadores[i] = jog;
            }
            encontrou = 1;
        }
        i++;
    }
    if(!encontrou)
    {
        if(i < maxArray)
        {
            jogadores[i] = jog;
            (*tamArray)++;
        }
        else if(jogadores[*tamArray].score > jog.score)
        {
            jogadores[*tamArray] = jog;
        }
    }
    OrdenaListaJogadores(jogadores, *tamArray);
}

void troca(JOGADOR *a, JOGADOR *b)
{
    JOGADOR aux = *a;
    a = b;
    b = &aux;
}
