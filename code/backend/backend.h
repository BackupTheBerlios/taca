
#ifndef _backend_h_
#define _backend_h_

#define MAXSTR 256

struct cluster_t {
    char nom[MAXSTR];
    struct cluster_t *next;
};

struct node_t {
    char nom[MAXSTR];
    struct node_t *next;
    struct cluster_t *parent;
};

struct proces_t {
    char nom[MAXSTR];
    struct proces_t *next;
    struct node_t *parent;
};

int num_clusters ();
int llistar_clusters (struct cluster_t *llista, int maxnum);
int obtenir_cluster (const char *nom, struct cluster_t *cluster);
int cluster_valid (struct cluster_t *cluster);

int num_nodes (struct cluster_t *cluster_id);
int llistar_nodes (struct cluster_t *cluster_id, struct node_t *llista, int maxnum);
int obtenir_node (struct cluster_t *cluster_id, const char *nom, struct node_t *node);
int node_valid (struct node_t *node);

int num_procs (struct node_t *node_id);
int llistar_procs (struct node_t *node_id, struct proces_t *lista, int maxnum);
int obtenir_proc (struct node_t *node_id, const char *nom, struct proces_t *proces);
int proc_valid (struct proces_t *proces);



int migrar (struct proces_t  *proces, struct node_t *node_desti);

/* El node pot rebre procs. nous ? */
int node_bloquejat_rebre (struct node_t *node);

/* El node pot enviar procs. nous ? */
int node_bloquejat_enviar (struct node_t *node);

/* El proces no es pot migrar */
int proces_bloquejat (struct proces_t *proc);

#endif

