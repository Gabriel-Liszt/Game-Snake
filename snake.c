//20:00

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ternios.h>

#define COL 60
#define LIN 30

int main(){
    //Esconde o cursor
    printf("\e[?25l");

    //modo canonico
    struct ternios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int quit = 0;
    int x[1000], y[1000];
    
    //Construção da plataforma
    while(!quit){
        printf("[");
        for(int i = 0; i < COL; i++){
            printf("-");
        }
        printf("]\n");

        for(int j = 0; j < LIN; j++){
        printf("|");
        for(int i = 0; i < COL; i++){
            printf(".");
        }
        printf("|\n");
    }

    printf("[");
        for(int i = 0; i < COL; i++){
            printf("-");
        }
        printf("]\n");

    //movimenta o cursor para cima
    printf("\e[%iA", COL + 2);

    int cabeca = 0, cauda = 0;
    x[cabeca] = COL / 2;
    y[cabeca]= LIN / 2;
    int gameover = 0;
    int xdir = 1, ydir = 0;

    while(!quit && !gameover){
        //limpar cauda da cobra
        printf("\e[%iB\e[%iC°", y[cauda] + 1, x[cauda] + 1);
        printf("\e[%iF", y[cauda] + 1);

        cauda = (cauda + 1) % 1000;
        int novacabeca = (cabeca + 1) % 1000;
        x[novacabeca] = (x[cabeca] + xdir + COL) % COL;
        y[novacabeca] = (y[cabeca] + ydir + LIN) % LIN;
        cabeca = novacabeca;

        //desenhar cabeca da cobra
        printf("\e[%iB\e[%iCY", y[cabeca] + 1, x[cabeca] + 1);
        printf("\e[%iF", y[cabeca] + 1);
        fflush(stdout);
        
        usleep(5 * 1000000 / 60);

        //ler teclado
        struct timeval tv;
        fd_set fds;
        tv.tv_sec = 0;
        tv.tv_usec = 0; 

        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
        if(FD_ISSEt(STDIN_FILENO, &fds)){
            int ch = getchar();
            if(ch == 27 || ch =='q'){
                quit = 1;
            }else if(ch == 'h' && xdir != 1){
                xdir = -1;
                ydir = 0;
            }else if(ch == 'l' && xdir != -1){
                xdir = -1;
                ydir = 0;
            }else if(ch == 'j' && ydir != 1){
                xdir = 0;
                ydir = -1;
            }else if(ch == 'k' && ydir != -1){
                xdir = 0;
                ydir = 1;
            }
        }
    }
}

    //mostra o cursor
    printf("\e[?25h");
    return 0;
}