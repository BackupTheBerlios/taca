
#include "backend.h"
#include <linux/mm.h>
#include <linux/proc_fs.h>
#include <linux/list.h>


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
	
	struct proc_dir_entry *aux;
	int res = 0;

	
	struct nameidata nd;



	printk (" **mosix.c: num_nodes\n");
	
	/*
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
	*/

	/*
	res = path_lookup("/proc/hpc/nodes/3",LOOKUP_DIRECTORY,&nd);
	if (res == 0) {
		struct list_head *head = &nd.dentry->d_subdirs;
		struct dentry *pos;
		printk("**%s: Hi ha alguna cosa\n",__FILE__);
		for (pos = list_entry((head)->next, typeof(*pos), d_subdirs),
				prefetch(pos->d_subdirs.next);
				&pos->d_subdirs != (head);
				pos = list_entry(pos->d_subdirs.next, typeof(*pos), d_subdirs),
				prefetch(pos->d_subdirs.next)) {
			printk("**%s: dentry_aux->d_name->name=%s\n",__FILE__,pos->d_name.name);
			res++;
		}
	}
	else {
		printk("**%s: No hem trobat res\n",__FILE__);
	}
	*/

	res = sys_open("/proc/hpc/nodes", O_DIRECTORY, O_RDONLY);
	if (res > 0) {
		int tam = 4096;
		char buf[tam];
		printk("  ** mosix.c: sys_open OK\n");
		res = sys_read(res, buf, tam);
		printk("  ** mosix.c: sys_read=%d\n",res);
		if (res>0)
			printk("  ** mosix.c: sys_read->\n%s\n",buf);
	}
	else {
		printk("  ** mosix.c: sys_open KO\n");
	}

	printk("  ** mosix.c:  El numero de nodes es: %d\n",res);
	return res;
}

int llistar_nodes (struct cluster_t *cluster_id, struct node_t *llista)
{

	struct proc_dir_entry *aux;
	int i;
	
	printk (" ** mosix.c: llistar_nodes\n");
	
	// Busquem el proc_dir_entry de /proc/hpc
	aux = cerca_proc(&proc_root,"hpc");
	
	// Entrem al directori nodes
	aux = cerca_proc(aux,"nodes");
	
	// Assignem els noms dels nodes que te
	aux = aux->subdir;
	i=0;
	while (aux != NULL ) {		
		printk("  ** mosix.c: llistar_nodes -> generando nodo:%s\n", aux->name);
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
		printk("  ** mosix.c: cerca_proc Estoy en:%s\n",aux->name);
		if (!strcmp(aux->name,nom)) trobat = 1;
		else aux = aux->next;
	}
	
	if (aux==NULL) printk("  ** mosix.c : cerca_proc -> Ha pasado algo mu malo!\n");
	else printk("  ** mosix.c : cerca_proc -> directorio buscado:%s, encontrado:%s\n",nom,aux->name);
	return aux;
}

int path_lookup(const char *path, unsigned flags, struct nameidata *nd)
{
	int error = 0;
	if (path_init(path, flags, nd))
		error = path_walk(path, nd);
	return error;
}


