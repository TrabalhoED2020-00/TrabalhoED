#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 10

typedef struct aluno{
	char nome[50];
	int matricula;
	char curso[3];
	char disciplina[4];
	int faltas;
	int nota;
	char mencao[2];
} Aluno;

Aluno* CriarLista (){
	Aluno* alunos = (Aluno*)malloc(sizeof(Aluno)*MAX);
	if (alunos == NULL){
		printf("\nSem memoria disponivel!\n");
		exit(0);
	}
	return alunos;
}

int Dados (FILE* arquivo, Aluno* alunos){
	int nlinhas = 0;
	while(fscanf(arquivo, "%[^;];%d;%[^;];%[^;];%d;%d;%s", alunos[nlinhas].nome, &alunos[nlinhas].matricula, alunos[nlinhas].curso, alunos[nlinhas].disciplina, &alunos[nlinhas].faltas, &alunos[nlinhas].nota, alunos[nlinhas].mencao) != EOF){
		nlinhas++;
	}
	return nlinhas;
}

void ListarDisciplina(FILE* arquivo, Aluno* alunos, int nlinhas){
	int cont, j, k, len;
	rewind(arquivo);
	printf("\nDisciplina   Aluno\n");
	
	for (j = 0; j < nlinhas; j++){
		cont = 0;
		for (k = 0; k < nlinhas; k++){
			if (strcmp(alunos[j].disciplina, alunos[k].disciplina) == 0){
				cont++;
			}
		}
		len = strlen(alunos[j].disciplina);
		if (len == 1){
			printf("%s            %d\n", alunos[j].disciplina, cont);
		}
		if (len == 2){
			printf("%s           %d\n", alunos[j].disciplina, cont);
		}
		if (len == 3){
			printf("%s          %d\n", alunos[j].disciplina, cont);
		}
	}
}

void AdicionarDisciplina (FILE *arquivo, Aluno *alunos, int nlinhas){
	char sigla[4];
	int j;
	printf("\nAdicionar disciplina\nDigite a sigla: ");
	scanf("%s", sigla);
	rewind(arquivo);
	for (j = 0; j < nlinhas; j++){
		if (strcmp(alunos[j].disciplina, sigla) == 0){
			printf("Disciplina ja existe!\n");
			return;
		}
	}
}

void RemoverDisciplina (FILE *arquivo, Aluno *alunos, int nlinhas){
	char sigla[4];
	int j, cont = 0;
	printf("\nRemover disciplina\nDigite a sigla: ");
	scanf("%s", sigla);
	rewind(arquivo);
	for (j = 0; j < nlinhas; j++){
		if (strcmp(alunos[j].disciplina, sigla) == 0){
			strcpy(alunos[j].disciplina, "0");
			cont++;
		}
	}
	if (cont == 0){
		printf("Disciplina nao cadastrada!\n");
	}
}

void SemDisciplina (FILE *arquivo, Aluno *alunos, int nlinhas){
	int i;
	printf("Nome       Matricula      Curos\n");
	for (i = 0; i < nlinhas; i++){
		if (strcmp(alunos[i].disciplina,"0") == 0){
			printf("%s      %d       %s\n", alunos[i].nome, alunos[i].matricula, alunos[i].curso);
		}
	}
}

void AdicionarAluno (FILE *arquivo, Aluno *alunos, int nlinhas){
	char nome[50], curso[4];
	int matricula;
	printf("Adicionar aluno\n");
	printf("Nome:");
	scanf("%s", nome);
	printf("Matricula: ");
	scanf("%d", &matricula);
	printf("Curso: ");
	scanf("%s", curso);
	strcpy(alunos[nlinhas+1].nome, nome);	
	alunos[nlinhas+1].matricula = matricula;
	strcpy(alunos[nlinhas+1].curso, curso);
}

int main(){
	int opcao, nlinhas;
	FILE* arqNome;
	Aluno* alunos = CriarLista();
	if ((arqNome = fopen("alunos.txt","at+")) == NULL){
		printf("\nNao foi possivel abrir o arquivo.\n");
		return 0;
	}
	nlinhas = Dados(arqNome, alunos);
	while(1){
		printf("Ola professor,\nX alunos nao estao matriculados.\nO que deseja fazer:\n");
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
		if (opcao == 1){
			ListarDisciplina(arqNome, alunos, nlinhas);
		}
		if (opcao == 2){
			AdicionarDisciplina(arqNome, alunos, nlinhas);
		}
		if (opcao == 3){
			RemoverDisciplina(arqNome, alunos, nlinhas);
		}
		if (opcao == 4){
			SemDisciplina(arqNome, alunos, nlinhas);
		}
		if (opcao == 5){
			AdicionarAluno(arqNome, alunos, nlinhas);
		}
		if (opcao == 10){
			fclose(arqNome);
			exit(0);
		}
		printf("Pressione enter para voltar");
		fflush(stdin);
		getchar();
		system("cls || clear");
	}
	return 0;
}