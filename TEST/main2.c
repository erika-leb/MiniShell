//J'avais des soucis de malloc quand j'essayais la solution de chatGPT donc autant
//partir de la et recreer un nouveau tableau qui recupere les infos de tableau et de mots_tableau
//il reste plus qu'a decourir comment supprimer les quotes et les espaces pui sles refoutres dans tableau[4]

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// #define TAILLE_INITIALE 20  // Taille initiale des chaînes
// #define TAILLE_NOUVELLE 50 // Nouvelle taille de la chaîne à l'indice 4

// int main() {
//     // Allouer un tableau de 5 chaînes de caractères (initialement nulles)
//     char **tableau = (char **)malloc(5 * sizeof(char *));
//     if (tableau == NULL) {
//         printf("Erreur d'allocation mémoire.\n");
//         return 1;
//     }

//     // Initialiser les chaînes (allocation de mémoire pour chaque chaîne)
//     for (int i = 0; i < 5; i++) {
//         tableau[i] = (char *)malloc(TAILLE_INITIALE * sizeof(char));
//         if (tableau[i] == NULL) {
//             printf("Erreur d'allocation mémoire pour tableau[%d].\n", i);
//             return 1;
//         }
//     }

//     // Remplir les chaînes initiales avec des valeurs
//     strcpy(tableau[0], "Chaine 1");
//     strcpy(tableau[1], "Chaine 2");
//     strcpy(tableau[2], "Chaine 3");
//     strcpy(tableau[3], "Chaine 4");
//     strcpy(tableau[4], "Chaine 5");

//     // Afficher les chaînes avant la modification
//     printf("Avant modification:\n");
//     for (int i = 0; i < 5; i++) {
//         printf("tableau[%d] = %s\n", i, tableau[i]);
//     }

//     // Agrandir manuellement la chaîne à l'indice 4
//     // 1. Allouer une nouvelle zone de mémoire pour la chaîne à l'indice 4
//     char *nouvelle_chaine = (char *)malloc(TAILLE_NOUVELLE * sizeof(char));
//     if (nouvelle_chaine == NULL) {
//         printf("Erreur d'allocation mémoire pour nouvelle chaîne.\n");
//         return 1;
//     }

//     // 2. Copier l'ancienne chaîne dans la nouvelle zone mémoire
//     strcpy(nouvelle_chaine, tableau[4]);

//     // 3. Libérer l'ancienne mémoire allouée pour tableau[4]
//     free(tableau[4]);

//     // 4. Allouer de la mémoire pour la nouvelle chaîne dans tableau[4]
//     tableau[4] = nouvelle_chaine;

//     // 5. Ajouter de nouveaux caractères à la chaîne agrandie
//     strcat(tableau[4], " - Modification réussie!");

//     // Afficher les chaînes après modification
//     printf("\nAprès modification:\n");
//     for (int i = 0; i < 5; i++) {
//         printf("tableau[%d] = %s\n", i, tableau[i]);
//     }
//     // // Libérer la mémoire allouée
//     // for (int i = 0; i < 5; i++) {
//     //     free(tableau[i]);
//     // }
//     // free(tableau);

//     // Séparer tableau[4] en mots et les afficher dans de nouveaux tableaux
//     // Utiliser strtok pour découper la chaîne en mots
//     char *mot;
//     char **mots_tableau = malloc(20 * sizeof(char *));  // Allouer un tableau pour les mots
//     int i = 0;
    
//     mot = strtok(tableau[4], " ");  // Utiliser un espace comme délimiteur
//     while (mot != NULL) {
//         mots_tableau[i] = malloc(strlen(mot) + 1); // Allouer de la mémoire pour chaque mot
//         strcpy(mots_tableau[i], mot); // Copier le mot dans le tableau
//         printf("mots_tableau[%d] = %s\n", i, mots_tableau[i]); // Afficher le mot
//         mot = strtok(NULL, " "); // Passer au mot suivant
//         i++;
//     }

//     // Libérer la mémoire allouée pour les mots
//     for (int j = 0; j < i; j++) {
//         free(mots_tableau[j]);
//     }
//     free(mots_tableau);

//     // Libérer la mémoire allouée pour tableau
//     for (int i = 0; i < 5; i++) {
//         free(tableau[i]);
//     }
//     free(tableau);

//     return 0;
// }






//Code pour copier une chaine sans ses espaces ni guillemets,
//puis apres ft_concat rajouter les espaces et les guillemets

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void traiter_chaine(char *str1, char *str2) {
    int len = strlen(str1);
    
    // Allouer un tableau pour la chaîne sans espaces et guillemets
    char *modifiee = (char *)malloc((len + 1) * sizeof(char));
    if (modifiee == NULL) {
        printf("Erreur d'allocation mémoire.\n");
        return;
    }

    // Créer un tableau pour stocker les indices des espaces et guillemets
    int *indices = (int *)malloc(len * sizeof(int));
    if (indices == NULL) {
        printf("Erreur d'allocation mémoire pour indices.\n");
        free(modifiee);
        return;
    }
    
    // Variables pour suivre les positions dans les chaînes
    int modifiee_idx = 0;  // Position dans la chaîne sans espaces et guillemets
    int indices_idx = 0;   // Position dans le tableau des indices
    int i = 0;             // Position dans str1

    // Parcours de str1 pour retirer les espaces et guillemets et garder les indices
    while (i < len) {
        if (str1[i] == ' ' || str1[i] == '"' || str1[i] == '\'') {
            // Ajouter l'indice de l'espace ou du guillemet
            indices[indices_idx++] = i;
        } else {
            // Ajouter le caractère modifié dans la chaîne modifiée
            modifiee[modifiee_idx++] = str1[i];
        }
        i++;
    }
    modifiee[modifiee_idx] = '\0'; // Terminer la chaîne modifiée

    // Affichage de la chaîne modifiée pour vérification
    printf("Chaîne modifiée sans espaces et guillemets : %s\n", modifiee);

    // Coller la version modifiée dans str2 (initialement str2 contient quelque chose)
    strcpy(str2, "Texte initial dans str2. "); // str2 contient déjà quelque chose

    // Réintroduire les espaces et guillemets dans str2 à leurs positions initiales
    int modifiee_pos = strlen(str2); // Position de départ dans str2 pour ajouter les mots modifiés
    strcat(str2, modifiee); // Coller la chaîne modifiée

    int j = 0; // Position dans les indices
    while (j < indices_idx) {
        // Décalage des caractères de str2 à partir de l'indice
        int k = strlen(str2);
        while (k > indices[j] + modifiee_pos) {
            str2[k] = str2[k - 1];
            k--;
        }
        
        // Ajouter l'espace ou guillemet à la position
        if (str1[indices[j]] == ' ') {
            str2[indices[j] + modifiee_pos] = ' ';
        } else if (str1[indices[j]] == '"') {
            str2[indices[j] + modifiee_pos] = '"';
        } else if (str1[indices[j]] == '\'') {
            str2[indices[j] + modifiee_pos] = '\'';
        }
        j++;
    }

    // Libérer la mémoire allouée
    free(modifiee);
    free(indices);
}

int main() {
    char str1[] = "C'est un exemple de \"chaine\" avec des espaces.";
    char str2[200]; // Assurez-vous que str2 est assez grand pour contenir le résultat

    // Appeler la fonction pour traiter la chaîne
    traiter_chaine(str1, str2);

    // Afficher le résultat final
    printf("str2 après traitement : %s\n", str2);

    return 0;
}


