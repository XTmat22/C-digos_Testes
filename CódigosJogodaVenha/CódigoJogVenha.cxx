#include <iostream>
#include <random>    // Para geração de números aleatórios
#include <cstdlib>   // Necessário para system("clear")
#include <termios.h> // Para manipulação do terminal no Linux
#include <unistd.h>  // Para a função read()
using namespace std;  

// Criando nossa própria função getch() para Linux
char getch() {
    char buf = 0;
    struct termios old = {0};
    
    // Pega as configurações atuais do terminal
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
        
    // Desativa o modo canônico (não precisa de Enter) e o echo (não mostra a tecla na tela)
    old.c_lflag &= ~ICANON; 
    old.c_lflag &= ~ECHO;   
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    
    // Aplica as novas configurações
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
        
    // Lê um único caractere
    if (read(0, &buf, 1) < 0)
        perror ("read()");
        
    // Restaura as configurações originais do terminal para não quebrar o console depois
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror ("tcsetattr ~ICANON");
        
    return buf;
}

int main() {
    char escolha[8]; // Variável para armazenar a escolha do jogador.
    int seed[5]; 
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis1(1,9); // Primeiro algarismo da seed se o algoritmo começar.
    uniform_int_distribution<> dis2(1,8); // Primeiro algarismo da seed se o algoritno não começar.
    uniform_int_distribution<> dis3(1,7); // Segundo algarismo da seed se o algoritmo começar.
    uniform_int_distribution<> dis4(1,6); // Segundo algarismo da seed se o algoritno não começar.
    uniform_int_distribution<> dis5(1,5); // Terceiro algarismo da seed se o algoritmo começar.
    uniform_int_distribution<> dis6(1,4); // Terceiro algarismo da seed se o algoritno não começar.
    seed = {dis1(gen), dis2(gen), dis3(gen), dis4(gen), dis5(gen), dis6(gen)}; // Salvando a seed.
    int tabuleiro[3][3] = {{escolha[0],escolha[1],escolha[2]},{escolha[3],escolha[4],escolha[5]},{escolha[6],escolha[7],escolha[8]}}; // Criando o tabuleiro.
    cout << "Bem-vindo ao jogo da velha!" << endl;
    return 0;
}