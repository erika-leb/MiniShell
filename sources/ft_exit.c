/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 18:19:30 by ele-borg          #+#    #+#             */
/*   Updated: 2025/01/20 14:50:35 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"

//Seul le code d'erreur du dernier enfant compte
//Si y'a plus d'1 arguments alors exit ne s'execute pas !!! Ce sera a retranscrire
//quand j'integrerai exit dans le code d'Erika
//(A FAIRE)

static void ft_exitfail(t_built *built, t_gc *gc)
{
	char *code;

	code = built->cmd[1];
	ft_buff_error("minishell: exit: ", built->elements, gc);
	ft_buff_error(code, built->elements, gc);
	ft_buff_error(": numeric argument required\n", built->elements, gc);
	ft_write_error(built->elements, gc);
	(gc_cleanup(gc), free_std(), exit(2));//Verifier pertinence


}
static void ft_initexit(const char *str, int *i, int *neg, long *res)
{
	*i = 0;
	*neg = 1;
	*res = 0;
	//Pour gerer les exit "   42" :
	while ((str[*i] >= '\t' && str[*i] <= '\r') || str[*i] == ' ')
		(*i)++;
}

static void ft_fillexit(t_built *built, int *i, int neg, long *res, t_gc *gc)
{
    int	curr_digit;

    while (built->cmd[1][*i] >= '0' && built->cmd[1][*i] <= '9')
    {
        curr_digit = built->cmd[1][*i] - '0';

        // Vérifier les débordements avant d'ajouter le chiffre.
        //Comme pour l'overflow de ft_calloc dans libft, on divise pour verifier que res n'est pas trop grand
        if ((neg == 1 && *res > (LONG_MAX - curr_digit) / 10) ||
            (neg == -1 && *res > (LONG_MIN + curr_digit) / -10))
            ft_exitfail(built, gc);
        *res = (*res) * 10 + curr_digit;
        (*i)++;
    }
	//Pour gerer les cas exit "42   "
    while ((built->cmd[1][*i] >= '\t'
			&& built->cmd[1][*i] <= '\r') || built->cmd[1][*i] == ' ')
        (*i)++;
}

int ft_exit(t_built *built, t_gc *gc)
{
    int i;
    int neg;
    long res;

	if (!built->cmd[1])
		(gc_cleanup(gc), free_std(), exit(0));
    //si plus d'1 argument alors bash: exit: too many arguments
    //et on modifie le code d'erreur pour qu'il soit egal a 2.
    //C'est pour ca que mon ft_exit prendra en argument non pas const char *str
    //mais plutot un truc du style une liste chainee a analyser.
	if (built->cmd[2])
	{
		ft_buff_error("minishell: exit: too many arguments", built->elements, gc);
		ft_write_error(built->elements, gc);
		(gc_cleanup(gc), free_std(), exit(2));
	}

	ft_initexit(built->cmd[1], &i, &neg, &res);
    if (built->cmd[1][i] == '-')
    {
        neg = -1;
        i++;
    }
    else if (built->cmd[1][i] == '+')
        i++;
	//Pour eviter que le user mette des trucs du style "  + " ou +a :
    if (!(built->cmd[1][i] >= '0' && built->cmd[1][i] <= '9'))
        ft_exitfail(built, gc);
    ft_fillexit(built, &i, neg, &res, gc);
	//avant de return on verifie que soit on est en bout de chaine, soit il y a
	//que des espaces avant le bout de chaine sinon c'est que dans la chaine y'a des trucs
	//invalides ex : 43253a.
    if (built->cmd[1][i])
        ft_exitfail(built, gc);
    if (neg == -1)
        res = -res;
	(gc_cleanup(gc), free_std(), exit((unsigned int)res % 256));
    // exit((unsigned int)res % 256);
}

