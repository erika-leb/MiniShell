Guide d'utilisation du garbage collector

1) Initialisation avec gc_init : Avant toute utilisation, le garbage collector doit être prêt.
ex:
t_gc gc;
gc_init(&gc);

2) Allocation avec gc_malloc : Chaque allocation doit utiliser gc_alloc a la place de alloc ou calloc. La vérfication de la bonne allocation est intégrée a la fonction gc_malloc.

3) Réallocation avec gc_realloc : Chaque réallocation doit utiliser gc_realloc a la place de realloc. La vérfication de la bonne allocation est intégrée a la fonction gc_realloc.

4) Nettoyage avec gc_cleanup : Libère toute la mémoire à la fin du programme, sans avoir à suivre manuellement chaque allocation.