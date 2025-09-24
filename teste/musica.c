#include <stdio.h>
#include <windows.h>

int main(void) {
    printf("Tocando uma melodia simples no Windows...\n");

    int frequencia_parte1[] = {330, 370, 392, 392, 370, 330, 0, 330, 370, 392, 330, 370, 392};
    int duracao_parte1[] = {150, 150, 300, 150, 150, 300, 100, 150, 150, 150, 150, 150, 300};

    int frequencia_parte2[] = {523, 523, 493, 440, 440, 392};
    int duracao_parte2[] = {300, 150, 150, 300, 150, 450};

    while(1){
        for(int i = 0; i < 13; i++){
            Beep(frequencia_parte1[i], duracao_parte1[i]);
            Sleep(50);
        }

        for(int i = 0; i < 6; i++){
            Beep(frequencia_parte2[i], duracao_parte2[i]);
            Sleep(50);
        }
    }

    printf("Melodia finalizada.\n");

    return 0;
}