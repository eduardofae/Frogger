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

    CONTINUA = 0,
    SAPO_MORREU = 1,
    SAIR = 2,
    SAPO_SALVO = 3
};

enum
{
    COR_FUNDO = BLACK,
    COR_SAPO = GREEN,
    COR_CARRO = YELLOW,
    COR_CAMINHAO = WHITE,
    COR_BORDA = BLUE,
    COR_MENU = WHITE,
    COR_TEXTO = WHITE
};

enum
{
    LIM_ESQ = 1,
    LIM_DIR = 80,
    LIM_CIMA = 2,
    LIM_BAIXO = 26,

    TAM_BANNER = 20
};

enum
{
    LARGSAPO = 8,
    ALTURASAPO = 2,

    DISTANCIAX = LARGSAPO/2,
    DISTANCIAY = ALTURASAPO*2,

    POS_INIX = LIM_DIR/2 - LARGSAPO/2 + 1,
    POS_INIX2 = POS_INIX + (LARGSAPO - 1),
    POS_INIY = LIM_BAIXO + 1,
    POS_INIY2 = POS_INIY + (ALTURASAPO - 1),

    NUM_SAPOS = 6
};

enum
{
    LARGCAMINHAO = 8,
    LARGCARRO = 4,
    ALTURAVEICULO = 3,

    DIST_MIN = LARGSAPO*2 + (LARGSAPO / 2),
    DIST_VARIACAO = LARGSAPO / 2,

    DIST_MOV_VEIC = 1
};


enum
{
    CHEGADA = LIM_CIMA + ALTURASAPO + 1,
    MEIO_BAIXO = LIM_BAIXO - ((2 * DISTANCIAY) + 1),
    MEIO_CIMA = MEIO_BAIXO - (ALTURASAPO + 1),

    Y_INI_PISTA = CHEGADA + 1,
    LARG_PISTA = DISTANCIAY,
    X2_INI_PISTA = LIM_ESQ - 1,

    QTD_PISTA = 4,
    VEIC_PISTA = 15,

    QTD_VEICULOS = QTD_PISTA * VEIC_PISTA

};
enum
{
    DIR_ESQ = 0,
    DIR_DIR = 1,

    TIPO_CARRO = 1,
    TIPO_CAMINHAO = 0
};

typedef enum
{
    ESPERA = 1,
    ATIVO = 2,
    SALVO = 3,
    MORTO = 4
} STATUS_SAPO;

void borda();
void desenhaSapo(int x, int y);
void apagaSapo(int x, int y);
void desenhaAmbiente(int x, int y, int indice_sapo, int sapos_salvos, int sapos_mortos);

void jogo(STATUS_SAPO sapos[], int *xs1, int *xs2, int *ys1, int *ys2,
          int x1[QTD_VEICULOS], int y1[QTD_VEICULOS], int x2[QTD_VEICULOS], int y2[QTD_VEICULOS],
          int tipoVeiculo[QTD_VEICULOS], int distVeiculo[QTD_VEICULOS],
          short int veiculoValido[QTD_VEICULOS], int direcao[QTD_VEICULOS],
          int *indice_sapos, int *sapos_salvos, int *sapos_mortos);

void testaMov(int *xs1, int *xs2, int *ys1, int *ys2, char tecla);
void moveSapo(int *xs1, int *xs2, int *ys1, int *ys2, char tecla);
short desenhaVeiculo(int x, int y, int tipo, int direcao, COLORS cor);

void desenha_lista_veiculos(int pontox1[], int pontoy1[], int pontox2[], int pontoy2[],
                            int tipoVeiculo[], int distVeiculo[], int direcao[],short int veiculo_valido[]);

short testa_colisao(int xs1 ,int ys1 ,int xs2 ,int ys2, int x1, int y1, int x2, int y2);

void inicializar(STATUS_SAPO sapos[], int x1[QTD_VEICULOS], int y1[QTD_VEICULOS], int x2[QTD_VEICULOS], int y2[QTD_VEICULOS], int tipoVeiculo[QTD_VEICULOS], int distVeiculo[QTD_VEICULOS], int direcao[QTD_VEICULOS], int ypistas[QTD_PISTA]);

int mata_sapo(STATUS_SAPO sapos[],int *sapos_mortos, int *indice_sapo, int *xs1, int *xs2, int *ys1, int *ys2, int x1, int x2, int y1, int y2);
void explodeSapo(int x, int y);
void banner(char msg[TAM_BANNER]);
void placar(int indice_sapo, int sapos_salvos, int sapos_mortos);
int salva_sapo(int *sapos_salvos, STATUS_SAPO sapos[], int *indice_sapo, int *xs1, int *xs2, int *ys1, int *ys2);

int main()
{
    int xs1 = POS_INIX,
        xs2 = POS_INIX2,
        ys1 = POS_INIY,
        ys2 = POS_INIY2,
        indice_sapo=0,
        sapos_mortos=0,
        sapos_salvos=0;
    int x1[QTD_VEICULOS],
        y1[QTD_VEICULOS],
        x2[QTD_VEICULOS],
        y2[QTD_VEICULOS],
        tipoVeiculo[QTD_VEICULOS],
        distVeiculo[QTD_VEICULOS],
        direcao[QTD_VEICULOS],
        ypistas[QTD_PISTA];
    short int veiculoValido[QTD_VEICULOS] = {1};
    STATUS_SAPO sapos[NUM_SAPOS] = { ESPERA };

    srand(time(NULL));
    desenhaAmbiente(xs1, ys1, indice_sapo, sapos_salvos, sapos_mortos);
    inicializar(sapos, x1, y1, x2, y2, tipoVeiculo, distVeiculo, direcao, ypistas);
    jogo(sapos, &xs1,&xs2,&ys1,&ys2, x1, y1, x2, y2, tipoVeiculo, distVeiculo, veiculoValido, direcao, &indice_sapo, &sapos_salvos, &sapos_mortos);

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

void jogo(STATUS_SAPO sapos[], int *xs1, int *xs2, int *ys1, int *ys2,
          int x1[QTD_VEICULOS], int y1[QTD_VEICULOS], int x2[QTD_VEICULOS],
          int y2[QTD_VEICULOS], int tipoVeiculo[QTD_VEICULOS], int distVeiculo[QTD_VEICULOS],
          short int veiculoValido[QTD_VEICULOS], int direcao[QTD_VEICULOS],
          int *indice_sapos, int *sapos_salvos, int *sapos_mortos)
{
    int status = 0;
    int i;
    char tecla;

    while (status!=SAIR)
    {
        status=CONTINUA;
        desenha_lista_veiculos(x1, y1, x2, y2, tipoVeiculo, distVeiculo, direcao, veiculoValido);
        i = 0;
        while(i < QTD_VEICULOS && status==CONTINUA)
        {
            if(veiculoValido[i])
            {
                status = mata_sapo(sapos, sapos_mortos, indice_sapos, xs1, xs2,ys1, ys2, x1[i], x2[i], y1[i], y2[i]);
            }
          i++;
        }

        if(status == CONTINUA) {
            status = salva_sapo(sapos_salvos, sapos, indice_sapos, xs1, xs2,ys1, ys2);
        }

        if (status != CONTINUA) {
            placar(*indice_sapos,*sapos_salvos,*sapos_mortos);
        }

        if (status == SAIR) {
            banner("GAME OVER!!!");
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
                status = SAIR;
            }
        }
    }
}
void banner(char msg[TAM_BANNER]) {

    int meio = strlen(msg)/2;;

    textcolor(COR_TEXTO);

    gotoxy( LIM_DIR/2 - meio, MEIO_CIMA + 1 );
    printf("%s",msg);

}
void placar(int indice_sapo, int sapos_salvos, int sapos_mortos) {

    textcolor(COR_TEXTO);
    if (( NUM_SAPOS-1) - indice_sapo >= 0 ) {
        gotoxy(LIM_ESQ, LIM_BAIXO+1);
        printf("SAPOS EM ESPERA: %d",(NUM_SAPOS-1) - indice_sapo );
    }
    gotoxy(LIM_ESQ, LIM_BAIXO+2);
    printf("SAPOS MORTOS: %d", sapos_mortos);
    gotoxy(LIM_ESQ, LIM_BAIXO+3);
    printf("SAPOS SALVOS: %d", sapos_salvos );
}

int salva_sapo(int *sapos_salvos, STATUS_SAPO sapos[], int *indice_sapo, int *xs1, int *xs2, int *ys1, int *ys2)
{
    if(*ys2<=CHEGADA)
    {
        apagaSapo(*xs1,*ys1);
        (*sapos_salvos)++;

        *xs1 = POS_INIX;
        *xs2 = POS_INIX2;
        *ys1 = POS_INIY;
        *ys2 = POS_INIY2;

        sapos[*indice_sapo] = SALVO;
        (*indice_sapo)++;

        if (*indice_sapo == NUM_SAPOS)
        {
            return SAIR;
        }
        else
        {
            sapos[*indice_sapo] = ATIVO;
            desenhaSapo(*xs1,*ys1);
        }
        return SAPO_SALVO;
    }
    return CONTINUA;
}

int mata_sapo(STATUS_SAPO sapos[],int *sapos_mortos, int *indice_sapo,
              int *xs1, int *xs2, int *ys1, int *ys2,
              int x1, int x2, int y1, int y2) {


    if(testa_colisao(*xs1,*ys1,*xs2,*ys2,x1,y1,x2,y2))
    {
        explodeSapo(*xs1,*ys1);
        apagaSapo(*xs1,*ys1);

        *xs1 = POS_INIX;
        *xs2 = POS_INIX2;
        *ys1 = POS_INIY;
        *ys2 = POS_INIY2;

        sapos[*indice_sapo] = MORTO;
        (*sapos_mortos)++;
        (*indice_sapo)++;

        if (*indice_sapo == NUM_SAPOS)
        {
            return SAIR;
        }
        else
        {
            sapos[*indice_sapo] = ATIVO;
            desenhaSapo(*xs1,*ys1);
        }

        return SAPO_MORREU;
    }
    return CONTINUA;
}


void explodeSapo(int x, int y)
{

    textcolor(COR_SAPO);
    gotoxy(x, y);
    printf(" (O O)");
    gotoxy(x, y +1);
    printf("\\^{   ]^/");
    Sleep(100);

    textcolor(COR_FUNDO);
    gotoxy(x, y);
    printf(" (O O)");
    gotoxy(x, y +1);
    printf("\\^{   ]^/");

    textcolor(COR_SAPO);
    gotoxy(x, y);
    printf(" (O  O)");
    gotoxy(x, y +1);
    printf("\\ ^{   ]^ /");

    Sleep(100);

    textcolor(COR_FUNDO);
    gotoxy(x, y);
    printf(" (O  O)");
    gotoxy(x, y +1);
    printf("\\ ^{   ]^ /");

    textcolor(COR_SAPO);
    gotoxy(x, y);
    printf(" (O   O)");
    gotoxy(x, y +1);
    printf("\\ ^ {    ] ^ /");
    Sleep(100);

    textcolor(COR_FUNDO);
    gotoxy(x, y);
    printf(" (O   O)");
    gotoxy(x, y +1);
    printf("\\ ^ {    ] ^ /");

    textcolor(COR_SAPO);
    gotoxy(x, y);
    printf("o   o   o");
    gotoxy(x, y +1);
    printf("\\ o O    O o /");

    Sleep(100);
    textcolor(COR_FUNDO);
    gotoxy(x, y);
    printf("o   o   o");
    gotoxy(x, y +1);
    printf("\\ o O    O o /");

}

void inicializar(STATUS_SAPO sapos[], int x1[QTD_VEICULOS], int y1[QTD_VEICULOS], int x2[QTD_VEICULOS], int y2[QTD_VEICULOS],
                 int tipoVeiculo[QTD_VEICULOS], int distVeiculo[QTD_VEICULOS], int direcao[QTD_VEICULOS], int ypistas[QTD_PISTA])
{

    int i,j=0,k=0;
    int larg_veiculo, esp_vazio;

    sapos[0] = ATIVO;
    ypistas[0] = Y_INI_PISTA;

    for (i=1; i<QTD_PISTA; i++) {

        ypistas[i] = ypistas[i-1] + LARG_PISTA;

        if (ypistas[i] == MEIO_CIMA) {
            ypistas[i] = MEIO_BAIXO + 1;
        }
    }

    for (i=0; i<QTD_VEICULOS; i++)
    {
        y1[i] = ypistas[k];
        y2[i] = y1[i] + ALTURASAPO;
        tipoVeiculo[i] = rand() % 2;

        if (tipoVeiculo[i] == TIPO_CARRO ) {
            larg_veiculo = LARGCARRO;
        } else {
            larg_veiculo = LARGCAMINHAO;
        }

        distVeiculo[i] = DIST_MIN + rand() % DIST_VARIACAO;

        if (k%2) {
            direcao[i] = DIR_DIR;
        } else {
            direcao[i] = DIR_ESQ;
        }

        if (j == 0) {
            if (direcao[i] == DIR_DIR )
            {
                x2[i] = LIM_DIR - 1;
                x1[i] = x2[i] - larg_veiculo;
            }
            else
            {
                x1[i] = LIM_ESQ + 1;
                x2[i] = x1[i] + larg_veiculo;
            }
        } else {
            if (direcao[i] == DIR_DIR ) {
                x2[i] = x1[i-1] - distVeiculo[i];
                x1[i] = x2[i] - larg_veiculo;
            } else {
                x1[i] = x2[i-1] + distVeiculo[i];
                x2[i] = x1[i] + larg_veiculo;
            }
        }

        j++;
        if ( j==VEIC_PISTA )
        {
            k++;
            j=0;
        }
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
                            int tipoVeiculo[], int distVeiculo[], int direcao[],short int veiculo_valido[])
{
    int i;

    for(i=0; i<QTD_VEICULOS; i++) {
        veiculo_valido[i] = desenhaVeiculo(pontox1[i], pontoy1[i], tipoVeiculo[i], direcao[i], COR_FUNDO);

        if(direcao[i])
        {
            pontox1[i]+= DIST_MOV_VEIC;
            pontox2[i]+= DIST_MOV_VEIC;
        }
        else
        {
            pontox1[i]-= DIST_MOV_VEIC;
            pontox2[i]-= DIST_MOV_VEIC;
        }

        if(tipoVeiculo[i])
        {
            veiculo_valido[i] = desenhaVeiculo(pontox1[i], pontoy1[i], tipoVeiculo[i], direcao[i], COR_CARRO);
        }
        else
        {
            veiculo_valido[i] = desenhaVeiculo(pontox1[i], pontoy1[i], tipoVeiculo[i], direcao[i], COR_CAMINHAO);
        }
    }
    //int veiculo_valido[])

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

short desenhaVeiculo(int x, int y, int tipo, int direcao, COLORS cor)
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
    for(i = 0; i < VEIC_PISTA; i++)
    {
        tipos[i] = rand() % 10 < 5;
    }
    posicao[0] = - LARGCAMINHAO;
    for(i = 1; i < VEIC_PISTA; i++)
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

void desenhaAmbiente(int x, int y, int indice_sapo, int sapos_salvos, int sapos_mortos)
{
    gotoxy(1,1);
    textcolor(COR_MENU);
    printf(" INF1202 Eduardo & Jose     (C)arregar jogo   (P)ausa   (ESC)Sair   (R)Ranking");
    borda();
    desenhaSapo(x, y);

    placar(indice_sapo, sapos_salvos, sapos_mortos);
}
