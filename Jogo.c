#include<stdio.h>
#include<stdlib.h>
#include<locale.h>
#include<time.h>

typedef struct carta
{
	char num;
	int naipe; //1- copas, 2- ouros, 3- espadas, 4- paus
	int valor;
}Carta;

typedef struct baralho
{
	int topo;
	Carta baralho[40];
}Baralho;


Baralho* criar_baralho() //Aloca a memória para o baralho
{
	Baralho *b;
	b = (Baralho*)malloc(sizeof(Baralho));
	b->topo = 0;
	return b;
}

typedef struct jogador
{
	char nome[100];
	int num_vidas, mata, topo_mao, num_vitorias;
	Carta mao[5];
}Jogador;

typedef struct mesa
{
	Carta rodada_atual[8];
}Mesa;

void pegar_baralho(Baralho* b) //Insere todas as 40 cartas usadas no jogo no baralho
{
	int i;
	char num = '1';
	for(i = 0; i < 40; i++)
	{
		if(i < 7 || i >= 10 && i < 17 || i >= 20 && i < 27 || i >= 30 && i < 37) //Insere os números de 1 a 7
		{
			b->baralho[i].num = num;
			num++;
			if(num == '8')
			{
				num = '1';
			}
		}
		else if(i == 7 || i == 17 || i == 27 || i == 37) //Insere os valetes
		{
			b->baralho[i].num = 'J';
		}
		else if(i == 8 || i == 18 || i == 28 || i == 38) //Insere as rainhas
		{
			b->baralho[i].num = 'Q';
		}
		else if(i == 9 || i == 19 || i == 29 || i == 39) //Insere os reis
		{
			b->baralho[i].num = 'K';
		}
		
		if(i <= 9) //Insere o naipe de copas
		{
			b->baralho[i].naipe = 1;
		}
		else if(i <= 19) //Insere o naipe de ouro
		{
			b->baralho[i].naipe = 2;
		}
		else if(i <= 29) //Insere o naipe de espadas
		{
			b->baralho[i].naipe = 3;
		}
		else if(i <= 39) //Insere o naipe de paus
		{
			b->baralho[i].naipe = 4;
		}
		
		b->topo = b->topo + 1; //Incrementa o topo para cada carta
	}
	
	int array[40] = {27, 31, 35, 3, 6, 10, 39, 16, 20, 24, 26, 29, 33, 1, 4, 8, 37, 14, 18, 22, 38, 30, 34, 2, 5, 9, 12, 15, 19, 23, 28, 32, 36, 40, 7, 11, 13, 17, 21, 25};
	int i2;
	for(i2 = 0; i2 < 40; i2++)
	{
		b->baralho[i2].valor = array[i2]; //Atribui o valor de cada carta para serem usados nas comparações
	}
}

void embaralhar(Baralho* b) //Embaralha o baralho
{
	srand(time(NULL));
	int i;
	for (i = 0; i < 40; i++)
	{
		char swap_index = rand() % 40; //Gera os números aleatórios entre 1 e 40
    
    	char temp = b->baralho[i].num; //Troca o número de uma carta para uma posição aleatória
    	b->baralho[i].num = b->baralho[swap_index].num;
    	b->baralho[swap_index].num = temp;
    
    	char temp2 = b->baralho[i].naipe; //Em seguida troca o naipe
    	b->baralho[i].naipe = b->baralho[swap_index].naipe;
    	b->baralho[swap_index].naipe = temp2;
    
    	char temp3 = b->baralho[i].valor; //E por fim o valor
    	b->baralho[i].valor = b->baralho[swap_index].valor;
    	b->baralho[swap_index].valor = temp3;
	}
}

void imprimir_cartas(int num_cartas_rodada, Jogador j[8], int num_jogador) //Imprime as cartas na mão de um jogador
{
	FILE *arq = fopen("log.txt", "a"); //Abre o arquivo de texto para ser modificado e insere as informações no fim do arquivo
	printf("Cartas de %s:\n\n", j[num_jogador].nome);
	fprintf(arq, "Cartas de %s:\n\n", j[num_jogador].nome);
	fclose(arq); //Fecha o arquivo de texto
	int i;
	for(i = 0; i < num_cartas_rodada; i++)
	{
		imprimir_carta_singular(j[num_jogador].mao[i].num, j[num_jogador].mao[i].naipe);
	}
}

void imprimir_carta_singular(char num, int naipe) //Imprime uma única carta substituindo 1 por Ás, J por valete, Q por rainha, K por rei e os naipes pelos seus respectivos nomes
{
	FILE *arq = fopen("log.txt", "a"); //Abre o arquivo de texto para ser modificado e insere as informações no fim do arquivo
	if(num == 1)
	{
		printf("Ás ");
		fprintf(arq, "Ás ");
	}
	else if(num == 'J')
	{
		printf("Valete ");
		fprintf(arq, "Valete ");
	}
	else if(num == 'Q')
	{
		printf("Rainha ");
		fprintf(arq, "Rainha ");
	}
	else if(num == 'K')
	{
		printf("Rei ");
		fprintf(arq, "Rei ");
	}
	else
	{
		printf("%c ", num);
		fprintf(arq, "%c ", num);
	}
	printf("de ");
	fprintf(arq, "de ");
	if(naipe == 1)
	{
		printf("copas");
		fprintf(arq, "copas");
	}
	else if(naipe == 2)
	{
		printf("ouro");
		fprintf(arq, "ouro");
	}
	else if(naipe == 3)
	{
		printf("espadas");
		fprintf(arq, "espadas");
	}
	else if(naipe == 4)
	{
		printf("paus");
		fprintf(arq, "paus");
	}
	printf("\n");
	fprintf(arq, "\n");
	fclose(arq); //Fecha o arquivo de texto
}

void distribuir_mao(int num_jogadores, int num_cartas_rodada, Baralho* b, Jogador j[8]) //Retira do baralho e coloca na mão dos jogadores as cartas que serão usadas na rodada
{
	int i;
	for(i = 0; i < num_jogadores; i++)
	{
		int i2;
		for(i2 = 0; i2 < num_cartas_rodada; i2++)
		{
			j[i].mao[i2] = b->baralho[b->topo - 1];
			if(b->topo != 0)
			{
				b->topo = b->topo - 1;
			}
			j[i].topo_mao = num_cartas_rodada;
		}
	}
	b->topo = 0;
}

void qtde_rodadas_mata(int num_jogadores, int num_cartas_rodada, Jogador j[8]) //Salva quantas rodadas cada jogador mata
{
	int i;
	for(i = 0; i < num_jogadores; i++)
	{
		do
		{
			imprimir_cartas(num_cartas_rodada, j, i);
			FILE *arq = fopen("log.txt", "a"); //Abre o arquivo de texto para ser modificado e insere as informações no fim do arquivo
    		printf("\n%s: quantas rodadas você mata? ", j[i].nome);
    		scanf("%d", &j[i].mata);
    		fprintf(arq, "\n%s: quantas rodadas você mata? %d\n\n\n", j[i].nome, j[i].mata);
    		if(j[i].mata < 0 || j[i].mata > num_cartas_rodada)
    		{
    			printf("Você não pode matar mais do que a quantidade de cartas que você tem em mãos.\n\n");
    			fprintf(arq, "Você não pode matar mais do que a quantidade de cartas que você tem em mãos.\n\n");
    		}
    		else
    		{
    			printf("Mata: %d\n\n", j[i].mata);
			}
			fclose(arq); //Fecha o arquivo de texto
			system("pause");
			system("cls");
    	}while(j[i].mata < 0 || j[i].mata > num_cartas_rodada);
    }
    
    int soma_rodadas = 0;
    for(i = 0; i < num_jogadores; i++) //Soma o total de rodadas que todos os jogadores matam nesta partida
    {
    	soma_rodadas = soma_rodadas + j[i].mata;
	}
	FILE *arq = fopen("log.txt", "a"); //Abre o arquivo de texto para ser modificado e insere as informações no fim do arquivo
	printf("Total de rodadas: %d\n\n\n", soma_rodadas);
	fprintf(arq, "Total de rodadas: %d\n\n", soma_rodadas);
	fclose(arq); //Fecha o arquivo de texto
	system("pause");
    system("cls");
}

int jogar_cartas(int num_jogadores, int num_cartas_rodada, Jogador j[8], Mesa m) //Joga as cartas e as compara
{
	int i;
	for(i = 0; i < num_jogadores; i++)
	{
		int opcao[8];
		do
		{
			int i2;
			imprimir_cartas(num_cartas_rodada, j, i); //Imprime as cartas de cada jogador
			FILE *arq = fopen("log.txt", "a"); //Abre o arquivo de texto para ser modificado e insere as informações no fim do arquivo
			printf("\nQual carta você quer jogar? ");
			scanf("%d", &opcao[i]); //O jogador escolhe uma carta para jogar
			fprintf(arq, "\nQual carta você quer jogar? %d\n\n", opcao[i]);
			if(opcao[i] <= 0 || opcao[i] > num_cartas_rodada) //Se a carta escolhida não for válida repete a escolha
			{
				printf("Escolha uma opção válida!\n\n");
				fprintf(arq, "Escolha uma opção válida!\n\n\n");
				system("pause");
				system("cls");
			}
			fclose(arq);
		}while(opcao[i] <= 0 || opcao[i] > num_cartas_rodada);
		    	
		int i4;
		opcao[i] = opcao[i] - 1;
		m.rodada_atual[i] = j[i].mao[opcao[i]]; //Coloca na mesa na mesa
		for(i4 = opcao[i]; i4 < j[i].topo_mao; i4++)
		{
			j[i].mao[i4] = j[i].mao[i4 + 1]; //Tira a carta da mão
		}
		j[i].topo_mao--; //Diminui uma carta do total de cartas da mão
		imprimir_carta_singular(m.rodada_atual[i].num, m.rodada_atual[i].naipe); //Imprime a carta jogada
		printf("\n");
		system("pause");
		system("cls");
		FILE *arq = fopen("log.txt", "a"); //Abre o arquivo de texto para ser modificado e insere as informações no fim do arquivo
		printf("Cartas na mesa:\n\n");
		fprintf(arq, "\n\n\nCartas na mesa:\n\n");
		fclose(arq);
		int i3;
		for(i3 = 0; i3 < i + 1; i3++) //Imprime as cartas que estão na mesa
		{
		    imprimir_carta_singular(m.rodada_atual[i3].num, m.rodada_atual[i3].naipe);
		}
		arq = fopen("log.txt", "a"); //Abre o arquivo de texto para ser modificado e insere as informações no fim do arquivo
		printf("\n\n");
		fprintf(arq, "\n\n");
		fclose(arq); //Fecha o arquivo de texto
	}
	
	int vencedor = 0;
	for(i = 0; i < num_jogadores - 1; i++)
	{
		if(m.rodada_atual[vencedor].valor < m.rodada_atual[i + 1].valor) //Compara as cartas e a maior vence
		{
			vencedor = i + 1; //Quem jogou a maior carta é declarado vencedor da rodada
		}
	}
	FILE *arq = fopen("log.txt", "a"); //Abre o arquivo de texto para ser modificado e insere as informações no fim do arquivo
	printf("%s é o vendedor da rodada!\n\n", j[vencedor].nome);
	fprintf(arq, "%s é o vendedor da rodada!\n\n\n", j[vencedor].nome);
	fclose(arq); //Fecha o arquivo de texto
	return vencedor;
}

void testar_vidas(int num_jogadores, Jogador j[8]) //Calcula quantas vidas cada jogador perdeu na rodada
{
	FILE *arq = fopen("log.txt", "a"); //Abre o arquivo de texto para ser modificado e insere as informações no fim do arquivo
	int i;
	for(i = 0; i < num_jogadores; i++)
	{
		printf("%s disse que mataria %d e matou %d. ", j[i].nome, j[i].mata, j[i].num_vitorias);
		fprintf(arq, "%s disse que mataria %d e matou %d. ", j[i].nome, j[i].mata, j[i].num_vitorias);
		if(j[i].mata == j[i].num_vitorias) //Se ele acertou quantas mataria ele não perde nenhuma vida
		{
			printf("Parabéns!\n");
			fprintf(arq, "Parabéns!\n");
		}
		else //Se não acertou então perde vidas proporcionalmente ao erro
		{
			int perde_vidas = abs(j[i].mata - j[i].num_vitorias);
			j[i].num_vidas = j[i].num_vidas - perde_vidas;
			if(j[i].num_vidas < 0)
			{
				j[i].num_vidas = 0;
			}
			printf("Perde %d vidas! Restam %d vidas!\n", perde_vidas, j[i].num_vidas);
			fprintf(arq, "Perde %d vidas! Restam %d vidas!\n", perde_vidas, j[i].num_vidas);
		}
		j[i].num_vitorias = 0;
	}
	fprintf(arq, "\n\n");
	fclose(arq); //Fecha o arquivo de texto
}

int jogadores_restantes(int num_jogadores, Jogador j[8]) //Conta quantos jogadores restam e remove aqueles que não tem mais vidas
{
	FILE *arq = fopen("log.txt", "a"); //Abre o arquivo de texto para ser modificado e insere as informações no fim do arquivo
	int restam = 0;
	int i;
	for(i = 0; i < num_jogadores; i++)
	{
		if(j[i].num_vidas > 0) //Se o número de vidas for maior que 0 então soma 1 jogador restante a mais
		{
			restam++;
		}
		else //Se não remove os jogadores que não tem vidas restantes
		{
			printf("%s foi removido do jogo!\n", j[i].nome);
			fprintf(arq, "%s foi removido do jogo!\n\n\n", j[i].nome);
			num_jogadores--;
			int i2;
			for(i2 = i; i2 < num_jogadores; i2++)
			{
				j[i2] = j[i2 + 1];
			}
			i--;
		}
	}
	fclose(arq); //Fecha o arquivo de texto
	return restam;
}

void girar_mesa(int num_jogadores, Jogador j[8]) //Roda a mesa, fazendo o primeiro a jogar ser o último e o segundo ser o primeiro
{
	Jogador aux = j[0];
	int i;
	for(i = 0; i < num_jogadores - 1; i++)
	{
		j[i] = j[i + 1];
	}
	j[num_jogadores - 1] = aux;
}

int main(void)
{
	setlocale(LC_ALL, "Portuguese");
	
	FILE *arq;
	Baralho *b;
	int num_jogadores, num_cartas, num_cartas_rodada, rodada, partida, soma_rodadas, vencedor, restam, delta;
	Jogador j[8];
	Carta rodada_atual[8];
	Mesa m;
	
	num_cartas_rodada = 5;
	num_cartas = 5;
	partida = 1;
	delta = 1;
	
	b = criar_baralho(); //Cria o baralho
	
	do
	{
		printf("Quantos jogadores jogarão? (Entre 2 e 8 jogadores) "); //Decide o número de jogadores
		scanf("%d", &num_jogadores);
		
		arq = fopen("log.txt", "w"); //Abre o arquivo de texto para ser modificado e insere as informções no arquivo
		fprintf(arq, "Quantos jogadores jogarão? (Entre 2 e 8 jogadores) %d\n\n", num_jogadores);
		
		if(num_jogadores < 2 || num_jogadores > 8) //Se o número não estiver dentro do pedido repete o processo
		{
			printf("Favor inserir um número de jogadores válido.\n\n");
			fprintf(arq, "Favor inserir um número de jogadores válido.\n\n");
			system("pause");
			system("cls");
		}
		else //Se o número for válido então armazena os nomes dos jogadores e lhes dá suas vidas
		{
			int i;
			for(i = 0; i < num_jogadores; i++) //Salva os nomes dos jogadores
			{
				system("cls");
				printf("Digite o nome do jogador %d: ", i + 1);
				scanf("%s", j[i].nome);
				fprintf(arq, "Digite o nome do jogador %d: %s\n\n", i + 1, j[i].nome);
				j[i].num_vidas = 5;
				j[i].num_vitorias = 0;
			}
		}
		fclose(arq); //Fecha o arquivo de texto
	}while(num_jogadores < 2 || num_jogadores > 8);
	
	system("cls");
	
	do
	{
		arq = fopen("log.txt", "a"); //Abre o arquivo de texto para ser modificado e insere as informações no fim do arquivo
		printf("----------Partida %d----------\n\n", partida);
		fprintf(arq, "----------Partida %d----------\n\n", partida);
		fclose(arq); //Fecha o arquivo de texto
		pegar_baralho(b); //Preenche o baralho
		embaralhar(b); //Embaralha o baralho
		distribuir_mao(num_jogadores, num_cartas_rodada, b, j); //Entrega as cartas aos jogadores	
		qtde_rodadas_mata(num_jogadores, num_cartas_rodada, j); //Armazena a quantidade de cartas que cada jogador mata
		rodada = 1;
		do
		{
			while(j[0].topo_mao != 0) //Enquanto houverem cartas na mão
			{
				arq = fopen("log.txt", "a"); //Abre o arquivo de texto para ser modificado e insere as informações no fim do arquivo
				printf("-----Rodada %d-----\n\n", rodada);
				fprintf(arq, "-----Rodada %d-----\n\n", rodada);
				fclose(arq); //Fecha o arquivo de texto
			    vencedor = jogar_cartas(num_jogadores, num_cartas_rodada, j, m); //As cartas são jogadas e o vencedor da rodada é decidido
				j[vencedor].num_vitorias++; //O jogador vitorioso recebe 1 vitória
				num_cartas_rodada--; //O número de cartas para a próxima rodada é diminuído
				rodada++; //A rodada aumenta
				system("pause");
				system("cls");
			}
			testar_vidas(num_jogadores, j); //Calcula quantas vidas cada jogador perdeu
		}while(j[0].topo_mao != 0);
		restam = jogadores_restantes(num_jogadores, j); //Calcula o número de jogadores restantes
		num_jogadores = restam;
		partida++; //A partida aumenta
		num_cartas -= delta; //O número de cartas inicial para a próxima partida é alterado
		num_cartas_rodada = num_cartas;
		if(num_cartas == 0) //Se as cartas chegarem a 0 troca a ordem para crescente
		{
			delta = -1;
			num_cartas = 2;
			num_cartas_rodada = 2;
		}
		else if(num_cartas == 5) //Se as cartas chegarem a 5 troca a ordem para decrescente
		{
			delta = 1;
		}
		girar_mesa(num_jogadores, j); //Gira a ordem de jogada dos jogadores
		system("pause");
    	system("cls");
	}while(restam > 1); //Se restar apenas um jogador ou menos o jogo acaba
	arq = fopen("log.txt", "a"); //Abre o arquivo de texto para ser modificado e insere as informações no fim do arquivo
	if(restam == 1) //Se restou 1 jogador, então ele é declarado campeão
	{
		printf("---%s venceu!---\n\n", j[0].nome);
		fprintf(arq, "---%s venceu!---\n\n", j[0].nome);
	}
	else //Se não restaram jogadores então ninguém venceu
	{
		printf("---Ninguém venceu!---\n\n");
		fprintf(arq, "---Ninguém venceu!---\n\n");
	}
	fclose(arq); //Fecha o arquivo de texto
	return 0;
}
