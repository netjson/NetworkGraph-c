#include <stdio.h>
#include "../src/graph.h"
int main(){
  struct topology *topo = _init_topo(0);
  add_node(topo, "1");
  add_node(topo, "2");
  add_node(topo, "3");
  add_node(topo, "4");
  add_neigh(topo, "1", "2", 1.0);
  add_neigh(topo, "2", "3", 1.0);
  add_neigh(topo, "3", "4", 1.0);
  add_neigh(topo, "4", "1", 1.0);
  add_neigh(topo, "2", "4", 1.0);
  printf("%s\n", compose_netjson(topo));
}
