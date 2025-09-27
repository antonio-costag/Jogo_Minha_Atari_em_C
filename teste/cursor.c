#include <windows.h>
#include <stdio.h>

// Função para esconder o cursor
void hideCursor() {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(console, &cursorInfo);
    cursorInfo.bVisible = FALSE; // Define a visibilidade do cursor como falso
    SetConsoleCursorInfo(console, &cursorInfo);
}

// Função para mostrar o cursor novamente (opcional, mas útil)
void showCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(consoleHandle, &cursorInfo);
    cursorInfo.bVisible = TRUE; // Define a visibilidade do cursor como verdadeiro
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}


int main() {
    printf("O cursor esta visivel aqui.\n");
    
    // Pausa para o usuário ver o cursor
    system("pause");

    // Esconde o cursor
    hideCursor();

    printf("Agora o cursor esta escondido. Voce nao consegue ve-lo.\n");
    printf("Digite algo e pressione Enter para continuar (o cursor nao aparecera): ");
    
    // Aguarda o usuário digitar algo para provar que o console ainda funciona
    getchar();

    // Mostra o cursor novamente
    showCursor();

    printf("\nO cursor esta visivel novamente!\n");

    return 0;
}