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

DirecaoTeia direcaoAtualTeia = NENHUMA
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

// =================================================================================
// VARIÁVEIS DA API DO CONSOLE DO WINDOWS
// Variáveis para interagir com o console e desenhar a tela de forma eficiente.
// =================================================================================

HANDLE console;  // "Handle" ou identificador para o buffer de saída do console. Essencial para usar as funções da API.
// Buffer de caracteres em memória. O jogo é desenhado aqui primeiro e depois transferido para a tela de uma vez.
// Isso evita piscadas (flickering) e melhora o desempenho.
CHAR_INFO bufferConsole[LARGURA_TELA * (ALTURA_JOGO + 2)];
COORD tamanhoBuffer = {LARGURA_TELA, ALTURA_JOGO + 2}; // O tamanho do nosso buffer em memória.
COORD posicaoCaractere = {0, 0}; // Posição de origem (canto superior esquerdo) para copiar o buffer.
// A área retangular na tela do console onde nosso buffer será escrito.
SMALL_RECT areaEscritaConsole = {0, 0, LARGURA_TELA - 1, ALTURA_JOGO + ALTURA_UI};

// =================================================================================
// FUNÇÕES DE INICIALIZAÇÃO
// Preparam o estado inicial dos personagens e do jogo.
// =================================================================================

// Define o estado inicial do Homem-Aranha.
void InicializarMiranha(){
    miranha.x = 38; // Posição X inicial.
    miranha.y = 7;  // Posição Y inicial.

    miranha.cor = FOREGROUND_BLUE | FOREGROUND_INTENSITY; // Define a cor do personagem.

    miranha_cair = false;      // Garante que ele não comece caindo.
    quedaFatal = false;        // Garante que ele não comece em queda fatal.

    soltandoTeia = false;      // Não começa soltando teia.

    yPredioTela = 50;          // Posição inicial da "câmera" no prédio (começa perto do chão).

    direcaoAtualTeia = NENHUMA;// Nenhuma direção de teia no início.
    teiaAncorada = false;      // Nenhuma teia ancorada no início.
    tamanhoAtualTeia = 0;      // Reseta o tamanho da teia.

    tempoSegurandoEspaco = 0;  // Reseta o contador de tempo da teia.

    comprimentoTeiaVoo = 0;    // Reseta o comprimento da teia.

    tempoRestante = TEMPO_MAXIMO; // Define o tempo inicial do nível.
    acumuladorTempo = 0;       // Reseta o acumulador de tempo.
}

// Define o estado inicial do Duende Verde.
void InicializarDoente(){
    doente.x = 38; // Posição X inicial.
    doente.y = 0;  // Posição Y inicial.

    doente.cor = FOREGROUND_GREEN | FOREGROUND_INTENSITY; // Define a cor.

    doente.direita = true;  // Começa se movendo para a direita.
    doente.esquerda = false; // Não está se movendo para a esquerda.

    yDoenteTela = 3; // Posição vertical inicial do sprite.
}

// Inicializa o array de inimigos.
void InicializarInimigos() {
    // Loop por todos os slots de inimigos.
    for (int i = 0; i < MAX_INIMIGOS_JANELA; i++) {
        inimigos[i].ativo = false;      // Define cada inimigo como inativo.
        inimigos[i].tempoNoLugar = 0;   // Reseta seu temporizador.
    }
}

// Inicializa o array de bombas.
void InicializarBombas() {
    // Loop por todos os slots de bombas.
    for (int i = 0; i < MAX_BOMBAS; i++) {
        bombas[i].ativo = false;        // Define cada bomba como inativa.
        bombas[i].tempoNoLugar = 0;     // Reseta seu temporizador.
    }
}

// Agrupa todas as inicializações de um nível.
void InicializarNivel() {
    InicializarMiranha(); // Prepara o Homem-Aranha.
    InicializarDoente();  // Prepara o Duende Verde.
    InicializarInimigos();// Prepara os inimigos.
    InicializarBombas();  // Prepara as bombas.
}

// Inicializa o jogo pela primeira vez (ou após um game over).
void InicializarJogo() {
    pontuacao = 0; // Reseta a pontuação geral.
    vidas = 3;     // Define as vidas iniciais.
    InicializarNivel(); // Chama a função para configurar o primeiro nível.
}

// =================================================================================
// FUNÇÕES DE GERENCIAMENTO DE ESTADO DO JOGO
// Funções que controlam o fluxo principal do jogo, como perder vida ou vencer.
// =================================================================================

// Função chamada quando o jogador perde uma vida.
void PerderVida() {
    vidas--; // Decrementa o contador de vidas.
    if (vidas <= 0) { // Se as vidas acabaram...
        pontuacao = 0; // Reseta a pontuação.
        vidas = 3;     // Reseta as vidas para o valor inicial.
    }
    InicializarNivel(); // Reinicia o nível atual.
}

// Função chamada quando o jogador vence o nível.
void VencerNivel() {
    pontuacao *= 2;     // Dobra a pontuação como bônus.
    InicializarNivel(); // Prepara e inicia o próximo nível (que é o mesmo, mas com pontuação maior).
}


// =================================================================================
// FUNÇÕES DE DESENHO
// Responsáveis por colocar todos os elementos visuais no buffer do console.
// =================================================================================

// Desenha a parte visível do prédio no buffer.
void DesenharPredio(){
    // Loop pelas linhas da tela de jogo.
    // 'yPredioTela' atua como um "offset" ou "scroll" vertical.
    for(int i = yPredioTela; i < ALTURA_JOGO + yPredioTela; i++){
        // Loop pelas colunas do prédio.
        for(int j = 0; j < LARGURA_PREDIO; j++){
            // Calcula o índice no bufferConsole 1D.
            // A fórmula converte coordenadas 2D (linha, coluna) para um índice de array 1D.
            int indice = (i - yPredioTela + ALTURA_UI) * LARGURA_TELA + (((LARGURA_TELA - LARGURA_PREDIO) / 2) + j);
            // Verifica se a linha 'i' do prédio realmente existe no nosso array 'PREDIO'.
            if (i >= 0 && i < ALTURA_PREDIO) {
                // Copia o caractere do prédio para o buffer do console.
                bufferConsole[indice].Char.UnicodeChar = PREDIO[i][j];
                // Define os atributos de cor para o caractere do prédio.
                bufferConsole[indice].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY; // Cor amarela/dourada
            }
        }
    }
}

// Desenha o Homem-Aranha no buffer.
void DesenharMiranha(){
    // Verifica se o Homem-Aranha não está caindo.
    if(miranha_cair == false){
        // Se não está caindo, desenha o sprite normal.
        // Loop pelas linhas do sprite.
        for(int i = 0; i < ALTURA_MIRANHA; ++i){
            // Loop pelas colunas do sprite.
            for(int j = 0; j < LARGURA_MIRANHA; ++j){
                // Condição para não desenhar os espaços vazios do sprite, otimizando a aparência.
                if (CORPO_MIRANHA[i][j] != ' ' || (i == 0 && j == 2) || (i == 1 && j == 1)) {
                    // Calcula o índice no buffer do console.
                    int indice = (miranha.y + i + ALTURA_UI) * LARGURA_TELA + (miranha.x + j);
                    // Garante que o desenho não saia da área de jogo.
                    if ( (miranha.y + i) < ALTURA_JOGO) {
                        // Copia o caractere do sprite para o buffer.
                        bufferConsole[indice].Char.UnicodeChar = CORPO_MIRANHA[i][j];
                        // Define a cor.
                        bufferConsole[indice].Attributes = miranha.cor;
                    }
                }
            }
        }
    }
    else{ // Se o Homem-Aranha está caindo...
        // Desenha o sprite de queda.
        // Loop pelos caracteres do sprite de queda.
        for(int i = 0; i < LARGURA_MIRANHA; i++){
            // Calcula o índice no buffer.
            int indice = (miranha.y + ALTURA_UI) * LARGURA_TELA + miranha.x;
            // Garante que o desenho não saia da área de jogo.
             if (miranha.y < ALTURA_JOGO) {
                // Copia o caractere do sprite de queda para o buffer.
                bufferConsole[indice + i].Char.UnicodeChar = MIRANHA_CAINDO[i];
                // Define a cor.
                bufferConsole[indice + i].Attributes = miranha.cor;
             }
        }
    }
}

// Desenha o Duende Verde no buffer.
void DesenharDoente(){
    // Loop pelas linhas do sprite. O offset 'yDoenteTela' ajusta a posição vertical.
    for(int i = 0 + yDoenteTela; i < ALTURA_DOENTE; ++i){
        // Loop pelas colunas do sprite.
        for(int j = 0; j < LARGURA_DOENTE; ++j){
            // Condição para não desenhar os espaços vazios do sprite.
            if (DOENTE_VERDE[i][j] != ' ' || (i == 1 && j == 3) || (i == 0 && j == 2)) {
                // Calcula o índice no buffer.
                int indice = (doente.y + (i - yDoenteTela) + ALTURA_UI) * LARGURA_TELA + (doente.x + j);
                // Garante que o desenho não saia da área de jogo.
                if ( (doente.y + (i- yDoenteTela)) < ALTURA_JOGO){
                    // Copia o caractere do sprite para o buffer.
                    bufferConsole[indice].Char.UnicodeChar = DOENTE_VERDE[i][j];
                    // Define a cor.
                    bufferConsole[indice].Attributes = doente.cor;
                }
            }
        }
    }
}

// Desenha os inimigos das janelas no buffer.
void DesenharInimigos() {
    // Loop por todos os inimigos possíveis.
    for (int i = 0; i < MAX_INIMIGOS_JANELA; i++) {
        // Verifica se o inimigo atual está ativo.
        if (inimigos[i].ativo) {
            // Calcula a posição Y na tela, subtraindo o scroll do prédio.
            int inimigoTelaY = inimigos[i].y - yPredioTela;
            // Calcula a posição X na tela, adicionando o offset para centralizar o prédio.
            int inimigoTelaX = inimigos[i].x + ((LARGURA_TELA - LARGURA_PREDIO) / 2);

            // Loop pelas linhas do sprite do inimigo.
            for (int linha = 0; linha < ALTURA_INIMIGO_JANELA; linha++) {
                // Loop pelas colunas do sprite do inimigo.
                for (int coluna = 0; coluna < LARGURA_INIMIGO_JANELA; coluna++) {
                    // Não desenha os espaços vazios do sprite.
                    if (INIMIGO_JANELA_SPRITE[linha][coluna] != ' ') {
                        // Calcula as coordenadas finais na tela.
                        int finalY = inimigoTelaY + linha + ALTURA_UI;
                        int finalX = inimigoTelaX + coluna;
                        // Verifica se as coordenadas estão dentro dos limites da tela.
                        if (finalY >= ALTURA_UI && finalY < ALTURA_JOGO + ALTURA_UI && finalX >= 0 && finalX < LARGURA_TELA) {
                            // Calcula o índice no buffer.
                            int indice = finalY * LARGURA_TELA + finalX;
                            // Copia o caractere do sprite para o buffer.
                            bufferConsole[indice].Char.UnicodeChar = INIMIGO_JANELA_SPRITE[linha][coluna];
                            // Define a cor (vermelho intenso).
                            bufferConsole[indice].Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
                        }
                    }
                }
            }
        }
    }
}

// Desenha as bombas no buffer.
void DesenharBombas() {
    // Loop por todas as bombas possíveis.
    for (int i = 0; i < MAX_BOMBAS; i++) {
        // Verifica se a bomba está ativa.
        if (bombas[i].ativo) {
            // Calcula a posição Y na tela, considerando o scroll e a UI.
            int bombaTelaY = bombas[i].y - yPredioTela + ALTURA_UI;
            // Calcula a posição X na tela, considerando a centralização do prédio.
            int bombaTelaX = bombas[i].x + ((LARGURA_TELA - LARGURA_PREDIO) / 2);
            // Verifica se a bomba está dentro da área visível.
            if (bombaTelaY >= ALTURA_UI && bombaTelaY < ALTURA_JOGO + ALTURA_UI && bombaTelaX >= 0 && bombaTelaX < LARGURA_TELA) {
                // Calcula o índice no buffer.
                int indice = bombaTelaY * LARGURA_TELA + bombaTelaX;
                // Define o caractere da bomba.
                bufferConsole[indice].Char.UnicodeChar = '@';
                // Define a cor (vermelho intenso).
                bufferConsole[indice].Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
            }
        }
    }
}

// Desenha a teia do Homem-Aranha no buffer.
void DesenharTeia() {
    // Verifica se o jogador está no processo de disparar a teia.
    if (soltandoTeia && direcaoAtualTeia != ANCORA_LOCAL) {
        // Usa um switch para desenhar a teia na direção correta.
        switch (direcaoAtualTeia) {
            case CIMA:
                // Desenha uma linha vertical para cima.
                for (int i = 1; i <= comprimentoTeiaVoo; i++){
                    if (miranha.y - i >= 0){
                        int indice = (miranha.y - i + ALTURA_UI) * LARGURA_TELA + (miranha.x + 2);
                        bufferConsole[indice].Char.UnicodeChar = '0';
                        bufferConsole[indice].Attributes = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
                    }
                }
                break;
            case DIAGONAL_ESQUERDA:
                // Desenha uma linha diagonal para a esquerda.
                for (int i = 1; i <= comprimentoTeiaVoo; i++){
                    if (miranha.y - i >= 0 && miranha.x + 1 - i >= 0){
                        int indice = (miranha.y - i + ALTURA_UI) * LARGURA_TELA + (miranha.x + 1 - i);
                        bufferConsole[indice].Char.UnicodeChar = '0';
                        bufferConsole[indice].Attributes = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
                    }
                }
                break;
            case DIAGONAL_DIREITA:
                // Desenha uma linha diagonal para a direita.
                for (int i = 1; i <= comprimentoTeiaVoo; i++){
                    if (miranha.y - i >= 0 && miranha.x + 2 + i < LARGURA_TELA){
                        int indice = (miranha.y - i + ALTURA_UI) * LARGURA_TELA + (miranha.x + 2 + i);
                        bufferConsole[indice].Char.UnicodeChar = '0';
                        bufferConsole[indice].Attributes = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
                    }
                }
                break;
            default: break; // Caso padrão, não faz nada.
        }
    }
    // Se a teia não está sendo disparada, mas está ancorada...
    else if (teiaAncorada) {
        // Calcula a posição da âncora na tela.
        int ancoraTelaY = teiaAncoraPredioY - yPredioTela;
        int ancoraTelaX = teiaAncoraPredioX + ((LARGURA_TELA - LARGURA_PREDIO) / 2);

        // Define os pontos inicial (Homem-Aranha) e final (âncora) da linha.
        int x0 = miranha.x + 2;
        int y0 = miranha.y;
        int x1 = ancoraTelaX;
        int y1 = ancoraTelaY;

        // Implementação do Algoritmo de Linha de Bresenham para desenhar a teia.
        // Este algoritmo é eficiente para desenhar linhas em uma grade de pixels (ou caracteres).
        int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
        int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
        int err = dx + dy, e2;

        // Loop para desenhar cada ponto da linha.
        while (true) {
            // Verifica se o ponto atual está dentro da área de jogo.
            if (y0 >= 0 && y0 < ALTURA_JOGO && x0 >= 0 && x0 < LARGURA_TELA) {
                //   Desenha um caractere de teia no ponto.
                int indice = (y0 + ALTURA_UI) * LARGURA_TELA + x0;
                bufferConsole[indice].Char.UnicodeChar = '.';
                bufferConsole[indice].Attributes = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            }
            // Condição de parada: a linha chegou ao fim.
                if (x0 == x1 && y0 == y1){
                    break; // Sai do loop se chegou ao ponto final.
                }
            // Lógica do algoritmo de Bresenham para decidir o próximo ponto.
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

// Desenha a Interface do Usuário (UI) no buffer.
void DesenharUI() {
    // --- Desenha a Pontuação ---
    char textoPontos[30]; // Cria uma string para formatar o texto.
    sprintf(textoPontos, "PONTOS: %d", pontuacao); // Formata a string com a pontuação atual.
    int startX = (LARGURA_TELA - strlen(textoPontos)) / 2; // Calcula a posição inicial para centralizar o texto.
    // Loop para escrever o texto no buffer, na linha superior.
    for (int i = 0; i < strlen(textoPontos); i++) {
        bufferConsole[i + startX].Char.UnicodeChar = textoPontos[i];
    }

    // --- Desenha Vidas e Tempo ---
    int linhaInferiorY = ALTURA_JOGO + ALTURA_UI; // Define a linha Y para a UI inferior.

    // Desenha as vidas.
    char textoVidas[15];
    sprintf(textoVidas, "VIDAS: %d", vidas); // Formata a string de vidas.
    // Loop para escrever o texto no buffer, no canto inferior esquerdo.
    for (int i = 0; i < strlen(textoVidas); i++) {
        int indice = linhaInferiorY * LARGURA_TELA + i;
        bufferConsole[indice].Char.UnicodeChar = textoVidas[i];
    }

    // Desenha a barra de tempo.
    // Loop para desenhar um '#' para cada unidade de tempo restante.
    for (int i = 0; i < tempoRestante; i++) {
        int x = LARGURA_TELA - TEMPO_MAXIMO - 1 + i; // Calcula a posição X no canto inferior direito.
        int indice = linhaInferiorY * LARGURA_TELA + x;
        bufferConsole[indice].Char.UnicodeChar = '#'; // Caractere da barra de tempo.
        bufferConsole[indice].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY; // Cor da barra.
    }
}

// Função principal de desenho que organiza e exibe tudo.
void DesenharTela(){
    // 1. Limpa o buffer: Preenche todo o buffer com espaços vazios.
    for(int i = 0; i < LARGURA_TELA * (ALTURA_JOGO + 2); ++i){
        bufferConsole[i].Char.UnicodeChar = ESPACO_VAZIO;
        // Define uma cor de fundo padrão (branco).
        bufferConsole[i].Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
    }
    // 2. Desenha os elementos em ordem de "camadas" (do fundo para a frente).
    DesenharUI();       // UI fica no fundo para não sobrepor o jogo.
    DesenharPredio();   // O prédio é o fundo principal.
    DesenharBombas();   // Bombas sobre o prédio.
    DesenharInimigos(); // Inimigos sobre o prédio.
    // Condicional para desenhar a teia.
    if (soltandoTeia || teiaAncorada) {
        DesenharTeia();
    }
    DesenharDoente();   // O Duende Verde.
    DesenharMiranha();  // O Homem-Aranha é a camada mais à frente.

    // 3. Escreve o buffer inteiro na tela do console de uma só vez.
    // Esta é a função chave para uma animação suave.
    WriteConsoleOutputW(console, bufferConsole, tamanhoBuffer, posicaoCaractere, &areaEscritaConsole);
}

// =================================================================================
// FUNÇÕES DE LÓGICA E CONTROLE
// Gerenciam a entrada do jogador, a movimentação dos personagens e as regras do jogo.
// =================================================================================

// Processa a entrada do jogador relacionada ao uso da teia.
void ControlarTeia() {
    // Verifica se a tecla de espaço está pressionada.
    if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
        // Se a teia ainda não foi disparada e o jogador não está em queda fatal...
        if (!soltandoTeia && !quedaFatal) {
            teiaAncorada = false; // Garante que qualquer teia anterior seja solta.
            direcaoAtualTeia = NENHUMA; // Reseta a direção.

            // Verifica as teclas direcionais para definir a direção do disparo.
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

            // Se uma direção foi escolhida...
            if (direcaoAtualTeia != NENHUMA) {
                soltandoTeia = true;        // Ativa o estado de "disparando teia".
                tempoSegurandoEspaco = 0;   // Inicia o contador de tempo.
                comprimentoTeiaVoo = 1;     // A teia começa com comprimento 1.
            }
        }
        else { // Se o jogador continua segurando espaço...
            // Aumenta o comprimento da teia com base no tempo que a tecla é segurada.
            if (direcaoAtualTeia != ANCORA_LOCAL) {
                tempoSegurandoEspaco += ATRASO_TIQUE; // Incrementa o tempo.
                // Lógica para aumentar o comprimento da teia em estágios.
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
    // Se a tecla de espaço foi solta enquanto uma teia estava sendo disparada...
    else if (soltandoTeia) {
        soltandoTeia = false; // Desativa o estado de "disparando".

        // Variáveis para verificar colisões da teia.
        int teia_x_final_tela = -1, teia_y_final_tela = -1;
        bool colidiu = false;

        // Loop por cada segmento da teia disparada para verificar colisão.
        for (int i = 1; i <= comprimentoTeiaVoo; i++) {
            int teia_x_tela = -1, teia_y_tela = -1;
            // Calcula a posição de cada segmento.
            switch (direcaoAtualTeia) {
                case CIMA: teia_y_tela = miranha.y - i; teia_x_tela = miranha.x + 2; break;
                case DIAGONAL_ESQUERDA: teia_y_tela = miranha.y - i; teia_x_tela = miranha.x + 1 - i; break;
                case DIAGONAL_DIREITA: teia_y_tela = miranha.y - i; teia_x_tela = miranha.x + 2 + i; break;
                default: break;
            }

            // Armazena a posição final da ponta da teia.
            if (i == comprimentoTeiaVoo) {
                teia_x_final_tela = teia_x_tela;
                teia_y_final_tela = teia_y_tela;
            }

            // Se a posição do segmento é válida...
            if(teia_x_tela != -1) {
                // Verifica colisão com inimigos.
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
                if(colidiu) break; // Sai do loop se já colidiu.
                // Verifica colisão com bombas.
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
                if(colidiu) break; // Sai do loop se já colidiu.
            }
        }

        if (colidiu) {
            miranha_cair = true; // Se a teia acertou um inimigo, o Homem-Aranha perde o balanço e cai.
        } else if (direcaoAtualTeia == ANCORA_LOCAL) {
            // Lógica para ancorar a teia no local atual do Homem-Aranha.
            int teiaTelaY = miranha.y;
            int teiaTelaX = miranha.x + 2;
            int predioY = teiaTelaY + yPredioTela;
            int predioX = teiaTelaX - ((LARGURA_TELA - LARGURA_PREDIO) / 2);

            // Verifica se há uma parede ('|') no local para ancorar.
            if (predioY >= 0 && predioY < ALTURA_PREDIO && predioX >= 0 && predioX < LARGURA_PREDIO && PREDIO[predioY][predioX] == '|') {
                teiaAncorada = true;
                teiaAncoraPredioX = predioX;
                teiaAncoraPredioY = predioY;
                tamanhoAtualTeia = 0; // A teia começa totalmente recolhida.
                if (miranha_cair) { // Se estava caindo, se salva.
                    // Ajusta a câmera e a posição do Homem-Aranha.
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
            } else { // Se não conseguiu ancorar...
                if (!miranha_cair) {
                    miranha_cair = true; // Começa a cair.
                }
            }
        } else { // Lógica para ancorar a teia na ponta.
            int predioY = teia_y_final_tela + yPredioTela;
            int predioX = teia_x_final_tela - ((LARGURA_TELA - LARGURA_PREDIO) / 2);

            // Verifica se a ponta da teia acertou uma parede.
            if (direcaoAtualTeia != NENHUMA && predioY >= 0 && predioY < ALTURA_PREDIO && predioX >= 0 && predioX < LARGURA_PREDIO && PREDIO[predioY][predioX] == '|') {
                teiaAncorada = true;
                teiaAncoraPredioX = predioX;
                teiaAncoraPredioY = predioY;
                tamanhoAtualTeia = comprimentoTeiaVoo; // O tamanho inicial é o comprimento do disparo.
                if (miranha_cair) { // Se estava caindo, se salva.
                    // Ajusta a câmera e a posição.
                    yPredioTela = yPredioTela + (miranha.y - 7);
                    if (yPredioTela > (ALTURA_PREDIO - ALTURA_JOGO)) yPredioTela = ALTURA_PREDIO - ALTURA_JOGO;
                    if (yPredioTela < 0) yPredioTela = 0;
                    miranha.y = 7;
                    miranha_cair = false;
                }
            } else { // Se errou a parede...
                if (!miranha_cair) {
                    miranha_cair = true; // Começa a cair.
                }
            }
        }
        // Reseta o estado de disparo da teia.
        direcaoAtualTeia = NENHUMA;
        tempoSegurandoEspaco = 0;
        comprimentoTeiaVoo = 0;
    }
}

// Controla a movimentação do Homem-Aranha quando a teia está ancorada.
void MovimentacaoMiranha() {
    // Só executa se a teia estiver ancorada.
    if (teiaAncorada) {
        int ancoraTelaY = teiaAncoraPredioY - yPredioTela;
        int ancoraTelaX = teiaAncoraPredioX + ((LARGURA_TELA - LARGURA_PREDIO) / 2);

        // Se a tecla CIMA for pressionada e a teia não estiver totalmente recolhida...
        if ((GetAsyncKeyState(VK_UP) & 0x8000) && tamanhoAtualTeia > 0) {
            // Lógica para subir: move o Homem-Aranha ou a câmera.
            if (miranha.y > 7) {
                miranha.y--;
            }
            else if (yPredioTela > 0) {
                yPredioTela--;
            }
            else {
                miranha.y--;
            }

            // Ajusta a posição X para simular o balanço.
            if (miranha.x + 2 < ancoraTelaX) {
                miranha.x++;
            }
            if (miranha.x + 2 > ancoraTelaX) {
                miranha.x--;
            }

            tamanhoAtualTeia--; // Diminui o comprimento da teia.
            if (tamanhoAtualTeia == 0) {
                teiaAncorada = false; // Solta a teia se ela for totalmente recolhida.
            }
        }
        // Se a tecla BAIXO for pressionada e a teia não estiver no comprimento máximo...
        else if ((GetAsyncKeyState(VK_DOWN) & 0x8000) && tamanhoAtualTeia < 5 && miranha.y <= 7) {
            // Lógica para descer: move o Homem-Aranha ou a câmera.
            if (miranha.y < 7) {
                miranha.y++;
            } else if ((yPredioTela + ALTURA_JOGO) < ALTURA_PREDIO) {
                yPredioTela++;
            } else if (miranha.y < ALTURA_JOGO - ALTURA_MIRANHA) {
                miranha.y++;
            }

            // Ajusta a posição X para simular o balanço.
            if (miranha.x > 0 && miranha.x + 2 > ancoraTelaX) {
                miranha.x--;
            }
            if (miranha.x < LARGURA_TELA - LARGURA_MIRANHA && miranha.x + 2 < ancoraTelaX) {
                miranha.x++;
            }

            tamanhoAtualTeia++; // Aumenta o comprimento da teia.
        }
    }
}

// Controla a movimentação do Duende Verde.
void MovimentarDoente(){
    // Ajusta a posição Y do Duende Verde com base no scroll do prédio.
    if(yPredioTela == 0){ doente.y = 4; yDoenteTela = 0; }
    else if(yPredioTela == 1){ doente.y = 3; yDoenteTela = 0; }
    else if(yPredioTela == 2){ doente.y = 2; yDoenteTela = 0; }
    else if(yPredioTela == 3){ doente.y = 1; yDoenteTela = 0; }
    else if(yPredioTela == 4){ doente.y = 0; yDoenteTela = 0; }
    else if(yPredioTela == 5){ yDoenteTela = 1; doente.y = 0; }
    else if(yPredioTela == 6){ yDoenteTela = 2; doente.y = 0; }
    else if(yPredioTela == 7){ yDoenteTela = 3; doente.y = 0; }

    // Lógica de movimento horizontal: vai e volta.
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

// Gerencia o surgimento de novos inimigos nas janelas.
void GerenciarSpawnsInimigos() {
    // Chance de 3% de tentar gerar um inimigo a cada tique.
    if (rand() % 100 > 2) {
        return;
    }

    // Conta quantos inimigos já estão ativos.
    int inimigosAtivos = 0;
    for (int i = 0; i < MAX_INIMIGOS_JANELA; i++) {
        if (inimigos[i].ativo) {
            inimigosAtivos++;
        }
    }

    // Se houver espaço para mais inimigos...
    if (inimigosAtivos < MAX_INIMIGOS_JANELA) {
        // Encontra um "slot" vazio no array de inimigos.
        int slotVazio = -1;
        for (int i = 0; i < MAX_INIMIGOS_JANELA; i++) {
            if (!inimigos[i].ativo) {
                slotVazio = i;
                break;
            }
        }

        // Se encontrou um slot...
        if (slotVazio != -1) {
            // Cria uma lista de janelas disponíveis na tela.
            COORD janelasDisponiveis[NUM_POSICOES_JANELAS];
            int countDisponiveis = 0;

            for (int i = 0; i < NUM_POSICOES_JANELAS; i++) {
                // Verifica se a janela está na área visível.
                bool naTela = posicoesJanelas[i].Y >= yPredioTela && posicoesJanelas[i].Y < yPredioTela + ALTURA_JOGO;
                if (naTela) {
                    // Verifica se a janela já está ocupada por outro inimigo.
                    bool posicaoOcupada = false;
                    for (int j = 0; j < MAX_INIMIGOS_JANELA; j++) {
                        if (inimigos[j].ativo && inimigos[j].x == posicoesJanelas[i].X + 1 && inimigos[j].y == posicoesJanelas[i].Y + 2) {
                            posicaoOcupada = true;
                            break;
                        }
                    }
                    if (!posicaoOcupada) {
                        // Adiciona a janela à lista de disponíveis.
                        janelasDisponiveis[countDisponiveis] = posicoesJanelas[i];
                        countDisponiveis++;
                    }
                }
            }

            // Se houver janelas disponíveis...
            if (countDisponiveis > 0) {
                // Escolhe uma aleatoriamente e cria o inimigo.
                int posIndex = rand() % countDisponiveis;
                inimigos[slotVazio].ativo = true;
                inimigos[slotVazio].x = janelasDisponiveis[posIndex].X + 1;
                inimigos[slotVazio].y = janelasDisponiveis[posIndex].Y + 2;
                inimigos[slotVazio].tempoNoLugar = 0;
            }
        }
    }
}

// Gerencia o surgimento de novas bombas no prédio.
void GerenciarSpawnsBombas() {
    // Chance de 2% de tentar gerar uma bomba a cada tique.
    if (rand() % 100 > 1) {
        return;
    }
    // Lógica similar à dos inimigos para encontrar um slot vazio e uma posição disponível.
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

            // Percorre as linhas do prédio onde as bombas podem aparecer.
            for (int y = 4; y <= 22; y++) {

                // Percorre as colunas do prédio.
                for (int x = 0; x < LARGURA_PREDIO; x++) {
                    // Só pode colocar bomba em uma parede ('|').
                    if (PREDIO[y][x] == '|') {
                        bool naTela = y >= yPredioTela && y < yPredioTela + ALTURA_JOGO;
                        if (naTela) {
                            bool ocupada = false; // Verifica se já tem outra bomba.
                            for(int j = 0; j < MAX_BOMBAS; j++) {
                                if(bombas[j].ativo && bombas[j].x == x && bombas[j].y == y) {
                                    ocupada = true;
                                    break;
                                }
                            }
                            bool ocupadaPeloMiranha = false; // Verifica se o Homem-Aranha está no local.
                            int bombaTelaX = x + ((LARGURA_TELA - LARGURA_PREDIO) / 2);
                            int bombaTelaY = y - yPredioTela;
                            if(bombaTelaX >= miranha.x && bombaTelaX < miranha.x + LARGURA_MIRANHA &&
                               bombaTelaY >= miranha.y && bombaTelaY < miranha.y + ALTURA_MIRANHA) {
                                ocupadaPeloMiranha = true;
                            }

                            if(!ocupada && !ocupadaPeloMiranha) { // Se o local está livre...
                                // Adiciona à lista de posições disponíveis.
                                bombasDisponiveis[countDisponiveis].X = x;
                                bombasDisponiveis[countDisponiveis].Y = y;
                                countDisponiveis++;
                            }
                        }
                    }
                }
            }

            if(countDisponiveis > 0) { // Se achou um lugar...
                // Escolhe um aleatoriamente e cria a bomba.
                int posIndex = rand() % countDisponiveis;
                bombas[slotVazio].ativo = true;
                bombas[slotVazio].x = bombasDisponiveis[posIndex].X;
                bombas[slotVazio].y = bombasDisponiveis[posIndex].Y;
                bombas[slotVazio].tempoNoLugar = 0;
            }
        }
    }
}

// Atualiza o estado dos inimigos.
void AtualizarInimigos() {
    for (int i = 0; i < MAX_INIMIGOS_JANELA; i++) {
        if (inimigos[i].ativo) {
            inimigos[i].tempoNoLugar += ATRASO_TIQUE; // Incrementa o temporizador.
            // Se o inimigo ficou 4 segundos no mesmo lugar...
            if (inimigos[i].tempoNoLugar >= 4000) {
                // Tenta encontrar uma nova janela vazia para se mover.
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
                    if (!posicaoOcupada) { // Se encontrou uma vazia...
                        // Move o inimigo para a nova posição.
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

// Atualiza o estado das bombas.
void AtualizarBombas() {
    for (int i = 0; i < MAX_BOMBAS; i++) {
        if (bombas[i].ativo) {
            bombas[i].tempoNoLugar += ATRASO_TIQUE; // Incrementa o temporizador.
            // Se a bomba ficou 4 segundos ativa...
            if (bombas[i].tempoNoLugar >= 4000) {
                bombas[i].ativo = false; // Ela desaparece (é desarmada).
            }
        }
    }
}

// Verifica as condições que fazem o Homem-Aranha cair.
void MiranhaCair(){
    // Condição 1: Saiu pelos lados do prédio.
    if(miranha.x <= (((LARGURA_TELA - LARGURA_PREDIO) / 2) - 1) || miranha.x >= (LARGURA_PREDIO + ((LARGURA_TELA - LARGURA_PREDIO) / 2)) - LARGURA_MIRANHA){
        miranha_cair = true;
    }
    // Condição 2: Não está ancorado, não está soltando teia e não está tocando o prédio.
    if (!teiaAncorada && !soltandoTeia && !miranha_cair) {
        bool tocandoPredio = false;
        // Verifica se qualquer parte do sprite do Homem-Aranha está sobre uma parede.
        for (int i = 0; i < ALTURA_MIRANHA; i++) {
            for (int j = 0; j < LARGURA_MIRANHA; j++) {
                if (CORPO_MIRANHA[i][j] == ' ') { continue; } // Pula os espaços vazios do sprite.
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

        if (!tocandoPredio) { // Se não está tocando...
            miranha_cair = true; // Cai.
        }
    }
    // Se está no estado de queda...
    if (miranha_cair) {
        teiaAncorada = false; // Solta qualquer teia.
        if((ALTURA_JOGO + yPredioTela) < 65){ // Se não chegou no chão...
            yPredioTela++; // A câmera "sobe" (o cenário "desce"), simulando a queda.
        }
    }
}

// Verifica se o Homem-Aranha em queda atingiu o chão.
void MiranhaBaterChao(){
    // Se está caindo e a câmera já mostrou todo o prédio...
    if(miranha_cair == true && (ALTURA_JOGO + yPredioTela) == 65){
        miranha.y++; // O Homem-Aranha continua caindo para fora da tela.
        if(miranha.y >= ALTURA_JOGO - ALTURA_MIRANHA){
            PerderVida(); // Perde uma vida ao sair da tela por baixo.
        }
    }
}

// Garante que o Homem-Aranha não saia dos limites da tela.
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

// Verifica todas as colisões entre as entidades do jogo.
void VerificarColisoes() {
    if (quedaFatal) {
        return;
    } // Não verifica colisões se estiver em queda fatal.

    // Define as "hitboxes" (retângulos de colisão) do Duende e do Miranha.
    int doente_x1 = doente.x, doente_y1 = doente.y, doente_x2 = doente.x + LARGURA_DOENTE, doente_y2 = doente.y + ALTURA_DOENTE;
    int miranha_x1 = miranha.x, miranha_y1 = miranha.y, miranha_x2 = miranha.x + LARGURA_MIRANHA, miranha_y2 = miranha.y + ALTURA_MIRANHA;

    // Colisão Miranha vs. Duende (teste de intersecção de retângulos).
    if (miranha_x1 < doente_x2 && miranha_x2 > doente_x1 && miranha_y1 < doente_y2 && miranha_y2 > doente_y1) {
        miranha_cair = true;
        return;
    }

    // Colisão Miranha vs. Inimigos da Janela.
    for (int i = 0; i < MAX_INIMIGOS_JANELA; i++) {
        if (inimigos[i].ativo) {
            int inimigoTelaY = inimigos[i].y - yPredioTela, inimigoTelaX = inimigos[i].x + ((LARGURA_TELA - LARGURA_PREDIO) / 2);
            int inimigo_x1 = inimigoTelaX, inimigo_y1 = inimigoTelaY, inimigo_x2 = inimigoTelaX + LARGURA_INIMIGO_JANELA, inimigo_y2 = inimigoTelaY + ALTURA_INIMIGO_JANELA;
            if (miranha_x1 < inimigo_x2 && miranha_x2 > inimigo_x1 && miranha_y1 < inimigo_y2 && miranha_y2 > inimigo_y1) {
                pontuacao += 30; // Ganha pontos.
                inimigos[i].ativo = false;
                return; // Inimigo derrotado.
            }
        }
    }

    // Colisão Miranha vs. Bombas.
    for(int i = 0; i < MAX_BOMBAS; i++) {
        if(bombas[i].ativo) {
            int bombaTelaY = bombas[i].y - yPredioTela;
            int bombaTelaX = bombas[i].x + ((LARGURA_TELA - LARGURA_PREDIO) / 2);
            if(miranha_x1 < bombaTelaX + 1 && miranha_x2 > bombaTelaX && miranha_y1 < bombaTelaY + 1 && miranha_y2 > bombaTelaY) {
                pontuacao += 80; // Ganha pontos.
                bombas[i].ativo = false;
                return; // Bomba desarmada.
            }
        }
    }

    // Colisão da Teia (quando ancorada) com outras entidades.
    if (teiaAncorada) {
        // Usa o algoritmo de Bresenham novamente para percorrer a linha da teia.
        int ancoraTelaY = teiaAncoraPredioY - yPredioTela, ancoraTelaX = teiaAncoraPredioX + ((LARGURA_TELA - LARGURA_PREDIO) / 2);
        int x0 = miranha.x + 2, y0 = miranha.y;
        int x1 = ancoraTelaX, y1 = ancoraTelaY;
        int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
        int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
        int err = dx + dy, e2;
        while (true) {
            // Verifica se algum ponto da teia toca o Duende.
            if (yPredioTela <= 4) {
                if (x0 >= doente_x1 && x0 < doente_x2 && y0 >= doente_y1 && y0 < doente_y2) {
                    miranha_cair = true;
                    return;

                }
            }
            // Verifica se algum ponto da teia toca um inimigo.
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
            // Verifica se algum ponto da teia toca uma bomba.
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

    // Verifica se o Miranha alcançou o topo do prédio (objetivo).
    for (int i = 0; i < ALTURA_MIRANHA; ++i) {
        for (int j = 0; j < LARGURA_MIRANHA; ++j) {
            if (CORPO_MIRANHA[i][j] != ' ') {
                int miranhaTelaX = miranha.x + j, miranhaTelaY = miranha.y + i;
                int predioY = miranhaTelaY + yPredioTela, predioX = miranhaTelaX - ((LARGURA_TELA - LARGURA_PREDIO) / 2);
                if (predioY >= 0 && predioY < ALTURA_PREDIO && predioX >= 0 && predioX < LARGURA_PREDIO) {
                    // O caractere '#' marca o objetivo.
                    if (PREDIO[predioY][predioX] == '#') {
                        VencerNivel(); // Venceu!
                        return;
                    }
                }
            }
        }
    }
}

// Atualiza o temporizador do jogo.
void AtualizarTempo() {
    acumuladorTempo += ATRASO_TIQUE; // Acumula o tempo de cada tique.
    // Quando o tempo acumulado atinge o limite...
    if (acumuladorTempo >= DECREMENTO_TEMPO) {
        tempoRestante--;   // Decrementa uma barra de tempo.
        acumuladorTempo = 0; // Reseta o acumulador.
    }
    // Se o tempo acabar...
    if (tempoRestante <= 0 && !miranha_cair) {
        miranha_cair = true; // Força o Homem-Aranha a cair.
        quedaFatal = true;   // Marca a queda como fatal (não pode ser salva com a teia).
    }
}

// =================================================================================
// FUNÇÃO PRINCIPAL (main)
// O ponto de entrada do programa e o loop principal do jogo.
// =================================================================================
int main(){
    // Inicializa o gerador de números aleatórios com a hora atual, para que os spawns sejam diferentes a cada jogo.
    srand(time(NULL));
    // Configura o console para exibir corretamente caracteres UTF-8 (como os usados no prédio).
    SetConsoleOutputCP(CP_UTF8);

    InicializarJogo(); // Prepara o jogo para começar.
    // Obtém o "handle" do console, necessário para as funções de desenho.
    console = GetStdHandle(STD_OUTPUT_HANDLE);

    // O loop principal do jogo. Roda indefinidamente até que o programa seja fechado.
    while (true)
    {
        // --- SEÇÃO DE ATUALIZAÇÃO (LÓGICA) ---
        // A ordem aqui é importante.
        GerenciarSpawnsInimigos(); // Tenta criar novos inimigos.
        GerenciarSpawnsBombas();   // Tenta criar novas bombas.

        AtualizarInimigos();       // Atualiza a posição dos inimigos existentes.
        AtualizarBombas();         // Atualiza o estado das bombas existentes.
        AtualizarTempo();          // Atualiza o cronômetro.

        ControlarTeia();           // Processa a entrada do jogador para a teia.

        MiranhaCair();             // Verifica se o Homem-Aranha deve cair.
        MovimentacaoMiranha();     // Processa a movimentação do Homem-Aranha na teia.

        MovimentarDoente();        // Move o Duende Verde.

        VerificarColisoes();       // Checa todas as colisões.

        CorrigirPosicaoMiranha();  // Garante que o Homem-Aranha não saia da tela.
        MiranhaBaterChao();        // Verifica se o Homem-Aranha caiu no chão.

        // --- SEÇÃO DE DESENHO ---
        DesenharTela(); // Desenha tudo que foi atualizado.

        // Pausa o jogo por um curto período para controlar a velocidade.
        Sleep(ATRASO_TIQUE);
    }
    return 0; // Fim do programa (teoricamente nunca alcançado devido ao loop infinito).
}

