#include<stdio.h>
#include<string.h>
#include<time.h>
#include<conio2.h>
#include"arquivos.h"

/////////////////////////////////////////////////
//                 QUESTAO 3                   //
/////////////////////////////////////////////////
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

/////////////////////////////////////////////////
//                 QUESTAO 4                   //
/////////////////////////////////////////////////
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
