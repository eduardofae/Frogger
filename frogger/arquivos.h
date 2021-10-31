# define TAM_NOME 20
# define NUM_SAPOS 6

enum
{
    QTD_PISTA = 4,
    VEIC_PISTA = 15,
    QTD_VEICULOS = QTD_PISTA * VEIC_PISTA
};

typedef struct  // essa proposta esta um pouco diferente do enunciado do Trabalho final.
{
    char nome [TAM_NOME];
    int saposSalvos;
    time_t inicioJogo;
    int tempoJogo;
    long int score;
    int sapoAtual;
    int saposEspera;
} JOGADOR;

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

typedef enum
{
    DIR_ESQUERDA = 0,
    DIR_DIREITA = 1,
    DIR_BAIXO = 2,
    DIR_CIMA = 3
} DIRECAO_MOVIMENTO;

typedef struct
{
    int x;
    int y;
} COORDENADA;

typedef enum
{
    ESPERA = 1,
    ATIVO = 2,
    SALVO = 3,
    MORTO = 4
} STATUS_SAPO;

typedef struct
{
    COORDENADA envelope[2]; // pontos do envelope do sapo
    STATUS_SAPO status;
    COR cor;
    DIRECAO_MOVIMENTO dir;  // indica que direcao o sapo vai
    int fase;               //campo opcional se quiserem trocar o desenho do sapo em cada fase
} SAPO ;

typedef enum
{
    CAMINHAO = 0,
    CARRO = 1
} TIPO_VEICULO;

typedef struct
{
    COORDENADA envelope[2]; // pontos do envelope do ve�culo
    TIPO_VEICULO tipo; // sedan, onibus, esporte
    COR cor;
    DIRECAO_MOVIMENTO dir; // indica que direcao o veiculo vai
    int distancia;
    int pista; // opcional se quiser tratar pista junto com o carro
    int valido; // opcional 1 se veiculo esta na area de jogo, 0 se nao esta
    int fase; //campo opcional se quiserem trocar o desenho do carro em cada fase
} VEICULO;

typedef struct
{
    JOGADOR jogador;
    int fase;
    SAPO listaSapos[NUM_SAPOS];
    VEICULO listaVeiculos[QTD_VEICULOS];

}ESTADO;

int leJogoSalvo(ESTADO *estado, char nome[]);
int salvaEstadoDoJogo(ESTADO estado);
int AbreArqJog(FILE *arqtxt, char nome[]);
int LeArqTexto(FILE *arqtxt, ESTADO estado, JOGADOR jogadores[]);
void OrdenaListaJogadores(JOGADOR jogadores[], int tamArray);
void InsereJogador(JOGADOR jogadores[], JOGADOR jog, int *tamArray, int maxArray);
void troca(JOGADOR *a, JOGADOR *b);

