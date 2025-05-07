#include<iostream>
#include<vector>
#include<omp.h>
#include<chrono>
#include<queue>
#include<stack>
using namespace std;
void bfs(vector<vector<int>> &adj,int start){
    queue<int> q;
    vector<bool> visited(adj.size(),false);
    q.push(start);
    visited[start]=true;
    while(!q.empty()){
        int node=q.front();
        q.pop();
        for(int neighbor : adj[node]){
            if(!visited[neighbor]){
                q.push(neighbor);
                visited[neighbor]=true;
            }
        }
    }


}
void bfsp(vector<vector<int>> &adj,int start){
    queue<int> q;
    vector<bool> visited(adj.size(),false);
    q.push(start);
    visited[start]=true;
    #pragma omp parallel 
    {
    while(!q.empty()){
        int node;
        #pragma omp critical
        {
        if(!q.empty()){
            
            node=q.front();
            q.pop();
        }
        }
        for(int neighbor : adj[node]){
            #pragma omp critical
            if(!visited[neighbor]){
                q.push(neighbor);
                visited[neighbor]=true;
            }
        }
    }
}

}

void dfs(vector<vector<int>> &adj,int start){
    stack<int> q;
    vector<bool> visited(adj.size(),false);
    q.push(start);
    visited[start]=true;
    while(!q.empty()){
        int node=q.top();
        q.pop();
        for(int neighbor : adj[node]){
            if(!visited[neighbor]){
                q.push(neighbor);
                visited[neighbor]=true;
            }
        }
    }


}
void dfsp(vector<vector<int>> &adj,int start){
    stack<int> q;
    vector<bool> visited(adj.size(),false);
    q.push(start);
    visited[start]=true;
    #pragma omp parallel 
    while(!q.empty()){
        int node;
        #pragma omp critical
        if(!q.empty()){
            node=q.top();
            q.pop();
        }
        for(int neighbor : adj[node]){
            #pragma omp critical
            if(!visited[neighbor]){
                q.push(neighbor);
                visited[neighbor]=true;
            }
        }
    }


}
void generate(vector<vector<int>> &adj,int u,int v){
    adj[u].push_back(v);
    adj[v].push_back(u);
}
int main(){
    int size=1000;
    cout<<"Enter size : ";
    cin>> size;
    vector<vector<int>> adj(size);
    for(int i=0;i<size;i++){
        generate(adj,rand()%size,rand()%size);
    }
    
    
    double seqBfsStart, seqBfsEnd, parBfsStart, parBfsEnd, seqBfsTime, parBfsTime;
    double seqDfsStart, seqDfsEnd, parDfsStart, parDfsEnd, seqDfsTime, parDfsTime;
    
    seqDfsStart = omp_get_wtime();
    dfs(adj, 0);
    seqDfsEnd = omp_get_wtime();
    seqDfsTime = (seqDfsEnd - seqDfsStart);
    cout<<"Sequential DFS Time : "<<seqDfsTime<<endl<<endl;
    
    parDfsStart = omp_get_wtime();
    dfsp(adj,  0);
    parDfsEnd = omp_get_wtime();
    parDfsTime = (parDfsEnd - parDfsStart);
    cout<<"Parallel DFS Time : "<<parDfsTime<<endl<<endl;

    cout<<"DFS Speedup : "<<(seqDfsTime/parDfsTime)<<endl<<endl;
    
    seqBfsStart = omp_get_wtime();
    bfs(adj,  0);
    seqBfsEnd = omp_get_wtime();
    seqBfsTime = (seqBfsEnd - seqBfsStart);
    cout<<"Sequential BFS Time : "<<seqBfsTime<<endl<<endl;
    
    parBfsStart = omp_get_wtime();
    bfsp(adj,  0);
    parBfsEnd = omp_get_wtime();
    parBfsTime = (parBfsEnd - parBfsStart);
    cout<<"Prallel BFS Time : "<<parBfsTime<<endl<<endl;

    cout<<"BFS Speedup : "<<(seqBfsTime/parBfsTime)<<endl<<endl;

}