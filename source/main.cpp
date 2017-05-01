#include <iostream>

#include "BiconnectedComponent.h"
#include <map>

using namespace std;

int main(int argc, char ** argv){
	char* graph;
	if(argc==2){
		graph = (char*)argv[1];
	}
	else {
		graph = (char*)"../dataset/example2.txt";
	}

	BiconnectedComponent<char> BC;
	BC.load_graph(graph);
	BC.find_biconn_com();
	BC.show_result();

	return 0;
}
