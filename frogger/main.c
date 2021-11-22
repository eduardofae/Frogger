/*
Frogger UFRGS (EDUARDO e JOSE)

Status dos requisitos:
1 ok
2 ok
3 ok
4 ok
5 ok
6 7 8 ok
9 ok
10 ok
11 ok
12 nok função ranking não implementada
13 ok
14 ok
15 ok 
16 nok mostra o ranking e funciona porém não mostra o ranking com o r
17 ok
18 ok
19 ok
20 ok
21 ok
22 nok não implementado
23 nok isso é possível porém não foi implementado
24 ok

*/

// libs utilizadas
#include<stdio.h>
#include<conio2.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<windows.h>
#include<time.h>
#include<ctype.h>
#include"arquivos.h" // lib que criamos para lidar com os arquivos

// constantes auxiliares para o jogo
# define LINHA '-'
# define COLUNA '|'
# define PUNICAO_MORTE 30
# define MAX_JOGADORES 5

// temos aqui series diferentes de enums auxiliares que vão
// desde lidar com as teclas do teclado até auxiliar nos
// x y das pistas
enum
{
    ENTER = 13,
    ESPACO = 32,
    ESC = 27,
    PAUSA = 112,
    CARREGAR = 99,
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
    POS_INIY2 = POS_INIY + (ALTURASAPO - 1)
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
    X2_INI_PISTA = LIM_ESQ - 1
};

// prototipos das funções

void borda();
void desenhaSapo(SAPO sapo);
void apagaSapo(SAPO sapo);
void desenhaAmbiente(ESTADO estado);
void jogo(ESTADO *estado);
void testaMov(SAPO *sapo, char tecla);
void moveSapo(SAPO *sapo, char tecla);
void desenhaVeiculo(VEICULO veiculo, COLORS cor);
void desenha_lista_veiculos(VEICULO listaVeiculos[]);
short testa_colisao(SAPO sapo, VEICULO veiculo);
void inicializar(ESTADO *estado);
int mata_sapo(SAPO listaSapos[], JOGADOR *jog, VEICULO veiculo);
void explodeSapo(SAPO sapo);
void banner(char msg[TAM_BANNER], int y);
void placar(JOGADOR jog);
int salvaSapo(JOGADOR *jog, SAPO listaSapos[]);
void inicializaJogador(JOGADOR *jog);
void inicializaSapos(SAPO listaSapos[]);
void inicializaVeiculos(VEICULO veiculos[]);
int imprimeDentro(char desenho[ALTURAVEICULO][LARGCAMINHAO], VEICULO veiculo, int largura);
void calculaScore(JOGADOR *jog);
void desenhaCarro(VEICULO carro, COLORS cor);
void desenhaCaminhao(VEICULO caminhao, COLORS cor);
void pedeNome(JOGADOR *jog);
void pausa(ESTADO *estado);
void instanciaJogo(ESTADO *estado);
void apagaAmbiente();
void OrdenaListaJogadores(JOGADOR jogadores[], int tamArray);
void InsereJogador(JOGADOR jogadores[], JOGADOR jog, int *tamArray, int maxArray);
void MostraListaJogadores(JOGADOR jogadores[], JOGADOR jog, int tamArray);
void troca(JOGADOR *a, JOGADOR *b);
void mostraRanking(JOGADOR jogadores[], ESTADO estado);

// na main usamos a estrutura estado que vai ter a lista de sapos, o jogador, a lista de 
// veiculos e a fase atual, nosso jogo não troca de fase mas ficou com isso nas estruturas
// para ser possível fazer com facilidade
// na main também temos a lista de jogadores que será mostrada no ranking
// criamos a semente randomica que auxiliará na inicialização geral do jogo
// isolamos as inicializações na função inicializar e mandamos por referencia o estado atual pois
// ele será modificado la dentro
// depois usamos desenhaAmbiente para desenhar o ambiente, fazemos isso passando o estao atual
// depois a função jogo que tem o loop principal do jogo
// quando o jogo acaba aqui na main chamamos a função mostrar ranking mandando aquela lista de jogadores
// e a variavel estado
// usamos a função gotoxy no final apenas para o cursor final ficar posicionado em uma posição que não 
// atrapalhe a visualização da tela final do jogo e por fim a main retorna 0 indicando que tudo ok 
// na execução
int main()
{
    ESTADO estado;
    JOGADOR jogadores[MAX_JOGADORES];

    srand(time(NULL));

    inicializar(&estado);
    desenhaAmbiente(estado);

    jogo(&estado);

    mostraRanking(jogadores, estado);

    gotoxy(LIM_ESQ, LIM_BAIXO+4);
    return(0);
}

// função inicializar recebe estado mas pega cada parte separadamente
// e as inicializa
void inicializar(ESTADO *estado)
{
    inicializaJogador(&estado->jogador); // jogador é uma estrutura dentro de estado que deve ser passado por referencia, então chegamos ae jogador e então damos sua referencia


    // os demais inicializa são arrays e portanto ja são passados por referencia

    inicializaSapos(estado->listaSapos);

    inicializaVeiculos(estado->listaVeiculos);
}

// percorremos a lista colocando os sapos em espera , damos a cor padrão do sapo
// e usamos as consta de posição para dizer que a posição inicial de todos os sapos
// é o local de onde ele parte no seu momento inicial
// no fim dizemos que o primeiro sapo é ativo
void inicializaSapos(SAPO listaSapos[])
{
    int i;
    for(i=0; i<NUM_SAPOS; i++) {

        listaSapos[i].status = ESPERA;
        listaSapos[i].cor = COR_SAPO;

        listaSapos[i].envelope[0].x = POS_INIX;
        listaSapos[i].envelope[0].y = POS_INIY;
        listaSapos[i].envelope[1].x = POS_INIX2;
        listaSapos[i].envelope[1].y = POS_INIY2;
    }
    listaSapos[0].status = ATIVO;
}

void inicializaVeiculos(VEICULO veiculos[])
{

    int i,posPista=0,pista=0;
    int largVeiculo;
    int ypistas[QTD_PISTA]; // array que tem os y das pistas será nossa lista de pistas

    ypistas[pista] = Y_INI_PISTA; // primeira pista recebe a posição da pista mais acima na tela

    // inicialização das alturas das pistas
    for (i= pista + 1; i<QTD_PISTA; i++) // vai percorrer todas as pistas a partir da segunda
    {
        ypistas[i] = ypistas[i-1] + LARG_PISTA; // posição da proxima pista é posição da pista anterior - largura padrão 

        // quando a posição coincide com a linha do meio da parte de cima então dizemos para
        // pular o vale central e ter seu proximos y 1 abaixo da linha inferior
        if (ypistas[i] == MEIO_CIMA)
        {
            ypistas[i] = MEIO_BAIXO + 1;
        }
    }

    // inicializa as distancias entre os veiculos
    // perceba que pesa bastante no nosso loop o fato de termos
    // optado por fazer um array que contém todos os veiculos
    for (i=0; i<QTD_VEICULOS; i++)
    {
        veiculos[i].envelope[0].y = ypistas[pista]; // y mais superior do envelope
        veiculos[i].envelope[1].y = veiculos[i].envelope[0].y + ALTURASAPO; // y mais abaixo do envelope é o y do envelope anterior mais a altura do sapo
        veiculos[i].tipo = rand() % 2; // sorteia o tipo do veiculo

	// conforme o tipo do veiculo escolhe sua largura
        switch (veiculos[i].tipo)
        {
            case CARRO:
                largVeiculo = LARGCARRO;
                break;
            case CAMINHAO:
                largVeiculo = LARGCAMINHAO;
                break;
        }

        veiculos[i].distancia = DIST_MIN + rand() % DIST_VARIACAO; // soma a distancia minima aceitavel com uma variação sorteada

	// optamos por colocar as pistas impar indo pra direita 
        if (pista % 2)
        {
            veiculos[i].dir = DIR_DIREITA;
        }
        else
        {
            veiculos[i].dir = DIR_ESQUERDA;
        }

        // determina em que local estará o carro 
        if (posPista == 0) // se é o primeiro carro trata especial
        {
		// se a direção do carro é pra direita e é o primeiro então será proximo a parede da direita o x do seu envelope
		// mais a direita e logo o outro sera o mesmo - sua largura definida anteriormente
		// e o analogo para esquerda
            if (veiculos[i].dir == DIR_DIREITA )
            {
                veiculos[i].envelope[1].x = LIM_DIR - 1;
                veiculos[i].envelope[0].x = veiculos[i].envelope[1].x - largVeiculo;
            }
            else
            {
                veiculos[i].envelope[0].x = LIM_ESQ + 1;
                veiculos[i].envelope[1].x = veiculos[i].envelope[0].x + largVeiculo;
            }
        }
        else // formula geral após tratamento da excessão 
        {
		// para todo veiculo após ja tratada a excessão indo para a direita pegue o x do envelope mais a esquerda
		// e então some a distancia definida previamente e este é o x do envelope atual mais a direita
		// que servira como base para o x mais a esquerda deste envelope diminuida a largura da veiculo
		// pois os carros estão indo da esquerda para direita
		// e o analogo para o proximo caso
            if (veiculos[i].dir == DIR_DIREITA )
            {
                veiculos[i].envelope[1].x = veiculos[i-1].envelope[0].x - veiculos[i].distancia;
                veiculos[i].envelope[0].x = veiculos[i].envelope[1].x - largVeiculo;
            }
            else
            {
                veiculos[i].envelope[0].x = veiculos[i-1].envelope[1].x + veiculos[i].distancia;
                veiculos[i].envelope[1].x = veiculos[i].envelope[0].x + largVeiculo;
            }
        }

        posPista++; // essa variavel vai de 0 até VEIC_PISTA que é a quantidade de carros por pista
        if ( posPista == VEIC_PISTA ) // quando posPista chegar nesse valor ele é zerado e avançamos para a o loop tratar da proxima pista
        {
            pista++;
            posPista=0;
        }
    }
}

// recebe por referencia e apenas iniciliza os valores básicos do jogador
void inicializaJogador(JOGADOR *jog)
{
    strcpy(jog->nome,"");
    jog->saposSalvos = 0;
    jog->inicioJogo = time(NULL);
    jog->tempoJogo = 0;
    jog->score = 0;
    jog->sapoAtual = 0;
    jog->saposEspera = NUM_SAPOS - 1;
}

// desenha bordas do jogo
void borda()
{
    int i;
    textcolor(COR_BORDA);
    // desenha Linhas Verticais
    for(i = LIM_CIMA; i <= LIM_BAIXO; i++)
    {
        putchxy(LIM_ESQ, i, COLUNA);
        putchxy(LIM_DIR, i, COLUNA);
    }
    // desenha Linhas Horizontais
    for(i = LIM_ESQ; i <= LIM_DIR; i++)
    {
        putchxy(i, LIM_CIMA, LINHA);
        putchxy(i, LIM_BAIXO, LINHA);
        // desenha Linhas de separacao
        putchxy(i, CHEGADA, LINHA);
        putchxy(i, MEIO_BAIXO, LINHA);
        putchxy(i, MEIO_CIMA, LINHA);
    }
}

// loop principal
void jogo(ESTADO *estado)
{
    STATUS_JOGO status = CONTINUA;
    int i;
    char tecla;

    while (status!=SAIR) // vai ficar nesse loop até o status de sair ser alcançado
    {
        status=CONTINUA;
        desenha_lista_veiculos(estado->listaVeiculos); // manda a lista de veiculos para ser desenhada na tela nas suas posições atuais
        i = 0;
        while(i < QTD_VEICULOS && status==CONTINUA)  // percorre toda a lista de veiculos ou até perceber que algum veiculo matou o sapo
        {
            if(estado->listaVeiculos[i].valido) // só entra no carro que esta dentro da area jogavel
            {
                status = mata_sapo(estado->listaSapos, &estado->jogador, estado->listaVeiculos[i]); // verifica se o sapo esta vivo e retorna o status disso pra fora
            }
            i++;
        }

        if(status == CONTINUA) // se o sapo esta vivo ver se ele foi salvo
        {
            status = salvaSapo(&estado->jogador, estado->listaSapos);
        }

        if (status != CONTINUA) // mostra o placar se o sapo esta vivo
        {
            placar(estado->jogador);
        }

        if (status == SAIR) // se sair do jogo ou morrer exibe banner e calcula score do jogador
        {
            banner("GAME OVER!!!", 0);
            calculaScore(&estado->jogador);
        }

        if(_kbhit()) // percebe se alguma tecla foi pressionada
        {
            tecla = getch(); // pega o que foi pressionado 
            switch(tecla) // verifica o que foi pressionado e então trata, caso seja a tecla de movimento pega a proxima que vai ser o movimento em si
			  // e então trata o movimento do sapo
            {
            case ESPECIAL:
                tecla = getch();
                moveSapo(&estado->listaSapos[estado->jogador.sapoAtual], tecla);
                break;
            case PAUSA: // trata pausa
                pausa(estado);
                desenhaSapo(estado->listaSapos[estado->jogador.sapoAtual]);
                break;
            case CARREGAR: // trata carregamento de save a partir do arquivo
                instanciaJogo(estado);
                break;
            case ESC: // sai do jogo
                status = SAIR;
                break;
            }
        }
    }
}

// função auxiliar para exibir uma mensagem na tela
void banner(char msg[TAM_BANNER], int y)
{

    int meio = strlen(msg)/2;;

    gotoxy( LIM_ESQ + 1, (MEIO_CIMA + 1) + y );
    textcolor(COR_FUNDO);
    printf("%*s", LIM_DIR - (LIM_ESQ + 1), "");

    gotoxy( LIM_DIR/2 - meio, (MEIO_CIMA + 1) + y );
    textcolor(COR_TEXTO);
    printf("%s",msg);

}

// mostra o placar do jogo
void placar(JOGADOR jog)
{
    textcolor(COR_TEXTO);

    gotoxy(LIM_ESQ, LIM_BAIXO+1);
    printf("SAPOS EM ESPERA: %d", jog.saposEspera);

    gotoxy(LIM_ESQ, LIM_BAIXO+2);
    printf("SAPOS MORTOS: %d", jog.sapoAtual - jog.saposSalvos);

    gotoxy(LIM_ESQ, LIM_BAIXO+3);
    printf("SAPOS SALVOS: %d", jog.saposSalvos );
}

// função para ver se o jogodor esta no ponto de salvamento
int salvaSapo(JOGADOR *jog, SAPO listaSapos[])
{
    if(listaSapos[jog->sapoAtual].envelope[1].y<=CHEGADA) // se no envelope do sapo atual o y mais acima esta dentro da area de salvamento então 
								// o sapo foi salvo
    {
        apagaSapo(listaSapos[jog->sapoAtual]);
        jog->saposSalvos++;

        listaSapos[jog->sapoAtual].status =  SALVO;

        jog->sapoAtual++;

        if (jog->sapoAtual == NUM_SAPOS)  // se ja acabou o total de sapos então retorna status de finalização
        {
            return SAIR;
        }
        else // senão define o próximo sapo com status de ativo, desenha ele e ajusta o total de sapos em espera 
        {
            listaSapos[jog->sapoAtual].status = ATIVO;
            desenhaSapo(listaSapos[jog->sapoAtual]);
            jog->saposEspera--;
        }
        return SAPO_SALVO; // status indicando que um sapo foi salvo
    }
    return CONTINUA; // status indicando que o jogo apenas deve continuar pois o sapo não foi salvo
}

int mata_sapo(SAPO listaSapos[], JOGADOR *jog, VEICULO veiculo)
{

    if(testa_colisao(listaSapos[jog->sapoAtual], veiculo))
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
            jog->saposEspera--;
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

short testa_colisao(SAPO sapo, VEICULO veiculo)
{

    if ( ( (sapo.envelope[0].x <= veiculo.envelope[1].x && sapo.envelope[0].x >= veiculo.envelope[0].x ) ||
          (sapo.envelope[1].x >= veiculo.envelope[0].x && sapo.envelope[1].x <= veiculo.envelope[1].x ) ) &&
        (sapo.envelope[1].y == veiculo.envelope[1].y) )
    {
        return 1;
    }
    return 0;
}

void desenha_lista_veiculos(VEICULO listaVeiculos[])
{
    int i;

    for(i=0; i<QTD_VEICULOS; i++)
    {
        desenhaVeiculo(listaVeiculos[i], COR_FUNDO);

        if(listaVeiculos[i].dir == DIR_DIREITA)
        {
            listaVeiculos[i].envelope[0].x += DIST_MOV_VEIC;
            listaVeiculos[i].envelope[1].x += DIST_MOV_VEIC;
        }
        else
        {
            listaVeiculos[i].envelope[0].x -= DIST_MOV_VEIC;
            listaVeiculos[i].envelope[1].x -= DIST_MOV_VEIC;
        }

        switch(listaVeiculos[i].tipo)
        {
            case CARRO:
                desenhaVeiculo(listaVeiculos[i], COR_CARRO);
                break;
            case CAMINHAO:
                desenhaVeiculo(listaVeiculos[i], COR_CAMINHAO);
                break;
        }
    }

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

void desenhaVeiculo(VEICULO veiculo, COLORS cor)
{
    // imprime veiculo conforme o tipo
    // switch para adicionar carros
    // conforme a necessidade e tirar facil tambem
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

void desenhaCarro(VEICULO carro, COLORS cor)
{
    char desenho[ALTURAVEICULO][LARGCAMINHAO];

    textcolor(cor);

    if(carro.dir == DIR_DIREITA )
    {
        strcpy(desenho[0], "o o");
        strcpy(desenho[1], "HHH>");
        strcpy(desenho[2], "o o");
    }
    else
    {
        strcpy(desenho[0], " o o");
        strcpy(desenho[1], "<HHH");
        strcpy(desenho[2], " o o");
    }
    // talvez tenha que passar com pelo menos um dos indices
    carro.valido = imprimeDentro(desenho, carro, LARGCARRO);
}

void desenhaCaminhao(VEICULO caminhao, COLORS cor)
{
    char desenho[ALTURAVEICULO][LARGCAMINHAO];

    textcolor(cor);

    if(caminhao.dir == DIR_DIREITA)
    {
        strcpy(desenho[0], " OO   o");
        strcpy(desenho[1], "-[[[[[(8");
        strcpy(desenho[2], " OO   o");
    }
    else
    {
        strcpy(desenho[0], " o  OO");
        strcpy(desenho[1], "8)]]]]]-");
        strcpy(desenho[2], " o  OO");
    }
    // talvez tenha que passar com pelo menos um dos indices
    caminhao.valido = imprimeDentro(desenho, caminhao, LARGCAMINHAO);
}

int imprimeDentro(char desenho[ALTURAVEICULO][LARGCAMINHAO], VEICULO veiculo, int largura)
{
    int i, dentro = 0;
    for(i = 0; i < largura; i++)
    {
        if(i + veiculo.envelope[0].x < LIM_DIR && veiculo.envelope[0].x + i > LIM_ESQ)
        {
            dentro = 1;
            putchxy(i + veiculo.envelope[0].x, veiculo.envelope[0].y, desenho[0][i]);
            putchxy(i + veiculo.envelope[0].x, veiculo.envelope[0].y + 1, desenho[1][i]);
            putchxy(i + veiculo.envelope[0].x, veiculo.envelope[0].y + 2, desenho[2][i]);
            // to do
            // fazer um for ate altura
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

void desenhaAmbiente(ESTADO estado)
{
    gotoxy(1,1);
    textcolor(COR_MENU);
    printf(" INF1202 Eduardo & Jose     (C)arregar jogo   (P)ausa   (ESC)Sair   (R)Ranking");
    borda();

    desenhaSapo(estado.listaSapos[estado.jogador.sapoAtual]);

    placar(estado.jogador);
   
    timer(estado.jogador);
}

void timer(JOGADOR jog) {
    gotoxy(LIM_BAIXO, LIM_DIR-5);
    printf("%ld", jog->inicioJogo-time(null));
}

void calculaScore(JOGADOR *jog)
{
    int score;

    jog->tempoJogo += time(NULL) - jog->inicioJogo;
    pedeNome(jog);

    // quanto menor o score, melhor
    // punicao de 30 segundos para cada sapo morto
    score = jog->tempoJogo + ((NUM_SAPOS - jog->saposSalvos) * PUNICAO_MORTE);

    jog->score = score;

    gotoxy(LIM_ESQ, LIM_BAIXO+4);
    printf("SCORE: %d", score);
}

void pedeNome(JOGADOR *jog)
{
    banner("Digite seu nome: ", 1);
    scanf("%s", jog->nome);
    banner("", 1);
}

void pausa(ESTADO *estado)
{
    int erro; // 0 - sem erro, 1 - erro
    estado->jogador.tempoJogo = time(NULL) - estado->jogador.inicioJogo;
    pedeNome(&estado->jogador);
    erro = salvaEstadoDoJogo(*estado);
    if(erro)
    {
        banner("ERRO NA ABERTURA/GRAVACAO DO ARQUIVO", 1);
    }
}

void instanciaJogo(ESTADO *estado)
{
    char nome[TAM_BANNER];
    int erro = 0; // 0 - sem erro, 1 - erro

    pedeNome(&estado->jogador);

    strcpy(nome, estado->jogador.nome);
    strcat(nome, ".bin");

    erro = leJogoSalvo(estado, nome);
    if(erro)
    {
        banner("ERRO NA ABERTURA/LEITURA DO ARQUIVO", 1);
    }
    else
    {
        estado->jogador.inicioJogo = time(NULL);

        apagaAmbiente();
        desenhaAmbiente(*estado);
    }
}

void apagaAmbiente()
{
    int i, j;
    textcolor(COR_FUNDO);
    gotoxy(1,1);
    for(i = 0; i < LIM_DIR; i++)
    {
        for(j = 0; j < LIM_BAIXO + 3; j++)
        {
            putchxy(i,j,' ');
        }
    }
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
    OrdenaListaJogadores(jogadores, *tamArray);
    while(!encontrou && i < *tamArray)
    {
        if(strcmp(jog.nome, jogadores[i].nome) == 0)
        {
            if(jog.score < jogadores[i].score)
            {
                jogadores[i] = jog;
            }
            encontrou = 1;
        }
        i++;
    }
    if(!encontrou && strcmp(jog.nome, "") != 0)
    {
        if(i < maxArray)
        {
            jogadores[i] = jog;
            (*tamArray)++;
        }
        else if(jogadores[*tamArray - 1].score > jog.score)
        {
            jogadores[*tamArray - 1] = jog;
        }
    }
    OrdenaListaJogadores(jogadores, *tamArray);
}

void MostraListaJogadores(JOGADOR jogadores[], JOGADOR jog, int tamArray)
{
    int i, jogNaLista = 0;
    textcolor(COR_TEXTO);
    for(i = 0; i < tamArray; i++)
    {
        if(strcmp(jogadores[i].nome, jog.nome) == 0)
        {
            jogNaLista = 1;
        }
        gotoxy(LIM_DIR + 1, LIM_CIMA + i);
        printf("%d - %-*s (%03ld)", i + 1, TAM_BANNER, jogadores[i].nome, jogadores[i].score);
    }
    if(!jogNaLista && strcmp(jog.nome, "") != 0)
    {
        gotoxy(LIM_DIR + 1, LIM_CIMA + i);
        printf("%d - %-*s (%03ld)", i + 1, TAM_BANNER, jog.nome, jog.score);
    }
}

void troca(JOGADOR *a, JOGADOR *b)
{
    JOGADOR aux = *a;
    *a = *b;
    *b = aux;
}

void mostraRanking(JOGADOR jogadores[], ESTADO estado)
{
    FILE *arqtxt = 0;
    int semErro, tamArray;
    semErro = AbreArqJog(&arqtxt);
    if(semErro)
    {
        tamArray = LeArqTexto(arqtxt, jogadores, MAX_JOGADORES);
        InsereJogador(jogadores, estado.jogador, &tamArray, MAX_JOGADORES);
        MostraListaJogadores(jogadores, estado.jogador, tamArray);
        semErro = SalvaListaJogadores(arqtxt, jogadores, tamArray);
        fclose(arqtxt);
    }
}

