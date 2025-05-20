#pragma once

// Function signature for commands
typedef int (*BuiltinFunc)(int argc, char **argv);

// A builtin entry into the table
typedef struct Builtin {
  const char *name;
  BuiltinFunc func;
} builtin_t;

// Forward declare vars
extern builtin_t builtins[];
extern int num_builtins;

// Builtins
int exit_command(int argc, char **argv);
int echo_command(int argc, char **argv);
int type_command(int argc, char **argv);
int pwd_command(int argc, char **argv);
int cd_command(int argc, char **argv);
