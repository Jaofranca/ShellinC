#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define TRUE 1
//pelas pesquisas o máximo de caracteres 
//que o comandline suporta no windows xp é 2048, nos windows seguintes
//o numero é maior, como eu não sei onde vai ser usado deixei assim mesmo
#define INPUT_SIZE 2048 

#define COMMAND_SIZE 128
#define PATH "/bin/"
#define COMMAND_DELIMITER " \n"


void user_input (char cmd[], char *params[]) {
  printf("~$ ");
  
  char line[INPUT_SIZE];//input do usuário
  char *next; //auxiliar para o split da line
  char *commands[COMMAND_SIZE]; // armazena o comando e os parâmetros
  int i = 0;

  fgets(line, INPUT_SIZE, stdin); //Função para dividir a string em substrings
  next = strtok(line, COMMAND_DELIMITER);
// pega o comando e os parâmetros e armazenam na array commands
  while (next != NULL) {
    commands[i++] = next;
    next = strtok(NULL, COMMAND_DELIMITER); // NULL é necessário para chamar o próximo após o espaço
  }
  strcpy(cmd, commands[0]); // primeiro elemento é o comando
  // os demais elementos são parâmetros
  for (int j = 0; j < i; j++) {
    params[j] = commands[j];
  }
  params[i] = NULL; // NULL indica o fim da lista de parâmetros

}

int main(void) {
  int pid, status;
  char relative_cmd[COMMAND_SIZE], cmd[COMMAND_SIZE + sizeof(PATH)], *params[COMMAND_SIZE];

  char *envp[] = { (char*) "PATH=/bin", 0 }; // variável de ambiente

  while (TRUE) {
    user_input(relative_cmd, params);

    pid = fork();
    if (pid < 0) { // valores < 0 indicam que a criação do processo não funcionou
      printf("Unable to fork.\n");
      continue;
    }

    if (pid != 0) { // waitpid espera o processo filho ser criada
      waitpid(-1, &status, 0);
      
    } else { // zero indica que o processo filho foi criado
      strcpy(cmd, PATH);
      strcat(cmd, relative_cmd);
      execve (cmd, params, envp);
    }

    if (strcmp(cmd, "exit") == 0) break; // permite a saída do shell
  }

  return 0;
}