#ifndef BICONNECTEDCOMPONENT_H
#define BICONNECTEDCOMPONENT_H

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <map>
#include <stack>
using namespace std;

class BiconnectedComponent{
public:
	BiconnectedComponent(char* graph){
		load_graph(graph);
	}
	~BiconnectedComponent(){}

	struct edge{
		int v1,v2;
		bool visited;
		float weight;
		edge(int v1, int v2):v1(v1),v2(v2){visited = false;};
		edge(int v1, int v2, float weight):v1(v1),v2(v2),weight(weight){visited = false;};
	};

	void find_biconn_com(){
		int start = graphs.begin()->first;
		
		cutCnt[start] = -1;
		dfs_tarjan(start);
		cal_cut_vertex();
		cal_edge_biconn_com();
	}

	void find_biconn_com(int start){
		cutCnt[start] = -1;
		dfs_tarjan(start);
		cal_cut_vertex();
		cal_edge_biconn_com();
	}

	void dfs_tarjan(int v){
		// cout<<"visit order: "<<v<<endl;
		index++;
		preorder[v] = index;
		low[v] = index;
		st.push(v);
		bool brige = true;

		if(graphs[v].empty()) return;
		for(int i=0;i<graphs[v].size();i++){
			edge* e = graphs[v][i];
			int u = e->v1==v?e->v2:e->v1;
			brige = true;
			if(!e->visited){
				// stEdge.push(e);
				e->visited= true;
				
				if(!preorder[u]){

					leaf[v] = false;
					
					dfs_tarjan(u);
					if(low[u]<low[v])
						low[v]=low[u];

					//cut certex
					if(low[u]>=preorder[v]){

						cutCnt[v]++;
						vector<int> tmp;
						tmp.push_back(v);
						tmp.push_back(st.top());
						
						// vector<edge *> tmpEdge;
						// tmpEdge.push_back(stEdge.top());

						while(st.top() != u){
							st.pop();
							tmp.push_back(st.top());
							brige= false;

							// stEdge.pop();
							// tmpEdge.push_back(stEdge.top());
						}
						st.pop();
						// stEdge.pop();

						

						if(brige)
							bridges.push_back(e);
						// else{
						// 	componentEdges.push_back(tmpEdge);
						// }
						components.push_back(tmp);

					}

				}
				else if(preorder[u]<low[v])
					low[v] = preorder[u];
			}
		}

	}

	void dfs(int v,vector<edge*>& compon,map<int,vector<edge *> >& graph){
		marked[v] = true;

		for(int i=0; i<graph[v].size(); i++){
			edge* e = graphs[v][i];
			int u = e->v1==v?e->v2:e->v1;
			if(!e->visited){
				e->visited = true;
				compon.push_back(e);
				if(!marked[u])				
					dfs(u,compon,graph);
			}
		}
	}

	void cal_edge_biconn_com(){
		//delete briges from the origin graph
		map<int,vector<edge *> > graph = graphs;
		for(int i=0;i<bridges.size();i++){
			int x,y;
			x= bridges[i]->v1;
			y= bridges[i]->v2;
			for(int j=0;j<graph[x].size();j++){
				if(graph[x][j] == bridges[i]){
					graph[x].erase(graph[x].begin()+j);
					break;
				}
			}

			for(int j=0;j<graph[y].size();j++){
				if(graph[y][j] == bridges[i]){
					graph[y].erase(graph[y].begin()+j);
					break;
				}
			}
		}

		//set edges as unvisited
		map<int,vector<edge *> > ::iterator it1 = graph.begin();
		for(;it1!=graph.end();it1++){
			for(int i=0;i<it1->second.size();i++){
				it1->second[i]->visited = false;
			}
		}

		// map<int,vector<edge *> > ::iterator it1 = graph.begin();
		// for(;it1!=graph.end();it1++){
		// 	cout<<it1->first<<" :  ";
		// 	for(int i=0;i<it1->second.size();i++){
		// 		cout<<"("<<it1->second[i]->v1<<","<<it1->second[i]->v2<<")\t";
		// 	}
		// 	cout<<endl;
		// }

		map<int,vector<edge *> >::iterator it = graph.begin();
		vector<edge *> tmp;
		for(;it!=graph.end();it++){
			if(!marked[it->first]){
				tmp.clear();
				dfs(it->first,tmp, graph);
				// cout<<tmp.size()<<endl;
				componentEdges.push_back(tmp);
			}
		}

	}

	void cal_cut_vertex(){
		map<int,int>::iterator it = cutCnt.begin();
		if(it->second>=1) cutVertices.push_back(it->first);
		for(++it;it!=cutCnt.end();it++){
			if(it->second){
				cutVertices.push_back(it->first);
			}
		}


		map<int, bool> :: iterator it1=leaf.begin();
		for(;it1!=leaf.end();it1++)
			if(it1->second)
				leaves.push_back(it1->first);
	}

	void show_result(){
		cout<<"Cut Vertices:"<<endl;
		for(int i=0;i<cutVertices.size();i++)
			cout<<cutVertices[i]<<endl;
		cout<<endl;

		cout<<"Bridges:"<<endl;
		for(int i=0;i<bridges.size();i++)
			cout<<bridges[i]->v1<<"\t"<<bridges[i]->v2<<endl;
		cout<<endl;


		cout<<"Vertex Biconnected Components:"<<endl;
		for(int i=0;i<components.size();i++){
			for(int j=0;j<components[i].size();j++){
				cout<<components[i][j]<<"\t";
			}
			cout<<endl;
		}
		cout<<endl;

		cout<<"Edge Biconnected Components:"<<endl;
		for(int i=0;i<componentEdges.size();i++){
			for(int j=0;j<componentEdges[i].size();j++){
				cout<<"("<<componentEdges[i][j]->v1<<","<<componentEdges[i][j]->v2<<")\t";
			}
			cout<<endl;
		}
		cout<<endl;

		// cout<<"Leaves:"<<endl;
		// for(int i=0;i<leaves.size();i++)
		// 	cout<<leaves[i]<<endl;
		// cout<<endl;
	}

private:

	long long num_V;
	long long num_E;

	map<int,vector<edge *> > graphs;
	

	map<int,bool> marked;
	map<int,int> preorder;
	map<int,int> low;
	int index;
	stack<int> st;
	map<int, int> cutCnt;
	map<int, bool> leaf;
	
	vector<vector<int> > components;
	vector<vector<edge *> > componentEdges;
	vector<int> cutVertices;
	vector<edge *> bridges;
	vector<int> leaves;


	void load_graph(char* graph)
	{
		/*load_graph*/
		ifstream fp;
		fp.open(graph,ios::in);
		if(fp.fail())
		{
			cout<<"file open failed"<<endl;
			fp.close();
		}

		else
		{
			fp>>num_V>>num_E;
			
			while(!fp.eof()) //
	        {
	        	int x,y;
	        	// float weight;
	  			fp>>x;
	  			fp>>y;
	  			// fp>>weight;
	  			edge * tmp = new edge(x,y);
	  			graphs[x].push_back(tmp);
	  			graphs[y].push_back(tmp);
	  			preorder[x]=0;
	  			preorder[y]=0;
	  			low[x]=0;
	  			low[y]=0;
	  			cutCnt[x]=0;
	  			cutCnt[y]=0;
	  			leaf[x] = true;
	  			leaf[y] = true;
	  			marked[x]=false;
	  			marked[y]=false;
	  		}
	  	}
  		fp.close();
	}

};
#endif