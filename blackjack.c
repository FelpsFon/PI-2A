#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "card.h"
#include "draw.h"
#include "term.h"

int console_cols, console_rows;

typedef struct Player {
	char nomePlayer[50];
	int ganhou, perdeu, dinheiro, indice;
	struct Player *proximo;
	t_manilha manilha; // lista encadeada de cartas
} t_player;

typedef struct {
    t_player *primeiro;
    t_player *ultimo;
} t_lista;

void on_console_window_change() {
  tc_get_cols_rows(&console_cols, &console_rows);
}

void criar_jogador(int numJogadores, t_lista *lista);
void novo_jogo(t_card *cartas, t_lista *lista);
void dinamica_jogo (int numJogadores, t_card *cartas, t_lista *jogadores);
void reiniciar_jogo(t_card *cartas, t_lista *jogadores);
int print_carta(t_card *cartas, t_player *jogador);
int dealer(int totalDealer);
void acessar_ranking();
void zerar_ranking();
void salvar_ranking(t_lista *lista);

int main() {
  // criar deck
  t_card cartas;
	t_lista *lista = malloc(sizeof(t_lista));
  lista->primeiro = NULL;
  lista->ultimo = NULL;

  while(1) {
    system("clear"); // linux
    // ststem("cls"); // windows
    int opcao = 0;
  		printf("1 - Novo Jogo\n");
      printf("2 - Acessar Ranking\n");
			printf("3 - Zerar Ranking\n");
      printf("4 - Sair.\n");
      printf("Opção: ");
      scanf("%d%*c", &opcao);

    switch(opcao) {
      case 1:
        novo_jogo(&cartas, lista);
        break;
      case 2: 
        acessar_ranking();
        break;
			case 3:
				zerar_ranking();
				break;
      case 4:
				system("clear");
				printf("Obrigado por jogar!\n");
        return 0;
    	default:
        printf("Opção inválida!");
        getchar();
    }
  }
}

void novo_jogo(t_card *cartas, t_lista *lista) {
	int numJogadores;
	/*on_console_window_change();
	term_move_xy(0,0); //move o cursor do console pra esquerda no topo?

	tc_echo_off();
  tc_canon_off(); //desabilitam a escrita no console

	tc_echo_on();
  tc_canon_on(); //re-habilitam a escrita no console */

	printf("Insira o número de jogadores: ");
	scanf("%d*c", &numJogadores);
	getchar();

	criar_jogador(numJogadores, lista);
	dinamica_jogo(numJogadores, cartas, lista);
	salvar_ranking(lista);
}

void criar_jogador(int numJogadores, t_lista *lista) { //inserir player na lista
	for(int i = 1; i <= numJogadores; i++) {
    // passo 1: Criar novo player.
    t_player *player = NULL;
    player = malloc(sizeof(t_player));

    printf("Insira seu nome: ");
		scanf("%50[^\n]%*c", player->nomePlayer); //Lê no máximo 50 caracteres, até encontrar um \n

		player->dinheiro = 500;
    player->proximo = NULL;
		player->indice = i;
		
    // passo 2: Inserir no fim da lista.
    if(lista->primeiro == NULL) {
        // Se a lista estiver vazia
        lista->primeiro = player;
        lista->ultimo = player;
				player->proximo = player;
    } else {
        // inserção no fim
        lista->ultimo->proximo = player;
        lista->ultimo = player;
				player->proximo = lista->primeiro;
    }
	}
}

void dinamica_jogo(int numJogadores, t_card *cartas, t_lista *jogadores) {
	t_player *jogador = jogadores->primeiro;
	jogador->manilha.pontuacao = 0;
	int rodadas = 1, aposta, totalDealer = 0;
	char HitStay;

	while(rodadas <= 21)
		//While loop usado para continuar perguntando ao jogador para Hit ou Stay vinte e uma vezes, pois tem chance de que o jogador gere 1ponto vinte e uma vezes consecutivas
  {
		if(numJogadores > 1)
		{
			if((jogador != jogadores->primeiro) || (rodadas > 1))
				printf("\n\n-----------Próximo jogador-----------\n");
			
			printf("\nJogador(a) atual: %s\n", jogador->nomePlayer);
		}
		
		if(jogador->dinheiro <= 0) //Uma vez que o usuario nao tem mais dinheiro, o jogo acaba e solicita ao usuario para jogar novamente 
  	{
    	printf("Você está falido. Fim de Jogo");
    	jogador->dinheiro = 500;
    	reiniciar_jogo(cartas, jogadores);
  	}
		printf("\nDinheiro: $%d\n", jogador->dinheiro); //Imprime quantidade de dinheiro que o jogador tem
  
		jogador->manilha.pontuacao = jogador->manilha.pontuacao + (print_carta(cartas, jogador)); //Gera uma carta aleatoria
  	printf("\nSua pontuação é: %d\n", jogador->manilha.pontuacao); //Imprime pontuação total do jogador
  	totalDealer = dealer(totalDealer); //Computa e imprime o total do dealer

		if((rodadas == 1) && (jogador->indice <= numJogadores))
		{
 			printf("\n\nInsira sua aposta: $");
 			scanf("%d%*c", &aposta);
 			if (aposta > jogador->dinheiro) //Se o jogador tenta apostar mais dinheiro do que tem 
 			{
  			printf("\nVocê não pode apostar mais dinheiro do que você tem.");
  			printf("\nInsira aposta: ");
  			scanf("%d%*c", &aposta);
 			}
		}
		
    if(jogador->manilha.pontuacao == 21) //Se o total do jogador for 21, ganha 
    {
      printf("\nIncrível! Você Ganhou!\n");
      jogador->ganhou = jogador->ganhou + 1;
      jogador->dinheiro = jogador->dinheiro + aposta;
      printf("\nVocê tem %d Vitórias e %d Derrotas. Parabéns!\n", jogador->ganhou, jogador->perdeu);
      totalDealer = 0;
      reiniciar_jogo(cartas, jogadores);
    }
      
    if(jogador->manilha.pontuacao > 21) //Se o total do jogador for acima de 21, perde 
    {
      printf("\nPoxa, você passou longe.\n");
      jogador->perdeu = jogador->perdeu + 1;
      jogador->dinheiro = jogador->dinheiro - aposta;
      printf("\nVocê tem %d Vitórias e %d Derrotas. Parabéns!\n", jogador->ganhou, jogador->perdeu);
      totalDealer=0;
      reiniciar_jogo(cartas, jogadores);
    }
      
    if(jogador->manilha.pontuacao <= 21) //Se o total do jogador é menor que 21, peça para pegar mais uma carta ou parar (hit ou stay)
    {
      printf("\nVocê gostaria de dar Hit ou Stay?");
      scanf("%c%*c", &HitStay);
			
      while((HitStay != 'H') && (HitStay != 'h') && (HitStay != 'S') && (HitStay != 's')) //Se escolha invalida for selecionada
      {                                                                          
      	printf("\n");
        printf("Entrada inválida. Por favor digite H para Hit ou S para Stay.\n");
        scanf("%c%*c",&HitStay);
      }
 
    if((HitStay == 'H') || (HitStay == 'h')) //Se "Hit", continua 
    {
      jogador->manilha.pontuacao = jogador->manilha.pontuacao + (print_carta(cartas, jogador)); //Gera uma carta aleatoria
  		printf("\nSua pontuação é: %d\n", jogador->manilha.pontuacao);
      dealer(totalDealer);
			
      if(totalDealer == 21) //Se o total do dealer é 21, perdeu
      {
        printf("\nDealer tem cartas melhores. Você Perdeu.\n");
        jogador->perdeu = jogador->perdeu + 1;
        jogador->dinheiro = jogador->dinheiro - aposta;
        printf("\nVocê tem %d Vitórias e %d Derrotas. Incrível!\n", jogador->ganhou, jogador->perdeu);
        totalDealer = 0;
        reiniciar_jogo(cartas, jogadores);
      }
    	if(totalDealer > 21) //Se o total do dealer for acima de 21, ganha 
      {                     
        printf("\nDealer Ultrapassou! Você Ganhou!\n");
        jogador->ganhou = jogador->ganhou + 1;
    		jogador->dinheiro = jogador->dinheiro + aposta;
        printf("\nVocê tem %d Vitórias e %d Derrotas. Incrível!\n", jogador->ganhou, jogador->perdeu);
        totalDealer = 0;
        reiniciar_jogo(cartas, jogadores);
      }
    }
    if((HitStay == 'S') || (HitStay == 's')) //Se "Stay", não continua
    {
      printf("\nVocê escolheu Stay at %d. Boa Escolha!\n", jogador->manilha.pontuacao);
      do { //Se escolher stay, dealer continua 
				totalDealer = dealer(totalDealer);
     	
				if(totalDealer >= 17)
     		{
      		if(jogador->manilha.pontuacao >= totalDealer) //Se o total do jogador é maior ou igual ao total do dealer, ganha 
      		{
         		printf("\nInacreditável! Você Ganhou!\n");
         		jogador->ganhou = jogador->ganhou + 1;
         		jogador->dinheiro = jogador->dinheiro + aposta;
         		printf("\nVocê tem %d Vitórias e %d Derrotas. Incrível!\n", jogador->ganhou, jogador->perdeu);
         		totalDealer = 0;
         		reiniciar_jogo(cartas, jogadores);
      		}
				
      		if(jogador->manilha.pontuacao < totalDealer) //Se o total do jogador é menor que o total do dealer, perde 
      		{
         		printf("\nDealer tem cartas melhores. Você Perdeu.\n");
         		jogador->perdeu = jogador->perdeu + 1;
         		jogador->dinheiro = jogador->dinheiro - aposta;
         		printf("\nVocê tem %d Vitórias e %d Derrotas. Incrível!\n", jogador->ganhou, jogador->perdeu);
         		totalDealer = 0;
         		reiniciar_jogo(cartas, jogadores);
      		}
      	
					if(totalDealer > 21) //Se o total do dealer é maor que 21, ganha
      		{
         		printf("\nInacreditável! Você Ganhou!\n");
         		jogador->ganhou = jogador->ganhou + 1;
         		jogador->dinheiro = jogador->dinheiro + aposta;
         		printf("\nVocê tem %d Vitórias e %d Derrotas. Incrível!\n", jogador->ganhou, jogador->perdeu);
         		totalDealer = 0;
         		reiniciar_jogo(cartas, jogadores);
      		}
     		}
			} while(totalDealer < 17);
    }
  }
		if(numJogadores > 1) { jogador = jogador->proximo; }
		if(jogador->indice >= numJogadores) rodadas++; //Enquanto o total do jogador e o total do dealer forem menores que 21, refazer while loop
	} //Finalizar o loop while
	printf("%s venceu!\n", jogador->nomePlayer);
}

int dealer(int totalDealer) //Função para a IA do dealer jogar
{
  int z, d;
  
  if(totalDealer < 17)
  {
    srand((unsigned) time(NULL) + 1); //Gera carta aleatoria para a função rand()
    z = rand() % 13 + 1;
    
		if(z <= 10) //Se o numero gerado aleatoriamente é 10 ou menor, guarda o valor
      d = z;
      
    if(z > 11) //Se o numero gerado aleatoriamente é maior que 11, mudar valor pra 10 
      d = 10;
      
    if(z == 11) //Se o numero gerado aleatoriamente é 11 (Ace), mudar valor para 11 ou 1 dependendo do total do dealer 
    {
      if(totalDealer <= 10)
        d = 11;
      else
        d = 1;
    }
		
    totalDealer = totalDealer + d;
  }
  printf("\nO Dealer tem o total de %d", totalDealer); //Imprime o total do dealer
	return totalDealer;
}

int print_carta(t_card *cartas, t_player *jogador) //Gera carta aleatoria 
{
  srand((unsigned) time(NULL)); //Gera uma carta aleatoria para a função rand()
  int l, k, random_card = rand() % 4 + 1;

  if(random_card == 1) //Paus
  {
    srand((unsigned) time(NULL)); //Gera carta aleatoria para a função rand()
    k = rand() % 13 + 1;
     
    if(k <= 9) //Se o numero gerado aleatoriamente é 9 ou menor, imprime carta com o numero 
    {
    	//Carta de Paus
    	printf("%s%s", TC_BG_WHT, TC_BLK);
    	printf("%d      \n", k);
    	printf("%s      \n", PAUS);
    	printf("   %s   \n", PAUS);
    	printf("      %s\n", PAUS);
    	printf("      %d\n", k);
			printf("%s%s\n", TC_BG_NRM, TC_RST);
    }
     
    if(k == 10) //Se o nuemro gerado aleatoriamente é 10, imprime carta com J (jack)
    {
    	//Carta de Paus
   	 	printf("%s%s", TC_BG_WHT, TC_BLK);
    	printf("J      \n");
    	printf("%s      \n", PAUS);
    	printf("   %s   \n", PAUS);
    	printf("      %s\n", PAUS);
    	printf("      J\n");
			printf("%s%s\n", TC_BG_NRM, TC_RST);
    }
		
    if(k == 11) //Se o numero gerado aleatoriamente é 11, imprime carta com A (Ace)
    {
    	//Carta de Paus
    	printf("%s%s", TC_BG_WHT, TC_BLK);
    	printf("A      \n");
    	printf("%s      \n", PAUS);
    	printf("   %s   \n", PAUS);
    	printf("      %s\n", PAUS);
    	printf("      A\n");
			printf("%s%s\n", TC_BG_NRM, TC_RST);

			if(l <= 10) //Se o numero gerado aleatoriamente é Ace, mudar valor para 11 ou 1 dependendo do total do jogador
      	k = 11;
    	else
      	k = 1;
    }
		
    if(k == 12) //Se o numero gerado aleatoriamente é 12, imprime carta com Q (queen/rainha)
    {
   	 //Carta de Paus
    	printf("%s%s", TC_BG_WHT, TC_BLK);
    	printf("Q      \n");
    	printf("%s      \n", PAUS);
    	printf("   %s   \n", PAUS);
    	printf("      %s\n", PAUS);
    	printf("      Q\n");
			printf("%s%s\n", TC_BG_NRM, TC_RST);
    	k = 10; //Valor da carta é 10
    }
		
    if(k == 13) //Se o nuemro gerado aleatoriamente é 13, imprime carta com K (king/rei)
    {
    	//Carta de Paus
    	printf("%s%s", TC_BG_WHT, TC_BLK);
    	printf("K      \n");
    	printf("%s      \n", PAUS);
    	printf("   %s   \n", PAUS);
    	printf("      %s\n", PAUS);
    	printf("      K\n");
			printf("%s%s\n", TC_BG_NRM, TC_RST);
    	k = 10; //Valor da carta é 10
    }
		
  	l = k;
  }
	
  if(random_card == 2) //Ouros
  {
    srand((unsigned) time(NULL)); //Gera carta aleatoria para a função rand()
    k = rand() % 13 + 1;
     
    if(k <= 9) //Se o nuemro gerado aleatoriamente ´we 9 ou menor, imprime carta com o nuemro
    {
    	//Carta de Ouros
    	printf("%s%s", TC_BG_WHT, TC_RED);
    	printf("%d      \n", k);
    	printf("%s      \n", OUROS);
    	printf("   %s   \n", OUROS);
    	printf("      %s\n", OUROS);
    	printf("      %d\n", k);
			printf("%s%s\n", TC_BG_NRM, TC_RST);
    }
     
    if(k == 10) //Se o nuemro gerado aleatoriamente é 10, imprime carta com J (Jack) 
    {
    	//Carta de Ouros
    	printf("%s%s", TC_BG_WHT, TC_RED);
    	printf("J      \n");
    	printf("%s      \n", OUROS);
    	printf("   %s   \n", OUROS);
    	printf("      %s\n", OUROS);
    	printf("      J\n");
			printf("%s%s\n", TC_BG_NRM, TC_RST);
    }
     
    if(k == 11) //Se o numero gerado aleatoriamente é 11, imprime carta com A (Ace) 
    {
    	//Carta de Ouros
    	printf("%s%s", TC_BG_WHT, TC_RED);
    	printf("A      \n");
    	printf("%s      \n", OUROS);
    	printf("   %s   \n", OUROS);
    	printf("      %s\n", OUROS);
    	printf("      A\n");
			printf("%s%s\n", TC_BG_NRM, TC_RST);
    	
			if(l <= 10) //Se o numero gerado aleatoriamnete é Ace, muda valor para 11 ou 1 dependendo do total do jogador 
      	k = 11;
    	else
      	k = 1;
    }
     
    if(k == 12) //Se o numero gerado aleatoriamente é 12, imprime carta com Q (quenn/rainha) 
    {
    	//Carta de Ouros
    	printf("%s%s", TC_BG_WHT, TC_RED);
    	printf("Q      \n");
    	printf("%s      \n", OUROS);
    	printf("   %s   \n", OUROS);
    	printf("      %s\n", OUROS);
    	printf("      Q\n");
			printf("%s%s\n", TC_BG_NRM, TC_RST);
    	k=10; //Valor da carta é 10
    }
     
    if(k == 13) //Se o nuemro gerado aleatoriamente é 13, imprime carta com K (king/rei) 
    {
    //Carta de Ouros
    	printf("%s%s", TC_BG_WHT, TC_RED);
    	printf("K      \n");
    	printf("%s      \n", OUROS);
    	printf("   %s   \n", OUROS);
    	printf("      %s\n", OUROS);
    	printf("      K\n");
			printf("%s%s\n", TC_BG_NRM, TC_RST);
    	k = 10; //Valor da carta é 10
    }
    l = k;
  }
	
  if(random_card == 3) //Copas 
  {
    srand((unsigned) time(NULL)); //Gera carta aleatoria para a função rand()
    k = rand() % 13 + 1;
     
    if(k <= 9) //Se o numero gerado aleatoriamente é 9 ou menor, imprime carta com o numero 
    {
    	//Carta de Copas
    	printf("%s%s", TC_BG_WHT, TC_RED);
    	printf("%d      \n", k);
    	printf("%s      \n", COPAS);
    	printf("   %s   \n", COPAS);
    	printf("      %s\n", COPAS);
    	printf("      %d\n", k);
			printf("%s%s\n", TC_BG_NRM, TC_RST);
    }
     
    if(k == 10) //Se o numero gerado aleatoriamente é 10, imprime carta com J (Jack)
    {
    	//Carta de Copas
    	printf("%s%s", TC_BG_WHT, TC_RED);
    	printf("J      \n");
    	printf("%s      \n", COPAS);
    	printf("   %s   \n", COPAS);
    	printf("      %s\n", COPAS);
    	printf("      J\n");
			printf("%s%s\n", TC_BG_NRM, TC_RST);
    }
     
    if(k == 11) //Se o numero gerado aleatoriamante é 11, imprime carta com A (Ace) 
    {
    	//Carta de Copas
    	printf("%s%s", TC_BG_WHT, TC_RED);
    	printf("A      \n");
    	printf("%s      \n", COPAS);
    	printf("   %s   \n", COPAS);
    	printf("      %s\n", COPAS);
    	printf("      A\n");
			printf("%s%s\n", TC_BG_NRM, TC_RST);
			
    	if(l <= 10) //Se o numero gerado aleatoriamente é Ace, mudar valor para 11 ou 1 dependendo do total do dealer 
        k = 11;
      else
        k = 1;
    }
     
    if(k == 12) //Se o numero gerado aletoriamente é 12, imprime carta com Q (quenn/rainah)
    {
    	//Carta de Copas
    	printf("%s%s", TC_BG_WHT, TC_RED);
    	printf("Q      \n");
    	printf("%s      \n", COPAS);
    	printf("   %s   \n", COPAS);
    	printf("      %s\n", COPAS);
    	printf("      Q\n");
			printf("%s%s\n", TC_BG_NRM, TC_RST);
    	k = 10; //Valor da carta é 10 
    }
     
    if(k == 13) //Se o numero gerado aleatoriamente é 13, imprime carta com K (king/rei)
    {
    	//Carta de Copas
    	printf("%s%s", TC_BG_WHT, TC_RED);
    	printf("K      \n");
    	printf("%s      \n", COPAS);
    	printf("   %s   \n", COPAS);
    	printf("      %s\n", COPAS);
    	printf("      K\n");
			printf("%s%s\n", TC_BG_NRM, TC_RST);
    	k = 10; //Valor da carta é 10 
    }
    l = k;
  }
	
  if(random_card == 4) //Espadas
  {
    srand((unsigned) time(NULL)); //Gera carta aleatoria para a função rand()
    k = rand() % 13 + 1;
		
    if(k <= 9) //Se o nuemro gerado aleatoriamente é 9 ou menor, imprime carta com o nuemro 
    {
    	//Carta de Espadas
    	printf("%s%s", TC_BG_WHT, TC_BLK);
    	printf("%d      \n", k);
    	printf("%s      \n", ESPADAS);
    	printf("   %s   \n", ESPADAS);
    	printf("      %s\n", ESPADAS);
    	printf("      %d\n", k);
			printf("%s%s\n", TC_BG_NRM, TC_RST);
    }
     
    if(k == 10) //Se o numero gerado aleatoriamente é 10, imprime carta com J (Jack)
    {
    	//Carta de Espadas
    	printf("%s%s", TC_BG_WHT, TC_BLK);
    	printf("J      \n");
    	printf("%s      \n", ESPADAS);
    	printf("   %s   \n", ESPADAS);
    	printf("      %s\n", ESPADAS);
    	printf("      J\n");
			printf("%s%s\n", TC_BG_NRM, TC_RST);
    }
     
    if(k == 11) //Se o numero gerado aleatoriamente é 11, imprime carta com A (Ace) 
    {
    	//Carta de espadas
    	printf("%s%s", TC_BG_WHT, TC_BLK);
    	printf("A      \n");
    	printf("%s      \n", ESPADAS);
    	printf("   %s   \n", ESPADAS);
    	printf("      %s\n", ESPADAS);
    	printf("      A\n");
			printf("%s%s\n", TC_BG_NRM, TC_RST);
			
    	if(l <= 10) //Se o nuemro gerado aleatoriamente é Ace, mudar valor para 11 ou 1 dependendo do total do dealer 
        k = 11;
      else
        k = 1;
    }
     
    if(k == 12) //Se o numero gerado aleatoriamente é 12, imprime carta com Q (quenn/rainha) 
    {
    	//Carta de Espadas
    	printf("%s%s", TC_BG_WHT, TC_BLK);
    	printf("Q      \n");
    	printf("%s      \n", ESPADAS);
    	printf("   %s   \n", ESPADAS);
    	printf("      %s\n", ESPADAS);
    	printf("      Q\n");
			printf("%s%s\n", TC_BG_NRM, TC_RST);
    	k = 10; //Valor da carta é 10
    }
     
    if(k == 13) //Se o numero gerado aleatoriamente é 13, imprime carta com K (king/rei) 
    {
    	//Carta de Espadas
    	printf("%s%s", TC_BG_WHT, TC_BLK);
    	printf("K      \n");
    	printf("%s      \n", ESPADAS);
    	printf("   %s   \n", ESPADAS);
    	printf("      %s\n", ESPADAS);
    	printf("      K\n");
			printf("%s%s\n", TC_BG_NRM, TC_RST);
    	k = 10; //Valor da carta é 10
    }
    l = k;
  }
	return l;
}

void reiniciar_jogo(t_card *cartas, t_lista *jogadores) //Pergunta ao player se deseja jogar novamente
{
	t_card *novasCartas;
  char SimNao;
    
  printf("\nGostaria de jogar novamente? (s/n)");
  scanf("\n%c",&SimNao);
 
  while((SimNao!='S') && (SimNao!='s') && (SimNao!='N') && (SimNao!='n')) //Se uma entrada inválida for inserida
  {                                                                          
    printf("\n");
    printf("Entrada inválida, insira 's' para sim, e 'n' para não.\n");
    scanf("%c",&SimNao);
  }
 
  if((SimNao == 'S') || (SimNao == 's')) //Se sim, reinicia o jogo
  {
    system("clear");
    novo_jogo(novasCartas, jogadores);
  }
  
  else if((SimNao == 'N') || (SimNao == 'n')) //Se não, sai do jogo
  {
    salvar_ranking(jogadores);
    printf("\nObrigado por jogar!\n\n");
    system("pause");
    exit(0);
  }
  return;
}

void salvar_ranking(t_lista *lista) { //Apende nova pontuação no final do ranking.txt
	FILE *fp = fopen("./ranking.txt", "a"); 
	if(fp != NULL) {
		t_player *p = lista->primeiro;
		do {
			fprintf(fp, "%s | Partidas vencidas: %d | Partidas Perdidas: %d\n", p->nomePlayer, p->ganhou, p->perdeu);
			p = p->proximo;
		} while(p != lista->primeiro);
	}
	fprintf(fp, "\n-----Fim de Jogo-----\n");
	fclose(fp);
	getchar();
}

void acessar_ranking() { //Imprimir tudo no arquivo ranking.txt
	FILE *fp = fopen("./ranking.txt", "r");
	char ch; int k;

	if((ch = fgetc(fp)) != EOF)
	{
		printf("%c", ch);
		while((ch = fgetc(fp)) != EOF) {
  	    printf("%c", ch);
  	}
	} else printf("Ranking vazio!");
	fclose(fp);
	getchar();
}

void zerar_ranking() { //Substitui o ranking.txt por nada
	char ch;
	
	printf("Zerar ranking? (s/n): ");
	scanf("%c", &ch);

	if(ch == 's') {
		FILE *fp = fopen("./ranking.txt", "w");
		fprintf(fp, "");
		fclose(fp);
		
		printf("Ranking zerado!");
	} else printf("Ok!");
	
	getchar(); getchar();
}

/*
imprimir carta simples:
void draw_card_simples(int x, int y, t_card card) {
term_move_xy(x,y);

printf("%s%s%d%s", TC_BG_WHT, card.color, card.value, card.naipe);
//cor de fundo, cor do texto, valor da carta, náipe
}
draw_card_simples(10, 10, card) //x, y, struct card

TC_RST = Terminal Color Reset
*/

/*

int console_cols, console_rows;

void on_console_window_change() {
  tc_get_cols_rows(&console_cols, &console_rows);
}

void print_main_menu() {
  term_move_xy(0,0);
  printf("1 - Novo jogo\n");
  printf("2 - Pontuação\n");
  printf("3 - Creditos\n");
  printf("0 - Sair\n");
  printf("Opção: ");
}

int main(){
  tc_get_cols_rows(&console_cols, &console_rows);
  
	tc_echo_off();
  tc_canon_off();

  t_card card = {1, COPAS, TC_RED, 1};
  
  term_cls();
  draw_card(1,1, card);

  draw_empty_card(40,1);
  draw_empty_card(53,1);
  draw_empty_card(66,1);
  draw_empty_card(79,1);

  card.is_hidden = 0;
  draw_card(1,10, card);
  
  card.value++; card.naipe = PAUS; card.color = TC_BLK;
  card.is_hidden = 1;
  draw_card(14,10, card);
  card.is_hidden = 0;
  draw_card(15,11, card);

  
  card.value++; card.naipe = OUROS; card.color = TC_RED;
  card.is_hidden = 1;
  draw_card(27,10, card);
  draw_card(28,11, card);
  card.is_hidden = 0;
  draw_card(29,12, card);

  card.value++; card.naipe = ESPADAS; card.color = TC_BLK;
  card.is_hidden = 1;
  draw_card(40,10, card);
  draw_card(41,11, card);
  draw_card(42,12, card);
  card.is_hidden = 0;
  draw_card(43,13, card);

  card.value++; card.naipe = COPAS; card.color = TC_RED;
  card.is_hidden = 1;
  draw_card(53,10, card);
  draw_card(54,11, card);
  draw_card(55,12, card);
  draw_card(56,13, card);
  card.is_hidden = 0;
  draw_card(57,14, card);

  card.value++; card.naipe = PAUS; card.color = TC_BLK;
  card.is_hidden = 1;
  draw_card(66,10, card);
  draw_card(67,11, card);
  draw_card(68,12, card);
  draw_card(69,13, card);
  draw_card(70,14, card);
  card.is_hidden = 0;
  draw_card(71,15, card);

  card.value++; card.naipe = OUROS; card.color = TC_RED;
  card.is_hidden = 1;
  draw_card(79,10, card);
  draw_card(80,11, card);
  draw_card(81,12, card);
  draw_card(82,13, card);
  draw_card(83,14, card);
  draw_card(84,15, card);
  card.is_hidden = 0;
  draw_card(85,16, card);
    //cg_drawline(1,1,console_cols/2, console_rows/2);
    //animate_card(console_cols-7, 1, console_cols/2, console_rows/2, card);
    term_move_xy(console_cols, console_rows);
    puts("#");
  getchar();
	return 0;

}//main*/ //programa referência terminal_io