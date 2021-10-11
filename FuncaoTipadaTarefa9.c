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

enum
{
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

enum
{
    COR_FUNDO = BLACK,
    COR_SAPO = GREEN,
    COR_CARRO = YELLOW,
    COR_CAMINHAO = WHITE,
    COR_BORDA = BLUE,
    COR_MENU = WHITE
};

enum
{
    LIM_ESQ = 1,
    LIM_DIR = 80,
    LIM_CIMA = 2,
    LIM_BAIXO = 26,
};

enum
{
    LARGSAPO = 8,
    ALTURASAPO = 2,

    DISTANCIAX = LARGSAPO,
    DISTANCIAY = ALTURASAPO*2,

    POS_INIX = LIM_DIR/2 - LARGSAPO/2 + 1,
    POS_INIX2 = POS_INIX + (LARGSAPO - 1),
    POS_INIY = LIM_BAIXO + 1,
    POS_INIY2 = POS_INIY + (ALTURASAPO - 1)
};

enum
{
    LARGCAMINHAO = 8,
    LARGCARRO = 4,
    ALTURAVEICULO = 3,

    DIST_MIN = LARGSAPO + (LARGSAPO / 2),
    DIST_VARIACAO = LARGSAPO / 2,

    DIST_MOVIMENTO = 1
};


enum
{
    CHEGADA = LIM_CIMA + ALTURASAPO + 1,
    MEIO_BAIXO = LIM_BAIXO - ((2 * DISTANCIAY) + 1),
    MEIO_CIMA = MEIO_BAIXO - (ALTURASAPO + 1),

    PISTA1 = CHEGADA + 1,
    PISTA2 = MEIO_CIMA - ALTURAVEICULO - 1,
    PISTA3 = MEIO_BAIXO + 1,
    PISTA4 = LIM_BAIXO - ALTURAVEICULO - 1,

    QTD_PISTA = 4,
    LIM_PISTA = 5,

    QTD_VEICULOS = QTD_PISTA * LIM_PISTA
};

void borda();
void desenhaSapo(int x, int y);
void apagaSapo(int x, int y);
void desenhaAmbiente(int x, int y);
void jogo(int *xs1, int *xs2, int *ys1, int *ys2, int x1[QTD_VEICULOS], int y1[QTD_VEICULOS], int x2[QTD_VEICULOS], int y2[QTD_VEICULOS], int tipoVeiculo[QTD_VEICULOS], int distVeiculo[QTD_VEICULOS], int veiculoValido[QTD_VEICULOS]);
void testaMov(int *xs1, int *xs2, int *ys1, int *ys2, char tecla);
void moveSapo(int *xs1, int *xs2, int *ys1, int *ys2, char tecla);
short desenhaVeiculo(int x, int y, int tipo, int direcao);
void apagaVeiculo(int x, int y);
void criaVeiculos(int tipos[], int posicao[]);
void desenha_lista_veiculos(int pontox1[QTD_VEICULOS], int pontoy1[QTD_VEICULOS], int pontox2[QTD_VEICULOS], int pontoy2[QTD_VEICULOS], int tipoVeiculo[QTD_VEICULOS], int distVeiculo[QTD_VEICULOS], int direcao, int veiculo_valido[QTD_VEICULOS]);
short testa_colisao(int xs1 ,int ys1 ,int xs2 ,int ys2, int x1, int y1, int x2, int y2);

int main()
{
    int xs1 = POS_INIX,
        xs2 = POS_INIX2,
        ys1 = POS_INIY,
        ys2 = POS_INIY2;
    int x1[QTD_VEICULOS],
        y1[QTD_VEICULOS],
        x2[QTD_VEICULOS],
        y2[QTD_VEICULOS],
        tipoVeiculo[QTD_VEICULOS],
        distVeiculo[QTD_VEICULOS];
    short int veiculoValido[QTD_VEICULOS];

    srand(time(NULL));
    desenhaAmbiente(xs1, ys1);

    jogo(&xs1,&xs2,&ys1,&ys2, x1, y1, x2, y2, tipoVeiculo, distVeiculo, veiculoValido);
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
        // Linhas de separacao
        putchxy(i, CHEGADA, LINHA);
        putchxy(i, MEIO_BAIXO, LINHA);
        putchxy(i, MEIO_CIMA, LINHA);
    }
}

void jogo(int *xs1, int *xs2, int *ys1, int *ys2, int x1[QTD_VEICULOS], int y1[QTD_VEICULOS], int x2[QTD_VEICULOS],
          int y2[QTD_VEICULOS], int tipoVeiculo[QTD_VEICULOS], int distVeiculo[QTD_VEICULOS], int veiculoValido[QTD_VEICULOS])
{
    int para = 0;
    int i;
    char tecla;
    short int colidiu = 0;

    while (!para)
    {
        desenha_lista_veiculos();
        i = 0;
        while(i < QTD_VEICULOS && !colidiu)
        {
            if(veiculoValido[i])
            {
                colidiu = testa_colisao(*xs1,*ys1,*xs2,*ys2,x1[i],y1[i],x2[i],y2[i]);
            }
            i++;
        }
        if(_kbhit())
        {
            tecla = getch();
            if(tecla == ESPECIAL)
            {
                tecla = getch();
                moveSapo(xs1,xs2,ys1,ys2, tecla);
            }
            else if (tecla == ESC)
            {
                para = 1;
            }
        }
        if (colidiu)
            para = 1;
    }
}

short testa_colisao(int xs1 ,int ys1 ,int xs2 ,int ys2, int x1, int y1, int x2, int y2) {

    if ( ( (xs1 <= x2 && xs1 >= x1 ) || ( xs2 >= x1 && xs2 <= x2 ) ) && (ys2 == y2) )
    {
        return 1;
    }
    return 0;
}

void desenha_lista_veiculos(int pontox1[], int pontoy1[], int pontox2[], int pontoy2[],
                            int tipoVeiculo[], int distVeiculo[], int direcao, int veiculo_valido[])
{
    int i;

    for(i=0; i<QTD_VEICULOS; i++) {
        veiculo_valido[i] = desenhaVeiculo(pontox1[i], pontoy1[i], pontox2[i], pontoy2[i], tipoVeiculo[i], COR_FUNDO);

        if(direcao)
        {
            x1[i]+= DIST_MOVIMENTO;
            y1[i]+= DIST_MOVIMENTO;
            x2[i]+= DIST_MOVIMENTO;
            y2[i]+= DIST_MOVIMENTO;
        }
        else
        {
            x1[i]-= DIST_MOVIMENTO;
            y1[i]-= DIST_MOVIMENTO;
            x2[i]-= DIST_MOVIMENTO;
            y2[i]-= DIST_MOVIMENTO;
        }

        if(tipoVeiculo[i])
        {
            veivulo_valido[i] = desenhaVeiculo(pontox1[i], pontoy1[i], pontox2[i], pontoy2[i], tipoVeiculo[i], COR_CARRO);
        }
        else
        {
            veivulo_valido[i] = desenhaVeiculo(pontox1[i], pontoy1[i], pontox2[i], pontoy2[i], tipoVeiculo[i], COR_CAMINHAO);
        }
    }
    int veiculo_valido[])

    Sleep(100);
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

short desenhaVeiculo(int x, int y, int tipo, int direcao, COLLORS cor)
{
    int i, largura;
    short int dentro = 0;
    char veiculo[3][9];
    textcolor(cor);
    if(tipo)
    {
        if(direcao)
        {
            strcpy(veiculo[0], "o o");
            strcpy(veiculo[1], "HHH>");
            strcpy(veiculo[2], "o o");
            largura = LARGCARRO;
        }
        else
        {
            strcpy(veiculo[0], " o o");
            strcpy(veiculo[1], "<HHH");
            strcpy(veiculo[2], " o o");
            largura = LARGCARRO;
        }
    }
    else
    {
        if(direcao)
        {
            strcpy(veiculo[0], " OO   o");
            strcpy(veiculo[1], "-[[[[[(8");
            strcpy(veiculo[2], " OO   o");
            largura = LARGCAMINHAO;
        }
        else
        {
            strcpy(veiculo[0], " o  OO");
            strcpy(veiculo[1], "8)]]]]]-");
            strcpy(veiculo[2], " o  OO");
            largura = LARGCAMINHAO;
        }
    }
    for(i = 0; i < largura; i++)
    {
        if(i + x < LIM_DIR && x + i > LIM_ESQ)
        {
            dentro = 1;
            putchxy(i + x, y, veiculo[0][i]);
            putchxy(i + x, y + 1, veiculo[1][i]);
            putchxy(i + x, y + 2, veiculo[2][i]);
        }
    }
    return dentro;
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
        dist = DIST_MIN + rand() % DIST_VARIACAO;
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

void desenhaAmbiente(int x, int y)
{
    gotoxy(1,1);
    textcolor(COR_MENU);
    printf(" INF1202 Eduardo & Jose     (C)arregar jogo   (P)ausa   (ESC)Sair   (R)Ranking");
    borda();
    desenhaSapo(x, y);
}
