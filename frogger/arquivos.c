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
    int erro = 1;
    if(!arq)
    {
        erro--;
    }
    else
    {
        fread(estado, sizeof(ESTADO), 1, arq);
        if(ferror(arq))
        {
            erro--;
        }
        fclose(arq);
    }
    return erro;
}

int AbreArqJog(FILE **arqtxt)
{
    *arqtxt = fopen("FROGGERJog.txt", "r+");
    if(!arqtxt)
    {
        return 0;
    }
    return 1;
}

int LeArqTexto(FILE *arqtxt, JOGADOR jogadores[], int maxArray)
{
    int i = 0;
    while(!feof(arqtxt) && i < maxArray)
    {
        fscanf(arqtxt, "%[^#]", jogadores[i].nome);
        jogadores[i].nome[strlen(jogadores[i].nome) - 1] = '\0';
        fscanf(arqtxt, "#%d ", &jogadores[i].saposEspera);
        fscanf(arqtxt, "#%d ", &jogadores[i].saposSalvos);
        fscanf(arqtxt, "#%d ", &jogadores[i].tempoJogo);
        fscanf(arqtxt, "#%ld \n", &jogadores[i].score);
        i++;
    }
    return i;
}

int SalvaListaJogadores(FILE *arqtxt, JOGADOR jogadores[], int tamArray)
{
    int i;
    if(!arqtxt)
    {
        return 0;
    }
    rewind(arqtxt);
    gotoxy(1,36);
    for(i = 0; i < tamArray; i++)
    {
        fprintf(arqtxt,"%s #%d #%d #%d #%ld %c", jogadores[i].nome, jogadores[i].saposEspera, jogadores[i].saposSalvos, jogadores[i].tempoJogo, jogadores[i].score, '\0');
        if(i != tamArray - 1)
        {
            fputc('\n', arqtxt);
        }
    }
    return 1;
}
