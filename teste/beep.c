#include <stdio.h>
#include <windows.h> // Biblioteca padrão do Windows que contém a função Beep()

int main(void) {
    printf("Tocando uma melodia simples no Windows...\n");

    while(1){
        // Beep(frequência_em_Hz, duração_em_milissegundos);
    
        // Uma melodia simples: Dó, Ré, Mi, Fá
        Beep(261, 500); // Dó (C4)
        Sleep(50);      // Pequena pausa
        Beep(293, 500); // Ré (D4)
        Sleep(50);
        Beep(329, 500); // Mi (E4)
        Sleep(50);
        Beep(349, 500); // Fá (F4)
        Sleep(50);
        Beep(392, 500); // Sol (G4)
        Sleep(50);
        Beep(440, 500); // La (A4)
        Sleep(50);
        Beep(493, 500); // Si (B4)
    }

    printf("Melodia finalizada.\n");

    return 0;
}