#include <stdio.h>
#include <unistd.h>

int main(void)
{
    printf("Le programme attend indéfiniment. Appuyez sur Ctrl+C pour quitter.\n");

    // Boucle infinie
    while (1)
    {
        sleep(1); // Fait une pause de 1 seconde dans chaque itération
    }

    return 0;
}
