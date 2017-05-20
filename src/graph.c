#include "graph.h"
/**
* Add a node to the topology data structure
* @param struct topology*  pointer to the topology data structure
* @param const char* string containing the id of the new node
* @return 1 on success, 0 otherwise
*/
int add_node(struct topology * topo, const char *id)
{
	struct node *temp = topo->first;
	topo->first=(struct node*)malloc(sizeof(struct node));
	topo->first->next=temp;
	topo->first->id=strdup(id);
	topo->first->neighbor_list=0;
	return 1;
}

/**
* Find a node in the topology data structure
* @param struct topology*  pointer to the topology data structure
* @param const char* string containing the id of the searched node
* @return pointer to the node on success, 0 otherwise
*/
struct node* find_node(struct topology *topo,const char *id)
{
	struct node *punt;
	for(punt=topo->first; punt!=0; punt=punt->next){
		if(strcmp(punt->id, id)==0){
			return punt;
		}
	}
	return 0;
}

/**
* Add a neighbor to the node
* @param struct topology*  pointer to the topology data structure
* @param const char* string containing the id of the source node
* @param const char* string containing the id of the target node
* @param const double  cost of the edge
* @return 1 on success, 0 otherwise
*/
int add_neigh(struct topology *topo, const char *source, const char *id, const double weight)
{
	struct neighbor *temp;
	struct node* n;
	if((n=find_node(topo, source))==0)
		return 0;
	temp=n->neighbor_list;
	n->neighbor_list=(struct neighbor*)malloc(sizeof(struct neighbor));
	if((n->neighbor_list->id=find_node(topo, id))==0)
		return 0;
	n->neighbor_list->weight=weight;
	n->neighbor_list->next=temp;
	return 1;

}

/**
* Initialize the topology data structure
* @param int number of chars of the id (0 ipv6, 1 ipv4)
* @return pointer to the topology
*/
struct topology * _init_topo(int type)
{
	struct topology *topo = (struct topology*)malloc(sizeof(struct topology));
	if(type==0){
		topo->id_lenght=39;
	}else if(type ==1){
		topo->id_lenght=15;
	}
	topo->first=0;
	return topo;
}


/**
* Destroy topology and dealloc
* @param struct topology * pointer to the structure
**/
void destroy_topo(struct topology *topo)
{
	struct node *n_temp, *punt=topo->first;
	while(punt){
		struct neighbor *n=punt->neighbor_list;
		while(n){
			struct neighbor *temp=n->next;
			free(n);
			n=temp;
		}
		free(punt->id);
		n_temp=punt->next;
		free(punt);
		punt=n_temp;
	}
	free(topo->protocol);
	free(topo->self_id);
	free(topo);
}

/*
*Compose a NetJSON object and return its string representation1
*/
const char * compose_netjson(struct topology * c_topo){
	struct node *punt;
	json_object *jtopo = json_object_new_object();
	json_object *jnodes = json_object_new_array();
	//compose the list of nodes
	for(punt=c_topo->first; punt!=0; punt=punt->next){
		json_object *node = json_object_new_object();
		json_object *node_id = json_object_new_string(punt->id);
		json_object_object_add(node, "id", node_id);
		json_object_array_add(jnodes, node);
	}
	json_object_object_add(jtopo, "nodes", jnodes);
	//compose the list of edges
	json_object *jedges = json_object_new_array();
	for(punt=c_topo->first; punt!=0; punt=punt->next){
		struct neighbor* neigh;
		for(neigh=punt->neighbor_list; neigh!=0; neigh=neigh->next){
			const char* source = punt->id;
			const char* target = neigh->id->id;
			double cost = neigh->weight;
			json_object *edge = json_object_new_object();
			json_object *edge_src = json_object_new_string(source);
			json_object *edge_tgt = json_object_new_string(target);
			json_object *edge_cst = json_object_new_double(cost);
			json_object_object_add(edge, "source", edge_src);
			json_object_object_add(edge, "target", edge_tgt);
			json_object_object_add(edge, "cost", edge_cst);
			json_object_array_add(jedges, edge);
    }
	}
	json_object_object_add(jtopo, "links", jedges);
	return json_object_to_json_string(jtopo);
}

/**
* Parse NetJson format
* @param char* buffer containing the serialized json
* @return representation of the graph as "struct topology*" type
*/
struct topology * parse_netjson(char* buffer)
{
	struct topology *c_topo= _init_topo(0);
	json_object *topo = json_tokener_parse(buffer);
	if(!topo) return 0;
	json_object_object_foreach(topo, key, val) {
		if(strcmp(key, "protocol")==0)
		c_topo->protocol=strdup(json_object_get_string(val));
		if(strcmp(key,"router_id")==0){
			c_topo->self_id=strdup(json_object_get_string(val));
		}
		if(strcmp(key, "nodes")==0){
			int i, arraylen;
			json_object *array;
			json_object_object_get_ex(topo, key, &array);
			arraylen = json_object_array_length(array);
			for(i=0; i<arraylen; i++){
				json_object *elem =json_object_array_get_idx(array,i);
				json_object_object_foreach(elem, key, val){
					if(strcmp(key, "id")==0){
						add_node(c_topo, json_object_get_string(val));
					}

				}
			}
		}
		if(strcmp(key, "links")==0){
			int i;
			json_object *jarray;
			json_object_object_get_ex(topo, key, &jarray);
			int arraylen = json_object_array_length(jarray);
			for(i=0; i<arraylen; i++){
				const char *source=0, *target=0;
				double cost=0;
				json_object *elem =json_object_array_get_idx(jarray,i);
				json_object_object_foreach(elem, key, val) {
					if(strcmp(key, "source")==0){
						source=json_object_get_string(val);
					}
					if(strcmp(key, "target")==0){
						target=json_object_get_string(val);
					}
					if(strcmp(key, "cost")==0){
						cost=json_object_get_double(val);
					}
					if(source && target && cost){
						/*printf("   %s %s %f\n", source, target, cost);*/
						if(!add_neigh(c_topo, source, target, cost)){
							printf("error\n");
							return 0;
						}

						source = target =0;
						cost =0;
					}
				}

			}
		}
	}
	json_object_put(topo);
	return c_topo;
}
