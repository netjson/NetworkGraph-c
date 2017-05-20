#ifndef SRC_PARSER_H_
#define SRC_PARSER_H_

#include <json-c/json.h>
#include <stdio.h>
#include <string.h>

/* topology structures definition*/
struct topology{
	int id_lenght;
	char *protocol;
	char *self_id;
	struct node *first;

};

struct node{
	char *id;
	struct neighbor *neighbor_list;
	struct node *next;
};


struct neighbor{
	struct node *id;
	float weight;
	struct neighbor *next;
};

struct topology * parse_netjson(char* buffer);
const char * compose_netjson(struct topology * c_topo);

int add_node(struct topology * topo, const char *id);
struct topology * _init_topo(int type);
int add_neigh(struct topology *topo, const char *source, const char *id, const double weight);
void destroy_topo(struct topology *topo);
struct node* find_node(struct topology *topo, const char *id);

void destroy_topo(struct topology *topo);

#endif /* SRC_PARSER_H_ */
