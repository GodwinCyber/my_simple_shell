#include "aileswhale.h"
/**
 * builtin_cmd - verifies whether the command is a builtin
 * @vars: variables to be checked
 *
 * Return: pointer to the function if it's a builtin, otherwise NULL
 */


void (*builtin_cmd(vars_t *vars))(vars_t *vars)
{
	unsigned int i;
	builtins_t check[] = {
		{"exit", exit_program},
		{"env", print_the_enviroment},
		{"setenv", create_new_env},
		{"unsetenv", remove_new_env},
		{"cd", new_the_cd},
		{"help", new_help_builtin_cmd},


		{NULL, NULL}};

	for (i = 0; check[i].f != NULL; i++)
	{

		if (strcmp(vars->array_tok[0], check[i].names) == 0)
			break;
	}
	if (check[i].f != NULL)
		check[i].f(vars);
	return (check[i].f);
}

/**
 * exit_program - exitthe program
 * @vars: variables
 * Return: void
 */
void exit_program(vars_t *vars)
{

	int status;

	if (strcmp(vars->array_tok[0], "exit") ==
			0 &&
			vars->array_tok[1] != NULL)

	{

		status = atoi(vars->array_tok[1]);

		if (status == -1)
		{
			vars->status = 2;
			print_error_message(vars, ": Illegal number: ");
			print_msg(vars->array_tok[1]);
			print_msg("\n");
			return;
		}
		vars->status = status;
	}
	free(vars->cmd);
	free(vars->array_tok);


	free_shell_env(vars->env);
	free(vars->buff);
	exit(vars->status);
}

/**
 * print_the_enviroment - prints the current environment
 * @vars: a struct of variables
 * Return: void.
 */
void print_the_enviroment(vars_t *vars)
{
	unsigned int i;

	for (i = 0; vars->env[i]; i++)
	{
		_print(vars->env[i]);
		_print("\n");
	}
	vars->status = 0;
}

/**
 * create_new_env - create a new environment variable,
 * or edit an existing variable
 * @vars: a pointer to a struct of variables
 *
 * Return: void
 */
void create_new_env(vars_t *vars)
{
	char **keys;
	char *var;

	if (vars->array_tok[1] == NULL || vars->array_tok[2] == NULL)
	{

		print_error_message(vars, ": Incorrect number of arguments\n");
		vars->status = 2;
		return;
	}
	keys = find_keys(vars->env, vars->array_tok[1]);
	if (keys == NULL)
	{
		add_keys(vars);
	}
	else
	{
		var = add_values(vars->array_tok[1], vars->array_tok[2]);
		if (var == NULL)
		{
			print_error_message(vars, NULL);
			free(vars->buff);
			free(vars->array_tok);
			free(vars->cmd);
			free_shell_env(vars->env);
			exit(127);
		}
		free(*keys);
		*keys = var;
	}
	vars->status = 0;
}

/**
 * remove_new_env - remove an environment variable
 * @vars: a pointer to a struct of variables
 *
 * Return: void
 */
void remove_new_env(vars_t *vars)
{
	char **keys, **new_env;
	unsigned int i, j;

	if (vars->array_tok[1] == NULL)
	{
		print_error_message(vars, ": Incorrect number of arguments\n");
		vars->status = 2;
		return;
	}
	keys = find_keys(vars->env, vars->array_tok[1]);
	if (keys == NULL)
	{
		print_error_message(vars, ": No variable to unset");
		return;
	}
	for (i = 0; vars->env[i] != NULL; i++)
		;
	new_env = malloc(sizeof(char *) * i);
	if (new_env == NULL)
	{
		print_error_message(vars, NULL);
		vars->status = 127;
		exit_program(vars);
	}
	for (i = 0; vars->env[i] != *keys; i++)
		new_env[i] = vars->env[i];
	for (j = i + 1; vars->env[j] != NULL; j++, i++)
		new_env[i] = vars->env[j];
	new_env[i] = NULL;
	free(*keys);
	free(vars->env);
	vars->env = new_env;
	vars->status = 0;
}
