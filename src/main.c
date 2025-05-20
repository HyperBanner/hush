#include "hush_builtins.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// For execve
extern char **environ;

// Helper func to execute commands
int run_command(int argc, char **argv) {
  // if no args, do nothing
  if (argc == 0)
    return 0;

  // Check if it's a builtin
  for (int i = 0; i < num_builtins; ++i) {
    if (strcmp(argv[0], builtins[i].name) == 0) {
      return builtins[i].func(argc, argv);
    }
  }

  // Check PATH for external command
  char *path_env = getenv("PATH");
  if (!path_env)
    path_env = "/bin:/usr/bin"; // fallback

  char *path_copy = strdup(path_env); // duplicate to tokenize safely
  char *dir = strtok(path_copy, ":");

  while (dir) {
    char full_path[512];
    snprintf(full_path, sizeof(full_path), "%s/%s", dir, argv[0]);

    if (access(full_path, X_OK) == 0) {
      pid_t pid = fork();
      if (pid == 0) {
        // In child
        argv[argc] = NULL;
        execve(full_path, argv, environ);
        perror("execve failed"); // if execve fails
        exit(1);
      } else if (pid > 0) {
        // In parent
        int status;
        waitpid(pid, &status, 0);
        free(path_copy);
        return 0;
      } else {
        perror("fork failed");
        free(path_copy);
        return 1;
      }
    }

    dir = strtok(NULL, ":");
  }

  // No executable found
  printf("%s: command not found\n", argv[0]);
  return 0;
}

// Main loop
int main() {
  // declare some vars used in the loop
  char input[256];
  char *argv[16];
  int argc;
  int exit_shell = 0;

  while (!exit_shell) {
    // Flush after every printf
    setbuf(stdout, NULL);
    printf("$ ");

    // Wait for user input and dont't go further until assigned
    if (!fgets(input, sizeof(input), stdin))
      break;

    // Parse into argv
    argc = 0;
    char *token = strtok(input, " \t\n");
    while (token && argc < 15) {
      argv[argc++] = token;
      token = strtok(NULL, " \t\n");
    }
    argv[argc] = NULL;

    // run command with arguments
    if (argc > 0) {
      int result = run_command(argc, argv);
      if (result == -1)
        exit_shell = 1;
    }
  }

  return 0;
}
