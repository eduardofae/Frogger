/*
Frogger UFRGS (EDUARDO e JOSE)
*/
#include<stdio.h>
#include<conio2.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<windows.h>
#include<time.h>

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
    BAIXO = 80
};

typedef enum
{
    CONTINUA = 0,
    SAPO_MORREU = 1,
    SAIR = 2,
    SAPO_SALVO = 3
}STATUS_JOGO;

typedef enum
{
    COR_FUNDO = BLACK,
    COR_SAPO = GREEN,
    COR_CARRO = YELLOW,
    COR_CAMINHAO = WHITE,
    COR_BORDA = BLUE,
    COR_MENU = WHITE,
    COR_TEXTO = WHITE,
    BRANCO = WHITE
}COR;

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

typedef enum
{
    DIR_ESQUERDA = 0,
    DIR_DIREITA = 1,
    DIR_BAIXO = 2,
    DIR_CIMA = 3
} DIRECAO_MOVIMENTO;

typedef enum
{
    CAMINHAO = 0,
    CARRO = 1
} TIPO_VEICULO;

typedef enum
{
    ESPERA = 1,
    ATIVO = 2,
    SALVO = 3,
    MORTO = 4
} STATUS_SAPO;

typedef struct
{
    int x;
    int y;
} COORDENADA;

typedef struct  // essa proposta esta um pouco diferente do enunciado do Trabalho final.
{
    char nome [TAM_BANNER];
    int saposSalvos;
    time_t inicioJogo;
    int tempoJogo;
    int score;
    int sapoAtual;
} JOGADOR;

typedef struct
{
    COORDENADA envelope[2]; // pontos do envelope do sapo
    STATUS_SAPO status;
    COR cor;
    DIRECAO_MOVIMENTO dir;  // indica que direção o sapo vai
    int fase;               //campo opcional se quiserem trocar o desenho do sapo em cada fase
} SAPO ;

typedef struct
{
    COORDENADA envelope[2]; // pontos do envelope do veículo
    TIPO_VEICULO tipo; // sedan, ônibus, esporte
    COR cor;
    DIRECAO_MOVIMENTO dir; // indica que direção o veículo vai
    int distancia;
    int pista; // opcional se quiser tratar pista junto com o carro
    int valido; // opcional 1 se veiculo esta na área de jogo, 0 se não esta
    int fase; //campo opcional se quiserem trocar o desenho do carro em cada fase
} VEICULO;

void borda();
void desenhaSapo(SAPO sapo);
void apagaSapo(SAPO sapo);
void desenhaAmbiente(SAPO listaSapos[], JOGADOR jog);

void jogo(SAPO listaSapos[],JOGADOR *jog,
          int x1[QTD_VEICULOS], int y1[QTD_VEICULOS], int x2[QTD_VEICULOS], int y2[QTD_VEICULOS],
          int tipoVeiculo[QTD_VEICULOS], int distVeiculo[QTD_VEICULOS],
          short int veiculoValido[QTD_VEICULOS], int direcao[QTD_VEICULOS]);

void testaMov(SAPO *sapo, char tecla);
void moveSapo(SAPO *sapo, char tecla);
void desenhaVeiculo(VEICULO veiculo, COLORS cor);

void desenha_lista_veiculos(int pontox1[], int pontoy1[], int pontox2[], int pontoy2[],
                            int tipoVeiculo[], int distVeiculo[], int direcao[],short int veiculo_valido[]);

short testa_colisao(int xs1,int ys1,int xs2,int ys2, int x1, int y1, int x2, int y2);

void inicializar(JOGADOR *jog, SAPO lista_sapos[],
                 int tipoVeiculo[QTD_VEICULOS], int distVeiculo[QTD_VEICULOS], int direcao[QTD_VEICULOS], int ypistas[QTD_PISTA]);

int mata_sapo(SAPO listaSapos,JOGADOR *jog,int x1, int x2, int y1, int y2);
void explodeSapo(int x, int y);
void banner(char msg[TAM_BANNER]);
void placar(JOGADOR jog);
int salvaSapo(JOGADOR *jog, SAPO listaSapos[]);
void inicializaJogador(JOGADOR *jog);

int main()
{
    int

        x1[QTD_VEICULOS],
        y1[QTD_VEICULOS],
        x2[QTD_VEICULOS],
        y2[QTD_VEICULOS],

        tipoVeiculo[QTD_VEICULOS],
        distVeiculo[QTD_VEICULOS],
        direcao[QTD_VEICULOS],
        ypistas[QTD_PISTA];

    short int veiculoValido[QTD_VEICULOS] = {1};

    //VEICULO lista_veiculos[QTD_VEICULOS];

    SAPO listaSapos[NUM_SAPOS];

    JOGADOR jogador;

    srand(time(NULL));

    inicializar(&jogador, listaSapos, tipoVeiculo, distVeiculo, direcao, ypistas);
    desenhaAmbiente( listaSapos, jogador );

    jogo(listaSapos, &jogador, x1, y1, x2, y2, tipoVeiculo, distVeiculo, veiculoValido, direcao);

    gotoxy(LIM_ESQ, LIM_BAIXO+4);
    return(0);
}

void inicializar(JOGADOR *jog, SAPO listaSapos[],
                 int tipoVeiculo[QTD_VEICULOS], int distVeiculo[QTD_VEICULOS], int direcao[QTD_VEICULOS], int ypistas[QTD_PISTA])
{

    int i,j=0,k=0;
    int larg_veiculo;

    inicializaJogador(jog);

    inicializaSapos(listaSapos);

    ypistas[0] = Y_INI_PISTA;

    for (i=1; i<QTD_PISTA; i++)
    {

        ypistas[i] = ypistas[i-1] + LARG_PISTA;

        if (ypistas[i] == MEIO_CIMA)
        {
            ypistas[i] = MEIO_BAIXO + 1;
        }
    }

    for (i=0; i<QTD_VEICULOS; i++)
    {
        y1[i] = ypistas[k];
        y2[i] = y1[i] + ALTURASAPO;
        tipoVeiculo[i] = rand() % 2;

        if (tipoVeiculo[i] == TIPO_CARRO )
        {
            larg_veiculo = LARGCARRO;
        }
        else
        {
            larg_veiculo = LARGCAMINHAO;
        }

        distVeiculo[i] = DIST_MIN + rand() % DIST_VARIACAO;

        if (k%2)
        {
            direcao[i] = DIR_DIR;
        }
        else
        {
            direcao[i] = DIR_ESQ;
        }

        if (j == 0)
        {
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
        }
        else
        {
            if (direcao[i] == DIR_DIR )
            {
                x2[i] = x1[i-1] - distVeiculo[i];
                x1[i] = x2[i] - larg_veiculo;
            }
            else
            {
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

void inicializaSapos(SAPO listaSapos[]) {
    int i;
    for(i=0; i<NUM_SAPOS; i++) {

        listaSapos[i].status = ESPERA;
        listaSapos[i].cor = COR_SAPO;

        listaSapos[i].envelope[0].x = POS_INIX;
        listaSapos[i].envelope[0].y = POS_INIY;
        listaSapos[i].envelope[1].x = POS_INIX2;
        listaSapos[i].envelope[1].y = POS_INIY2;
    }
    listaSapos[0]->status = ATIVO;
}

void inicializaVeiculos(VEICULO veiculos[]) {
//todo
}

void inicializaJogador(JOGADOR *jog)
{
    strcpy(jog->nome,"");
    jog->saposSalvos = 0;
    jog->inicioJogo = time(NULL);
    jog->tempoJogo = 0;
    jog->score = 0;
    jog->sapoAtual = 0;
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

void jogo(SAPO listaSapos[],JOGADOR *jog,
          int x1[QTD_VEICULOS], int y1[QTD_VEICULOS], int x2[QTD_VEICULOS], int y2[QTD_VEICULOS],
          int tipoVeiculo[QTD_VEICULOS], int distVeiculo[QTD_VEICULOS],
          short int veiculoValido[QTD_VEICULOS], int direcao[QTD_VEICULOS])
{
    STATUS_JOGO status = CONTINUA;
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
                status = mata_sapo(listaSapos, jog, x1[i], x2[i], y1[i], y2[i]);

            }
            i++;
        }

        if(status == CONTINUA)
        {
            status = salvaSapo(jog, listaSapos);
        }

        if (status != CONTINUA)
        {
            placar(*jog);
        }

        if (status == SAIR)
        {
            banner("GAME OVER!!!");
        }

        if(_kbhit())
        {
            tecla = getch();
            if(tecla == ESPECIAL)
            {
                tecla = getch();
                moveSapo(&listaSapos[jog->sapoAtual], tecla);
            }
            else if (tecla == ESC)
            {
                status = SAIR;
            }
        }
    }
}

void banner(char msg[TAM_BANNER])
{

    int meio = strlen(msg)/2;;

    textcolor(COR_TEXTO);

    gotoxy( LIM_DIR/2 - meio, MEIO_CIMA + 1 );
    printf("%s",msg);

}

void placar(JOGADOR jog)
{

    textcolor(COR_TEXTO);

    if (( NUM_SAPOS-1) - jog.sapoAtual >= 0 )
    {
        gotoxy(LIM_ESQ, LIM_BAIXO+1);
        printf("SAPOS EM ESPERA: %d",(NUM_SAPOS-1) - jog.sapoAtual );
    }
    gotoxy(LIM_ESQ, LIM_BAIXO+2);
    printf("SAPOS MORTOS: %d", jog.sapoAtual - jog.saposSalvos);

    gotoxy(LIM_ESQ, LIM_BAIXO+3);
    printf("SAPOS SALVOS: %d", jog.saposSalvos );
}

int salvaSapo(JOGADOR *jog, SAPO listaSapos[])
{
    if(listaSapos[jog->sapoAtual].envelope[1].y<=CHEGADA)
    {
        apagaSapo(listaSapos[jog->sapoAtual]);
        jog->saposSalvos++;

        listaSapos[jog->sapoAtual].status =  SALVO;

        jog->sapoAtual++;

        if (jog->sapoAtual == NUM_SAPOS)
        {
            return SAIR;
        }
        else
        {
            listaSapos[jog->sapoAtual].status = ATIVO;
            desenhaSapo(listaSapos[jog->sapoAtual]);
        }
        return SAPO_SALVO;
    }
    return CONTINUA;
}

int mata_sapo(SAPO listaSapos,JOGADOR *jog,int x1, int x2, int y1, int y2)
{

    if(testa_colisao(listaSapos[jog->sapoAtual],x1,y1,x2,y2))
    {
        explodeSapo(listaSapos[jog->sapoAtual]);
        apagaSapo(listaSapos[jog->sapoAtual]);

        listaSapos[jog->sapoAtual].status = MORTO;

        jog->sapoAtual++;

        if (jog->sapoAtual == NUM_SAPOS)
        {
            return SAIR;
        }
        else
        {
            listaSapos[jog->sapoAtual].status = ATIVO;
            desenhaSapo(listaSapos[jog->sapoAtual]);
        }

        return SAPO_MORREU;
    }
    return CONTINUA;
}

void explodeSapo(SAPO sapo)
{
    textcolor(COR_SAPO);
    gotoxy(sapo.envelope[0].x, sapo.envelope[0].y);
    printf(" (O O)");
    gotoxy(sapo.envelope[0].x, sapo.envelope[0].y +1);
    printf("\\^{   ]^/");
    Sleep(100);

    textcolor(COR_FUNDO);
    gotoxy(sapo.envelope[0].x, sapo.envelope[0].y);
    printf(" (O O)");
    gotoxy(sapo.envelope[0].x, sapo.envelope[0].y +1);
    printf("\\^{   ]^/");

    textcolor(COR_SAPO);
    gotoxy(sapo.envelope[0].x, sapo.envelope[0].y);
    printf(" (O  O)");
    gotoxy(sapo.envelope[0].x, sapo.envelope[0].y +1);
    printf("\\ ^{   ]^ /");
    Sleep(100);

    textcolor(COR_FUNDO);
    gotoxy(sapo.envelope[0].x, sapo.envelope[0].y);
    printf(" (O  O)");
    gotoxy(sapo.envelope[0].x, sapo.envelope[0].y +1);
    printf("\\ ^{   ]^ /");

    textcolor(COR_SAPO);
    gotoxy(sapo.envelope[0].x, sapo.envelope[0].y);
    printf(" (O   O)");
    gotoxy(sapo.envelope[0].x, sapo.envelope[0].y +1);
    printf("\\ ^ {    ] ^ /");
    Sleep(100);

    textcolor(COR_FUNDO);
    gotoxy(sapo.envelope[0].x, sapo.envelope[0].y);
    printf(" (O   O)");
    gotoxy(sapo.envelope[0].x, sapo.envelope[0].y +1);
    printf("\\ ^ {    ] ^ /");

    textcolor(COR_SAPO);
    gotoxy(sapo.envelope[0].x, sapo.envelope[0].y);
    printf("o   o   o");
    gotoxy(sapo.envelope[0].x, sapo.envelope[0].y +1);
    printf("\\ o O    O o /");
    Sleep(100);

    textcolor(COR_FUNDO);
    gotoxy(sapo.envelope[0].x, sapo.envelope[0].y);
    printf("o   o   o");
    gotoxy(sapo.envelope[0].x, sapo.envelope[0].y +1);
    printf("\\ o O    O o /");
}

short testa_colisao(SAPO sapo, int x1, int y1, int x2, int y2)
{

    if ( ( (sapo.envelope[0].x <= x2 && sapo.envelope[0].x >= x1 ) || ( sapo.envelope[1].x >= x1 && sapo.envelope[1].x <= x2 ) ) && (sapo.envelope[1].y == y2) )
    {
        return 1;
    }
    return 0;
}

void desenha_lista_veiculos(VEICULO veiculos[])
{
    int i;

    for(i=0; i<QTD_VEICULOS; i++)
    {
        desenhaVeiculo(veiculos[i], COR_FUNDO);

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

void moveSapo(SAPO *sapo, char tecla)
{
    apagaSapo(*sapo);// apaga sapo
    testaMov(sapo, tecla );
    desenhaSapo(*sapo); // desenha novo sapo
}

void testaMov(SAPO *sapo, char tecla)
{
    switch(tecla)
    {
    case ESQ:
        if (sapo->envelope[0].x-DISTANCIAX > LIM_ESQ)
        {
            sapo->envelope[0].x -= DISTANCIAX;
            sapo->envelope[1].x -= DISTANCIAX;
            sapo->dir = DIR_ESQUERDA;
        }
        break;
    case DIR:
        if (sapo->envelope[1].x+DISTANCIAX < LIM_DIR)
        {
            sapo->envelope[0].x += DISTANCIAX;
            sapo->envelope[1].x += DISTANCIAX;
            sapo->dir = DIR_DIREITA;
        }
        break;
    case CIMA:
        if (sapo->envelope[0].y-DISTANCIAY > LIM_CIMA)
        {
            sapo->envelope[0].y -= DISTANCIAY;
            sapo->envelope[1].y -= DISTANCIAY;
            sapo->dir = DIR_CIMA;
        }
        break;
    case BAIXO:
        if (sapo->envelope[1].y+DISTANCIAY < LIM_BAIXO)
        {
            sapo->envelope[0].y += DISTANCIAY;
            sapo->envelope[1].y += DISTANCIAY;
            sapo->dir = DIR_BAIXO;
        }
        break;
    }
}

void desenhaSapo(SAPO sapo)
{
    textcolor(sapo.cor);
    gotoxy(sapo.envelope[0].x, sapo.envelope[0].y);
    printf("  (OO)");
    gotoxy(sapo.envelope[0].x, sapo.envelope[0].y +1);
    printf("\\^{  ]^/");
}

void apagaSapo(SAPO sapo)
{
    textcolor(COR_FUNDO);
    gotoxy(sapo.envelope[0].x, sapo.envelope[0].y);
    printf("  (OO)");
    gotoxy(sapo.envelope[0].x, sapo.envelope[0].y +1);
    printf("\\^{  ]^/");
}

int imprimeDentro(char desenho, int largura)
{
    int dentro = 0;
    for(i = 0; i < largura; i++)
    {
        if(i + x < LIM_DIR && x + i > LIM_ESQ)
        {
            dentro = 1;
            putchxy(i + x, y, desenho[0][i]);
            putchxy(i + x, y + 1, desenho[1][i]);
            putchxy(i + x, y + 2, desenho[2][i]);
            // todo
            // fazer um for até altura
        }
    }
    return dentro;
}

void desenhaCarro(VEICULO carro, COLORS cor)
{
    char veiculo[ALTURAVEICULO][LARGCARRO];

    textcolor(cor);

    if(carro.dir == DIR_DIREITA )
    {
        strcpy(veiculo[0], "o o");
        strcpy(veiculo[1], "HHH>");
        strcpy(veiculo[2], "o o");
    }
    else
    {
        strcpy(veiculo[0], " o o");
        strcpy(veiculo[1], "<HHH");
        strcpy(veiculo[2], " o o");
    }
    // talvez tenha que passar com pelo menos um dos indices
    carro.valido = imprimeDentro(veiculo,LARGCARRO);
}

void desenhaCaminhao(VEICULO caminhao, COLORS cor)
{
    char veiculo[ALTURAVEICULO][LARGCARRO];

    textcolor(cor);

    if(caminhao.direcao == DIR_DIREITA)
    {
        strcpy(veiculo[0], " OO   o");
        strcpy(veiculo[1], "-[[[[[(8");
        strcpy(veiculo[2], " OO   o");
    }
    else
    {
        strcpy(veiculo[0], " o  OO");
        strcpy(veiculo[1], "8)]]]]]-");
        strcpy(veiculo[2], " o  OO");
    }
    // talvez tenha que passar com pelo menos um dos indices
    caminhao.valido = imprimeDentro(veiculo,LARGCAMINHAO);
}

void desenhaVeiculo(VEICULO veiculo, COLORS cor)
{
    // imprime veiculo conforme o tipo
    // switch para adicionar carros
    // conforme a necessidade e tirar facil também
    switch(veiculo.tipo)
    {
    case CARRO:
        desenhaCarro(veiculo, cor);
        break;
    case CAMINHAO:
        desenhaCaminhao(veiculo, cor);
        break;
    }
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

void desenhaAmbiente(SAPO listaSapos, JOGADOR jog)
{
    gotoxy(1,1);
    textcolor(COR_MENU);
    printf(" INF1202 Eduardo & Jose     (C)arregar jogo   (P)ausa   (ESC)Sair   (R)Ranking");
    borda();

    desenhaSapo(listaSapos[jog.sapoAtual]);

    placar(jog);
}
