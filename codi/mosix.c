
#include "generic.h"
#include <linux/mm.h>
#include <linux/proc_fs.h>


struct proc_dir_entry* cerca_proc(struct proc_dir_entry *parent, char *nom);


int num_clusters() 
{
	return 1;
}

int llistar_clusters (struct cluster_t *llista)
{
	//llista = (struct cluster_t *)kmalloc(sizeof(struct cluster_t) ,GFP_KERNEL);	
	llista->nom = "c1" ;
	llista->next = NULL;
	return 0;
}

int num_nodes (struct cluster_t *cluster_id)
{
	
	struct proc_dir_entry *aux = proc_root.subdir;
	int res = 0;
	
	// Busquem el proc_dir_entry de /proc/hpc
	aux = cerca_proc(&proc_root,"hpc");
	
	// Entrem al directori nodes
	aux = cerca_proc(aux,"nodes");
	
	// Comptem els nodes que te
	aux = aux->subdir;
	
	while (aux != NULL ) {
		res++;
		aux = aux->next;
	}

	return res;
}

int llistar_nodes (struct cluster_t *cluster_id, struct node_t *llista)
{
	struct proc_dir_entry *aux = proc_root.subdir;
	int i;
	
	// Busquem el proc_dir_entry de /proc/hpc
	aux = cerca_proc(&proc_root,"hpc");
	
	// Entrem al directori nodes
	aux = cerca_proc(aux,"nodes");
	
	// Assignem els nomos dels nodes que te
	aux = aux->subdir;
	i=0;
	while (aux != NULL ) {		
		llista[i].nom=aux->name;
		if (aux->next==NULL)
			llista[i].next=NULL;
		else llista[i].next=&llista[i+1];
		aux = aux->next;
	}
	
	return 0;
}

int num_procs (struct cluster_t *cluster_id, struct node_t *node_id) 
{
	return 3;
}

int llistar_procs (struct cluster_t *cluster_id, struct node_t *node_id, struct proces_t *llista)
{
	llista[0].nom = "p0";
	llista[0].next = &llista[1];
	llista[1].nom = "p1";
	llista[1].next = &llista[2];
	llista[2].nom = "p2";
	llista[2].next = NULL;

	return 0;
}


//int migrar (struct proces_t  *proces, struct node_t *node_desti);

/* El node pot rebre procs. nous ? */
//int node_bloquejat_rebre (struct node_t *node);

/* El node pot enviar procs. nous ? */
//int node_bloquejat_enviar (struct node_t *node);

/* El proces no es pot migrar */
//int proces_bloquejat (struct proces_t *proc);



// Funcions auxiliars

// Funcio que donat un directori pare i un nom, retorna el proc_dir_entry corresponent
// al subdirectori de nom = <nom>
struct proc_dir_entry* cerca_proc(struct proc_dir_entry *parent, char *nom) {	
	
	int trobat = 0;
	struct proc_dir_entry *aux = parent->subdir; 
	
	while (aux != NULL && trobat==0) {
		if (!strcmp(aux->name,nom)) trobat = 1;
		else aux = aux->next;
	}
	
	return aux;
}
