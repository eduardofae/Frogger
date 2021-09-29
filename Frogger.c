/*
Frogger UFRGS (EDUARDO e JOSE)
*/
#include<stdio.h>
#include<conio2.h>
#include<stdlib.h>

# define LINHA '-'
# define COLUNA '|'
# define LARGURACOMANDOS -21



enum{
    ENTER = 13,
    ESPACO = 32,
    ESC = 27,
    PAUSA = 112,
    ESPECIAL = -32,

    ESQ = 75,
    DIR = 77,
    CIMA = 72,
    BAIXO = 80,
};

enum{
    COR_FUNDO = BLACK,
    COR_SAPO = GREEN
};

enum{
    LIM_ESQ = 1,
    LIM_DIR = 80,
    LIM_CIMA = 2,
    LIM_BAIXO = 26,
};

enum{
    LARGSAPO = 8,
    ALTURASAPO = 2,

    DISTANCIAX = LARGSAPO,
    DISTANCIAY = ALTURASAPO*2,

    POS_INIX = LIM_DIR/2 - LARGSAPO/2 + 1,
    POS_INIX2 = POS_INIX + LARGSAPO,
    POS_INIY = LIM_BAIXO + 1,
    POS_INIY2 = POS_INIY + ALTURASAPO
};

enum{
    CHEGADA = LIM_CIMA + ALTURASAPO + 1,
    MEIO_BAIXO = LIM_BAIXO - ((2 * DISTANCIAY) + 1),
    MEIO_CIMA = MEIO_BAIXO - (ALTURASAPO + 1)
};

void borda();
void desenhaSapo(int x, int y);
void apagaSapo(int x, int y);
void desenhaAmbiente(int x, int y);
void jogo(int *xs1, int *xs2, int *ys1, int *ys2);
void testaMov(int *xs1, int *xs2, int *ys1, int *ys2, char tecla);
void moveSapo(int *xs1, int *xs2, int *ys1, int *ys2, char tecla);

int main()
{
    int x1 = POS_INIX,
        x2 = POS_INIX2,
        y1 = POS_INIY,
        y2 = POS_INIY2;

    desenhaAmbiente(x1, y1);
    jogo(&x1,&x2,&y1,&y2);
    return(0);
}

void borda()
{
    int i;
    textcolor(BLUE);
    for(i = LIM_CIMA; i <= LIM_BAIXO; i++)
    {
        putchxy(LIM_ESQ, i, COLUNA);
        putchxy(LIM_DIR, i, COLUNA);
    }
    for(i = LIM_ESQ; i <= LIM_DIR; i++)
    {
        putchxy(i, LIM_CIMA, LINHA);
        putchxy(i, LIM_BAIXO, LINHA);
    }
}

void jogo(int *xs1, int *xs2, int *ys1, int *ys2)
{
    int para = 0;
    char tecla;
    while (!para)
    {
        if(_kbhit())
        {
            tecla = getch();
            if(tecla == ESPECIAL)
            {
                tecla = getch();
                moveSapo(xs1,xs2,ys1,ys2, tecla);
            } else if (tecla == ESC){
                para = 1;
            }
        }
    }
}

void moveSapo(int *xs1, int *xs2, int *ys1, int *ys2, char tecla)
{
    apagaSapo(*xs1,*ys1);// apaga sapo
    testaMov(xs1, xs2, ys1, ys2, tecla );
    desenhaSapo(*xs1, *ys1); // desenha novo sapo
}

void testaMov(int *xs1, int *xs2, int *ys1, int *ys2, char tecla)
{
    switch(tecla)
    {
    case ESQ:
        if (*xs1-DISTANCIAX > LIM_ESQ)
        {
            *xs1= *xs1-DISTANCIAX;
            *xs2= *xs2-DISTANCIAX;
        }
        break;
    case DIR:
        if (*xs2+DISTANCIAX < LIM_DIR)
        {
            *xs1= *xs1+DISTANCIAX;
            *xs2= *xs2+DISTANCIAX;
        }
        break;
    case CIMA:
        if (*ys1-DISTANCIAY > LIM_CIMA)
        {
            *ys1= *ys1-DISTANCIAY;
            *ys2= *ys2-DISTANCIAY;
        }
        break;
    case BAIXO:
        if (*ys2+DISTANCIAY < LIM_BAIXO)
        {
            *ys1= *ys1+DISTANCIAY;
            *ys2= *ys2+DISTANCIAY;
        }
        break;
    }
}

void desenhaSapo(int x, int y)
{
    textcolor(COR_SAPO);
    gotoxy(x, y);
    printf("  (OO)");
    gotoxy(x, y +1);
    printf("\\^{  ]^/");
}

void apagaSapo(int x, int y)
{
    textcolor(COR_FUNDO);
    gotoxy(x, y);
    printf("  (OO)");
    gotoxy(x, y +1);
    printf("\\^{  ]^/");
}

void desenhaAmbiente(int x, int y)
{
    int i;
    printf("   INF1202 Eduardo & Jose     (C)arregar jogo   (P)ausa   (ESC)Sair   (R)Ranking");
    borda();
    for(i = LIM_ESQ; i <= LIM_DIR; i++)
    {
        putchxy(i, CHEGADA, LINHA);
        putchxy(i, MEIO_BAIXO, LINHA);
        putchxy(i, MEIO_CIMA, LINHA);
    }
    desenhaSapo(x, y);
}
