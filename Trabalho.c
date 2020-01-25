#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct aluno{
	char nome[50];
	int matricula;
	char curso[3];
	char disciplina[3];
	int faltas;
	int nota;
	char mencao[2];
} Aluno;

Aluno* CriarLista (){
	Aluno* alunos = (Aluno*)malloc(sizeof(Aluno));
	if (alunos == NULL){
		printf("Sem memoria disponivel!\n");
		exit(0);
	}
	return alunos;
}

void ListarDisciplina(FILE* arquivo, Aluno* alunos){
	rewind(arquivo);
	printf("Disciplina   Aluno\n");
	while(fscanf(arquivo, "%[^;];%d;%[^;];%[^;];%d;%d;%s", alunos->nome, &alunos->matricula, alunos->curso, alunos->disciplina, &alunos->faltas, &alunos->nota, alunos->mencao) != EOF){
		printf("%s       \n", alunos->disciplina);
	}
}

void AdicionarDisciplina (FILE *arquivo, Aluno *alunos){
	char sigla[3];
	printf("Adicionar disciplina\nDigite a sigla: ");
	scanf("%s", sigla);
	rewind(arquivo);
	while(fscanf(arquivo, "%[^;];%d;%[^;];%[^;];%d;%d;%s", alunos->nome, &alunos->matricula, alunos->curso, alunos->disciplina, &alunos->faltas, &alunos->nota, alunos->mencao) != EOF){
		if (strcmp(alunos->disciplina, sigla) == 0){
			printf("Disciplina ja existe!\n");
			return;
		}
	}
}

int main(){
	int opcao;
	FILE* arqNome;
	Aluno* alunos = CriarLista();
	if ((arqNome = fopen("alunos.txt","at+")) == NULL){
		printf("Nao foi possivel abrir o arquivo.\n");
		return 0;
	}
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
			ListarDisciplina(arqNome, alunos);
		}
		if (opcao == 2){
			AdicionarDisciplina(arqNome, alunos);
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