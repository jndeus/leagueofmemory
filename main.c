// Os arquivos de cabeçalho
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <unistd.h>
#include <stdio.h>
#include <windows.h>

// Atributos da tela
#define LARGURA_TELA 800
#define ALTURA_TELA 600
// Atributos dos menus
#define LARGURA_ITEM_MENU 120
#define ALTURA_ITEM_MENU 40
#define ESPACO_ENTRE_ITENS 5
#define TOTAL_DE_ITENS 2
// Atributos das cartas
#define IND1 6
#define IND2 5
#define LARGURA_CARTA 100
#define ALTURA_CARTA 80
#define TOTAL_CARTAS 15

int main(void)
{
    srand(time(NULL));
    ALLEGRO_DISPLAY *janela = NULL;
    ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
    ALLEGRO_FONT *fonte = NULL;
    ALLEGRO_BITMAP *background = NULL, *cartas[TOTAL_CARTAS], *menu = NULL, *itens_menu[TOTAL_DE_ITENS], *tras_carta = NULL, *voltar = NULL;
    int B[IND1][IND2], A[IND1][IND2], acertos = 0, erros = 0, numJogadas = 0, memoriaProg[30], contMemoriaProg[30], errosProg = 0, acertosProg = 0, numJogadasProg = 0, chave1 = -1, chave2 = -1;
    char pontos[5], jogadas[5];
    int tela_atual = 0; // Indicara qual a tela o usuário está no momento, 0 = Menu, 1 = Jogo 2 = Creditos
    // Sair = Flag que condicionará nosso looping, i = variavel de controle for, vez = Flag para informar se é a vez do jogador(0) ou do programa(1)
    // Dificuldade: 3 = facil, 2 = medio, 1 = dificl, 0 = hardcore
    int x, y, sair = 0, i = 0, vez = 0, dificuldade = 0;

    if(!inicializar_allegro()){
        return -1;
    }
    janela = inicializar_janela(janela);
    if(!janela){
        return -1;
    }
    if(!inicializar_mouse(janela)){
        al_destroy_display(janela);
        return -1;
    }

    fonte = carrega_fonte();
    if(!fonte){
        al_destroy_display(janela);
        return -1;
    }
    fila_eventos = inicializar_fila_eventos(fila_eventos, janela);
    if(!fila_eventos){
        al_destroy_display(janela);
        return -1;
    }

    background = al_load_bitmap("imagens\\Background.jpg");
    menu = al_load_bitmap("imagens\\Menu.jpg");
    tras_carta = al_load_bitmap("imagens\\FundoCartas.jpg");
    voltar = al_load_bitmap("imagens\\Voltar.jpg");
    carrega_cartas(cartas);
    carrega_itens_menu(itens_menu);

    zeraMat(B);
    zeraVet(memoriaProg, 30);
    zeraVet(contMemoriaProg, 30);
    preencheMat(A);
    valor(B);
    while (!sair)
    {
        // Verificamos se há eventos na fila
        while (!al_is_event_queue_empty(fila_eventos))
        {
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos, &evento);

            if ( evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                sair = 1;
            }else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
            {
                if(tela_atual == 0){
                    int y = ALTURA_TELA/2+20, x = LARGURA_TELA/2-50;
                    for(i = 0; i < TOTAL_DE_ITENS; i++){
                        if(evento.mouse.x >= x
                           && evento.mouse.x <= x + LARGURA_ITEM_MENU
                           && evento.mouse.y >= y
                           && evento.mouse.y <= y + ALTURA_ITEM_MENU){
                            tela_atual = i + 1;
                            break;
                        }
                        y += ALTURA_ITEM_MENU+ESPACO_ENTRE_ITENS;
                    }
                }else if(tela_atual == 1){
                    if(acertos+acertosProg < 15 && (chave1 == -1 || chave2 == -1) && vez == 0){
                        int i,j, x = 245, y=50, cont = 0;
                        for(i = 0; i < IND1; i++){
                            for(j = 0; j < IND2; j++){
                                cont++;
                                if(evento.mouse.x >= x
                                   && evento.mouse.x <= x + LARGURA_CARTA
                                   && evento.mouse.y >= y
                                   && evento.mouse.y <= y + ALTURA_CARTA){
                                    if(chave1 == -1){
                                        chave1 = virarCarta(A, B, cont, dificuldade, vez, memoriaProg, contMemoriaProg);
                                    }else{
                                        chave2 = virarCarta(A, B, cont, dificuldade, vez, memoriaProg, contMemoriaProg);
                                    }
                                }
                                x += LARGURA_CARTA+5;
                            }
                            x = 245;
                            y += ALTURA_CARTA+5;
                        }

                    }
                    if(evento.mouse.x >= 50
                       && evento.mouse.x <= 50 + LARGURA_ITEM_MENU
                       && evento.mouse.y >= 260
                       && evento.mouse.y <= 260 + ALTURA_ITEM_MENU){
                            tela_atual = 0;
                            zeraMat(B);
                            zeraVet(memoriaProg, 30);
                            zeraVet(contMemoriaProg, 30);
                            preencheMat(A);
                            valor(B);
                            acertos = acertosProg = erros = errosProg = numJogadas = numJogadasProg = 0;
                    }
                } else if(tela_atual == 2){
                    if(evento.mouse.x >= LARGURA_TELA/2 - LARGURA_ITEM_MENU/2
                       && evento.mouse.x <= LARGURA_TELA/2 + LARGURA_ITEM_MENU/2
                       && evento.mouse.y >= 110
                       && evento.mouse.y <= 110 + ALTURA_ITEM_MENU){
                            tela_atual = 0;
                       }
                }
            }
        }

        if(vez == 1 && acertos+acertosProg < 15){
                if(chave1 == -1){
                    jogadaPrograma(A, B, &chave1, 0, acertos+acertosProg, memoriaProg, contMemoriaProg, dificuldade);
                }else{
                    jogadaPrograma(A, B, &chave2, chave1, acertos+acertosProg, memoriaProg, contMemoriaProg, dificuldade);
                }
        }
        // Limpamos a tela
        al_clear_to_color(al_map_rgb(255, 255, 255));

        // Desenhamos os elementos na tela
        al_set_target_bitmap(al_get_backbuffer(janela));

        if(tela_atual == 0){
            al_draw_bitmap(menu, 0, 0, 0);
            int y = ALTURA_TELA/2+20;
            for(i = 0; i < TOTAL_DE_ITENS; i++){
                al_draw_bitmap(itens_menu[i], LARGURA_TELA/2-50, y, 0);
                y += ALTURA_ITEM_MENU+ESPACO_ENTRE_ITENS;
            }
        }else if(tela_atual == 1){
            al_draw_bitmap(background, 0, 0, 0);
            desenha_cartas(tras_carta, cartas, A, B);
            if(vez == 0){
                al_draw_text(fonte, al_map_rgb(0, 0, 0), 20, 120, ALLEGRO_ALIGN_LEFT, "Sua vez");
            }else if(vez == 1){

                al_draw_text(fonte, al_map_rgb(0, 0, 0), 20, 120, ALLEGRO_ALIGN_LEFT, "Vez do computador");
            }
            sprintf(pontos, "%d", acertos-erros);
            sprintf(jogadas, "%d", numJogadas);
            al_draw_text(fonte, al_map_rgb(0, 0, 0), 20, 140, ALLEGRO_ALIGN_LEFT, "Seus pontos: ");
            al_draw_text(fonte, al_map_rgb(0, 0, 0), 115, 140, ALLEGRO_ALIGN_LEFT, pontos);
            al_draw_text(fonte, al_map_rgb(0, 0, 0), 20, 160, ALLEGRO_ALIGN_LEFT, "Jogadas (voce): ");
            al_draw_text(fonte, al_map_rgb(0, 0, 0), 130, 160, ALLEGRO_ALIGN_LEFT, jogadas);

            sprintf(pontos, "%d", acertosProg-errosProg);
            sprintf(jogadas, "%d", numJogadasProg);
            al_draw_text(fonte, al_map_rgb(0, 0, 0), 20, 180, ALLEGRO_ALIGN_LEFT, "Pontos do computador: ");
            al_draw_text(fonte, al_map_rgb(0, 0, 0), 190, 180, ALLEGRO_ALIGN_LEFT, pontos);
            al_draw_text(fonte, al_map_rgb(0, 0, 0), 20, 200, ALLEGRO_ALIGN_LEFT, "Jogadas (comp): ");
            al_draw_text(fonte, al_map_rgb(0, 0, 0), 135, 200, ALLEGRO_ALIGN_LEFT, jogadas);
            if(acertos+acertosProg == 15){
                    if(acertos-erros < acertosProg-errosProg){
                        al_draw_text(fonte, al_map_rgb(0, 0, 0), 20, 240, ALLEGRO_ALIGN_LEFT, "Que pena voce perdeu :(");
                    } else if(acertos-erros == acertosProg-errosProg){
                        al_draw_text(fonte, al_map_rgb(0, 0, 0), 20, 240, ALLEGRO_ALIGN_LEFT, "Ops.. acho que empatou.");
                    } else if(acertos-erros > acertosProg-errosProg){
                        al_draw_text(fonte, al_map_rgb(0, 0, 0), 20, 240, ALLEGRO_ALIGN_LEFT, "Parabens, voce ganhou!!");
                    }
            }
            al_draw_bitmap(voltar, 55, 260, 0);
        }else if(tela_atual == 2){
            // Texto centralizado
            al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA/2, 30, ALLEGRO_ALIGN_CENTRE, "Creditos");
            al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA/2, 50, ALLEGRO_ALIGN_CENTRE, "Joao N de Deus");
            al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA/2, 70, ALLEGRO_ALIGN_CENTRE, "Yuri Rosales");
            al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA/2, 90, ALLEGRO_ALIGN_CENTRE, "Guilherme dos Santos");
            al_draw_bitmap(voltar, LARGURA_TELA/2-LARGURA_ITEM_MENU/2, 110, 0);

        }

        // Atualiza a tela
        al_flip_display();

        if(tela_atual == 1){
            if(chave1 > -1 && chave2 > -1){
                numeroBUSCA(A, B, &acertos, &erros, &numJogadas, &acertosProg, &errosProg, &numJogadasProg, &chave1, &chave2, &vez, memoriaProg, contMemoriaProg);
            }else if((chave1 > -1 || chave2 > -1) && vez == 1){
                Sleep(1000);
            }
        }
    }

    // Desaloca os recursos utilizados na aplicação
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);

    return 0;
}

int inicializar_allegro(){
    if (!al_init())
    {
        fprintf(stderr, "Falha ao inicializar a Allegro.\n");
        return 0;
    }
    if (!al_init_image_addon())
    {
        fprintf(stderr, "Falha ao inicializar add-on image.\n");
        return 0;
    }

    //Inicialização do add-on para uso de fontes
    al_init_font_addon();
    // Inicialização do add-on para uso de fontes True Type
    if (!al_init_ttf_addon())
    {
        fprintf(stderr, "Falha ao inicializar add-on allegro_ttf.\n");
        return 0;
    }

    return 1;

}
int inicializar_janela(ALLEGRO_DISPLAY *janela){
    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!janela)
    {
        fprintf(stderr, "Falha ao criar janela.\n");
        return 0;
    }

    // Configura o título da janela
    al_set_window_title(janela, "Jogo da memória");
    return janela;
}

int inicializar_mouse(ALLEGRO_DISPLAY *janela){
    if (!al_install_mouse())
    {
        fprintf(stderr, "Falha ao inicializar o mouse.\n");
        return 0;
    }
    // Atribui o cursor padrão do sistema para ser usado
    if (!al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT))
    {
        fprintf(stderr, "Falha ao atribuir ponteiro do mouse.\n");
        return 0;
    }
    return 1;
}

int inicializar_fila_eventos(ALLEGRO_EVENT_QUEUE *fila_eventos, ALLEGRO_DISPLAY *janela){
    fila_eventos = al_create_event_queue();
    if (!fila_eventos)
    {
        fprintf(stderr, "Falha ao inicializar o fila de eventos.\n");
        return 0;
    }

    // Dizemos que vamos tratar os eventos vindos do mouse
    al_register_event_source(fila_eventos, al_get_mouse_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    return fila_eventos;
}

void carrega_fonte(ALLEGRO_FONT *fonte){
    // Carregando o arquivo de fonte
    fonte = al_load_font("BOD_BLAR.TTF", 14, 0);
    if (!fonte)
    {
        fprintf(stderr, "Falha ao carregar fonte.\n");
        return 0;
    }
    return fonte;
}
void carrega_cartas(ALLEGRO_BITMAP *cartas[TOTAL_CARTAS]){
    cartas[0] = al_load_bitmap("imagens\\cartas\\Ahri.png");
    cartas[1] = al_load_bitmap("imagens\\cartas\\AurelionSol.png");
    cartas[2] = al_load_bitmap("imagens\\cartas\\Azir.png");
    cartas[3] = al_load_bitmap("imagens\\cartas\\Draven.png");
    cartas[4] = al_load_bitmap("imagens\\cartas\\Fiora.png");
    cartas[5] = al_load_bitmap("imagens\\cartas\\JarvanIV.png");
    cartas[6] = al_load_bitmap("imagens\\cartas\\Kennen.png");
    cartas[7] = al_load_bitmap("imagens\\cartas\\KogMaw.png");
    cartas[8] = al_load_bitmap("imagens\\cartas\\Riven.png");
    cartas[9] = al_load_bitmap("imagens\\cartas\\Thresh.png");
    cartas[10] = al_load_bitmap("imagens\\cartas\\TwistedFate.png");
    cartas[11] = al_load_bitmap("imagens\\cartas\\Vayne.png");
    cartas[12] = al_load_bitmap("imagens\\cartas\\Xerath.png");
    cartas[13] = al_load_bitmap("imagens\\cartas\\Yasuo.png");
    cartas[14] = al_load_bitmap("imagens\\cartas\\Zac.png");
}

void carrega_itens_menu(ALLEGRO_BITMAP *itens_menu[TOTAL_DE_ITENS]){
    itens_menu[0] = al_load_bitmap("imagens\\Jogar.jpg");
    itens_menu[1] = al_load_bitmap("imagens\\Creditos.jpg");
}

void desenha_cartas(ALLEGRO_DISPLAY *tras_carta, ALLEGRO_DISPLAY *cartas[TOTAL_CARTAS], int matA[IND1][IND2], int matB[IND1][IND2]){
    int i,j, x = 245, y=50;
    for(i = 0; i < IND1; i++){
        for(j = 0; j < IND2; j++){
            if(matA[i][j] > 100){
                al_draw_bitmap(cartas[matB[i][j]-1], x, y, 0);
            }else{
                al_draw_bitmap(tras_carta, x, y, 0);
            }
            x += LARGURA_CARTA+5;
        }
        x = 245;
        y += ALTURA_CARTA+5;
    }
}

void preencheMat(int mat[IND1][IND2]){
    int i,j,k = 1;
    for(i = 0; i < IND1; i++){
        for(j = 0; j < IND2; j++){
            mat[i][j] = k;
            k ++;
        }
    }
}
int buscaLinear(int vetor[], int tamanho, int elementoProcurado) {
    int i;
    for (i = 0; i < tamanho; i++) {
         if (vetor[i] == elementoProcurado) {
             return i;
         }
    }

    return -1;
}
int preencheVetor(int vet[], int tam){
    int i;
    for(i = 0; i < tam; i++){
        vet[i] = i+1;
    }
}
int zeraMat(int mat[IND1][IND2]){
    int i, j;
    for(i = 0; i < IND1; i++){
        for(j = 0; j < IND2; j++){
            mat[i][j] = 0;
        }
    }
}
int zeraVet(int vet[], int tam){
    int i;
    for(i = 0; i < tam; i++){
        vet[i] = 0;
    }
}
void embaralhar(int vet[], int vetSize)
{
    int i;
	for (i = 0; i < vetSize; i++)
	{
		int r = rand() % vetSize;

		int temp = vet[i];
		vet[i] = vet[r];
		vet[r] = temp;
	}
}
void valor(int mat[IND1][IND2]){
    int i,j, numAle, cont = 0, vet1[15], vet2[15];
    preencheVetor(vet1, 15);
    preencheVetor(vet2, 15);
    embaralhar(vet1, 15);
    embaralhar(vet2, 15);
    for(i = 0; i < IND1; i++){
        for(j = 0; j < IND2; j++){
            if(cont < 15){
                mat[i][j] = vet1[cont];
            }else{
                mat[i][j] = vet2[cont-15];
            }
            cont++;
        }
    }
}

int isExisteNaMatriz(int mat[IND1][IND2], int elementoProcurado) {
     int i, j;
     for (i = 0; i < IND1; i++) {
         for (j = 0; j< IND2; j++) {
             if (mat[i][j] == elementoProcurado) {
                 return 1;
             }
         }
     }
     return 0;
}

int virarCarta(int matA[IND1][IND2], int matB[IND1][IND2], int chave, int dificuldade, int vez, int memoriaProg[30], int contMemoriaProg[30]){
    int i, j, retorno = -1;
    for(i = 0; i < IND1; i++){
        for(j = 0; j < IND2; j++){
            if(matA[i][j] == chave){
                matA[i][j] += 100;
                retorno = chave;
                break;
            }
        }
    }
    if((dificuldade < 2 || vez == 1) && retorno > 0){
        if(memoriaProg[retorno-1] == 0){
            memoriaProg[retorno-1] = getValorChave(matA, matB, retorno);
            contMemoriaProg[retorno-1] += 1;
        }else if(contMemoriaProg[retorno - 1] < dificuldade+1){
            contMemoriaProg[retorno-1] += 1;
        }
    }
    return retorno;
}

int getValorChave(int matA[IND1][IND2], int matB[IND1][IND2], int chave){
    int i,j, valor = 0;
    for(i = 0; i < IND1; i++){
        for(j = 0; j < IND2; j++){
            if(matA[i][j] < 100){
                if(matA[i][j] == chave){
                    valor = matB[i][j];
                    break;
                }
            }else{
                if(matA[i][j] - 100 == chave){
                    valor = matB[i][j];
                    break;
                }
            }
        }
        if(valor > 0){
            break;
        }
    }
}

void numeroBUSCA(int matA[IND1][IND2], int matB[IND1][IND2], int *acertos, int *erros, int *numJogadas, int *acertosProg, int *errosProg, int *numJogadasProg, int *chave1, int *chave2, int *vez, int memoriaProg[30], int contMemoriaProg[30]){
    int i,j, i1, j1, i2, j2, valor1 = 0, valor2 = 0;
    for(i = 0; i < IND1; i++){
        for(j = 0; j < IND2; j++){
            if(matA[i][j] - 100 == *chave1){
                valor1 = matB[i][j];
                i1 = i;
                j1 = j;
            } else if(matA[i][j] - 100 == *chave2){
                valor2 = matB[i][j];
                i2 = i;
                j2 = j;
            }
        }
    }
    if(valor1 == valor2){
        memoriaProg[*chave1-1] = -1;
        memoriaProg[*chave2-1] = -1;
        contMemoriaProg[*chave1-1] = -1;
        contMemoriaProg[*chave2-1] = -1;
        if(*vez == 0){
            *acertos += 1;
        }else{
            *acertosProg +=1;
        }
    }else {
        matA[i1][j1] = *chave1;
        matA[i2][j2] = *chave2;
        if(*vez == 0){
            *erros += 1;
        }else{
            *errosProg +=1;
        }
        Sleep(1000);
    }

    *chave1 = *chave2 = -1;

    if(*vez == 0){
        *numJogadas += 1;
        *vez = 1;
    }else{
        *numJogadasProg += 1;
        *vez = 0;
    }
}

int buscaMemoriaProg(int memoriaProg[30], int contMemoriaProg[30], int dificuldade, int chaveRef, int matA[IND1][IND2], int matB[IND1][IND2]){
    int i, j, valorChaveRef = getValorChave(matA, matB, chaveRef), chavesViradas[30], valorChavesViradas[30], cont = 0;
    zeraVet(chavesViradas, 30);
    zeraVet(valorChavesViradas, 30);
    if(chaveRef < 1){
        for(i = 0; i < 30; i++){
            if(contMemoriaProg[i] > dificuldade){
                chavesViradas[cont] = i+1;
                valorChavesViradas[cont] = memoriaProg[i];
                cont++;
            }
        }
        for(i = 0; i < cont; i++){
            for(j = 0; j < cont; j++){
                if(valorChavesViradas[i] == valorChavesViradas[j] && i!=j){
                    return chavesViradas[i];
                }
            }
        }
    }else{
        for(i = 0; i < 30; i++){
            if(contMemoriaProg[chaveRef-1] > dificuldade && chaveRef-1 != i && memoriaProg[i] == valorChaveRef){
                return i+1;
            }
        }
    }
    return 0;
}

void jogadaPrograma(int matA[IND1][IND2], int matB[IND1][IND2], int *chave, int chaveRef, int totAcertos, int memoriaProg[30], int contMemoriaProg[30], int dificuldade){
    int chaveAle = -1, i, j, chavesFaltantes[30], cont = 0, buscaMemoria;
    do{
        buscaMemoria = buscaMemoriaProg(memoriaProg, contMemoriaProg, dificuldade, chaveRef, matA, matB);
        if(buscaMemoria > 0){
            chaveAle = buscaMemoria;
        }else if(totAcertos > 13){ //Isso foi necessário pois quando chegava nas últimas peças o loop de randomico fica quase infinito até ele selecionar exatamente a chave faltante iria demorar.
            for(i = 0; i < IND1; i++){
                for(j = 0; j < IND2; j++){
                    if(matA[i][j] < 100){
                        chavesFaltantes[cont] = matA[i][j];
                        cont++;
                    }
                }
            }
            if(cont > 0){
                chaveAle = chavesFaltantes[rand() % cont-1];
            }
        }else{
            chaveAle = 1+(rand() % 30);
        }
        if(chaveAle > 0){
            *chave = virarCarta(matA, matB, chaveAle, dificuldade, 1, memoriaProg, contMemoriaProg);
        }
    }while(*chave < 1);
}
