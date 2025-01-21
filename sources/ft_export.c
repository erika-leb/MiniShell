#include "../minishell.h"
//static
void ft_printexport(const t_env *head)
{
    const t_env *current = head;
    //Tout mettre dans un buffer, comme pour l'erreur de ft_exit
    while (current) {
        if (current->key)
            printf("export %s=\"%s\"\n", current->name, current->key);
        else
            printf("export %s\n", current->name);
        current = current->next;
    }
}
//static
// static void ft_freelexport(t_env *head)
// {
// 	t_env *current = head;
// 	while (current)
// 	{
// 		t_env *temp = current;
// 		current = current->next;
// 		free(temp->name);
// 		free(temp->key);
// 		free(temp);
// 	}
// }

//NB : quand on aura fini export il faudra reecrire ft_getenvv car a partir de mtn
//on travaille plus avec l'environnement bash mais avec notre tableau envv ??
//static
void ft_adder(t_env **head, char *str, t_gc *gc)
{
    char  **adder;

	adder = gc_calloc(2 + 1, sizeof(char *), gc);//gc_cleaner
    adder[0] = ft_cut(str, '=', 0, gc);//dans l'exportation je concatene/expand deja bien.
    adder[1] = ft_cut(str, '=', 1, gc);
    *head = ft_addenvnode(*head, adder[0], adder[1], gc);
	gc_remove(gc, adder[0]);//useless
	gc_remove(gc, adder[1]);//useless
	gc_remove(gc, adder);//useless aussi I guess
}

//J'ai aussi remarque que si le user ecrit bonjour="cava \"oui\" et toi"  alors ca donne bonjour="cava \"oui\" et toi"
//                                                                            au lieu de bonjour="cava \oui\ et toi"
//Je pense que bash concatene deja les argv donc pour voir comment se comporte reellement ft_export dans minishell il faut
//le tester grandeur nature.
//A l'affichage (quand je fais ft_export(env, NULL)) il faudra juste ajouter des \ devant les $ et les " (ft_write)

//a chaque fois que je fais cd je peux directement fair appel a ft_export
//pour modifier OLPWD et PWD
char **ft_export(char **env, char **argv, t_gc *gc)
{
	// Gerer le cas ou env est NULL : env -i ./minishell (voir bloc note)
	t_env *head;
	char  **adder;
	int   i;

	head = NULL;
	i = -1;
	while (env[++i])
		ft_adder(&head, env[i], gc);
	if (!argv[1])
		return (ft_bbsort(head), ft_printexport(head), gc_remove(gc, head), NULL);
	if (argv[1][0] == '-')
	{
		write(2, "minishell: export: No option(s) allowed\n", 41);
		gc_remove(gc, head);
		exit(2);
	}
	i = 0;
	while (argv[++i])
	{
		if (!ft_exparser(argv[i]))
			ft_adder(&head, argv[i], gc);
	}
	adder = ft_ltoa(head, gc);
	gc_remove(gc, head);
	//Retirer la variable _ dans la structure env de minishell
	//(ca n'apparait pas dans bash --posix) ??
	//Sur ?? Car ca apparait dans bash --posix

	gc_remove(gc, env);
	env = adder;
	return (adder);
}
