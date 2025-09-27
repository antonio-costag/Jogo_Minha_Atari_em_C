#include <stdio.h>
#include <windows.h> // Biblioteca padrão do Windows que contém a função Beep()

int main(void) {
    printf("Tocando uma melodia simples no Windows...\n");

    /*while(1){
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
        Beep(493, 500); // Si (B4)/
    }*/

    while (1)
    {
        // Início rápido e um pouco mais agudo para chamar a atenção
        Beep(440, 150); // Nota A4
        Sleep(50);      // Pequena pausa

        // Sequência principal descendente, criando a sensação de "queda"
        Beep(392, 200); // Nota G4
        Beep(349, 200); // Nota F4
        Beep(330, 200); // Nota E4
        Beep(294, 400); // Nota D4 (um pouco mais longa)

        // Pausa dramática antes da conclusão
        Sleep(300);

        // Conclusão com notas graves e longas para dar peso e finalidade
        Beep(261, 500); // Nota C4 (grave)
        Sleep(150);     // Pausa curta antes do fim
        Beep(196, 1000); // Nota G3 (muito grave e longa, o "fim" definitivo)

        printf("Fim.\n");
    
        Sleep(5000); 
    }

    printf("Melodia finalizada.\n");

    return 0;
}