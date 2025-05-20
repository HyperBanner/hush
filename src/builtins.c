#include "hush_builtins.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Builtins table and count
builtin_t builtins[] = {
    {"exit", exit_command}, {"echo", echo_command}, {"type", type_command},
    {"pwd", pwd_command},   {"cd", cd_command},
};
int num_builtins = sizeof(builtins) / sizeof(builtins[0]);

// builtin implementations

int exit_command(int argc, char **argv) { return -1; }

int echo_command(int argc, char **argv) {
  for (int i = 1; i < argc; ++i)
    printf("%s ", argv[i]);
  printf("\n");
  return 0;
}

int type_command(int argc, char **argv) {
  if (argc < 2) {
    printf("Usage: type command_name ...\n");
    return 0;
  }

  for (int i = 1; i < argc; ++i) {
    int found = 0;

    // Check if it's a builtin
    for (int j = 0; j < num_builtins; ++j) {
      if (strcmp(argv[i], builtins[j].name) == 0) {
        printf("%s is a shell builtin\n", argv[i]);
        found = 1;
        break;
      }
    }

    if (!found) {
      // Check PATH for external command
      char *path_env = getenv("PATH");
      if (!path_env)
        path_env = "/bin:/usr/bin"; // fallback
      char *path_copy = strdup(path_env);
      char *dir = strtok(path_copy, ":");

      while (dir) {
        char full_path[512];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, argv[i]);
        if (access(full_path, X_OK) == 0) {
          printf("%s is %s\n", argv[i], full_path);
          found = 1;
          break;
        }
        dir = strtok(NULL, ":");
      }

      free(path_copy);
    }

    if (!found) {
      printf("%s: not found\n", argv[i]);
    }
  }

  return 0;
}

int pwd_command(int argc, char **argv) {
  if (argc > 1) {
    printf("pwd: too many arguments");
    return 1;
  }

  char *cwd = getenv("PWD");
  printf("%s\n", cwd);
  return 0;
}

int cd_command(int argc, char **argv) {
  const char *path;

  if (argc < 2) {
    // No argument -> change to HOME
    path = getenv("HOME");
    if (!path) {
      fprintf(stderr, "cd: HOME not set\n");
      return 1;
    }
  } else if (strcmp(argv[1], "-") == 0) {
    // cd - -> change to OLDPWD
    path = getenv("OLDPWD");
    if (!path) {
      fprintf(stderr, "cd: OLDPWD not set\n");
      return 1;
    }
    printf("%s\n", path); // print the path we're changing to
  } else if (argv[1][0] == '~') {
    // Handle ~ or ~/path
    const char *home = getenv("HOME");
    if (!home) {
      fprintf(stderr, "cd: HOME not set\n");
      return 1;
    }

    // Build full path
    static char full_path[512];
    snprintf(full_path, sizeof(full_path), "%s%s", home, argv[1] + 1);
    path = full_path;
  } else {
    // Handle normal absolute or relative path
    path = argv[1];
  }

  // Save old PWD
  char oldpwd[512];
  if (getcwd(oldpwd, sizeof(oldpwd)) == NULL) {
    perror("getcwd");
    return 1;
  }

  // Try to change directory
  if (chdir(path) != 0) {
    fprintf(stderr, "cd: %s: %s\n", path, strerror(errno));
    return 1;
  }

  // Update OLDPWD and PWD
  setenv("OLDPWD", oldpwd, 1);
  char cwd[512];
  if (getcwd(cwd, sizeof(cwd)) != NULL) {
    setenv("PWD", cwd, 1);
  }

  return 0;
}
