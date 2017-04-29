#include <iostream>

#include "BiconnectedComponent.h"

using namespace std;

int main(int argc, char ** argv){
	char* graph;
	if(argc==2){
		graph = (char*)argv[1];
	}
	else {
		graph = (char*)"../dataset/test.txt";
	}

	BiconnectedComponent * BC = new BiconnectedComponent(graph); 
	BC->find_biconn_com();
	BC->show_result();

	return 0;
}
