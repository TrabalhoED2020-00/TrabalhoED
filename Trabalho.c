#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct aluno{
	char nome[51];
	int matricula;
	char curso[3];
	char disciplina[4];
	int faltas;
	int nota;
	char mencao[3];
	struct aluno *prox;
	struct aluno *ant;
} Aluno;

typedef struct Lista{
	int tam;
	Aluno *inicio;
	Aluno *fim;
}lista;

typedef struct materia{
	int nalunos;
	char disciplina[4];
	struct materia *prox;
	struct materia *ant;
}Materia;

typedef struct ListaMat{
	int tam;
	Materia *inicio; 
	Materia *fim;
}listamat;

lista* CriarLista (){
	lista* LISTA = (lista*)malloc(sizeof(lista));
	if (LISTA == NULL){
		printf("\nSem memoria disponivel!\n");
		exit(0);
	}
	LISTA->inicio = NULL;
	LISTA->fim = NULL;
	LISTA->tam = 0;
	return LISTA;
}

listamat* CriarListaMat (){
	listamat* LISTAMAT = (listamat*)malloc(sizeof(listamat));
	if (LISTAMAT == NULL){
		printf("\nSem memoria disponivel!\n");
		exit(0);
	}
	LISTAMAT->inicio = NULL;
	LISTAMAT->fim = NULL;
	LISTAMAT->tam = 0;
	return LISTAMAT;
}

void insereMaterias (listamat* LISTAMAT, char nome[51], char disciplina[]){
	int cont = 0;
	Materia* novo = (Materia*)malloc(sizeof(Materia));
	Materia* atual;
	if (novo != NULL){
		if (LISTAMAT != NULL){
			atual = LISTAMAT->inicio;
			while(atual != NULL){
				if(strcmp(atual->disciplina, disciplina) == 0){
					cont++;
					atual->nalunos++;
				}
				atual = atual->prox;
			}
		}
		if (cont == 0){
			strcpy(novo->disciplina, disciplina);
			if (nome != NULL){
				novo->nalunos = 1;
			}
			else{
				novo->nalunos = 0;
			}
			novo->prox = NULL;
			novo->ant = LISTAMAT->fim;
			if (LISTAMAT->fim != NULL){
				LISTAMAT->fim->prox = novo;
				LISTAMAT->fim = novo;
			}
			else{
				LISTAMAT->inicio = novo;
				LISTAMAT->fim = novo;
			}
			LISTAMAT->tam++;
		}
		else{
			free(novo);
		}
	}	
}

void insereFim (lista* LISTA, char nome[], int matricula, char curso[], char disciplina[], int faltas, int nota, char mencao[]){
	Aluno *novo = (Aluno*)malloc(sizeof(Aluno));
	if (novo != NULL){
		strcpy(novo->nome, nome);
		novo->matricula = matricula;
		strcpy(novo->curso, curso);
		strcpy(novo->disciplina, disciplina);
		novo->faltas = faltas;
		novo->nota = nota;
		strcpy(novo->mencao, mencao);
		
		novo->prox = NULL;
		novo->ant = LISTA->fim;
		
		if (LISTA->fim != NULL){
			LISTA->fim->prox = novo;
			LISTA->fim = novo;
		}
		else{
			LISTA->inicio = novo;
			LISTA->fim = novo;
		}
	}
	LISTA->tam++;
}

void dados (FILE* arquivo, lista* LISTA, listamat* LISTAMAT){
	char nome[51], curso[3], disciplina[4], mencao[3];
	int matricula, faltas, nota;
	while(!feof(arquivo)){
		fscanf(arquivo, "%[^;];%d;%[^;];%[^;];%d;%d;%s", nome, &matricula, curso, disciplina, &faltas, &nota, mencao);
		insereFim(LISTA, nome, matricula, curso, disciplina, faltas, nota, mencao);
		insereMaterias(LISTAMAT, nome, disciplina);
		
	}
}

void imprime(lista *lista){
	Aluno *noAux;
	noAux = lista->inicio;
	while(noAux != NULL){
		printf("%s  %d  %s  %s  %d  %d  %s", noAux->nome, noAux->matricula, noAux->curso, noAux->disciplina, noAux->faltas, noAux->nota, noAux->mencao);
		noAux = noAux->prox;
	}
	printf("\n");
}

void ListarDisciplina(listamat* LISTAMAT){
	int len;
	Materia *atual;
	atual = LISTAMAT->inicio;
	printf("\nDisciplina   Alunos\n");
	while (atual != NULL){
		len = strlen(atual->disciplina);
		if (len == 1){
			printf("%s            %d\n", atual->disciplina, atual->nalunos);
		}
		if (len == 2){
			printf("%s           %d\n", atual->disciplina, atual->nalunos);
		}
		if (len == 3){
			printf("%s          %d\n", atual->disciplina, atual->nalunos);
		}
		atual = atual->prox;
	}
}

void AdicionarDisciplina (listamat* LISTAMAT){
	Materia *atual;
	char sigla[4];
	int i;
	atual = LISTAMAT->inicio;
	printf("\nAdicionar disciplina\nDigite a sigla: ");
	scanf("%s", sigla);
	for (i = 0; i < LISTAMAT->tam; i++){
		if (strcmp(atual->disciplina, sigla) == 0){
			printf("Disciplina ja existe!\n");
			return;
		}
		atual = atual->prox;
	}
	insereMaterias(LISTAMAT, 0, sigla);
}

void RemoverDisciplina (lista* LISTA, listamat *LISTAMAT){
	char sigla[4];
	int i, cont = 0;
	Aluno *atual = LISTA->inicio;
	Materia *atual2 = LISTAMAT->inicio;
	printf("\nRemover disciplina\nDigite a sigla: ");
	scanf("%s", sigla);
	while (atual2 != NULL){
		if (strcmp(atual2->disciplina, sigla) == 0){
			cont++;
			if (atual2 == LISTAMAT->inicio){
				LISTAMAT->inicio = LISTAMAT->inicio->prox;
				LISTAMAT->inicio->ant = NULL;
			}
			else{
				if (atual2 == LISTAMAT->fim){
					LISTAMAT->fim = LISTAMAT->fim->ant;
					LISTAMAT->fim->prox = NULL;
				}
				else{
					atual2->ant->prox = atual2->prox;
					atual2->prox->ant = atual2->ant;
				}
			}
			free(atual2);
			LISTAMAT->tam--;
		}
		atual2 = atual2->prox;
	}
	if (cont == 0){
		printf("Disciplina nao cadastrada!\n");
	}
	else{
		for (i = 0; i < LISTA->tam; i++){
			if (strcmp(atual->disciplina, sigla) == 0){
				strcpy(atual->disciplina, "N/D");
			}
			atual = atual->prox;
		}
	}
}

void SemDisciplina (lista* LISTA){
	int i;
	Aluno *atual = LISTA->inicio;
	printf("\nNome       Matricula      Curso\n");
	for (i = 0; i < LISTA->tam; i++){
		if (strcmp(atual->disciplina,"N/D") == 0){
			printf("%s      %d       %s", atual->nome, atual->matricula, atual->curso);
		}
		atual = atual->prox;
	}
	printf("\n");
}

void AdicionarAluno (lista* LISTA){
	char nome[51], curso[4];
	int matricula;
	printf("Adicionar aluno\n");
	printf("Nome:");
	scanf("%s", nome);
	printf("Matricula: ");
	scanf("%d", &matricula);
	printf("Curso: ");
	scanf("%s", curso);
	insereFim(LISTA, nome, matricula, curso, "N/D", 0, 11, "SN");
}

void RemoverAluno (lista* LISTA){
	int matricula, i, cont = 0, existe = 0;
	Aluno *atual = LISTA->inicio;
	printf("\nRemover aluno\nMatricula: ");
	scanf("%d", &matricula);
	for (i = 0; i < LISTA->tam; i++){
		if (matricula == atual->matricula){
			existe++;
			if (strcmp(atual->disciplina, "N/D") != 0){
				cont++;
				break;
			}
		}
		atual = atual->prox;
	}
	if (cont == 0){
		atual = LISTA->inicio;
		while(atual != NULL){
			if (matricula == atual->matricula){
				if (atual == LISTA->inicio){
					LISTA->inicio = LISTA->inicio->prox;
					LISTA->inicio->ant = NULL;
				}
				else{
					if (atual == LISTA->fim){
						LISTA->fim = LISTA->fim->ant;
						LISTA->fim->prox = NULL;
					}
					else{
						atual->ant->prox = atual->prox;
						atual->prox->ant = atual->ant;
					}
				}
				free(atual);
				LISTA->tam--;
			}
			atual = atual->prox;
		}
	}
	if (existe == 0){
		printf("Matricula nao encontrada!\n");
	}
}

void IncluirAluno (lista* LISTA, listamat* LISTAMAT){
	int matricula, existemat = 0, existeal = 0;
	char disciplina[4];	
	Materia* atualmat = LISTAMAT->inicio;
	Aluno* atualal = LISTA->inicio;
	printf("Incluir aluno\nMatricula: ");
	scanf("%d", &matricula);
	printf("Disciplina: ");
	scanf("%s", disciplina);
	while(atualmat != NULL){
		if (strcmp(atualmat->disciplina, disciplina) == 0){
			existemat++;
			break;
		}
		atualmat = atualmat->prox;
	}
	while(atualal != NULL){
		if (atualal->matricula == matricula){
			existeal++;
			break;
		}
		atualal = atualal->prox;
	}
	if (existemat != 0 && existeal != 0){
		atualmat->nalunos++;
	}
	else{
		if (existemat == 0){
			printf("Disciplina nao encontrada!\n");
		}
		if (existeal == 0){
			printf("Aluno nao encontrado!\n");
		}
	}
}

void Salvar (FILE* arquivo, lista* LISTA){
	Aluno* atual = LISTA->inicio;
	while(atual != NULL){
		fprintf(arquivo,"%s;%d;%s;%s;%d;%d;%s", atual->nome, atual->matricula, atual->curso, atual->disciplina, atual->faltas, atual->nota, atual->mencao);
		atual = atual->prox;
	}
}

int main(){
	int opcao;
	FILE* arqNome;
	lista* LISTA = CriarLista();
	listamat* LISTAMAT = CriarListaMat();
	if ((arqNome = fopen("alunos.txt","at+")) == NULL){
		printf("\nNao foi possivel abrir o arquivo.\n");
		return 0;
	}
	dados(arqNome, LISTA, LISTAMAT);
	while(1){
		system("cls || clear");
		printf("Ola professor,\nX aluno(s) nao estao matriculados.\nO que deseja fazer:\n");
		printf("1 Listar disciplinas\n");
		printf("2 Adicionar disciplina\n");
		printf("3 Remover disciplina\n");
		printf("4 Listar alunos sem disciplina\n");
		printf("5 Adicionar aluno\n");
		printf("6 Remover aluno\n");
		printf("7 Incluir aluno em disciplina\n");
		printf("8 Gerenciar disciplina\n");
		printf("9 Salvar alteracoes\n");
		printf("10 Sair\n");
		printf("Digite a opcao: ");
		scanf("%d", &opcao);
		switch (opcao){
				case 1: ListarDisciplina(LISTAMAT); break;
				case 2: AdicionarDisciplina(LISTAMAT); break;
				case 3: RemoverDisciplina(LISTA, LISTAMAT); break;
				case 4: SemDisciplina(LISTA); break;
				case 5: AdicionarAluno(LISTA); break;
				case 6: RemoverAluno(LISTA); break;
				case 7: IncluirAluno(LISTA, LISTAMAT); break;
				case 8: break;
				case 9: Salvar(arqNome, LISTA); break;
				case 10: fclose(arqNome); exit(0); break;
				case 11: imprime(LISTA); break;
				default: printf("\nComando invalido!\n");
		}
		printf("Pressione enter para voltar");
		fflush(stdin);
		getchar();
	}
	return 0;
}