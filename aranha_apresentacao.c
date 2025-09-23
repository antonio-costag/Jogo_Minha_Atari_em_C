#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>
#include <wchar.h>

#define LARGURA_TELA 80 
#define ALTURA_JOGO 15 
#define ALTURA_UI 1 
#define ESPACO_VAZIO ' '
#define ATRASO_TIQUE 100 

#define ALTURA_MIRANHA 2  
#define LARGURA_MIRANHA 4  
#define ALTURA_DOENTE 3   
#define LARGURA_DOENTE 6 
#define ALTURA_INIMIGO_JANELA 2 
#define LARGURA_INIMIGO_JANELA 5

#define MAX_INIMIGOS_JANELA 4
#define MAX_BOMBAS 4

#define MIRANHA_CAINDO "|_/o"

#define ALTURA_PREDIO 65 
#define LARGURA_PREDIO 58 

#define TEMPO_MAXIMO 20
#define DECREMENTO_TEMPO 2000

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
    NENHUMA,
    CIMA,
    DIAGONAL_ESQUERDA,
    DIAGONAL_DIREITA,
    ANCORA_LOCAL
} DirecaoTeia;

DirecaoTeia direcaoAtualTeia = NENHUMA;
bool soltandoTeia = false;
bool teiaAncorada = false;
int teiaAncoraPredioX = 0, teiaAncoraPredioY = 0;
int tamanhoAtualTeia = 0;
int tempoSegurandoEspaco = 0;
int comprimentoTeiaVoo = 0;

bool miranha_cair = false;
bool quedaFatal = false;
int yPredioTela = 0;
int yDoenteTela = 0;

int tempoRestante;
int acumuladorTempo;
int pontuacao; 
int vidas;

const wchar_t *CORPO_MIRANHA[ALTURA_MIRANHA] = {
    L"|o /",
    L"/ | "
};

const wchar_t *DOENTE_VERDE[ALTURA_DOENTE] = {
    L"=- V-=",
    L"  | | ",
    L" /--\\ "
};

const wchar_t *INIMIGO_JANELA_SPRITE[ALTURA_INIMIGO_JANELA] = {
    L"| o |",
    L" / \\ "
};

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

MIRANHA miranha;
DOENTE doente;
INIMIGO_JANELA inimigos[MAX_INIMIGOS_JANELA];
BOMBA bombas[MAX_BOMBAS];

const COORD posicoesJanelas[] = {
    {4, 24}, {18, 24}, {32, 24}, {46, 24},
    {4, 30}, {18, 30}, {32, 30}, {46, 30},
    {4, 36}, {18, 36}, {32, 36}, {46, 36},
    {4, 42}, {18, 42}, {32, 42}, {46, 42},
    {4, 48}, {18, 48}, {32, 48}, {46, 48},
    {4, 54}, {18, 54}, {32, 54}, {46, 54},
};

const int NUM_POSICOES_JANELAS = sizeof(posicoesJanelas) / sizeof(COORD);

HANDLE console;
CHAR_INFO bufferConsole[LARGURA_TELA * (ALTURA_JOGO + 2)];
COORD tamanhoBuffer = {LARGURA_TELA, ALTURA_JOGO + 2};
COORD posicaoCaractere = {0, 0};
SMALL_RECT areaEscritaConsole = {0, 0, LARGURA_TELA - 1, ALTURA_JOGO + ALTURA_UI};

void InicializarMiranha(){
    miranha.x = 38;
    miranha.y = 7;

    miranha.cor = FOREGROUND_BLUE | FOREGROUND_INTENSITY;

    miranha_cair = false;
    quedaFatal = false;

    soltandoTeia = false;

    yPredioTela = 50;

    direcaoAtualTeia = NENHUMA;
    teiaAncorada = false;
    tamanhoAtualTeia = 0;

    tempoSegurandoEspaco = 0;

    comprimentoTeiaVoo = 0;

    tempoRestante = TEMPO_MAXIMO;
    acumuladorTempo = 0;
}

void InicializarDoente(){
    doente.x = 38;
    doente.y = 0;

    doente.cor = FOREGROUND_GREEN | FOREGROUND_INTENSITY;

    doente.direita = true;
    doente.esquerda = false; 

    yDoenteTela = 3;
}

void InicializarInimigos() {
    for (int i = 0; i < MAX_INIMIGOS_JANELA; i++) {
        inimigos[i].ativo = false;
        inimigos[i].tempoNoLugar = 0;
    }
}

void InicializarBombas() {
    for (int i = 0; i < MAX_BOMBAS; i++) {
        bombas[i].ativo = false;
        bombas[i].tempoNoLugar = 0;
    }
}

void InicializarNivel() {
    InicializarMiranha();
    InicializarDoente();
    InicializarInimigos();
    InicializarBombas();
}

void InicializarJogo() {
    pontuacao = 0;
    vidas = 3;
    InicializarNivel();
}

void PerderVida() {
    vidas--;
    if (vidas <= 0) {
        pontuacao = 0;
        vidas = 3;
    }
    InicializarNivel();
}

void VencerNivel() {
    pontuacao *= 2;
    InicializarNivel();
}

void DesenharPredio(){
    for(int i = yPredioTela; i < ALTURA_JOGO + yPredioTela; i++){
        for(int j = 0; j < LARGURA_PREDIO; j++){
            int indice = (i - yPredioTela + ALTURA_UI) * LARGURA_TELA + (((LARGURA_TELA - LARGURA_PREDIO) / 2) + j);
            if (i >= 0 && i < ALTURA_PREDIO) {
                bufferConsole[indice].Char.UnicodeChar = PREDIO[i][j];
                bufferConsole[indice].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            }
        }
    }
}

void DesenharMiranha(){
    if(miranha_cair == false){
        for(int i = 0; i < ALTURA_MIRANHA; ++i){
            for(int j = 0; j < LARGURA_MIRANHA; ++j){
                if (CORPO_MIRANHA[i][j] != ' ' || (i == 0 && j == 2) || (i == 1 && j == 1)) {
                    int indice = (miranha.y + i + ALTURA_UI) * LARGURA_TELA + (miranha.x + j);
                    if ( (miranha.y + i) < ALTURA_JOGO) {
                        bufferConsole[indice].Char.UnicodeChar = CORPO_MIRANHA[i][j];
                        bufferConsole[indice].Attributes = miranha.cor;
                    }
                }
            }
        }
    }
    else{
        for(int i = 0; i < LARGURA_MIRANHA; i++){
            int indice = (miranha.y + ALTURA_UI) * LARGURA_TELA + miranha.x;
             if (miranha.y < ALTURA_JOGO) {
                bufferConsole[indice + i].Char.UnicodeChar = MIRANHA_CAINDO[i];
                bufferConsole[indice + i].Attributes = miranha.cor;
             }
        }
    }
}

void DesenharDoente(){
    for(int i = 0 + yDoenteTela; i < ALTURA_DOENTE; ++i){
        for(int j = 0; j < LARGURA_DOENTE; ++j){
            if (DOENTE_VERDE[i][j] != ' ' || (i == 1 && j == 3) || (i == 0 && j == 2)) {
                int indice = (doente.y + (i - yDoenteTela) + ALTURA_UI) * LARGURA_TELA + (doente.x + j);
                if ( (doente.y + (i- yDoenteTela)) < ALTURA_JOGO){
                    bufferConsole[indice].Char.UnicodeChar = DOENTE_VERDE[i][j];
                    bufferConsole[indice].Attributes = doente.cor;
                }
            }
        }
    }
}

void DesenharInimigos() {
    for (int i = 0; i < MAX_INIMIGOS_JANELA; i++) {
        if (inimigos[i].ativo) {

            int inimigoTelaY = inimigos[i].y - yPredioTela;
            int inimigoTelaX = inimigos[i].x + ((LARGURA_TELA - LARGURA_PREDIO) / 2);

            for (int linha = 0; linha < ALTURA_INIMIGO_JANELA; linha++) {
                for (int coluna = 0; coluna < LARGURA_INIMIGO_JANELA; coluna++) {
                    if (INIMIGO_JANELA_SPRITE[linha][coluna] != ' ') {

                        int finalY = inimigoTelaY + linha + ALTURA_UI;
                        int finalX = inimigoTelaX + coluna;

                        if (finalY >= ALTURA_UI && finalY < ALTURA_JOGO + ALTURA_UI && finalX >= 0 && finalX < LARGURA_TELA) {
                            int indice = finalY * LARGURA_TELA + finalX;

                            bufferConsole[indice].Char.UnicodeChar = INIMIGO_JANELA_SPRITE[linha][coluna];
                            bufferConsole[indice].Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
                        }
                    }
                }
            }
        }
    }
}

void DesenharBombas() {
    for (int i = 0; i < MAX_BOMBAS; i++) {
        if (bombas[i].ativo) {

            int bombaTelaY = bombas[i].y - yPredioTela + ALTURA_UI;
            int bombaTelaX = bombas[i].x + ((LARGURA_TELA - LARGURA_PREDIO) / 2);

            if (bombaTelaY >= ALTURA_UI && bombaTelaY < ALTURA_JOGO + ALTURA_UI && bombaTelaX >= 0 && bombaTelaX < LARGURA_TELA) {
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
                for (int i = 1; i <= comprimentoTeiaVoo; i++){
                    if (miranha.y - i >= 0){
                        int indice = (miranha.y - i + ALTURA_UI) * LARGURA_TELA + (miranha.x + 2);
                        bufferConsole[indice].Char.UnicodeChar = '0';
                        bufferConsole[indice].Attributes = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
                    }
                }
                break;
            case DIAGONAL_ESQUERDA:
                for (int i = 1; i <= comprimentoTeiaVoo; i++){
                    if (miranha.y - i >= 0 && miranha.x + 1 - i >= 0){
                        int indice = (miranha.y - i + ALTURA_UI) * LARGURA_TELA + (miranha.x + 1 - i);
                        bufferConsole[indice].Char.UnicodeChar = '0';
                        bufferConsole[indice].Attributes = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
                    }
                }
                break;
            case DIAGONAL_DIREITA:
                for (int i = 1; i <= comprimentoTeiaVoo; i++){
                    if (miranha.y - i >= 0 && miranha.x + 2 + i < LARGURA_TELA){
                        int indice = (miranha.y - i + ALTURA_UI) * LARGURA_TELA + (miranha.x + 2 + i);
                        bufferConsole[indice].Char.UnicodeChar = '0';
                        bufferConsole[indice].Attributes = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
                    }
                }
                break;
            default: break;
        }
    }
    else if (teiaAncorada) {
        int ancoraTelaY = teiaAncoraPredioY - yPredioTela;
        int ancoraTelaX = teiaAncoraPredioX + ((LARGURA_TELA - LARGURA_PREDIO) / 2);

        int x0 = miranha.x + 2;
        int y0 = miranha.y;
        int x1 = ancoraTelaX;
        int y1 = ancoraTelaY;

        int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
        int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
        int err = dx + dy, e2;

        while (true) {
            if (y0 >= 0 && y0 < ALTURA_JOGO && x0 >= 0 && x0 < LARGURA_TELA) {
                int indice = (y0 + ALTURA_UI) * LARGURA_TELA + x0;
                bufferConsole[indice].Char.UnicodeChar = '.';
                bufferConsole[indice].Attributes = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            }
            if (x0 == x1 && y0 == y1){
                break;
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
    char textoPontos[30];
    sprintf(textoPontos, "PONTOS: %d", pontuacao);
    int startX = (LARGURA_TELA - strlen(textoPontos)) / 2;
    for (int i = 0; i < strlen(textoPontos); i++) {
        bufferConsole[i + startX].Char.UnicodeChar = textoPontos[i];
    }

    int linhaInferiorY = ALTURA_JOGO + ALTURA_UI;

    char textoVidas[15];
    sprintf(textoVidas, "VIDAS: %d", vidas);
    for (int i = 0; i < strlen(textoVidas); i++) {
        int indice = linhaInferiorY * LARGURA_TELA + i;
        bufferConsole[indice].Char.UnicodeChar = textoVidas[i];
    }

    for (int i = 0; i < tempoRestante; i++) {
        int x = LARGURA_TELA - TEMPO_MAXIMO - 1 + i;
        int indice = linhaInferiorY * LARGURA_TELA + x;
        bufferConsole[indice].Char.UnicodeChar = '#';
        bufferConsole[indice].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    }
}

void DesenharTela(){
    for(int i = 0; i < LARGURA_TELA * (ALTURA_JOGO + 2); ++i){
        bufferConsole[i].Char.UnicodeChar = ESPACO_VAZIO;
        bufferConsole[i].Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
    }

    DesenharUI();
    DesenharPredio();
    DesenharBombas();
    DesenharInimigos();

    if (soltandoTeia || teiaAncorada) {
        DesenharTeia();
    }
    DesenharDoente();
    DesenharMiranha();

    WriteConsoleOutputW(console, bufferConsole, tamanhoBuffer, posicaoCaractere, &areaEscritaConsole);
}

void ControlarTeia() {
    if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
        if (!soltandoTeia && !quedaFatal) {
            teiaAncorada = false;
            direcaoAtualTeia = NENHUMA;

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
                soltandoTeia = true;
                tempoSegurandoEspaco = 0;
                comprimentoTeiaVoo = 1;
            }
        }
        else { 
            if (direcaoAtualTeia != ANCORA_LOCAL) {
                tempoSegurandoEspaco += ATRASO_TIQUE;
                if (tempoSegurandoEspaco > 150 && comprimentoTeiaVoo == 1) {
                    comprimentoTeiaVoo = 2;
                } else if (tempoSegurandoEspaco > 300 && comprimentoTeiaVoo == 2) {
                    comprimentoTeiaVoo = 3;
                } else if (tempoSegurandoEspaco > 450 && comprimentoTeiaVoo == 3) {
                    comprimentoTeiaVoo = 4;
                } else if (tempoSegurandoEspaco > 600 && comprimentoTeiaVoo == 4) {
                    comprimentoTeiaVoo = 5;
                }
            }
        }
    }
    else if (soltandoTeia) {
        soltandoTeia = false;

        int teia_x_final_tela = -1, teia_y_final_tela = -1;
        bool colidiu = false;

        for (int i = 1; i <= comprimentoTeiaVoo; i++) {
            int teia_x_tela = -1, teia_y_tela = -1;

            switch (direcaoAtualTeia) {
                case CIMA: teia_y_tela = miranha.y - i; teia_x_tela = miranha.x + 2; break;
                case DIAGONAL_ESQUERDA: teia_y_tela = miranha.y - i; teia_x_tela = miranha.x + 1 - i; break;
                case DIAGONAL_DIREITA: teia_y_tela = miranha.y - i; teia_x_tela = miranha.x + 2 + i; break;
                default: break;
            }

            if (i == comprimentoTeiaVoo) {
                teia_x_final_tela = teia_x_tela;
                teia_y_final_tela = teia_y_tela;
            }

            if(teia_x_tela != -1) {
                for (int j = 0; j < MAX_INIMIGOS_JANELA; j++) {
                    if(inimigos[j].ativo) {
                        int inimigoTelaY = inimigos[j].y - yPredioTela, inimigoTelaX = inimigos[j].x + ((LARGURA_TELA - LARGURA_PREDIO) / 2);
                        if(teia_x_tela >= inimigoTelaX && teia_x_tela < inimigoTelaX + LARGURA_INIMIGO_JANELA && teia_y_tela >= inimigoTelaY && teia_y_tela < inimigoTelaY + ALTURA_INIMIGO_JANELA) {
                            colidiu = true;
                            inimigos[j].ativo = false;
                            break;
                        }
                    }
                }
                if(colidiu) break;
                for (int j = 0; j < MAX_BOMBAS; j++) {
                    if(bombas[j].ativo) {
                        int bombaTelaY = bombas[j].y - yPredioTela, bombaTelaX = bombas[j].x + ((LARGURA_TELA - LARGURA_PREDIO) / 2);
                        if(teia_x_tela == bombaTelaX && teia_y_tela == bombaTelaY) {
                            colidiu = true;
                            bombas[j].ativo = false;
                            break;
                        }
                    }
                }
                if(colidiu) break;
            }
        }

        if (colidiu) {
            miranha_cair = true;
        } 
        else if (direcaoAtualTeia == ANCORA_LOCAL) {
            int teiaTelaY = miranha.y;
            int teiaTelaX = miranha.x + 2;
            int predioY = teiaTelaY + yPredioTela;
            int predioX = teiaTelaX - ((LARGURA_TELA - LARGURA_PREDIO) / 2);

            if (predioY >= 0 && predioY < ALTURA_PREDIO && predioX >= 0 && predioX < LARGURA_PREDIO && PREDIO[predioY][predioX] == '|') {
                teiaAncorada = true;
                teiaAncoraPredioX = predioX;
                teiaAncoraPredioY = predioY;
                tamanhoAtualTeia = 0;

                if (miranha_cair) {
                    yPredioTela = yPredioTela + (miranha.y - 7);
                    if (yPredioTela > (ALTURA_PREDIO - ALTURA_JOGO)) {
                        yPredioTela = ALTURA_PREDIO - ALTURA_JOGO;
                    }
                    if (yPredioTela < 0) {
                        yPredioTela = 0;
                    }
                    miranha.y = 7;
                    miranha_cair = false;
                }

            } else {
                if (!miranha_cair) {
                    miranha_cair = true;
                }
            }
        } else {
            int predioY = teia_y_final_tela + yPredioTela;
            int predioX = teia_x_final_tela - ((LARGURA_TELA - LARGURA_PREDIO) / 2);

            if (direcaoAtualTeia != NENHUMA && predioY >= 0 && predioY < ALTURA_PREDIO && predioX >= 0 && predioX < LARGURA_PREDIO && PREDIO[predioY][predioX] == '|') {
                teiaAncorada = true;
                teiaAncoraPredioX = predioX;
                teiaAncoraPredioY = predioY;
                tamanhoAtualTeia = comprimentoTeiaVoo;
                
                if (miranha_cair) {

                    yPredioTela = yPredioTela + (miranha.y - 7);
                    if (yPredioTela > (ALTURA_PREDIO - ALTURA_JOGO)) yPredioTela = ALTURA_PREDIO - ALTURA_JOGO;
                    if (yPredioTela < 0) yPredioTela = 0;
                    miranha.y = 7;
                    miranha_cair = false;
                }
            } else {
                if (!miranha_cair) {
                    miranha_cair = true;
                }
            }
        }

        direcaoAtualTeia = NENHUMA;
        tempoSegurandoEspaco = 0;
        comprimentoTeiaVoo = 0;
    }
}

void MovimentacaoMiranha() {
    if (teiaAncorada) {
        int ancoraTelaY = teiaAncoraPredioY - yPredioTela;
        int ancoraTelaX = teiaAncoraPredioX + ((LARGURA_TELA - LARGURA_PREDIO) / 2);

        if ((GetAsyncKeyState(VK_UP) & 0x8000) && tamanhoAtualTeia > 0) {
            if (miranha.y > 7) {
                miranha.y--;
            }
            else if (yPredioTela > 0) {
                yPredioTela--;
            }
            else {
                miranha.y--;
            }

            if (miranha.x + 2 < ancoraTelaX) {
                miranha.x++;
            }
            if (miranha.x + 2 > ancoraTelaX) {
                miranha.x--;
            }

            tamanhoAtualTeia--;
            if (tamanhoAtualTeia == 0) {
                teiaAncorada = false;
            }
        }

        else if ((GetAsyncKeyState(VK_DOWN) & 0x8000) && tamanhoAtualTeia < 5 && miranha.y <= 7) {
            if (miranha.y < 7) {
                miranha.y++;
            } else if ((yPredioTela + ALTURA_JOGO) < ALTURA_PREDIO) {
                yPredioTela++;
            } else if (miranha.y < ALTURA_JOGO - ALTURA_MIRANHA) {
                miranha.y++;
            }

            if (miranha.x > 0 && miranha.x + 2 > ancoraTelaX) {
                miranha.x--;
            }
            if (miranha.x < LARGURA_TELA - LARGURA_MIRANHA && miranha.x + 2 < ancoraTelaX) {
                miranha.x++;
            }

            tamanhoAtualTeia++;
        }
    }
}

void MovimentarDoente(){
    if(yPredioTela == 0){ doente.y = 4; yDoenteTela = 0; }
    else if(yPredioTela == 1){ doente.y = 3; yDoenteTela = 0; }
    else if(yPredioTela == 2){ doente.y = 2; yDoenteTela = 0; }
    else if(yPredioTela == 3){ doente.y = 1; yDoenteTela = 0; }
    else if(yPredioTela == 4){ doente.y = 0; yDoenteTela = 0; }
    else if(yPredioTela == 5){ yDoenteTela = 1; doente.y = 0; }
    else if(yPredioTela == 6){ yDoenteTela = 2; doente.y = 0; }
    else if(yPredioTela == 7){ yDoenteTela = 3; doente.y = 0; }

    if(doente.x < 65 && doente.direita == true){
        doente.x++;
        doente.esquerda = false;
        doente.direita = true;
    }
    else{
        doente.direita = false;
        doente.esquerda = true;
    }

    if(doente.x > 10 && doente.esquerda == true){
        doente.x--;
        doente.esquerda = true;
        doente.direita = false;
    }
    else{
        doente.esquerda = false;
        doente.direita = true;
    }
}

void GerenciarSpawnsInimigos() {
    if (rand() % 100 > 2) {
        return;
    }

    int inimigosAtivos = 0;
    for (int i = 0; i < MAX_INIMIGOS_JANELA; i++) {
        if (inimigos[i].ativo) {
            inimigosAtivos++;
        }
    }

    if (inimigosAtivos < MAX_INIMIGOS_JANELA) {
        int slotVazio = -1;
        for (int i = 0; i < MAX_INIMIGOS_JANELA; i++) {
            if (!inimigos[i].ativo) {
                slotVazio = i;
                break;
            }
        }

        if (slotVazio != -1) {
            COORD janelasDisponiveis[NUM_POSICOES_JANELAS];
            int countDisponiveis = 0;

            for (int i = 0; i < NUM_POSICOES_JANELAS; i++) {
                bool naTela = posicoesJanelas[i].Y >= yPredioTela && posicoesJanelas[i].Y < yPredioTela + ALTURA_JOGO;
                if (naTela) {
                    bool posicaoOcupada = false;
                    for (int j = 0; j < MAX_INIMIGOS_JANELA; j++) {
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

void GerenciarSpawnsBombas() {
    if (rand() % 100 > 1) {
        return;
    }

    int bombasAtivas = 0;
    for(int i = 0; i < MAX_BOMBAS; i++) {
        if(bombas[i].ativo) {
            bombasAtivas++;
        }
    }

    if(bombasAtivas < MAX_BOMBAS) {
        int slotVazio = -1;
        for(int i = 0; i < MAX_BOMBAS; i++) {
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
                        bool naTela = y >= yPredioTela && y < yPredioTela + ALTURA_JOGO;
                        if (naTela) {

                            bool ocupada = false;

                            for(int j = 0; j < MAX_BOMBAS; j++) {
                                if(bombas[j].ativo && bombas[j].x == x && bombas[j].y == y) {
                                    ocupada = true;
                                    break;
                                }
                            }
                            bool ocupadaPeloMiranha = false;
                            int bombaTelaX = x + ((LARGURA_TELA - LARGURA_PREDIO) / 2);
                            int bombaTelaY = y - yPredioTela;

                            if(bombaTelaX >= miranha.x && bombaTelaX < miranha.x + LARGURA_MIRANHA &&
                               bombaTelaY >= miranha.y && bombaTelaY < miranha.y + ALTURA_MIRANHA) {
                                ocupadaPeloMiranha = true;
                            }

                            if(!ocupada && !ocupadaPeloMiranha) {
                                bombasDisponiveis[countDisponiveis].X = x;
                                bombasDisponiveis[countDisponiveis].Y = y;
                                countDisponiveis++;
                            }
                        }
                    }
                }
            }

            if(countDisponiveis > 0) {
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
    for (int i = 0; i < MAX_INIMIGOS_JANELA; i++) {
        if (inimigos[i].ativo) {
            inimigos[i].tempoNoLugar += ATRASO_TIQUE;

            if (inimigos[i].tempoNoLugar >= 4000) {

                int tentativas = 0;
                while (tentativas < NUM_POSICOES_JANELAS) {
                    int posIndex = rand() % NUM_POSICOES_JANELAS;
                    bool posicaoOcupada = false;
                    for (int j = 0; j < MAX_INIMIGOS_JANELA; j++) {
                        if (i != j && inimigos[j].ativo && inimigos[j].x == posicoesJanelas[posIndex].X + 1 && inimigos[j].y == posicoesJanelas[posIndex].Y + 2) {
                            posicaoOcupada = true;
                            break;
                        }
                    }
                    if (!posicaoOcupada) {
                        inimigos[i].x = posicoesJanelas[posIndex].X + 1;
                        inimigos[i].y = posicoesJanelas[posIndex].Y + 2;
                        inimigos[i].tempoNoLugar = 0;
                        break;
                    }
                    tentativas++;
                }
            }
        }
    }
}

void AtualizarBombas() {
    for (int i = 0; i < MAX_BOMBAS; i++) {
        if (bombas[i].ativo) {
            bombas[i].tempoNoLugar += ATRASO_TIQUE;

            if (bombas[i].tempoNoLugar >= 4000) {
                bombas[i].ativo = false;
            }
        }
    }
}

void MiranhaCair(){
    if(miranha.x <= (((LARGURA_TELA - LARGURA_PREDIO) / 2) - 1) || miranha.x >= (LARGURA_PREDIO + ((LARGURA_TELA - LARGURA_PREDIO) / 2)) - LARGURA_MIRANHA){
        miranha_cair = true;
    }

    if (!teiaAncorada && !soltandoTeia && !miranha_cair) {
        bool tocandoPredio = false;

        for (int i = 0; i < ALTURA_MIRANHA; i++) {
            for (int j = 0; j < LARGURA_MIRANHA; j++) {
                if (CORPO_MIRANHA[i][j] == ' ') { continue; }
                int predioY = (miranha.y + i) + yPredioTela;
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

        if (!tocandoPredio) {
            miranha_cair = true;
        }
    }

    if (miranha_cair) {
        teiaAncorada = false;
        if((ALTURA_JOGO + yPredioTela) < 65){
            yPredioTela++;
        }
    }
}

void MiranhaBaterChao(){
    if(miranha_cair == true && (ALTURA_JOGO + yPredioTela) == 65){
        miranha.y++;
        if(miranha.y >= ALTURA_JOGO - ALTURA_MIRANHA){
            PerderVida();
        }
    }
}

void CorrigirPosicaoMiranha(){
    if(miranha.x <= 0) {
        miranha.x = 1;
    } else if(miranha.x >= LARGURA_TELA - LARGURA_MIRANHA) {
        miranha.x = LARGURA_TELA - LARGURA_MIRANHA - 1;
    }

    if(miranha.y <= 0) {
        miranha.y = 1;
    }
}

void VerificarColisoes() {
    if (quedaFatal) {
        return;
    }

    int doente_x1 = doente.x, doente_y1 = doente.y, doente_x2 = doente.x + LARGURA_DOENTE, doente_y2 = doente.y + ALTURA_DOENTE;
    int miranha_x1 = miranha.x, miranha_y1 = miranha.y, miranha_x2 = miranha.x + LARGURA_MIRANHA, miranha_y2 = miranha.y + ALTURA_MIRANHA;

    if (miranha_x1 < doente_x2 && miranha_x2 > doente_x1 && miranha_y1 < doente_y2 && miranha_y2 > doente_y1) {
        miranha_cair = true;
        return;
    }

    for (int i = 0; i < MAX_INIMIGOS_JANELA; i++) {
        if (inimigos[i].ativo) {
            int inimigoTelaY = inimigos[i].y - yPredioTela, inimigoTelaX = inimigos[i].x + ((LARGURA_TELA - LARGURA_PREDIO) / 2);
            int inimigo_x1 = inimigoTelaX, inimigo_y1 = inimigoTelaY, inimigo_x2 = inimigoTelaX + LARGURA_INIMIGO_JANELA, inimigo_y2 = inimigoTelaY + ALTURA_INIMIGO_JANELA;

            if (miranha_x1 < inimigo_x2 && miranha_x2 > inimigo_x1 && miranha_y1 < inimigo_y2 && miranha_y2 > inimigo_y1) {
                pontuacao += 30;
                inimigos[i].ativo = false;
                return;
            }
        }
    }

    for(int i = 0; i < MAX_BOMBAS; i++) {
        if(bombas[i].ativo) {
            int bombaTelaY = bombas[i].y - yPredioTela;
            int bombaTelaX = bombas[i].x + ((LARGURA_TELA - LARGURA_PREDIO) / 2);
            if(miranha_x1 < bombaTelaX + 1 && miranha_x2 > bombaTelaX && miranha_y1 < bombaTelaY + 1 && miranha_y2 > bombaTelaY) {
                pontuacao += 80;
                bombas[i].ativo = false;
                return;
            }
        }
    }

    if (teiaAncorada) {
        int ancoraTelaY = teiaAncoraPredioY - yPredioTela, ancoraTelaX = teiaAncoraPredioX + ((LARGURA_TELA - LARGURA_PREDIO) / 2);
        int x0 = miranha.x + 2, y0 = miranha.y;
        int x1 = ancoraTelaX, y1 = ancoraTelaY;
        int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
        int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
        int err = dx + dy, e2;
        while (true) {
            if (yPredioTela <= 4) {
                if (x0 >= doente_x1 && x0 < doente_x2 && y0 >= doente_y1 && y0 < doente_y2) {
                    miranha_cair = true;
                    return;

                }
            }
            for (int j = 0; j < MAX_INIMIGOS_JANELA; j++) {
                if(inimigos[j].ativo) {
                    int inimigoTelaY = inimigos[j].y - yPredioTela, inimigoTelaX = inimigos[j].x + ((LARGURA_TELA - LARGURA_PREDIO) / 2);
                    if(x0 >= inimigoTelaX && x0 < inimigoTelaX + LARGURA_INIMIGO_JANELA && y0 >= inimigoTelaY && y0 < inimigoTelaY + ALTURA_INIMIGO_JANELA) {
                        miranha_cair = true;
                        inimigos[j].ativo = false;
                        return;
                    }
                }
            }
            for (int j = 0; j < MAX_BOMBAS; j++) {
                if(bombas[j].ativo) {
                    int bombaTelaY = bombas[j].y - yPredioTela, bombaTelaX = bombas[j].x + ((LARGURA_TELA - LARGURA_PREDIO) / 2);
                    if(x0 == bombaTelaX && y0 == bombaTelaY) {
                        miranha_cair = true;
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
                int predioY = miranhaTelaY + yPredioTela, predioX = miranhaTelaX - ((LARGURA_TELA - LARGURA_PREDIO) / 2);

                if (predioY >= 0 && predioY < ALTURA_PREDIO && predioX >= 0 && predioX < LARGURA_PREDIO) {
                    if (PREDIO[predioY][predioX] == '#') {
                        VencerNivel();
                        return;
                    }
                }
            }
        }
    }
}

void AtualizarTempo() {
    acumuladorTempo += ATRASO_TIQUE;

    if (acumuladorTempo >= DECREMENTO_TEMPO) {
        tempoRestante--;
        acumuladorTempo = 0;
    }

    if (tempoRestante <= 0 && !miranha_cair) {
        miranha_cair = true;
        quedaFatal = true; 
    }
}

int main(){
    srand(time(NULL));
    SetConsoleOutputCP(CP_UTF8);
    InicializarJogo();
    console = GetStdHandle(STD_OUTPUT_HANDLE);

    while (true)
    {
        GerenciarSpawnsInimigos();
        GerenciarSpawnsBombas();

        AtualizarInimigos();
        AtualizarBombas();
        AtualizarTempo();

        ControlarTeia();

        MiranhaCair();
        MovimentacaoMiranha();

        MovimentarDoente();

        VerificarColisoes();

        CorrigirPosicaoMiranha();
        MiranhaBaterChao();

        DesenharTela();

        Sleep(ATRASO_TIQUE);
    }
    return 0;
}
