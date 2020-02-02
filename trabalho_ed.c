#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct aluno{
	char nome[51];
	int matricula;
	char curso[3];
	char disciplina[4];
	float faltas;
	float nota;
	char mencao[3];
	struct aluno *prox;
	struct aluno *ant;
}Aluno;

typedef struct Lista{
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
	Materia *inicio;
	Materia *fim;
}listamat;

typedef struct curso{
	char curso[3];
	int nalunos;
	int ss;
	int ms;
	int mm;
	int mi;
	int ii;
	int sr;
	struct curso *prox;
	struct curso *ant;
}Curso;

typedef struct ListaCurso{
	Curso *inicio;
	Curso *fim;
}listacurso;

lista* CriarLista ();
listamat* CriarListaMat ();
void insereMaterias (listamat* LISTAMAT, char nome[], char disciplina[]);
void insereFim (lista* LISTA, char nome[], int matricula, char curso[], char disciplina[], int faltas, int nota, char mencao[]);
void dados (FILE* arquivo, lista* LISTA, listamat* LISTAMAT, listacurso* LISTACURSO);
void imprime(lista *lista);
void ListarDisciplina(listamat* LISTAMAT);
void AdicionarDisciplina (listamat* LISTAMAT);
void RemoverDisciplina (lista* LISTA, listamat *LISTAMAT);
void listar_alunos_sem_disciplina (lista* LISTA);
void AdicionarAluno (lista* LISTA);
void RemoverAluno (lista* LISTA);
void IncluirAluno (lista* LISTA, listamat* LISTAMAT);
void Salvar (FILE* arquivo, lista* LISTA);
void Opcoes (lista* LISTA, listamat* LISTAMAT, listacurso* LISTACURSO, int nalunos, char disciplina[]);
void Gerenciar (lista* LISTA, listamat* LISTAMAT, listacurso* LISTACURSO);
void listar_alunos(char disciplina[],lista* LISTA);
void atribuir_nota(char sigla[], lista* LISTA, listamat* LISTAMAT);
void atribuir_faltas(char sigla[], lista* LISTA, listamat* LISTAMAT);
int remover_aluno_da_disciplina(listamat* LISTAMAT, lista* LISTA, char sigla[]);
int semDisciplina(lista* LISTA);
void processar_turma(lista* LISTA, listacurso* LISTACURSO, char sigla[]);
listacurso* criarListaCurso();
void insereCurso (listacurso* LISTACURSO, char nome[], char curso[]);

int main(){
	int opcao, cont;
	FILE* arqNome;
	lista* LISTA = CriarLista();
	listamat* LISTAMAT = CriarListaMat();
	listacurso* LISTACURSO = criarListaCurso();
	if((arqNome = fopen("alunos.txt","rt")) == NULL){
		printf("\nNao foi possivel abrir o arquivo.\n");
		return 0;
	}
	dados(arqNome, LISTA, LISTAMAT, LISTACURSO);
	while(1){
		cont = semDisciplina(LISTA);
		system("cls || clear");
		printf("Ola professor,\n%d aluno(s) nao estao matriculados.\nO que deseja fazer:\n", cont);
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
		switch(opcao){
			case 1: ListarDisciplina(LISTAMAT); break;
			case 2: AdicionarDisciplina(LISTAMAT); break;
			case 3: RemoverDisciplina(LISTA, LISTAMAT); break;
			case 4: listar_alunos_sem_disciplina(LISTA); break;
			case 5: AdicionarAluno(LISTA); break;
			case 6: RemoverAluno(LISTA); break;
			case 7: IncluirAluno(LISTA, LISTAMAT); break;
			case 8: Gerenciar (LISTA, LISTAMAT, LISTACURSO);break;
			case 9: Salvar(arqNome, LISTA); break;
			case 10: exit(0);
			case 11: imprime(LISTA); break;
			default: printf("\nComando invalido!\n");
		}
		printf("Pressione enter para voltar");
		fflush(stdin);
		getchar();
	}
	return 0;
}

int semDisciplina(lista* LISTA){
	int cont = 0;					//Conta quantos alunos estão sem disciplina;
	Aluno* atual = LISTA->inicio;
	while (atual != NULL){
		if (strcmp(atual->disciplina, "N/D") == 0){
			cont++;
		}
		atual = atual->prox;
	}
	return cont;
}

lista* CriarLista(){
	lista* LISTA = (lista*)malloc(sizeof(lista));
	if(LISTA == NULL){
		printf("\nSem memoria disponivel!\n");
		exit(0);
	}
	LISTA->inicio = NULL;
	LISTA->fim = NULL;
	return LISTA;
}

listamat* CriarListaMat(){
	listamat* LISTAMAT = (listamat*)malloc(sizeof(listamat));
	if(LISTAMAT == NULL){
		printf("\nSem memoria disponivel!\n");
		exit(0);
	}
	LISTAMAT->inicio = NULL;
	LISTAMAT->fim = NULL;
	return LISTAMAT;
}

listacurso* criarListaCurso(){
	listacurso* l = (listacurso*)malloc(sizeof(listacurso));
	if (l == NULL){
		printf("\nSem memoria disponivel\n");
		exit(0);
	}
	l->inicio = NULL;
	l->fim = NULL;
	return l;
}

void insereCurso (listacurso* LISTACURSO, char nome[], char curso[]){
	int cont = 0;
	Curso* novo = (Curso*)malloc(sizeof(Curso));
	Curso* atual;
	if(novo != NULL){
		if(LISTACURSO != NULL){
			atual = LISTACURSO->inicio;
			while(atual != NULL){
				if(strcmp(atual->curso, curso) == 0){
					cont++;
					atual->nalunos++;
				}
				atual = atual->prox;
			}
		}
		if(cont == 0){
			strcpy(novo->curso, curso);
			if(nome != NULL){
				novo->nalunos = 1;
			}
			else{
				novo->nalunos = 0;
			}
			novo->prox = NULL;
			novo->ant = LISTACURSO->fim;
			if(LISTACURSO->fim != NULL){
				LISTACURSO->fim->prox = novo;
				LISTACURSO->fim = novo;
			}
			else{
				LISTACURSO->inicio = novo;
				LISTACURSO->fim = novo;
			}
		}
		else{
			free(novo);
		}
	}
}

void insereMaterias(listamat* LISTAMAT, char nome[], char disciplina[]){
	int cont = 0;
	Materia* novo = (Materia*)malloc(sizeof(Materia));
	Materia* atual;
	if(novo != NULL){
		if(LISTAMAT != NULL){
			atual = LISTAMAT->inicio;
			while(atual != NULL){
				if(strcmp(atual->disciplina, disciplina) == 0){
					cont++;
					atual->nalunos++;
				}
				atual = atual->prox;
			}
		}
		if(cont == 0){
			strcpy(novo->disciplina, disciplina);
			if(nome != NULL){
				novo->nalunos = 1;
			}
			else{
				novo->nalunos = 0;
			}
			novo->prox = NULL;
			novo->ant = LISTAMAT->fim;
			if(LISTAMAT->fim != NULL){
				LISTAMAT->fim->prox = novo;
				LISTAMAT->fim = novo;
			}
			else{
				LISTAMAT->inicio = novo;
				LISTAMAT->fim = novo;
			}
		}
		else{
			free(novo);
		}
	}
}

void insereFim(lista* LISTA, char nome[], int matricula, char curso[], char disciplina[], int faltas, int nota, char mencao[]){
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
}

void dados(FILE* arquivo, lista* LISTA, listamat* LISTAMAT, listacurso* LISTACURSO){
	char nome[51], curso[3], disciplina[4], mencao[3];
	int matricula; 
	float faltas, nota;
	while(!feof(arquivo)){
		fscanf(arquivo, "%[^;];%d;%[^;];%[^;];%f;%f;%s", nome, &matricula, curso, disciplina, &faltas, &nota, mencao);
		fgetc (arquivo);
		insereFim(LISTA, nome, matricula, curso, disciplina, faltas, nota, mencao);
		insereMaterias(LISTAMAT, nome, disciplina);
		insereCurso(LISTACURSO, nome, curso);
	}
	fclose(arquivo);
}

void imprime(lista *lista){
	Aluno *noAux;
	noAux = lista->inicio;
	while(noAux != NULL){
		printf("%s  %d  %s  %s  %.2f  %.2f  %s\n", noAux->nome, noAux->matricula, noAux->curso, noAux->disciplina, noAux->faltas, noAux->nota, noAux->mencao);
		noAux = noAux->prox;
	}
}

void ListarDisciplina(listamat* LISTAMAT){
	int len;
	Materia *atual;
	atual = LISTAMAT->inicio;
	printf("\nDisciplina   Alunos\n");
	while(atual != NULL){
		len = strlen(atual->disciplina);
		if(len == 1){
			printf("%s            %d\n", atual->disciplina, atual->nalunos);
		}
		if(len == 2){
			printf("%s           %d\n", atual->disciplina, atual->nalunos);
		}
		if(len == 3){
			printf("%s          %d\n", atual->disciplina, atual->nalunos);
		}
		atual = atual->prox;
	}
}

void AdicionarDisciplina(listamat* LISTAMAT){
	Materia *atual;
	char sigla[4];
	atual = LISTAMAT->inicio;
	printf("\nAdicionar disciplina\nDigite a sigla: ");
	scanf("%s", sigla);
	while(atual != NULL){
		if(strcmp(atual->disciplina, sigla) == 0){
			printf("Disciplina ja existe!\n");
			return;
		}
		atual = atual->prox;
	}
	insereMaterias(LISTAMAT, 0, sigla);
}

void RemoverDisciplina(lista* LISTA, listamat *LISTAMAT){
	char sigla[4];
	int cont = 0, libera;
	Aluno *atual = LISTA->inicio;
	Materia *atual2 = LISTAMAT->inicio;
	Materia *aux;
	printf("\nRemover disciplina\nDigite a sigla: ");
	scanf("%s", sigla);
	while(atual2 != NULL){
		libera = 0;
		if(strcmp(atual2->disciplina, sigla) == 0){
			cont++;
			libera++;
			if(atual2 == LISTAMAT->inicio){
					LISTAMAT->inicio = LISTAMAT->inicio->prox;
					LISTAMAT->inicio->ant = NULL;
				}
				else{
					if(atual2 == LISTAMAT->fim){

						LISTAMAT->fim = LISTAMAT->fim->ant;
						LISTAMAT->fim->prox = NULL;

					}
				else{
					atual2->ant->prox = atual2->prox;
					atual2->prox->ant = atual2->ant;
				}
			}
		}
		aux = atual2;
		atual2 = atual2->prox;
		if (libera > 0){
			free(aux);
		}
	}
	if(cont == 0){
		printf("Disciplina nao cadastrada!\n");
	}
	else{
		while(atual != NULL){
			if(strcmp(atual->disciplina, sigla) == 0){
				strcpy(atual->disciplina, "N/D");
			}
			atual = atual->prox;
		}
	}
}

void listar_alunos_sem_disciplina(lista* LISTA){
	int len, espaco;
	Aluno *atual = LISTA->inicio;
	printf("\nNome                                               Matricula      Curso\n");
	while(atual != NULL){
		if(strcmp(atual->disciplina,"N/D") == 0){
			len = strlen(atual->nome);
			espaco = 51 - len;
			printf("%s", atual->nome);
			while (espaco){
				printf(" ");
				espaco--;
			}
			printf("%d       %s\n", atual->matricula, atual->curso);
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
	int matricula, cont = 0, existe = 0;
	Aluno *atual = LISTA->inicio;
	printf("\nRemover aluno\nMatricula: ");
	scanf("%d", &matricula);
	while(atual != NULL){
		if(matricula == atual->matricula){
			existe++;
			if(strcmp(atual->disciplina, "N/D") != 0){
				cont++;
				break;
			}
		}
		atual = atual->prox;
	}
	if(cont == 0){
		atual = LISTA->inicio;
		while(atual != NULL){
			if(matricula == atual->matricula){
				if(atual == LISTA->inicio){
					LISTA->inicio = LISTA->inicio->prox;
					LISTA->inicio->ant = NULL;
				}
				else{
					if(atual == LISTA->fim){
						LISTA->fim = LISTA->fim->ant;
						LISTA->fim->prox = NULL;
					}
					else{
						atual->ant->prox = atual->prox;
						atual->prox->ant = atual->ant;
					}
				}
				free(atual);
			}
			atual = atual->prox;
		}
	}
	if (existe == 0){
		printf("Matricula nao encontrada!\n");
	}
}

void IncluirAluno(lista* LISTA, listamat* LISTAMAT){
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
		atualal = LISTA->inicio;
		while (atualal != NULL){
			if (atualal->matricula == matricula && strcmp(atualal->disciplina, "N/D") == 0){
				strcpy(atualal->disciplina, disciplina);
				break;
			}
			atualal = atualal->prox;
		}
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

void Salvar(FILE* arquivo, lista* LISTA){
	Aluno* atual = LISTA->inicio;
	if((arquivo = fopen("alunos.txt","wt")) == NULL){
		printf("\nNao foi possivel abrir o arquivo.\n");
		return;
	}
	while(atual != NULL){
		if (atual->prox == NULL){
			fprintf(arquivo,"%s;%d;%s;%s;%.2f;%.2f;%s", atual->nome, atual->matricula, atual->curso, atual->disciplina, atual->faltas, atual->nota, atual->mencao);
		}
		else{
			fprintf(arquivo,"%s;%d;%s;%s;%.2f;%.2f;%s\n", atual->nome, atual->matricula, atual->curso, atual->disciplina, atual->faltas, atual->nota, atual->mencao);
		}
		atual = atual->prox;
	}
	printf("Alteracoes salvas com sucesso!\n");
	fclose(arquivo);
}

void Gerenciar (lista* LISTA, listamat* LISTAMAT, listacurso* LISTACURSO){
	char sigla[4];
	Materia* atualmat = LISTAMAT->inicio;
	printf("Gerenciar disciplina: ");
	scanf("%s", sigla);
	while(atualmat != NULL){
		if(strcmp(atualmat->disciplina, sigla) == 0){
			break;
		}
		atualmat = atualmat->prox;
	}	
	if(atualmat == NULL){
		printf("Disciplina nao encontrada!\n");
		return;
	}
	Opcoes (LISTA, LISTAMAT, LISTACURSO, atualmat->nalunos, atualmat->disciplina);
}

void Opcoes (lista* LISTA, listamat* LISTAMAT, listacurso* LISTACURSO, int nalunos, char disciplina[]){
	int opcao;
	int x;
	while(1){
		x = 0;
		system("cls || clear");
		printf("Gerenciar disciplina: %s", disciplina);
		printf("\nQuantidade de alunos: %d", nalunos);
		printf("\nOpcoes:\n");
		printf("1 Listar alunos\n");
		printf("2 Remover aluno da disciplina\n");
		printf("3 Atribuir nota a aluno\n");
		printf("4 Atribuir faltas a aluno\n");
		printf("5 Processar turma\n");
		printf("6 Voltar\n");
		scanf("%d", &opcao);
		switch(opcao){
			case 1: listar_alunos(disciplina, LISTA); break;
			case 2:
				x = remover_aluno_da_disciplina(LISTAMAT, LISTA, disciplina); 
				if (x == 1){
					nalunos--;
				}
				break;
			case 3:	atribuir_nota(disciplina, LISTA, LISTAMAT); break;
			case 4:	atribuir_faltas(disciplina, LISTA, LISTAMAT); break;
			case 5: processar_turma(LISTA, LISTACURSO, disciplina); break;
			case 6:	return;
			default: printf("\nComando invalido!\n");
		}
		printf("Pressione enter para voltar");
		fflush(stdin);
		getchar();
	}
}

void listar_alunos(char sigla[], lista* LISTA){
	int len, espaco;
	if (LISTA->inicio == NULL) {
		printf("Lista vazia!\n");
		return;
	}
	Aluno* anterior = NULL;
	Aluno* atual = LISTA->inicio->prox; 
  
  	while (atual != NULL) {
		Aluno* aux = LISTA->inicio;
		Aluno* aux_ant = NULL;
		while (aux->nota >= atual->nota && aux != atual) {
		  aux_ant = aux;
		  aux = aux->prox;
		}
		
		if(anterior != NULL && aux != atual && strcmp(aux->disciplina, sigla) == 0 && strcmp(atual->disciplina, sigla) == 0){
			anterior->prox = atual->prox;
			if (aux_ant == NULL) {
				LISTA->inicio = atual;
			}
			else {
				aux_ant->prox = atual;
			}
			atual->prox = aux;
		}
		anterior = atual;
		atual = atual->prox;
	}
	
	atual = LISTA->inicio;
	printf("Matricula   Nome                                              Faltas  Nota    Mencao\n");
	while(atual != NULL){
		if(strcmp(atual->disciplina, sigla) == 0){
			len = strlen(atual->nome);
			espaco = 50 - len;
			printf("%d    %s", atual->matricula, atual->nome);
			while (espaco){
				printf(" ");
				espaco--;
			}
			if(atual->faltas < 10){
				printf("%.2f    ", atual->faltas);
			}
			else{
				printf("%.2f   ", atual->faltas);
			}
			if(atual->nota == 10){
				printf("%.2f   %s\n", atual->nota, atual->mencao);
			}
			else{
				printf("%.2f    %s\n", atual->nota, atual->mencao);
			}
		}
		atual = atual->prox;
	}
}

void atribuir_nota(char sigla[], lista* LISTA, listamat* LISTAMAT){
	int mat;
	float Nota;
	Aluno* atual = LISTA->inicio; 	
	printf("Atribuir nota a aluno de %s\n", sigla);
	printf("Matricula: ");
	scanf("%d", &mat);
	//Verifica matricula
	while(atual != NULL){
		if(mat == atual->matricula && strcmp(atual->disciplina, sigla) == 0){
			printf("Nota: ");
			scanf("%f", &Nota);
			atual->nota = Nota;
			if (atual->faltas <= 25 && atual->nota > 0){
				if (Nota > 0 && Nota < 3){
					strcpy(atual->mencao, "II");
				}
				if (Nota >= 3 && Nota < 5){
					strcpy(atual->mencao, "MI");
				}
				if (Nota >= 5 && Nota < 7){
					strcpy(atual->mencao, "MM");
				}
				if (Nota >= 7 && Nota < 9){
					strcpy(atual->mencao, "MS");
				}
				if (Nota >= 9 && Nota <= 10){
					strcpy(atual->mencao, "SS");
				}
			}
			else{
				strcpy(atual->mencao, "SR");
			}
			return;
		}
		atual = atual->prox;
	}
	printf("Matricula nao encontrada\n");
return ;
}

void atribuir_faltas(char sigla[], lista* LISTA, listamat* LISTAMAT){
	int mat;
	float faltas;
	Aluno* atual = LISTA->inicio; 	
	printf("Atribuir faltas a aluno de %s\n", sigla);
	printf("Matricula: ");
	scanf("%d", &mat);
	//Verifica matricula
	while(atual != NULL){
		if(mat == atual->matricula && strcmp(atual->disciplina, sigla) == 0){
			printf("Faltas(%%): ");
			scanf("%f", &faltas);
			atual->faltas = faltas;
			if (atual->faltas > 25){
				strcpy(atual->mencao, "SR");
			}
			return;
		}
		atual = atual->prox;
	}
	printf("Matricula nao encontrada\n");
return ;
}

int remover_aluno_da_disciplina(listamat* LISTAMAT, lista* LISTA, char sigla[]){
	int mat;
	Aluno *atual = LISTA->inicio;
	Materia *atual2 = LISTAMAT->inicio;
	printf("Digite a matricula do aluno a ser removido de %s: ", sigla);
	scanf("%d", &mat);
	while (atual != NULL){
		if(mat == atual->matricula && strcmp(atual->disciplina, sigla) == 0){
			//Caso for a primeira matricula
			if(atual->ant==NULL){
				LISTA->inicio = LISTA->inicio->prox; 
				break;
			}
			//Caso for a ultima matricula
			else{
				if(atual->prox==NULL){
					LISTA->fim = LISTA->fim->ant;
					break; 
				}else{
					atual->ant->prox = atual->prox;
					atual->prox->ant = atual->ant;
					break;
				}
			}
		}
		atual = atual->prox;
	}
	if(atual != NULL){
		while(atual2 != NULL){
			if(strcmp(atual2->disciplina, sigla) == 0){
				atual2->nalunos--;
				break;
			}
			atual2 = atual2->prox;
		}
		free(atual);
		return 1;
	}
	else{
		printf("Matricula nao encontrada\n");
	}
	return 0;
}

void processar_turma(lista* LISTA, listacurso* LISTACURSO, char sigla[]){
	int aprovados, reprovados;
	float p_aprovados, p_reprovados, nalunos;
	Aluno* atualal;
	Curso* atualcurso = LISTACURSO->inicio;
	printf("\nCurso  Matriculados  Aprovados    Reprovados\n");
	while (atualcurso != NULL){
		atualcurso->ii = 0;
		atualcurso->mi = 0;
		atualcurso->mm = 0;
		atualcurso->ms = 0;
		atualcurso->ss = 0;
		atualcurso->sr = 0;
		nalunos = 0;
		aprovados = 0;
		reprovados = 0;
		atualal = LISTA->inicio;
		while(atualal != NULL){
			if(strcmp(atualal->curso, atualcurso->curso) == 0){
				if(strcmp(atualal->disciplina, sigla) == 0){
					nalunos++;
					if(atualal->nota >= 5){
						aprovados++;
					}
					else{
						reprovados++;
					}	
					if (strcmp(atualal->curso, atualcurso->curso) == 0){
						if (atualal->faltas <= 25 && atualal->nota > 0){
							if (atualal->nota > 0 && atualal->nota < 3){
								atualcurso->ii++;
							}
							if (atualal->nota >= 3 && atualal->nota < 5){
								atualcurso->mi++;
							}
							if (atualal->nota >= 5 && atualal->nota < 7){
								atualcurso->mm++;
							}
							if (atualal->nota >= 7 && atualal->nota < 9){
								atualcurso->ms++;
							}
							if (atualal->nota >= 9 && atualal->nota <= 10){
								atualcurso->ss++;
							}
						}
						else{
							atualcurso->sr++;
						}
					}
				}
			}
			atualal = atualal->prox;
		}
		//nalunos = atualcurso->nalunos;
		if(nalunos > 0){
			p_aprovados = (aprovados/nalunos)*100;
			p_reprovados = (reprovados/nalunos)*100;
			if(atualcurso->nalunos < 10){
				printf("%s     %.0f             %.2f%%(%d)", atualcurso->curso, nalunos, p_aprovados, aprovados);
			}
			else{
				printf("%s     %.0f            %.2f%%(%d)", atualcurso->curso, nalunos, p_aprovados, aprovados);
			}
			if(p_aprovados < 10){
				printf("     %.2f%%(%d)\n", p_reprovados, reprovados);
			}
			else{
				if(p_aprovados > 10 && p_aprovados < 100){
					printf("    %.2f%%(%d)\n", p_reprovados, reprovados);
				}
				else{
					printf("   %.2f%%(%d)\n", p_reprovados, reprovados);
				}
			}
		}
		atualcurso = atualcurso->prox;
	}
	printf("\n\nMencao\n");
	printf("Curso   SS  MS  MM  MI  II  SR\n");
	atualcurso = LISTACURSO->inicio;
	while(atualcurso != NULL){
		printf("%s      %d   %d   %d   %d   %d   %d\n", atualcurso->curso, atualcurso->ss, atualcurso->ms, atualcurso->mm, atualcurso->mi, atualcurso->ii, atualcurso->sr);
		atualcurso = atualcurso->prox;
	}
}
