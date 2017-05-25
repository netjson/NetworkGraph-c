#ifndef SRC_PARSER_H_
#define SRC_PARSER_H_
#ifdef JSONC
	#include <json-c/json.h>
#else
	#include "lib/cJSON.h"
#endif
#include <stdio.h>
#include <string.h>

/* topology structures definition*/
struct ng_topology{
	int id_lenght;
	char *protocol;
	char *self_id;
	struct ng_node *first;

};

struct ng_node{
	char *id;
	struct ng_neighbor *neighbor_list;
	struct ng_node *next;
};


struct ng_neighbor{
	struct ng_node *id;
	float weight;
	struct ng_neighbor *next;
};

struct ng_topology * parse_netjson(char* buffer);
const char * compose_netjson(struct ng_topology * c_topo);

int add_node(struct ng_topology * topo, const char *id);
struct ng_topology * _init_topo(int type);
int add_neigh(struct ng_topology *topo, const char *source, const char *id, const double weight);
void destroy_topo(struct ng_topology *topo);
struct ng_node* find_node(struct ng_topology *topo, const char *id);

void destroy_topo(struct ng_topology *topo);

#endif /* SRC_PARSER_H_ */
