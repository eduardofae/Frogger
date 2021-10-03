/*
Frogger UFRGS (EDUARDO e JOSE)
*/
#include<stdio.h>
#include<conio2.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<windows.h>

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
    COR_SAPO = GREEN,
    COR_CARRO = YELLOW,
    COR_CAMINHAO = WHITE,
    COR_BORDA = BLUE,
    COR_MENU = WHITE
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
    POS_INIX2 = POS_INIX + (LARGSAPO - 1),
    POS_INIY = LIM_BAIXO + 1,
    POS_INIY2 = POS_INIY + (ALTURASAPO - 1)
};

enum{
    LARGCAMINHAO = 8,
    LARGCARRO = 4,
    ALTURAVEICULO = 3
};


enum{
    CHEGADA = LIM_CIMA + ALTURASAPO + 1,
    MEIO_BAIXO = LIM_BAIXO - ((2 * DISTANCIAY) + 1),
    MEIO_CIMA = MEIO_BAIXO - (ALTURASAPO + 1),

    PISTA1 = CHEGADA + 1,
    PISTA2 = MEIO_CIMA - ALTURAVEICULO - 1,
    PISTA3 = MEIO_BAIXO + 1,
    PISTA4 = LIM_BAIXO - ALTURAVEICULO - 1,

    QTD_PISTA = 4,
    LIM_PISTA = 5
};


void borda();
void desenhaSapo(int x, int y);
void apagaSapo(int x, int y);
void desenhaAmbiente(int x, int y);
void jogo(int *xs1, int *xs2, int *ys1, int *ys2);
void testaMov(int *xs1, int *xs2, int *ys1, int *ys2, char tecla);
void moveSapo(int *xs1, int *xs2, int *ys1, int *ys2, char tecla);
void desenhaVeiculo(int x, int y, int tipo);
void apagaVeiculo(int x, int y);
void criaVeiculos(int tipos[], int posicao[]);
void criaPistas(int tipos_veiculos[QTD_PISTA][LIM_PISTA], int pos_veiculos[QTD_PISTA][LIM_PISTA]);

int main()
{
    int x1 = POS_INIX,
        x2 = POS_INIX2,
        y1 = POS_INIY,
        y2 = POS_INIY2;

    srand(time(NULL));
    desenhaAmbiente(x1, y1);

    jogo(&x1,&x2,&y1,&y2);
    return(0);
}

void borda()
{
    int i;
    textcolor(COR_BORDA);
    // Linhas Verticais
    for(i = LIM_CIMA; i <= LIM_BAIXO; i++)
    {
        putchxy(LIM_ESQ, i, COLUNA);
        putchxy(LIM_DIR, i, COLUNA);
    }
    // Linhas Horizontais
    for(i = LIM_ESQ; i <= LIM_DIR; i++)
    {
        putchxy(i, LIM_CIMA, LINHA);
        putchxy(i, LIM_BAIXO, LINHA);
        // Linhas de separação
        putchxy(i, CHEGADA, LINHA);
        putchxy(i, MEIO_BAIXO, LINHA);
        putchxy(i, MEIO_CIMA, LINHA);
    }
}

void jogo(int *xs1, int *xs2, int *ys1, int *ys2)
{
    int para = 0;
    char tecla;
    int tipo_pista[QTD_PISTA][LIM_PISTA];
    int pos_pistas[QTD_PISTA][LIM_PISTA];
    criaPistas(tipo_pista, pos_pistas);
    while (!para)
    {
        pos_pistas[0][0]++;
        pos_pistas[0][1]++;
        pos_pistas[0][2]++;
        desenhaVeiculo(pos_pistas[0][0], PISTA1 ,tipo_pista[0][0]);
        desenhaVeiculo(pos_pistas[0][1], PISTA1 ,tipo_pista[0][1]);
        desenhaVeiculo(pos_pistas[0][2], PISTA1 ,tipo_pista[0][2]);
        Sleep(100);
        apagaVeiculo(pos_pistas[0][0], PISTA1);
        apagaVeiculo(pos_pistas[0][1], PISTA1);
        apagaVeiculo(pos_pistas[0][2], PISTA1);
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

void desenhaVeiculo(int x, int y, int tipo)
{
    int i, largura;
    char veiculo[3][9];
    if(tipo)
    {
        textcolor(COR_CARRO);
        strcpy(veiculo[0], "o o");
        strcpy(veiculo[1], "HHH>");
        strcpy(veiculo[2], "o o");
        largura = LARGCARRO;
    }
    else
    {
        textcolor(COR_CAMINHAO);
        strcpy(veiculo[0], " OO   o");
        strcpy(veiculo[1], "-[[[[[(8");
        strcpy(veiculo[2], " OO   o");
        largura = LARGCAMINHAO;
    }
    for(i = 0; i < largura; i++)
    {
        if(i + x < LIM_DIR && x + i > LIM_ESQ)
        {
            putchxy(i + x, y, veiculo[0][i]);
            putchxy(i + x, y + 1, veiculo[1][i]);
            putchxy(i + x, y + 2, veiculo[2][i]);
        }
    }
}

void apagaVeiculo(int x, int y)
{
    int i, largura;
    char veiculo[3][9];

    textcolor(COR_FUNDO);
    strcpy(veiculo[0], " OO   o");
    strcpy(veiculo[1], "-[[[[[(8");
    strcpy(veiculo[2], " OO   o");
    largura = LARGCAMINHAO;

    for(i = 0; i < largura; i++)
    {
        if(i + x < LIM_DIR && x + i > LIM_ESQ)
        {
            putchxy(i + x, y, veiculo[0][i]);
            putchxy(i + x, y + 1, veiculo[1][i]);
            putchxy(i + x, y + 2, veiculo[2][i]);
        }
    }
}

void criaVeiculos(int tipos[], int posicao[])
{
    int i, dist;
    for(i = 0; i < LIM_PISTA; i++)
    {
        tipos[i] = rand() % 10 < 5;
    }
    posicao[0] = - LARGCAMINHAO;
    for(i = 1; i < LIM_PISTA; i++)
    {
        dist = LARGSAPO + rand() % (LARGSAPO / 2);
        if(tipos[i])
        {
            posicao[i] = posicao[i - 1] - LARGCARRO - dist;
        }
        else
        {
            posicao[i] = posicao[i - 1] - LARGCAMINHAO - dist;
        }
    }
}

void criaPistas(int tipos_veiculos[QTD_PISTA][LIM_PISTA], int pos_veiculos[QTD_PISTA][LIM_PISTA])
{
    int i;
    for(i = 0; i < QTD_PISTA; i++)
    {
        criaVeiculos(tipos_veiculos[i], pos_veiculos[i]);
    }
}

void desenhaAmbiente(int x, int y)
{
    int i;
    gotoxy(1,1);
    textcolor(COR_MENU);
    printf(" INF1202 Eduardo & Jose     (C)arregar jogo   (P)ausa   (ESC)Sair   (R)Ranking");
    borda();
    desenhaSapo(x, y);
}
