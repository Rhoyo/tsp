#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <math.h>
#include <bits/stdc++.h>
#include <time.h>

using namespace std;

struct v{
	int id;
	int x;
	int y;
	int key=INT_MAX;
	bool in=false;

};

int weight(struct v v0, struct v v1){
	float x= pow(v0.x-v1.x,2);
	float y= pow(v0.y-v1.y,2);
	float val= sqrt(x+y);

	return round(val);
}

//establish MST to run in depth first traversal
vector<vector<int>>  mst(vector<v> graph, vector<vector<int>>& weights){
	vector<vector<int>> ms;
	
	//setting up weight matrix
	graph[0].key=0;

	int parent[graph.size()];
	parent[0]=-1;

	for(int x=0;x<graph.size();x++){
		weights.push_back(vector<int>());
		
		for(int y=0;y<graph.size();y++){
			weights[x].push_back(weight(graph[x],graph[y]));
		}
	}


	//prim's algorithm
	for(int c=0;c<graph.size()-1;c++){ // makes sure #edges is vertices-1
		int nxt, min=INT_MAX;
		
		for(int i=0;i<graph.size();i++){
			if(graph[i].in==false && graph[i].key<min){
				min= graph[i].key;
			 	nxt = i;
			}
		}
		graph[nxt].in=true; //declare as seen
		
		for(int i=0;i<graph.size();i++){//finding lowest value and inputting as key
			if(weights[nxt][i]<graph[i].key && graph[i].in==false){	
				graph[i].key=weights[nxt][i];
				parent[i]=graph[nxt].id;
			}
		}
	}
	//establish mst as adjaceny matrix for dfs
	for(int i=0;i<graph.size();i++){
		ms.push_back(vector<int>());
		for(int x=0;x<graph.size();x++)
			ms[i].push_back(0);
	}

	//find connecting vertices in MST, put into adj matrix
	for(int i=1;i<graph.size();i++)
		ms[parent[i]][i]=1;

	return ms;
}


//geeksforgeeks.org/implementation-of-dfs-using-adjacency-matrix
void dfs(int start, vector<vector<int>> ms, vector<bool>& visited, vector<int>& d){
	//push back the current visited vertex	
	d.push_back(start);
	visited[start] = true;
	
	for(int i=0;i<ms.size();i++){
		if(ms[start][i]==1 && (!visited[i]))
			dfs(i,ms,visited,d);
	}
}

//add edge from last to the first to create cycle from generated vertex list, d
int calculate(vector<vector<int>> weights, vector<int> d){
	int length=0;
	
	for(int i=0; i<d.size()-1;i++)
		length+=weights[d[i]][d[i+1]];	
	length+=weights[d[d.size()-1]][d[0]];
	
	return length;	
}


int main(){	
	ifstream input;
	input.open("./HW7W21/tsp_example_5.txt");
	if(!input)
		return 1;
		
	ofstream out;
	out.open("tsp5.txt.tour");
	if(!out)
		return 1;
		
	int size;
	vector<v> graph;
	vector<vector<int>> ms;
	input >> size;
	struct v fill;
	int in;
	vector<vector<int>> weights;

	for(int i=0; i<size; i++){
		input >> in;
		fill.id=in;

		input >> in;
		fill.x=in;

		input >> in;
		fill.y=in;

		graph.push_back(fill);
	}
	clock_t t2,t1;
	
	t2=clock();


	//store mst in ms adjacency matrix
	ms=mst(graph,weights);

	//establish bool vector for dfs function
	vector<bool> visited;
	for(int i=0;i<ms.size();i++)
		visited.push_back(false);

	//create empty vertex list
	vector<int> d;
	
	dfs(0,ms,visited,d);

	t1=clock();
	float elapse=(float)t1 - (float)t2;
	cout<<"Time: "<<elapse<<" "<<elapse/CLOCKS_PER_SEC<<"s"<<endl;

	//tsp length is outputted to terminal and also written to the file
	int length=calculate(weights,d);
	cout<<"TSP: "<<length<<endl;

	out<<length<<"\n";
	for(int i=0;i<d.size();i++)
		out<<d[i]<<"\n";

	
	input.close();
	out.close();	
}
