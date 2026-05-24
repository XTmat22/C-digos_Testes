#include <iostream>
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

// Função para demonstrar que a matriz pode ser alterada pelo próprio código
void preencherPeloCodigo(int matriz[3][3]) {
    int contador = 1;
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            matriz[i][j] = contador++; // Preenche com números de 1 a 9
        }
    }
}

int main() {
    // 1. Cria a matriz inicial uma única vez e preenche com zeros
    int matriz[3][3] = {0}; 
    
    // Controles de posição do cursor
    int linhaAtual = 0;
    int colunaAtual = 0;
    bool executando = true;

    while(executando) {
        // Limpa a tela no Linux
        system("clear"); 
        
        cout << "=== MATRIZ INTERATIVA 3x3 (LINUX) ===\n";
        cout << "Navegacao: W (Cima), S (Baixo), A (Esquerda), D (Direita)\n";
        cout << "Acoes: [ESPACO] Editar Valor | [C] Preencher via Codigo | [Q] Sair\n\n";

        // 2. Desenha a matriz na tela
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++) {
                if(i == linhaAtual && j == colunaAtual) {
                    cout << "[" << matriz[i][j] << "]\t"; 
                } else {
                    cout << " " << matriz[i][j] << " \t";
                }
            }
            cout << "\n\n"; 
        }

        // 3. Captura a tecla usando nossa função nativa do Linux
        char tecla = getch();

        // 4. Lógica de atualização
        switch(tolower(tecla)) {
            case 'w': 
                if(linhaAtual > 0) linhaAtual--; 
                break;
            case 's': 
                if(linhaAtual < 2) linhaAtual++; 
                break;
            case 'a': 
                if(colunaAtual > 0) colunaAtual--; 
                break;
            case 'd': 
                if(colunaAtual < 2) colunaAtual++; 
                break;
            case ' ': 
                cout << "\nDigite o novo valor para a posicao [" << linhaAtual << "][" << colunaAtual << "]: ";
                cin >> matriz[linhaAtual][colunaAtual]; 
                break;
            case 'c': 
                preencherPeloCodigo(matriz); 
                break;
            case 'q': 
                executando = false;
                break;
        }
    }

    // Tela final após sair do loop
    system("clear");
    cout << "Programa encerrado. Matriz final:\n\n";
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            cout << matriz[i][j] << "\t";
        }
        cout << "\n";
    }
    return 0;
}