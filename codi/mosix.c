#include "generic.h"
#include <linux/mm.h>


//struct cluster_t clus = {.nom = "123",
//			 .next = NULL};

struct node_t n3 = {	.nom = "n3",
			.next = NULL};
struct node_t n2 = {	.nom = "n2",
			.next = &n3 };
struct node_t n1 = {	.nom = "n1",
			.next = &n2};

struct proces_t p3 = {	.nom = "proc3",
			.next = NULL};

struct proces_t p2 = {	.nom = "proc2",
			.next = &p3};
struct proces_t p1 = {	.nom = "proc1",
			.next = &p2};

int llistar_clusters (struct cluster_t *llista)
{
	//llista = (struct cluster_t *)kmalloc(sizeof(struct cluster_t) ,GFP_KERNEL);	
	llista->nom = "c1" ;
	llista->next = NULL;
	return 0;
}

int llistar_nodes (struct cluster_t *cluster_id, struct node_t *llista)
{
	//llista[0] = &n1;
	//llista[1] = &n2;
	//llista[2] = &n3;
	
}

int liistar_procs (struct node_t *node_id, struct proces_t *llista)
{
	//llista[0] = &p1;
	//llista[1] = &p2;
	//llista[2] = &p3;
}


//int migrar (struct proces_t  *proces, struct node_t *node_desti);

/* El node pot rebre procs. nous ? */
//int node_bloquejat_rebre (struct node_t *node);

/* El node pot enviar procs. nous ? */
//int node_bloquejat_enviar (struct node_t *node);

/* El proces no es pot migrar */
//int proces_bloquejat (struct proces_t *proc);

