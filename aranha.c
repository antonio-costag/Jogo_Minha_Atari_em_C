#include <stdio.h>      // Inclui a biblioteca de entrada e saída padrão, usada para funções como sprintf.
#include <stdlib.h>     // Inclui a biblioteca padrão, usada para funções como rand() (geração de números aleatórios) e abs() (valor absoluto).
#include <stdbool.h>    // Inclui a biblioteca para usar o tipo de dado booleano (true/false), tornando o código mais legível.
#include <time.h>       // Inclui a biblioteca de tempo, usada para inicializar a semente do gerador de números aleatórios (srand).
#include <windows.h>    // Inclui a biblioteca principal da API do Windows, necessária para controlar o console (cores, posição do cursor, etc.).
#include <wchar.h>      // Inclui a biblioteca para suporte a caracteres largos (wide characters), como os usados no desenho do prédio.

#define LARGURA_TELA 80         // Define a largura da janela do console em colunas de caracteres.
#define ALTURA_TELA 17           // Define a altura total da janela do console em linhas de caracteres.
#define ALTURA_JOGO_PRINCIPAL 15// Define a altura da área principal onde a jogabilidade acontece.
#define ALTURA_UI_SUPERIOR 1    // Define a altura da linha usada para a Interface do Usuário (UI) superior, os pontos.
#define ESPACO_VAZIO ' '        // Define o caractere usado para limpar a tela ou representar áreas vazias.
#define ATRASO_TIQUE 100        // Define o atraso em milissegundos entre cada "tique" (quadro) do jogo. Controla a velocidade geral.

#define ALTURA_MIRANHA 2        // Altura do sprite do Homem-Aranha em linhas.
#define LARGURA_MIRANHA 4       // Largura do sprite do Homem-Aranha em colunas.
#define ALTURA_DOENTE 3         // Altura do sprite do Duende Verde.
#define LARGURA_DOENTE 6        // Largura do sprite do Duende Verde.
#define ALTURA_INIMIGO_JANELA 2 // Altura do sprite do inimigo que aparece nas janelas.
#define LARGURA_INIMIGO_JANELA 5// Largura do sprite do inimigo da janela.

#define INICIO_TEIA_X 2 // Posição X relativa do ponto de onde a teia é disparada no sprite do Homem-Aranha.
#define MIRANHA_CENTRALIZADO_Y 7 // Posição Y onde o miranha fica centralizado na tela (enquanto não chegar no topo ou no chão Y sempre vai ser 7)

#define MAX_INIMIGOS_JANELA 24   // Número máximo de inimigos de janela que podem aparecer ao mesmo tempo.
#define MAX_BOMBAS 14            // Número máximo de bombas que podem estar ativas no cenário ao mesmo tempo.

#define MIRANHA_CAINDO "|_/o"   // Sprite especial para quando o Homem-Aranha está caindo.

#define ALTURA_PREDIO 65        // Altura total do prédio em linhas de caracteres. O jogo mostra apenas uma parte de cada vez.
#define LARGURA_PREDIO 58       // Largura total do prédio em colunas de caracteres.

#define TEMPO_MAXIMO 20         // Tempo inicial (em "barras") que o jogador tem para completar o nível.
#define DECREMENTO_TEMPO 2000   // Intervalo em milissegundos (2 segundos) para decrementar uma barra de tempo.

const wchar_t *PREDIO[ALTURA_PREDIO] = {
    L"                 ||||||||||||||||||||||||                 ",
    L"                 |||||||||||##|||||||||||                 ",
    L"                 |||||||||||##|||||||||||                 ",
    L"         ||||||||||||||||||||||||||||||||||||||||         ",
    L"         ||||||          ||||||||          ||||||         ",
    L"         ||  ||||      ||||    ||||      ||||  ||         ",
    L"         ||    ||||  ||||        ||||  ||||    ||         ",
    L"         ||      ||||||            ||||||      ||         ",
    L"         ||      ||||||            ||||||      ||         ",
    L"         ||    ||||  ||||        ||||  ||||    ||         ",
    L"         ||  ||||      ||||    ||||      ||||  ||         ",
    L"         ||||||          ||||||||          ||||||         ",
    L"     ||||||||||||||||||||||||||||||||||||||||||||||||     ",
    L"     ||||||              ||||||||              ||||||     ",
    L"     ||  ||||          ||||    ||||          ||||  ||     ",
    L"     ||    ||||      ||||        ||||      ||||    ||     ",
    L"     ||      ||||  ||||            ||||  ||||      ||     ",
    L"     ||        ||||||                ||||||        ||     ",
    L"     ||        ||||||                ||||||        ||     ",
    L"     ||      ||||  ||||            ||||  ||||      ||     ",
    L"     ||    ||||      ||||        ||||      ||||    ||     ",
    L"     ||  ||||          ||||    ||||          ||||  ||     ",
    L"     |||||              ||||||||||             ||||||     ",
    L"  ||||||||||||||||||||||||||||||||||||||||||||||||||||||  ",
    L"  ||        ||||||        ||||||        ||||||        ||  ",
    L"  ||        ||||||        ||||||        ||||||        ||  ",
    L"  ||        ||||||        ||||||        ||||||        ||  ",
    L"  ||        ||||||        ||||||        ||||||        ||  ",
    L"  ||||||||||||||||||||||||||||||||||||||||||||||||||||||  ",
    L"  ||||||||||||||||||||||||||||||||||||||||||||||||||||||  ",
    L"  ||        ||||||        ||||||        ||||||        ||  ",
    L"  ||        ||||||        ||||||        ||||||        ||  ",
    L"  ||        ||||||        ||||||        ||||||        ||  ",
    L"  ||        ||||||        ||||||        ||||||        ||  ",
    L"  ||||||||||||||||||||||||||||||||||||||||||||||||||||||  ",
    L"  ||||||||||||||||||||||||||||||||||||||||||||||||||||||  ",
    L"  ||        ||||||        ||||||        ||||||        ||  ",
    L"  ||        ||||||        ||||||        ||||||        ||  ",
    L"  ||        ||||||        ||||||        ||||||        ||  ",
    L"  ||        ||||||        ||||||        ||||||        ||  ",
    L"  ||||||||||||||||||||||||||||||||||||||||||||||||||||||  ",
    L"  ||||||||||||||||||||||||||||||||||||||||||||||||||||||  ",
    L"  ||        ||||||        ||||||        ||||||        ||  ",
    L"  ||        ||||||        ||||||        ||||||        ||  ",
    L"  ||        ||||||        ||||||        ||||||        ||  ",
    L"  ||        ||||||        ||||||        ||||||        ||  ",
    L"  ||||||||||||||||||||||||||||||||||||||||||||||||||||||  ",
    L"  ||||||||||||||||||||||||||||||||||||||||||||||||||||||  ",
    L"  ||        ||||||        ||||||        ||||||        ||  ",
    L"  ||        ||||||        ||||||        ||||||        ||  ",
    L"  ||        ||||||        ||||||        ||||||        ||  ",
    L"  ||        ||||||        ||||||        ||||||        ||  ",
    L"  ||||||||||||||||||||||||||||||||||||||||||||||||||||||  ",
    L"  ||||||||||||||||||||||||||||||||||||||||||||||||||||||  ",
    L"  ||        ||||||        ||||||        ||||||        ||  ",
    L"  ||        ||||||        ||||||        ||||||        ||  ",
    L"  ||        ||||||        ||||||        ||||||        ||  ",
    L"  ||        ||||||        ||||||        ||||||        ||  ",
    L"  ||||||||||||||||||||||||||||||||||||||||||||||||||||||  ",
    L"  ||       ||||       ||          ||       ||||       ||  ",
    L"  ||       ||||       ||          ||       ||||       ||  ",
    L"  ||       ||||       ||          ||       ||||       ||  ",
    L"  ||       ||||       ||          ||       ||||       ||  ",
    L"  ||||||||||||||||||||||          ||||||||||||||||||||||  ",
    L"  ||||||||||||||||||||||          ||||||||||||||||||||||  ",
};

typedef enum {
    NENHUMA,             // Nenhuma teia sendo disparada.
    CIMA,                // Teia disparada para cima.
    DIAGONAL_ESQUERDA,   // Teia disparada na diagonal superior esquerda.
    DIAGONAL_DIREITA,    // Teia disparada na diagonal superior direita.
    ANCORA_LOCAL         // Tentativa de ancorar a teia na posição atual do Homem-Aranha.
} DirecaoTeia;

DirecaoTeia direcaoAtualTeia = NENHUMA; // Armazena a direção atual do disparo da teia.
bool soltandoTeia = false;             // `true` se o jogador está segurando o botão para disparar a teia.
bool teiaAncorada = false;             // `true` se a teia está presa no prédio.
int teiaAncoradaNoPredioX = 0, teiaAncoradaNoPredioY = 0; // Coordenadas (relativas ao prédio) onde a teia está ancorada.
int tempoSegurandoEspaco = 0;          // Contador para determinar o comprimento da teia disparada.
int tamanhoTeiaDisparando = 0;            // O comprimento final da teia ao ser disparada.
int tamanhoTeiaAncorada = 0;              // Comprimento da teia quando ancorada (em segmentos).

bool quedaComum = false;      // `true` se o Homem-Aranha está em estado de queda.
bool quedaFatal = false;        // `true` se a queda é causada pelo fim do tempo (não pode ser salva).
int scrollCamera = 0;            // Posição Y (vertical) da "câmera" no prédio. Controla o scroll.
int scrollDoente = 0;            // Posição Y do Duende Verde, ajustada pelo scroll.

int tempoRestante;              // Contador para o tempo restante no nível.
int contadorTempo;            // Acumula o tempo dos tiques do jogo para decrementar `tempoRestante`.
int pontuacao;                  // Pontuação atual do jogador. 
int vidas;                      // Vidas restantes do jogador.

int maxInimigoAtual = 4;
int maxBombaAtual = 4;
int probabilidadeInimigo = 5;
int probabilidadeBomba = 2;

bool fim_som_disparo = false;
bool som_colidir_inimigo = false;
bool som_colidir_bomba = false;
bool som_colidir_doente = false;
bool musica = false;
bool game_over_musica = false;

const wchar_t *CORPO_MIRANHA[ALTURA_MIRANHA] = {
    L"|o /",
    L"/ | "
};  // Sprite do Homem-Aranha.

const wchar_t *DOENTE_VERDE[ALTURA_DOENTE] = {
    L"=- V-=",
    L"  | | ",
    L" /--\\ "
}; // Sprite do Duende Verde.

const wchar_t *INIMIGO_JANELA_SPRITE[ALTURA_INIMIGO_JANELA] = {
    L"| o |",
    L" / \\ "
}; // Sprite do inimigo da janela.

typedef struct {
    int x, y;
    WORD cor;
} MIRANHA;

typedef struct {
    int x, y;
    WORD cor;
    bool esquerda;
    bool direita;
} DOENTE;

typedef struct {
    int x, y;
    bool ativo;
    int tempoNoLugar;
} INIMIGO_JANELA;

typedef struct {
    int x, y;
    bool ativo;
    int tempoNoLugar;
} BOMBA;

MIRANHA miranha;                               // A instância única do jogador.
DOENTE doente;                                 // A instância única do Duende Verde.
INIMIGO_JANELA inimigos[MAX_INIMIGOS_JANELA];  // Um array para armazenar todos os inimigos da janela.
BOMBA bombas[MAX_BOMBAS];                      // Um array para armazenar todas as bombas.

const COORD posicoesJanelas[24] = {
    {4, 24}, {18, 24}, {32, 24}, {46, 24},
    {4, 30}, {18, 30}, {32, 30}, {46, 30},
    {4, 36}, {18, 36}, {32, 36}, {46, 36},
    {4, 42}, {18, 42}, {32, 42}, {46, 42},
    {4, 48}, {18, 48}, {32, 48}, {46, 48},
    {4, 54}, {18, 54}, {32, 54}, {46, 54},
};
const int NUM_POSICOES_JANELAS = 24;

HANDLE console;  // "Handle" ou identificador para o buffer de saída do console. Essencial para usar as funções da API.
CHAR_INFO bufferConsole[LARGURA_TELA * ALTURA_TELA];
COORD tamanhoBuffer = {LARGURA_TELA, ALTURA_TELA}; // O tamanho do nosso buffer em memória.
COORD posicaoCaractere = {0, 0}; // Posição de origem (canto superior esquerdo) para copiar o buffer.
SMALL_RECT areaEscritaConsole = {0, 0, LARGURA_TELA - 1, ALTURA_TELA - 1};

void InicializarMiranha(){
    miranha.x = 38; // Posição X inicial.
    miranha.y = MIRANHA_CENTRALIZADO_Y;  // Posição Y inicial.

    quedaComum = false;      // Garante que ele não comece caindo.
    quedaFatal = false;        // Garante que ele não comece em queda fatal.

    soltandoTeia = false;      // Não começa soltando teia.

    scrollCamera = 50;          // Posição inicial da "câmera" no prédio (começa perto do chão).

    direcaoAtualTeia = NENHUMA;// Nenhuma direção de teia no início.
    teiaAncorada = false;      // Nenhuma teia ancorada no início.
    tamanhoTeiaAncorada = 0;      // Reseta o tamanho da teia.

    tempoSegurandoEspaco = 0;  // Reseta o contador de tempo da teia.

    tamanhoTeiaDisparando = 0;    // Reseta o comprimento da teia.

    tempoRestante = TEMPO_MAXIMO; // Define o tempo inicial do nível.
    contadorTempo = 0;       // Reseta o acumulador de tempo.
}

void InicializarDoente(){
    doente.x = 38; // Posição X inicial.
    doente.y = 0;  // Posição Y inicial.

    doente.direita = true;  // Começa se movendo para a direita.
    doente.esquerda = false; // Não está se movendo para a esquerda.

    scrollDoente = 3; // Posição vertical inicial do sprite.
}

void InicializarInimigos() {
    // Loop por todos os slots de inimigos.
    for (int i = 0; i < maxInimigoAtual; i++) {
        inimigos[i].ativo = false;      // Define cada inimigo como inativo.
        inimigos[i].tempoNoLugar = 0;   // Reseta seu temporizador.
    }
}

void InicializarBombas() {
    // Loop por todos os slots de bombas.
    for (int i = 0; i < maxBombaAtual; i++) {
        bombas[i].ativo = false;        // Define cada bomba como inativa.
        bombas[i].tempoNoLugar = 0;     // Reseta seu temporizador.
    }
}

void InicializarNivel() {
    InicializarMiranha(); // Prepara o Homem-Aranha.
    InicializarDoente();  // Prepara o Duende Verde.
    InicializarInimigos();// Prepara os inimigos.
    InicializarBombas();  // Prepara as bombas.
}

void InicializarJogo() {
    pontuacao = 0; // Reseta a pontuação geral.
    vidas = 3;     // Define as vidas iniciais.
    InicializarNivel(); // Chama a função para configurar o primeiro nível.
}

void LimparBuffer(){
    for(int i = 0; i < ALTURA_TELA * LARGURA_TELA; i++){
        bufferConsole[i].Char.UnicodeChar = ESPACO_VAZIO;
        bufferConsole[i].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    }
}

void DesenharBuffer(){
    WriteConsoleOutputW(console, bufferConsole, tamanhoBuffer, posicaoCaractere, &areaEscritaConsole);
}

void GameOver(){
    game_over_musica = true;
    quedaComum = false;
    quedaFatal = false;
    
    
    char gameOver[10];
    char pontosTotais[50];
    sprintf(gameOver, "GAME OVER");
    sprintf(pontosTotais, "SUA PONTUACAO FINAL FOI: %d", pontuacao);

    int indiceGameOver = (ALTURA_TELA/2)*LARGURA_TELA + (LARGURA_TELA - strlen(gameOver))/2;
    int indicePontosTotais = (ALTURA_TELA/2 + 1)*LARGURA_TELA + (LARGURA_TELA - strlen(pontosTotais))/2;

    LimparBuffer();

    for(int i = 0; i < strlen(gameOver); i++){
        bufferConsole[indiceGameOver + i].Char.UnicodeChar = gameOver[i];
        bufferConsole[indiceGameOver + i].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    }

    for(int i = 0; i < strlen(pontosTotais); i++){
        bufferConsole[indicePontosTotais + i].Char.UnicodeChar = pontosTotais[i];
        bufferConsole[indicePontosTotais + i].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    }

    DesenharBuffer();

    Sleep(5000);
}

void PerderVida() {
    vidas--; // Decrementa o contador de vidas.
    if (vidas <= 0) { // Se as vidas acabaram...
        GameOver();
        pontuacao = 0; // Reseta a pontuação.
        vidas = 3;     // Reseta as vidas para o valor inicial.
    }
    InicializarNivel(); // Reinicia o nível atual.
}

void VencerNivel() {
    //pauda pra tocar a musica
    Sleep(3000);
    
    pontuacao *= 2;     // Dobra a pontuação como bônus.

    if(maxInimigoAtual + 2 <= MAX_INIMIGOS_JANELA){
        maxInimigoAtual = maxInimigoAtual + 2;
    }

    if(maxBombaAtual + 2 <= MAX_BOMBAS){
        maxBombaAtual = maxBombaAtual + 2;
    }

    if(probabilidadeInimigo < 100){
        probabilidadeInimigo = probabilidadeInimigo + 5;
    }

    if(probabilidadeBomba < 100){
        probabilidadeBomba = probabilidadeBomba + 2;
    }

    InicializarNivel(); // Prepara e inicia o próximo nível (que é o mesmo, mas com pontuação maior).
}

bool mostrarMenu = true;
int selecionado = 1;
void MudarSelecionado(){
    if(selecionado == 1){
        selecionado = 2;
    } else{
        selecionado = 1;
    }
    
}

void MenuComoJogar(){
    char textoComoJogar[61];
    char texto2[73];
    sprintf(textoComoJogar, "Segure Espaco + Direcao das Setinhas para disparar sua teia!");
    sprintf(texto2, "Quanto mais tempo voce segurar a barra de espaco, mais longe a teia vai!");

    int indiceComoJogar = ALTURA_TELA/2 * LARGURA_TELA + (LARGURA_TELA - strlen(textoComoJogar))/2;
    int indiceTexto2 = (ALTURA_TELA/2 + 1) * LARGURA_TELA + (LARGURA_TELA - strlen(texto2))/2;

    LimparBuffer();

    for(int i = 0; i < strlen(textoComoJogar); i++){
        bufferConsole[indiceComoJogar + i].Char.UnicodeChar = textoComoJogar[i];
        bufferConsole[indiceComoJogar + i].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    }

    for(int i = 0; i < strlen(texto2); i++){
        bufferConsole[indiceTexto2 + i].Char.UnicodeChar = texto2[i];
        bufferConsole[indiceTexto2+ i].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    }

    DesenharBuffer();

    Sleep(5000);

    LimparBuffer();

    DesenharBuffer();
}

void SelecionarMenu(){
    if(selecionado == 1){
        mostrarMenu = false;
    } else if(selecionado == 2){
        MenuComoJogar();
    }
}

void MenuInicial(){
    char jogar[6];
    sprintf(jogar, "JOGAR");

    int indiceJogar = ALTURA_TELA/2 * LARGURA_TELA + (LARGURA_TELA - strlen(jogar)) / 2;

    for (int i = 0; i < strlen(jogar); i++) {
        bufferConsole[indiceJogar + i].Char.UnicodeChar = jogar[i];
        if(selecionado == 1){
            bufferConsole[indiceJogar + i].Attributes = BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN| FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        } else {
            bufferConsole[indiceJogar + i].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        }
    }

    char comoJogar[11];
    sprintf(comoJogar, "COMO JOGAR");

    int indiceComoJogar = (ALTURA_TELA/2 + 1) * LARGURA_TELA + (LARGURA_TELA - strlen(comoJogar)) / 2;

    for(int i = 0; i < strlen(comoJogar); i++){
        bufferConsole[indiceComoJogar + i].Char.UnicodeChar = comoJogar[i];
        if(selecionado == 2){
            bufferConsole[indiceComoJogar + i].Attributes = BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        } else {
            bufferConsole[indiceComoJogar + i].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY; 
        }
    }
    


    if (GetAsyncKeyState(VK_UP) & 0x8000) {
        MudarSelecionado();
    } else if (GetAsyncKeyState(VK_DOWN) & 0x8000){
        MudarSelecionado();
    } else if(GetAsyncKeyState(VK_RETURN) & 0x8000) {
        SelecionarMenu();
    }

    DesenharBuffer();
}

void DesenharPredio(){
    for(int i = scrollCamera; i < ALTURA_JOGO_PRINCIPAL + scrollCamera; i++){
        for(int j = 0; j < LARGURA_PREDIO; j++){
            int indice = (i - scrollCamera + ALTURA_UI_SUPERIOR) * LARGURA_TELA + (((LARGURA_TELA - LARGURA_PREDIO) / 2) + j);
            if (i >= 0 && i < ALTURA_PREDIO) {
                bufferConsole[indice].Char.UnicodeChar = PREDIO[i][j];
                if(PREDIO[i][j] == '#'){
                    bufferConsole[indice].Attributes = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
                }
                else{
                    bufferConsole[indice].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
                }
            }
        }
    }
}

void DesenharMiranha(){
    if(quedaComum == false){
        for(int i = 0; i < ALTURA_MIRANHA; ++i){
            for(int j = 0; j < LARGURA_MIRANHA; ++j){
                if (CORPO_MIRANHA[i][j] != ' ' || (i == 0 && j == 2) || (i == 1 && j == 1)) {
                    int indice = (miranha.y + i + ALTURA_UI_SUPERIOR) * LARGURA_TELA + (miranha.x + j);
                    if ( (miranha.y + i) < ALTURA_JOGO_PRINCIPAL) {
                        bufferConsole[indice].Char.UnicodeChar = CORPO_MIRANHA[i][j];

                        if(CORPO_MIRANHA[i][j] == 'o'){
                            bufferConsole[indice].Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
                        }
                        else{
                            bufferConsole[indice].Attributes = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
                        }
                    }
                }
            }
        }
    }
    else{ // Se o Homem-Aranha está caindo...
        for(int i = 0; i < LARGURA_MIRANHA; i++){
            int indice = (miranha.y + ALTURA_UI_SUPERIOR) * LARGURA_TELA + miranha.x;
             if (miranha.y < ALTURA_JOGO_PRINCIPAL) {
                bufferConsole[indice + i].Char.UnicodeChar = MIRANHA_CAINDO[i];
                
                if(MIRANHA_CAINDO[i] == 'o' || MIRANHA_CAINDO[i] == '_'){
                    bufferConsole[indice + i].Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
                }
                else{
                    bufferConsole[indice + i].Attributes = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
                }
             }
        }
    }
}

void DesenharDoente(){
    for(int i = 0 + scrollDoente; i < ALTURA_DOENTE; ++i){
        for(int j = 0; j < LARGURA_DOENTE; ++j){
            if (DOENTE_VERDE[i][j] != ' ' || (i == 1 && j == 3) || (i == 0 && j == 2)) {
                int indice = (doente.y + (i - scrollDoente) + ALTURA_UI_SUPERIOR) * LARGURA_TELA + (doente.x + j);
                if ( (doente.y + (i- scrollDoente)) < ALTURA_JOGO_PRINCIPAL){
                    bufferConsole[indice].Char.UnicodeChar = DOENTE_VERDE[i][j];

                    if(i == 0){
                        bufferConsole[indice].Attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
                    }
                    else if(i == 1){

                        bufferConsole[indice].Attributes = FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY;
                    }
                    else{
                        bufferConsole[indice].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
                    }
                }
            }
        }
    }
}

void DesenharInimigos() {
    for (int i = 0; i < maxInimigoAtual; i++) {
        if (inimigos[i].ativo) {
            int inimigoTelaY = inimigos[i].y - scrollCamera;
            int inimigoTelaX = inimigos[i].x + ((LARGURA_TELA - LARGURA_PREDIO) / 2);

            for (int linha = 0; linha < ALTURA_INIMIGO_JANELA; linha++) {
                for (int coluna = 0; coluna < LARGURA_INIMIGO_JANELA; coluna++) {
                    if (INIMIGO_JANELA_SPRITE[linha][coluna] != ' ') {
                        int finalY = inimigoTelaY + linha + ALTURA_UI_SUPERIOR;
                        int finalX = inimigoTelaX + coluna;
                        if (finalY >= ALTURA_UI_SUPERIOR && finalY < ALTURA_JOGO_PRINCIPAL + ALTURA_UI_SUPERIOR && finalX >= 0 && finalX < LARGURA_TELA) {
                            int indice = finalY * LARGURA_TELA + finalX;
                            bufferConsole[indice].Char.UnicodeChar = INIMIGO_JANELA_SPRITE[linha][coluna];

                            if(INIMIGO_JANELA_SPRITE[linha][coluna] == 'o'){
                                bufferConsole[indice].Attributes = FOREGROUND_RED | FOREGROUND_BLUE;
                            }
                            else{
                                bufferConsole[indice].Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
                            }
                        }
                    }
                }
            }
        }
    }
}

void DesenharBombas() {
    for (int i = 0; i < maxBombaAtual; i++) {
        if (bombas[i].ativo) {
            int bombaTelaY = bombas[i].y - scrollCamera + ALTURA_UI_SUPERIOR;
            int bombaTelaX = bombas[i].x + ((LARGURA_TELA - LARGURA_PREDIO) / 2);
            if (bombaTelaY >= ALTURA_UI_SUPERIOR && bombaTelaY < ALTURA_JOGO_PRINCIPAL + ALTURA_UI_SUPERIOR && bombaTelaX >= 0 && bombaTelaX < LARGURA_TELA) {
                int indice = bombaTelaY * LARGURA_TELA + bombaTelaX;
                bufferConsole[indice].Char.UnicodeChar = '@';
                bufferConsole[indice].Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
            }
        }
    }
}

void DesenharTeia() {
    if (soltandoTeia && direcaoAtualTeia != ANCORA_LOCAL) {
        switch (direcaoAtualTeia) {
            case CIMA:
                for (int i = 1; i <= tamanhoTeiaDisparando; i++){
                    if (miranha.y - i >= 0){
                        int indice = (miranha.y - i + ALTURA_UI_SUPERIOR) * LARGURA_TELA + (miranha.x + INICIO_TEIA_X);
                        bufferConsole[indice].Char.UnicodeChar = '0';
                        bufferConsole[indice].Attributes = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
                    }
                }
                break;
            case DIAGONAL_ESQUERDA:
                for (int i = 1; i <= tamanhoTeiaDisparando; i++){
                    if (miranha.y - i >= 0 && miranha.x + 1 - i >= 0){
                        int indice = (miranha.y - i + ALTURA_UI_SUPERIOR) * LARGURA_TELA + (miranha.x + 1 - i);
                        bufferConsole[indice].Char.UnicodeChar = '0';
                        bufferConsole[indice].Attributes = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
                    }
                }
                break;
            case DIAGONAL_DIREITA:
                for (int i = 1; i <= tamanhoTeiaDisparando; i++){
                    if (miranha.y - i >= 0 && miranha.x + INICIO_TEIA_X + i < LARGURA_TELA){
                        int indice = (miranha.y - i + ALTURA_UI_SUPERIOR) * LARGURA_TELA + (miranha.x + INICIO_TEIA_X + i);
                        bufferConsole[indice].Char.UnicodeChar = '0';
                        bufferConsole[indice].Attributes = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
                    }
                }
                break;
            default: break; // Caso padrão, não faz nada.
        }
    }
    else if (teiaAncorada) {
        int ancoraTelaY = teiaAncoradaNoPredioY - scrollCamera;
        int ancoraTelaX = teiaAncoradaNoPredioX + ((LARGURA_TELA - LARGURA_PREDIO) / 2);

        int x0 = miranha.x + INICIO_TEIA_X;
        int y0 = miranha.y;
        int x1 = ancoraTelaX;
        int y1 = ancoraTelaY;

        int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
        int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
        int err = dx + dy, e2;

        while (true) {
            if (y0 >= 0 && y0 < ALTURA_JOGO_PRINCIPAL && x0 >= 0 && x0 < LARGURA_TELA) {
                int indice = (y0 + ALTURA_UI_SUPERIOR) * LARGURA_TELA + x0;
                bufferConsole[indice].Char.UnicodeChar = '.';
                bufferConsole[indice].Attributes = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            }
                if (x0 == x1 && y0 == y1){
                    break; // Sai do loop se chegou ao ponto final.
                }
            e2 = 2 * err;

            if (e2 >= dy){ 
                err += dy; 
                x0 += sx; 
            }

            if (e2 <= dx) { 
                err += dx; 
                y0 += sy; 
            }
        }
    }
}

void DesenharUI() {
    char textoPontos[30]; // Cria uma string para formatar o texto.
    sprintf(textoPontos, "PONTOS: %d", pontuacao); // Formata a string com a pontuação atual.
    int indiceTextoPontos = (LARGURA_TELA - strlen(textoPontos)) / 2; // Calcula a posição inicial para centralizar o texto.
    // Loop para escrever o texto no buffer, na linha superior.
    for (int i = 0; i < strlen(textoPontos); i++) {
        bufferConsole[i + indiceTextoPontos].Char.UnicodeChar = textoPontos[i];
    }

    int linhaInferiorY = ALTURA_JOGO_PRINCIPAL + ALTURA_UI_SUPERIOR; // Define a linha Y para a UI inferior.

    char textoVidas[15];
    sprintf(textoVidas, "VIDAS: %d", vidas); // Formata a string de vidas.
    for (int i = 0; i < strlen(textoVidas); i++) {
        int indice = linhaInferiorY * LARGURA_TELA + i;
        bufferConsole[indice].Char.UnicodeChar = textoVidas[i];
    }

    for (int i = 0; i < tempoRestante; i++) {
        int x = LARGURA_TELA - TEMPO_MAXIMO - 1 + i; // Calcula a posição X no canto inferior direito.
        int indiceTextoVidas = linhaInferiorY * LARGURA_TELA + x;
        bufferConsole[indiceTextoVidas].Char.UnicodeChar = '#'; // Caractere da barra de tempo.
        bufferConsole[indiceTextoVidas].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY; // Cor da barra.
    }
}

void DesenharTela(){
    LimparBuffer();
    
    DesenharUI();       // UI fica no fundo para não sobrepor o jogo.
    DesenharPredio();   // O prédio é o fundo principal.
    DesenharBombas();   // Bombas sobre o prédio.
    DesenharInimigos(); // Inimigos sobre o prédio.

    if (soltandoTeia || teiaAncorada) {
        DesenharTeia();
    }
    DesenharDoente();   // O Duende Verde.
    DesenharMiranha();  // O Homem-Aranha é a camada mais à frente.
 
    DesenharBuffer();
}

void ControlarTeia() {
    if (!soltandoTeia && !quedaFatal) {
        teiaAncorada = false; // Garante que qualquer teia anterior seja solta.
        direcaoAtualTeia = NENHUMA; // Reseta a direção.

        if (GetAsyncKeyState(VK_UP) & 0x8000){
            direcaoAtualTeia = CIMA;
        }
        else if (GetAsyncKeyState(VK_LEFT) & 0x8000){
            direcaoAtualTeia = DIAGONAL_ESQUERDA;   
        }
        else if (GetAsyncKeyState(VK_RIGHT) & 0x8000){
            direcaoAtualTeia = DIAGONAL_DIREITA;
        }
        else if (GetAsyncKeyState(VK_DOWN) & 0x8000){
            direcaoAtualTeia = ANCORA_LOCAL;
        }

        if (direcaoAtualTeia != NENHUMA) {
            soltandoTeia = true;        // Ativa o estado de "disparando teia".
            tempoSegurandoEspaco = 0;   // Inicia o contador de tempo.
            tamanhoTeiaDisparando = 1;     // A teia começa com comprimento 1.
        }
    }
    else { // Se o jogador continua segurando espaço...
        if (direcaoAtualTeia != ANCORA_LOCAL) {
            tempoSegurandoEspaco += ATRASO_TIQUE; // Incrementa o tempo.
            if (tempoSegurandoEspaco > 150 && tamanhoTeiaDisparando == 1) {
                tamanhoTeiaDisparando = 2;
            } else if (tempoSegurandoEspaco > 300 && tamanhoTeiaDisparando == 2) {
                tamanhoTeiaDisparando = 3;
            } else if (tempoSegurandoEspaco > 450 && tamanhoTeiaDisparando == 3) {
                tamanhoTeiaDisparando = 4;
            } else if (tempoSegurandoEspaco > 600 && tamanhoTeiaDisparando == 4) {
                tamanhoTeiaDisparando = 5;
            }
        }
    }
}

void ControlarTeia2(){
    soltandoTeia = false; // Desativa o estado de "disparando".

    int teia_x_final_tela = -1, teia_y_final_tela = -1;
    bool colidiu = false;

    for (int i = 1; i <= tamanhoTeiaDisparando; i++) {
        int teia_x_tela = -1, teia_y_tela = -1;
        switch (direcaoAtualTeia) {
            case CIMA: teia_y_tela = miranha.y - i; teia_x_tela = miranha.x + INICIO_TEIA_X; break;
            case DIAGONAL_ESQUERDA: teia_y_tela = miranha.y - i; teia_x_tela = miranha.x + 1 - i; break;
            case DIAGONAL_DIREITA: teia_y_tela = miranha.y - i; teia_x_tela = miranha.x + INICIO_TEIA_X + i; break;
            default: break;
        }

        if (i == tamanhoTeiaDisparando) {
            teia_x_final_tela = teia_x_tela;
            teia_y_final_tela = teia_y_tela;
        }

        if(teia_x_tela != -1) {
            for (int j = 0; j < maxInimigoAtual; j++) {
                if(inimigos[j].ativo) {
                    int inimigoTelaY = inimigos[j].y - scrollCamera, inimigoTelaX = inimigos[j].x + ((LARGURA_TELA - LARGURA_PREDIO) / 2);
                    if(teia_x_tela >= inimigoTelaX && teia_x_tela < inimigoTelaX + LARGURA_INIMIGO_JANELA && teia_y_tela >= inimigoTelaY && teia_y_tela < inimigoTelaY + ALTURA_INIMIGO_JANELA) {
                        colidiu = true;
                        inimigos[j].ativo = false;
                        break;
                    }
                }
            }
            if(colidiu) break; // Sai do loop se já colidiu.
            for (int j = 0; j < maxBombaAtual; j++) {
                if(bombas[j].ativo) {
                    int bombaTelaY = bombas[j].y - scrollCamera, bombaTelaX = bombas[j].x + ((LARGURA_TELA - LARGURA_PREDIO) / 2);
                    if(teia_x_tela == bombaTelaX && teia_y_tela == bombaTelaY) {
                        colidiu = true;
                        bombas[j].ativo = false;
                        break;
                    }
                }
            }
            if(colidiu) break; // Sai do loop se já colidiu.
        }
    }

    if (colidiu) {
        quedaComum = true; // Se a teia acertou um inimigo, o Homem-Aranha perde o balanço e cai.
    } else if (direcaoAtualTeia == ANCORA_LOCAL) {
        int teiaTelaY = miranha.y;
        int teiaTelaX = miranha.x + INICIO_TEIA_X;
        int predioY = teiaTelaY + scrollCamera;
        int predioX = teiaTelaX - ((LARGURA_TELA - LARGURA_PREDIO) / 2);

        if (predioY >= 0 && predioY < ALTURA_PREDIO && predioX >= 0 && predioX < LARGURA_PREDIO && PREDIO[predioY][predioX] == '|') {
            teiaAncorada = true;
            teiaAncoradaNoPredioX = predioX;
            teiaAncoradaNoPredioY = predioY;
            tamanhoTeiaAncorada = 0; // A teia começa totalmente recolhida.
            if (quedaComum) { // Se estava caindo, se salva.
                scrollCamera = scrollCamera + (miranha.y - MIRANHA_CENTRALIZADO_Y);
                if (scrollCamera > (ALTURA_PREDIO - ALTURA_JOGO_PRINCIPAL)) {
                    scrollCamera = ALTURA_PREDIO - ALTURA_JOGO_PRINCIPAL;
                }
                if (scrollCamera < 0) {
                    scrollCamera = 0;
                }
                miranha.y = MIRANHA_CENTRALIZADO_Y;
                quedaComum = false;
            }
        } else { // Se não conseguiu ancorar...
            if (!quedaComum) {
                quedaComum = true; // Começa a cair.
            }
        }
    } else { // Lógica para ancorar a teia na ponta.
        int predioY = teia_y_final_tela + scrollCamera;
        int predioX = teia_x_final_tela - ((LARGURA_TELA - LARGURA_PREDIO) / 2);

        if (direcaoAtualTeia != NENHUMA && predioY >= 0 && predioY < ALTURA_PREDIO && predioX >= 0 && predioX < LARGURA_PREDIO && PREDIO[predioY][predioX] == '|') {
            teiaAncorada = true;
            teiaAncoradaNoPredioX = predioX;
            teiaAncoradaNoPredioY = predioY;
            tamanhoTeiaAncorada = tamanhoTeiaDisparando; // O tamanho inicial é o comprimento do disparo.
            if (quedaComum) { // Se estava caindo, se salva.
                scrollCamera = scrollCamera + (miranha.y - MIRANHA_CENTRALIZADO_Y);
                if (scrollCamera > (ALTURA_PREDIO - ALTURA_JOGO_PRINCIPAL)) scrollCamera = ALTURA_PREDIO - ALTURA_JOGO_PRINCIPAL;
                if (scrollCamera < 0) scrollCamera = 0;
                miranha.y = MIRANHA_CENTRALIZADO_Y;
                quedaComum = false;
            }
        } else { // Se errou a parede...
            if (!quedaComum) {
                quedaComum = true; // Começa a cair.
            }
        }
    }
    direcaoAtualTeia = NENHUMA;
    tempoSegurandoEspaco = 0;
    tamanhoTeiaDisparando = 0;

}

void EscolherControleTeia() {
    if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
        ControlarTeia();
    } else if (soltandoTeia) {
        ControlarTeia2();
    }
}

void MovimentarMiranha() {  
    if (teiaAncorada) {
        int ancoraTelaY = teiaAncoradaNoPredioY - scrollCamera;
        int ancoraTelaX = teiaAncoradaNoPredioX + ((LARGURA_TELA - LARGURA_PREDIO) / 2);

        if ((GetAsyncKeyState(VK_UP) & 0x8000) && tamanhoTeiaAncorada > 0) {
            if (miranha.y > MIRANHA_CENTRALIZADO_Y) {
                miranha.y--;
            }
            else if (scrollCamera > 0) {
                scrollCamera--;
            }
            else {
                miranha.y--;
            }


            if (miranha.x + INICIO_TEIA_X < ancoraTelaX) {
                miranha.x++;
            }
            if (miranha.x + INICIO_TEIA_X > ancoraTelaX) {
                miranha.x--;
            }

            tamanhoTeiaAncorada--; // Diminui o comprimento da teia.
            if (tamanhoTeiaAncorada == 0) {
                teiaAncorada = false; // Solta a teia se ela for totalmente recolhida.
            }
        }
        else if ((GetAsyncKeyState(VK_DOWN) & 0x8000) && tamanhoTeiaAncorada < 5 && miranha.y <= MIRANHA_CENTRALIZADO_Y) {
            if (miranha.y < MIRANHA_CENTRALIZADO_Y) {
                miranha.y++;
            } else if ((scrollCamera + ALTURA_JOGO_PRINCIPAL) < ALTURA_PREDIO) {
                scrollCamera++;
            } else if (miranha.y < ALTURA_JOGO_PRINCIPAL - ALTURA_MIRANHA) {
                miranha.y++;
            }

            if (miranha.x > 0 && miranha.x + INICIO_TEIA_X > ancoraTelaX) {
                miranha.x--;
            }
            if (miranha.x < LARGURA_TELA - LARGURA_MIRANHA && miranha.x + INICIO_TEIA_X < ancoraTelaX) {
                miranha.x++;
            }

            tamanhoTeiaAncorada++; // Aumenta o comprimento da teia.
        }
    }
}

void MovimentarDoente(){
    if(scrollCamera == 0){ doente.y = 4; scrollDoente = 0; }
    else if(scrollCamera == 1){ doente.y = 3; scrollDoente = 0; }
    else if(scrollCamera == 2){ doente.y = 2; scrollDoente = 0; }
    else if(scrollCamera == 3){ doente.y = 1; scrollDoente = 0; }
    else if(scrollCamera == 4){ doente.y = 0; scrollDoente = 0; }
    else if(scrollCamera == 5){ scrollDoente = 1; doente.y = 0; }
    else if(scrollCamera == 6){ scrollDoente = 2; doente.y = 0; }
    else if(scrollCamera == 7){ scrollDoente = 3; doente.y = 0; }

    if(doente.x < 65 && doente.direita == true){
        doente.x++;
        doente.esquerda = false;
        doente.direita = true;
    }
    else{ // Inverte a direção quando atinge o limite direito.
        doente.direita = false;
        doente.esquerda = true;
    }

    if(doente.x > 10 && doente.esquerda == true){
        doente.x--;
        doente.esquerda = true;
        doente.direita = false;
    }
    else{ // Inverte a direção quando atinge o limite esquerdo.
        doente.esquerda = false;
        doente.direita = true;
    }
}

void SpawnInimigos() {
    if (rand() % 100 >= probabilidadeInimigo) {
        return;
    }

    int inimigosAtivos = 0;
    for (int i = 0; i < maxInimigoAtual; i++) {
        if (inimigos[i].ativo) {
            inimigosAtivos++;
        }
    }

    if (inimigosAtivos < maxInimigoAtual) {
        int slotVazio = -1;
        for (int i = 0; i < maxInimigoAtual; i++) {
            if (!inimigos[i].ativo) {
                slotVazio = i;
                break;
            }
        }

        if (slotVazio != -1) {
            COORD janelasDisponiveis[NUM_POSICOES_JANELAS];
            int countDisponiveis = 0;

            for (int i = 0; i < NUM_POSICOES_JANELAS; i++) {
                bool naTela = posicoesJanelas[i].Y >= scrollCamera && posicoesJanelas[i].Y < scrollCamera + ALTURA_JOGO_PRINCIPAL;
                if (naTela) {
                    bool posicaoOcupada = false;
                    for (int j = 0; j < maxInimigoAtual; j++) {
                        if (inimigos[j].ativo && inimigos[j].x == posicoesJanelas[i].X + 1 && inimigos[j].y == posicoesJanelas[i].Y + 2) {
                            posicaoOcupada = true;
                            break;
                        }
                    }
                    if (!posicaoOcupada) {
                        janelasDisponiveis[countDisponiveis] = posicoesJanelas[i];
                        countDisponiveis++;
                    }
                }
            }

            if (countDisponiveis > 0) {
                int posIndex = rand() % countDisponiveis;
                inimigos[slotVazio].ativo = true;
                inimigos[slotVazio].x = janelasDisponiveis[posIndex].X + 1;
                inimigos[slotVazio].y = janelasDisponiveis[posIndex].Y + 2;
                inimigos[slotVazio].tempoNoLugar = 0;
            }
        }
    }
}

void SpawnBombas() {
    if (rand() % 100 >= probabilidadeBomba) {
        return;
    }
    int bombasAtivas = 0;
    for(int i = 0; i < maxBombaAtual; i++) {
        if(bombas[i].ativo) {
            bombasAtivas++;
        }
    }

    if(bombasAtivas < maxBombaAtual) {
        int slotVazio = -1;
        for(int i = 0; i < maxBombaAtual; i++) {
            if(!bombas[i].ativo) {
                slotVazio = i;
                break;
            }
        }

        if(slotVazio != -1) {
            COORD bombasDisponiveis[LARGURA_PREDIO * ALTURA_PREDIO];
            int countDisponiveis = 0;

            for (int y = 4; y <= 22; y++) {

                for (int x = 0; x < LARGURA_PREDIO; x++) {
                    if (PREDIO[y][x] == '|') {
                        bool naTela = y >= scrollCamera && y < scrollCamera + ALTURA_JOGO_PRINCIPAL;
                        if (naTela) {
                            bool ocupada = false; // Verifica se já tem outra bomba.
                            for(int j = 0; j < maxBombaAtual; j++) {
                                if(bombas[j].ativo && bombas[j].x == x && bombas[j].y == y) {
                                    ocupada = true;
                                    break;
                                }
                            }
                            bool ocupadaPeloMiranha = false; // Verifica se o Homem-Aranha está no local.
                            int bombaTelaX = x + ((LARGURA_TELA - LARGURA_PREDIO) / 2);
                            int bombaTelaY = y - scrollCamera;
                            if(bombaTelaX >= miranha.x && bombaTelaX < miranha.x + LARGURA_MIRANHA &&
                               bombaTelaY >= miranha.y && bombaTelaY < miranha.y + ALTURA_MIRANHA) {
                                ocupadaPeloMiranha = true;
                            }

                            if(!ocupada && !ocupadaPeloMiranha) { // Se o local está livre...
                                bombasDisponiveis[countDisponiveis].X = x;
                                bombasDisponiveis[countDisponiveis].Y = y;
                                countDisponiveis++;
                            }
                        }
                    }
                }
            }

            if(countDisponiveis > 0) { // Se achou um lugar...
                int posIndex = rand() % countDisponiveis;
                bombas[slotVazio].ativo = true;
                bombas[slotVazio].x = bombasDisponiveis[posIndex].X;
                bombas[slotVazio].y = bombasDisponiveis[posIndex].Y;
                bombas[slotVazio].tempoNoLugar = 0;
            }
        }
    }
}

void AtualizarInimigos() {
    for (int i = 0; i < maxInimigoAtual; i++) {
        if (inimigos[i].ativo) {
            inimigos[i].tempoNoLugar += ATRASO_TIQUE; // Incrementa o temporizador.
            if (inimigos[i].tempoNoLugar >= 4000) {
                int tentativas = 0;
                while (tentativas < NUM_POSICOES_JANELAS) {
                    int posIndex = rand() % NUM_POSICOES_JANELAS;
                    bool posicaoOcupada = false;
                    for (int j = 0; j < maxInimigoAtual; j++) {
                        if (i != j && inimigos[j].ativo && inimigos[j].x == posicoesJanelas[posIndex].X + 1 && inimigos[j].y == posicoesJanelas[posIndex].Y + 2) {
                            posicaoOcupada = true;
                            break;
                        }
                    }
                    if (!posicaoOcupada) { // Se encontrou uma vazia...
                        inimigos[i].x = posicoesJanelas[posIndex].X + 1;
                        inimigos[i].y = posicoesJanelas[posIndex].Y + 2;
                        inimigos[i].tempoNoLugar = 0; // Reseta o temporizador.
                        break;
                    }
                    tentativas++;
                }
            }
        }
    }
}

void AtualizarBombas() {
    for (int i = 0; i < maxBombaAtual; i++) {
        if (bombas[i].ativo) {
            bombas[i].tempoNoLugar += ATRASO_TIQUE; // Incrementa o temporizador.
            if (bombas[i].tempoNoLugar >= 4000) {
                bombas[i].ativo = false; // Ela desaparece (é desarmada).
            }
        }
    }
}

void VerificarQuedaMiranha(){
    if(miranha.x <= (((LARGURA_TELA - LARGURA_PREDIO) / 2) - 1) || miranha.x >= (LARGURA_PREDIO + ((LARGURA_TELA - LARGURA_PREDIO) / 2)) - LARGURA_MIRANHA){
        quedaComum = true;
    }
    if (!teiaAncorada && !soltandoTeia && !quedaComum) {
        bool tocandoPredio = false;
        for (int i = 0; i < ALTURA_MIRANHA; i++) {
            for (int j = 0; j < LARGURA_MIRANHA; j++) {
                if (CORPO_MIRANHA[i][j] == ' ') { continue; } // Pula os espaços vazios do sprite.
                int predioY = (miranha.y + i) + scrollCamera;
                int predioX = (miranha.x + j) - ((LARGURA_TELA - LARGURA_PREDIO) / 2);

                if (predioY >= 0 && predioY < ALTURA_PREDIO && predioX >= 0 && predioX < LARGURA_PREDIO) {
                    if (PREDIO[predioY][predioX] == '|') {
                        tocandoPredio = true;
                        break;
                    }
                }
            }
            if (tocandoPredio) { break; }
        }

        if (!tocandoPredio) { // Se não está tocando...
            quedaComum = true; // Cai.
        }
    }
    if (quedaComum) {
        teiaAncorada = false; // Solta qualquer teia.
        if((ALTURA_JOGO_PRINCIPAL + scrollCamera) < 65){ // Se não chegou no chão...
            scrollCamera++; // A câmera "sobe" (o cenário "desce"), simulando a queda.
        }
    }
}

void VerificarMiranhaCaiuChao(){
    if(quedaComum == true && (ALTURA_JOGO_PRINCIPAL + scrollCamera) == 65){
        miranha.y++; // O Homem-Aranha continua caindo para fora da tela.
        if(miranha.y >= ALTURA_JOGO_PRINCIPAL - ALTURA_MIRANHA){
            PerderVida(); // Perde uma vida ao sair da tela por baixo.
        }
    }
}

void CorrigirPosicaoMiranha(){
    if(miranha.x <= 0) {
        miranha.x = 1; // Limite esquerdo.
    } else if(miranha.x >= LARGURA_TELA - LARGURA_MIRANHA) {
        miranha.x = LARGURA_TELA - LARGURA_MIRANHA - 1; // Limite direito.
    }

    if(miranha.y <= 0) {
        miranha.y = 1; // Limite superior.
    }
}

void VerificarColisoes() {
    if (quedaComum || quedaFatal) {
        return;
    } // Não verifica colisões se estiver em queda comum ou em queda fatal.

    int doente_x1 = doente.x, doente_y1 = doente.y, doente_x2 = doente.x + LARGURA_DOENTE, doente_y2 = doente.y + ALTURA_DOENTE;
    int miranha_x1 = miranha.x, miranha_y1 = miranha.y, miranha_x2 = miranha.x + LARGURA_MIRANHA, miranha_y2 = miranha.y + ALTURA_MIRANHA;

    if (miranha_x1 < doente_x2 && miranha_x2 > doente_x1 && miranha_y1 < doente_y2 && miranha_y2 > doente_y1) {
        quedaComum = true;
        return;
    }

    for (int i = 0; i < maxInimigoAtual; i++) {
        if (inimigos[i].ativo) {
            int inimigoTelaY = inimigos[i].y - scrollCamera, inimigoTelaX = inimigos[i].x + ((LARGURA_TELA - LARGURA_PREDIO) / 2);
            int inimigo_x1 = inimigoTelaX, inimigo_y1 = inimigoTelaY, inimigo_x2 = inimigoTelaX + LARGURA_INIMIGO_JANELA, inimigo_y2 = inimigoTelaY + ALTURA_INIMIGO_JANELA;
            if (miranha_x1 < inimigo_x2 && miranha_x2 > inimigo_x1 && miranha_y1 < inimigo_y2 && miranha_y2 > inimigo_y1) {
                pontuacao += 30; // Ganha pontos.
                som_colidir_inimigo = true;
                inimigos[i].ativo = false;
                return; // Inimigo derrotado.
            }
        }
    }

    for(int i = 0; i < maxBombaAtual; i++) {
        if(bombas[i].ativo) {
            int bombaTelaY = bombas[i].y - scrollCamera;
            int bombaTelaX = bombas[i].x + ((LARGURA_TELA - LARGURA_PREDIO) / 2);
            if(miranha_x1 < bombaTelaX + 1 && miranha_x2 > bombaTelaX && miranha_y1 < bombaTelaY + 1 && miranha_y2 > bombaTelaY) {
                pontuacao += 80; // Ganha pontos.
                som_colidir_bomba = true;
                bombas[i].ativo = false;
                return; // Bomba desarmada.
            }
        }
    }

    if (teiaAncorada) {
        int ancoraTelaY = teiaAncoradaNoPredioY - scrollCamera, ancoraTelaX = teiaAncoradaNoPredioX + ((LARGURA_TELA - LARGURA_PREDIO) / 2);
        int x0 = miranha.x + INICIO_TEIA_X, y0 = miranha.y;
        int x1 = ancoraTelaX, y1 = ancoraTelaY;
        int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
        int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
        int err = dx + dy, e2;
        while (true) {
            if (scrollCamera <= 4) {
                if (x0 >= doente_x1 && x0 < doente_x2 && y0 >= doente_y1 && y0 < doente_y2) {
                    som_colidir_doente = true;
                    quedaComum = true;
                    return;

                }
            }
            for (int j = 0; j < maxInimigoAtual; j++) {
                if(inimigos[j].ativo) {
                    int inimigoTelaY = inimigos[j].y - scrollCamera, inimigoTelaX = inimigos[j].x + ((LARGURA_TELA - LARGURA_PREDIO) / 2);
                    if(x0 >= inimigoTelaX && x0 < inimigoTelaX + LARGURA_INIMIGO_JANELA && y0 >= inimigoTelaY && y0 < inimigoTelaY + ALTURA_INIMIGO_JANELA) {
                        quedaComum = true;
                        inimigos[j].ativo = false;
                        return;
                    }
                }
            }
            for (int j = 0; j < maxBombaAtual; j++) {
                if(bombas[j].ativo) {
                    int bombaTelaY = bombas[j].y - scrollCamera, bombaTelaX = bombas[j].x + ((LARGURA_TELA - LARGURA_PREDIO) / 2);
                    if(x0 == bombaTelaX && y0 == bombaTelaY) {
                        quedaComum = true;
                        bombas[j].ativo = false;
                        return;
                    }
                }
            }

            if (x0 == x1 && y0 == y1) {
                break;
            }

            e2 = 2 * err;

            if (e2 >= dy) {
                err += dy;
                x0 += sx;
            }
            if (e2 <= dx) {
                err += dx;
                y0 += sy;
            }
        }
    }

    for (int i = 0; i < ALTURA_MIRANHA; ++i) {
        for (int j = 0; j < LARGURA_MIRANHA; ++j) {
            if (CORPO_MIRANHA[i][j] != ' ') {
                int miranhaTelaX = miranha.x + j, miranhaTelaY = miranha.y + i;
                int predioY = miranhaTelaY + scrollCamera, predioX = miranhaTelaX - ((LARGURA_TELA - LARGURA_PREDIO) / 2);
                if (predioY >= 0 && predioY < ALTURA_PREDIO && predioX >= 0 && predioX < LARGURA_PREDIO) {
                    if (PREDIO[predioY][predioX] == '#') { 
                        musica = true;

                        VencerNivel();
                        return;
                    }
                }
            }
        }
    }
}

void AtualizarTempo() {
    contadorTempo += ATRASO_TIQUE; // Acumula o tempo de cada tique.
    if (contadorTempo >= DECREMENTO_TEMPO) {
        tempoRestante--;   // Decrementa uma barra de tempo.
        contadorTempo = 0; // Reseta o acumulador.
    }
    if (tempoRestante <= 0 && !quedaComum) {
        quedaComum = true; // Força o Homem-Aranha a cair.
        quedaFatal = true;   // Marca a queda como fatal (não pode ser salva com a teia).
    }
}

void Carregando(){
    char textoCarregando[15];
    sprintf(textoCarregando, "Carregando...");

    int indiceCarregando = ALTURA_TELA/2 * LARGURA_TELA + (LARGURA_TELA - strlen(textoCarregando))/2;

    
    while (musica)
    {
        LimparBuffer();

        for(int i = 0; i < strlen(textoCarregando); i++){
            bufferConsole[indiceCarregando + i].Char.UnicodeChar = textoCarregando[i];
            bufferConsole[indiceCarregando + i].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        }
        
        DesenharBuffer(); 
    }

    Sleep(1000);
    musica = true;
}

DWORD WINAPI Jogo(LPVOID lpParam){ // Precisa desse LPVOID lpParam pra compilar pra mim
    srand(time(NULL));
    SetConsoleOutputCP(CP_UTF8);

    console = GetStdHandle(STD_OUTPUT_HANDLE);

    while(mostrarMenu){
        MenuInicial();
        Sleep(ATRASO_TIQUE + 25);
    }

    Carregando();
    InicializarJogo();

    while (true)
    {
        SpawnInimigos(); // Tenta criar novos inimigos.
        SpawnBombas();   // Tenta criar novas bombas.

        AtualizarInimigos();       // Atualiza a posição dos inimigos existentes.
        AtualizarBombas();         // Atualiza o estado das bombas existentes.
        AtualizarTempo();          // Atualiza o cronômetro.

        EscolherControleTeia();           // Processa a entrada do jogador para a teia.

        VerificarQuedaMiranha();             // Verifica se o Homem-Aranha deve cair.
        MovimentarMiranha();     // Processa a movimentação do Homem-Aranha na teia.

        MovimentarDoente();        // Move o Duende Verde.

        VerificarColisoes();       // Checa todas as colisões.

        CorrigirPosicaoMiranha();  // Garante que o Homem-Aranha não saia da tela.
        VerificarMiranhaCaiuChao();        // Verifica se o Homem-Aranha caiu no chão.

        DesenharTela(); // Desenha tudo que foi atualizado.

        Sleep(ATRASO_TIQUE);
    }

    return 0;
}

void SomDisparoTeia(){
    if (GetAsyncKeyState(VK_SPACE) & 0x8000 && !quedaFatal){
        if (GetAsyncKeyState(VK_UP) & 0x8000 || GetAsyncKeyState(VK_LEFT) & 0x8000 || GetAsyncKeyState(VK_RIGHT) & 0x8000){
            if(tamanhoTeiaDisparando < 5){
                Beep(349, 150); // Fá (F4)
                Sleep(100);
            }
            else if(tamanhoTeiaDisparando == 5 && !fim_som_disparo){
                Beep(349, 150); // Fá (F4)
                Sleep(100);
                fim_som_disparo = true;
            }
        }
    }   
        
    if(tamanhoTeiaDisparando == 0){
        fim_som_disparo = false;
    }
}

void SomTeiaAncorada(){
    if(teiaAncorada){
        if(GetAsyncKeyState(VK_UP) & 0x8000 || GetAsyncKeyState(VK_DOWN) & 0x8000){
            if(tamanhoTeiaAncorada < 5){
                Beep(261, 150);
                Sleep(100);
            } 
        }
    }
}

void SomQuedaMiranha(){
    if(quedaComum || quedaFatal){
        Beep(392, 150); // Sol (G4)
        Sleep(50);
        Beep(261, 150); // Dó (C4)
        Sleep(50);
        Beep(329, 150); // Mi (E4)
        Sleep(50);
    } 
}

void SomColodirElementos(bool colisao, int frequncia){
    if(colisao == true){
        Beep(frequncia, 150); // La (A4)
        Sleep(50);

        som_colidir_inimigo = false;
        som_colidir_bomba = false;
        som_colidir_doente = false;
    }
}  

void Musica(){
    musica = true;

    Beep(294, 225); // Nota D4
    Beep(294, 75);  // Nota D4
    Beep(440, 300); // Nota A4
    Sleep(150);     // Pausa

    Beep(294, 225); // Nota D4
    Beep(294, 75);  // Nota D4
    Beep(440, 300); // Nota A4
    Sleep(150);     // Pausa

    Beep(440, 150); // Nota A4
    Beep(392, 150); // Nota G4
    Beep(349, 150); // Nota F4
    Beep(330, 150); // Nota E4
    Beep(294, 150); // Nota D4
    Beep(330, 150); // Nota E4
    Beep(349, 150); // Nota F4
    Beep(294, 300); // Nota D4
    Sleep(1000);

    musica = false;
}

void GameOverMusica(){
    Sleep(500);
    Beep(440, 150); // Nota A4
    Sleep(50);      // Pequena pausa

    Beep(392, 200); // Nota G4
    Beep(349, 200); // Nota F4
    Beep(330, 200); // Nota E4
    Beep(294, 400); // Nota D4 (um pouco mais longa)

    Sleep(300);

    Beep(261, 500); // Nota C4 (grave)
    Sleep(150);     // Pausa curta antes do fim
    Beep(196, 1000); // Nota G3 (muito grave)

    game_over_musica = false;
}                                        
                                 
DWORD WINAPI EfeitosSonoros(LPVOID lpParam){ // Precisa desse LPVOID lpParam pra compilar pra mim
    while (mostrarMenu){
        Musica();
    }

    while(true){
        if(game_over_musica){
            musica = false;
            GameOverMusica();
        }
        else if(musica){
            Musica();
        }
        else{
            SomDisparoTeia();
            SomTeiaAncorada();
            SomQuedaMiranha();
            SomColodirElementos(som_colidir_inimigo, 392);
            SomColodirElementos(som_colidir_bomba, 440);
            SomColodirElementos(som_colidir_doente, 493);
        }
    }

    return 0;
}

int main(){
    HANDLE thread1;
    HANDLE thread2;

    thread1 = CreateThread(NULL, 0, Jogo, NULL, 0, NULL);
    thread2 = CreateThread(NULL, 0, EfeitosSonoros, NULL, 0, NULL);

    WaitForSingleObject(thread1, INFINITE);
    WaitForSingleObject(thread2, INFINITE);

    return 0;
}
