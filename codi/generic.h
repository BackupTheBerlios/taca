struct cluster_t {

	char *nom;
	struct cluster_t *next;
};

struct node_t {

	char *nom;
	struct node_t *next;
};

struct proces_t {
	
	char *nom;
	struct proces_t *next;
};


int num_clusters();
int llistar_clusters (struct cluster_t *llista);

int num_nodes(struct cluster_t *cluster_id);
int llistar_nodes (struct cluster_t *cluster_id, struct node_t *llista);

int num_procs(struct cluster_t *cluster_id, struct node_t *node_id);
int liistar_procs (struct cluster_t *cluster_id, struct node_t *node_id, struct proces_t *lista);



int migrar (struct proces_t  *proces, struct node_t *node_desti);

/* El node pot rebre procs. nous ? */
int node_bloquejat_rebre (struct node_t *node);

/* El node pot enviar procs. nous ? */
int node_bloquejat_enviar (struct node_t *node);

/* El proces no es pot migrar */
int proces_bloquejat (struct proces_t *proc);

