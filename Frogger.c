/*
Esse programa organiza a compra e venda de acoes de 4 empresas
*/
#include<stdio.h>
#include<conio2.h>
#include<stdlib.h>


# define XINI 1
# define XFIM 82
# define YINI 2
# define YFIM 26
# define LARGSAPO 8
# define ALTURASAPO 2
# define LINHA '='
# define COLUNA '*'

# define LARGURACOMANDOS -21

void borda();
void movimento();
void desenhaSapo(int[]);
void apagaSapo(int[]);
void desenhaAmbiente();

enum{
    ENTER = 13,
    ESPACO = 32,
    ESC = 27,
    PAUSA = 112,
    SPECIAL = 224,

    LEFT = 75,
    RIGHT = 77,
    UP = 72,
    DOWN = 80
};


int main()
{
    desenhaAmbiente();
    textcolor(WHITE);
    movimento();
    return(0);
}

void borda()
{
    int i;
    textcolor(BLUE);
    for(i = YINI; i <= YFIM; i++)
    {
        putchxy(XINI, i, COLUNA);
        putchxy(XFIM, i, COLUNA);
    }
    for(i = XINI; i <= XFIM; i++)
    {
        putchxy(i, YINI, LINHA);
        putchxy(i, YFIM, LINHA);
    }
}

void movimento()
{
    int posicao[2] = {(XFIM / 2 - LARGSAPO / 2) + 1, YFIM + 1};
    int stop = 0;
    int c;
    desenhaSapo(posicao);
    while (!stop)
    {
        if(_kbhit())
        {
            c = _getch();
            if(c == SPECIAL)
            {
                c = _getch();
                gotoxy(XINI, YFIM + 1);
                apagaSapo(posicao);
                switch(c)
                {
                case LEFT:
                    posicao[0] -= LARGSAPO + 1;
                    break;
                case RIGHT:
                    posicao[0] += LARGSAPO + 1;
                    break;
                case UP:
                    posicao[1] -= ALTURASAPO * 2;
                    break;
                case DOWN:
                    posicao[1] += ALTURASAPO * 2;
                    break;
                default:
                    ;
                }
                desenhaSapo(posicao);
            }
        }

    }
}

void desenhaSapo(int coord[])
{
    gotoxy(coord[0], coord[1]);
    printf("  (OO)");
    gotoxy(coord[0], coord[1] +1);
    printf("\\^{  ]^/");
    gotoxy(500,500);
}

void apagaSapo(int coord[])
{
    gotoxy(coord[0], coord[1]);
    printf("        ");
    gotoxy(coord[0], coord[1] +1);
    printf("        ");
}

void desenhaAmbiente()
{
    int i;
    printf("   INF1202 Eduardo & Jose     (C)arregar jogo   (P)ausa   (ESC)Sair   (R)Ranking");
    borda();
    for(i = XINI; i <= XFIM; i++)
    {
        putchxy(i, YINI + ALTURASAPO + 1, LINHA);
        putchxy(i, YFIM - ((2 * (ALTURASAPO * 2)) + 1), LINHA);
        putchxy(i, YFIM - (((2 * (ALTURASAPO * 2)) + 1) + ALTURASAPO + 1), LINHA);
    }


}
