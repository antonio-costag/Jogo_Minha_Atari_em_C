#include <stdio.h>
#include <windows.h>

//com essa biblioteca a função precisa seguir um formato em especifico pra poder criar uma thread.
//DWORD: É um tipo definido pela API do Windows, significa "Double Word" (palavra dupla), ou seja, um inteiro sem 
// sinal de 32 bits (unsigned long).

//WINAPI: É uma macro que define a convenção de chamada de função usada pela API do Windows 
// (__stdcall). Isso garante que os parâmetros sejam passados corretamente entre o Windows e suas funções.
DWORD WINAPI computador1(LPVOID lpParam){
    Beep(100, 800);
    return 0;
}

DWORD WINAPI computador2(LPVOID lpParam){
    Beep(500, 200);
    return 0;
}

int main(){

    //HANDLE: Também definido pela API do Windows, é um tipo genérico de ponteiro usado para 
    // identificar objetos do sistema operacional, como arquivos, threads, processos, etc.
    HANDLE thread1;
    HANDLE thread2;

    // Cria a primeira thread, que executará a função 'computador1'.
    // NULL: atributos de segurança padrão.
    // 0: tamanho de pilha padrão.
    // computador1: ponteiro para a função da thread.
    // NULL: nenhum parâmetro para a função da thread.
    // 0: a thread executa imediatamente após a criação.
    // NULL: o identificador da thread não é retornado.
    thread1 = CreateThread(NULL, 0, computador1, NULL, 0, NULL);
    thread2 = CreateThread(NULL, 0, computador2, NULL, 0, NULL);

    // Espera a primeira thread terminar sua execução antes de continuar.
    // INFINITE significa que vai esperar o tempo que for necessário.
    WaitForSingleObject(thread1, INFINITE);
    WaitForSingleObject(thread2, INFINITE);

    return 0;
}