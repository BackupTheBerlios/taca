
#include "backend.h"
#include <linux/slab.h>
#include <asm/string.h>


int num_clusters() 
{
    return 1;
}

int llistar_clusters (struct cluster_t *llista, int maxnum)
{
    int num = num_clusters();

    if (num > maxnum)
	return 0;
    strcpy(llista->nom,"c1");
    llista->next = NULL;

    return num;
}

int obtenir_cluster (const char *nom, struct cluster_t *cluster)
{
    int num = num_clusters();
    struct cluster_t *ptr, *llista = kmalloc(sizeof(struct cluster_t)*num, GFP_KERNEL);
    llistar_clusters(llista, num);
    for (ptr = llista; ptr != NULL; ptr = ptr->next) {
	if (!strcmp(ptr->nom, nom)) {
	    strcpy(cluster->nom, nom);
	    break;
	}
    }
    kfree(llista);
    return ptr != NULL;
}

int cluster_valid (struct cluster_t *cluster)
{
    return 1;
}

int num_nodes (struct cluster_t *cluster_id)
{
    return 3;
}

int llistar_nodes (struct cluster_t *cluster_id, struct node_t *llista, int maxnum)
{
    int num = num_nodes(cluster_id);

    if (num > maxnum)
	return 0;
    strcpy(llista[0].nom, "n0");
    llista[0].next = &llista[1];
    llista[0].parent = cluster_id;
    strcpy(llista[1].nom, "n1");
    llista[1].next = &llista[2];
    llista[1].parent = cluster_id;
    strcpy(llista[2].nom, "n2");
    llista[2].next = NULL;
    llista[2].parent = cluster_id;

    return num;
}

int obtenir_node (struct cluster_t *cluster_id, const char *nom, struct node_t *node)
{
    int num = num_nodes(cluster_id);
    struct node_t *ptr, *llista = kmalloc(sizeof(struct node_t)*num, GFP_KERNEL);
    llistar_nodes(cluster_id, llista, num);
    for (ptr = llista; ptr != NULL; ptr = ptr->next) {
	if (!strcmp(ptr->nom, nom)) {
	    strcpy(node->nom, nom);
	    node->parent = cluster_id;
	    break;
	}
    }
    kfree(llista);
    return ptr != NULL;
}

int node_valid (struct node_t *node)
{
    return 1;
}

/* cada quan volem que canvii l'estat dels "p2" (aparicio/desaparicio) */
#define MODUL 15
static int emulador ()
{
    struct timeval tv;
    do_gettimeofday(&tv);
    return tv.tv_sec%MODUL;
}

int num_procs (struct node_t *node_id) 
{
    return 2 + emulador();
}

int llistar_procs (struct node_t *node_id, struct proces_t *llista, int maxnum)
{
    int num = num_procs(node_id);

    if (num > maxnum)
	return 0;
    strcpy(llista[0].nom, "p0");
    llista[0].next = &llista[1];
    llista[0].parent = node_id;
    strcpy(llista[1].nom, "p1");
    llista[1].parent = node_id;
    if (emulador()) {
	llista[1].next = &llista[2];
	strcpy(llista[2].nom, "p2");
	llista[2].next = NULL;
	llista[2].parent = node_id;
    }
    else
	llista[1].next = NULL;

    return num;
}

int obtenir_proc (struct node_t *node_id, const char *nom, struct proces_t *proces)
{
    int num = num_procs(node_id);
    struct proces_t *ptr, *llista = kmalloc(sizeof(struct proces_t)*num, GFP_KERNEL);
    llistar_procs(node_id, llista, num);
    for (ptr = llista; ptr != NULL; ptr = ptr->next) {
	if (!strcmp(ptr->nom, nom)) {
	    strcpy(proces->nom, nom);
	    proces->parent = node_id;
	    if (!strcmp(nom, "p2") && !emulador())
		ptr = NULL;
	    break;
	}
    }
    kfree(llista);
    return ptr != NULL;
}

int proc_valid (struct proces_t *proces)
{
    if (!strcmp(proces->nom, "p2") && !emulador())
	return 0;
    else
	return 1;
}

//int migrar (struct proces_t  *proces, struct node_t *node_desti);

/* El node pot rebre procs. nous ? */
//int node_bloquejat_rebre (struct node_t *node);

/* El node pot enviar procs. nous ? */
//int node_bloquejat_enviar (struct node_t *node);

/* El proces no es pot migrar */
//int proces_bloquejat (struct proces_t *proc);

