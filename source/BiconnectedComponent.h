#ifndef BICONNECTEDCOMPONENT_H
#define BICONNECTEDCOMPONENT_H

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <map>
#include <stack>
#include <string>
using namespace std;

template <class V>
class BiconnectedComponent{
public:
	BiconnectedComponent(){
		depth =0;
	}
	~BiconnectedComponent(){}

	struct edge{
		V v1,v2;
		bool visited;
		float weight;
		edge(V v1, V v2):v1(v1),v2(v2){visited = false;};
		edge(V v1, V v2, float weight):v1(v1),v2(v2),weight(weight){visited = false;};
	};

	void find_biconn_com(){
		V start = graphs.begin()->first;
		
		cutCnt[start] = -1;
		dfs_tarjan(start);
		cal_cut_vertex();
		cal_edge_biconn_com();
	}

	void find_biconn_com(V start){
		cutCnt[start] = -1;
		dfs_tarjan(start);
		cal_cut_vertex();
		cal_edge_biconn_com();
	}

	void dfs_tarjan(V v){
		depth++;
		preorder[v] = depth;
		low[v] = depth;
		st.push(v);

		if(graphs[v].empty()) return;
		for(int i=0;i<graphs[v].size();i++){
			edge* e = graphs[v][i];
			V u = e->v1==v?e->v2:e->v1;
			
			if(!e->visited){ // avoid visiting parent vertex
				e->visited= true;
				
				if(!preorder[u]){ // vertex u is unvisited
					
					dfs_tarjan(u);
					if(low[u]<low[v])
						low[v]=low[u];

					//cut certex
					if(low[u]==preorder[v]){

						cutCnt[v]++;
						vector<V> tmp;
						tmp.push_back(v);
						tmp.push_back(st.top());
						

						//vertices in one component
						while(st.top() != u){
							st.pop();
							tmp.push_back(st.top());
						}
						st.pop();
		
						components.push_back(tmp);

					}
					if(low[u]>preorder[v]){
						cutCnt[v]++;
						vector<V> tmp;
						tmp.push_back(v);
						tmp.push_back(st.top());
						st.pop();
						components.push_back(tmp);
						bridges.push_back(e);
					}

				}
				else if(preorder[u]<low[v]) // back edge
					low[v] = preorder[u];
			}
		}

	}

	void cal_cut_vertex(){
		for(auto it : cutCnt){
			if(it.second){
				cutVertices.push_back(it.first);
			}
		}

	}


//edge components 
	void dfs(V v,vector<edge*>& compon,vector<V>& vertices){

		marked[v] = true;

		for(int i=0; i<graphs[v].size(); i++){
			edge* e = graphs[v][i];
			V u = e->v1==v?e->v2:e->v1;
			if(!e->visited){
				
				//if the edge e is in this component
				if(vertices.end()!=find(vertices.begin(),vertices.end(),u)){
					e->visited = true;
					
					compon.push_back(e);

					if(!marked[u])				
						dfs(u,compon,vertices);
				}
			}
		}
	}

	void cal_edge_biconn_com(){
		//set edges as unvisited
		for(const auto& it : graphs){
			for(int i=0;i<it.second.size();i++){
				it.second[i]->visited = false;
			}
		}

		vector<edge *> tmp;
		for(int i=0;i<components.size();i++){
			//skip the brige component
			// if(components[i].size()==2) continue;
			//set cut vertices as unvisited
			for(int j=0;j<cutVertices.size();j++)
				marked[cutVertices[i]]=false;

			tmp.clear();
			dfs(components[i][0],tmp,components[i]);
			componentEdges.push_back(tmp);
		}

	}




	void show_result(){
		cout<<"Cut Vertices:"<<endl;
		for(int i=0;i<cutVertices.size();i++)
			cout<<cutVertices[i]<<endl;
		cout<<endl;

		cout<<"Vertex Biconnected Components:"<<endl;
		for(int i=0;i<components.size();i++){
			cout<<"{  ";
			for(int j=0;j<components[i].size();j++){
				cout<<components[i][j]<<"  ";
			}
			cout<<"}"<<endl;
		}
		cout<<endl;

		cout<<"Edge Biconnected Components:"<<endl;
		for(int i=0;i<componentEdges.size();i++){
			cout<<"{  ";
			for(int j=0;j<componentEdges[i].size();j++){
				cout<<"("<<componentEdges[i][j]->v1<<","<<componentEdges[i][j]->v2<<")  ";
			}
			cout<<"}"<<endl;
		}
		cout<<endl;


		cout<<"Bridges:"<<endl;
		for(int i=0;i<bridges.size();i++)
			cout<<"("<<bridges[i]->v1<<","<<bridges[i]->v2<<")"<<endl;
		cout<<endl;

	}


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
	        	V x,y;
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
	  			marked[x]=false;
	  			marked[y]=false;
	  		}
	  	}
  		fp.close();
	}

private:

	long long num_V;
	long long num_E;

	map<V,vector<edge *> > graphs;
	

	map<V,bool> marked;
	map<V,int> preorder;
	map<V,int> low;
	int depth;
	stack<V> st;
	map<V, int> cutCnt;
	
	vector<vector<V> > components;
	vector<vector<edge *> > componentEdges;
	vector<V> cutVertices;
	vector<edge *> bridges;



};
#endif