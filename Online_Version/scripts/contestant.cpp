#include "bits/stdc++.h"
using namespace std;
#define all(s) s.begin(),s.end()
template < class P , class Q > ostream& operator<<(ostream& stream, pair < P , Q > v){ stream << "(" << v.fi << ',' << v.se << ")"; return stream;}
template < class T > ostream& operator<<(ostream& stream, const vector<T> v){ stream << "[ "; for (int i=0; i<(int)v.size(); i++) stream << v[i] << " "; stream << "]"; return stream;}
template < class T > T smin(T &a,T b) {if (a > b) a = b;return a;}
template < class T > T smax(T &a,T b) {if (a < b) a = b;return a;}
const int N = 512;
const int MAXPATHS = 2000;
const long double eps = 1e-6;
const long double oo = 1e9;//infinity
int OrderNode[N * N];

//Function that get bigger as `a` increases and `b` decreases
long double f(long long a,long long b) {
	return 1.0 * a / b;
}
vector < int > Order[N];
int freq[N];
vector < int > G[N];
int E[N][N];
int cnt[N];//cnt[i] -- the number of orders in vertex `i`
int Father[N];

/*	Cost[i] -- the cost of a path ending in vertex `i`
	Subpath[i] -- the actual path corresponding to 
	cost Cost[i]
*/
long double Cost[N];
vector < int > SubPath[N];

int n,m;
int Visited[N];
void DFS(int start,int node,int prev,int subRewards,int pathLength) {
	Visited[node] = 1;
	if (prev != -1) {
		Father[node] = prev;
		/*	The current path starting at `start` and ending at
			node seems to have a higher cost that the previously
			considered path, therefore we'll update the path with
			highest cost
		*/
		if (Cost[node] < f(subRewards,pathLength)) {
			Cost[node] = f(subRewards,pathLength);
			SubPath[node].clear();
			int cur = node;
			while (cur != start) {
				for (int cs = 0;cs < E[cur][Father[cur]];++cs)
					SubPath[node].push_back(cur);
				cur = Father[cur];
			}
			reverse(all(SubPath[node]));
		}
	} 
	for (auto it : G[node])
		if (it != prev && !Visited[it]) {
			DFS(start,it,node,subRewards + cnt[it],pathLength + E[node][it]);
		}
}

void TryPaths(int Node) {
	for (int i = 1;i <= n;++i)
		Cost[i] = -oo,SubPath[i].clear();
	for (int cs = 0;cs < MAXPATHS;++cs) {
		/*	To get a pseudo random sampling of paths we randomly
			shuffle the edges. We are sampling MAXPATH times and
			DFS takes O(N^2). In practice DFS should take O(N)
			since the number of times the Cost[i] is changed is
			small and is dependent on the input itself which deals
			with special graphs. The total complexity in practice
			should be of order O(N * MAXPATHS).
		*/
		for (int i = 1;i <= n;++i)
			random_shuffle(all(G[i])),Visited[i] = 0,Father[i] = -1;
		DFS(Node,Node,-1,0,0);
	}
}

int last = 0;
int Vertex = 1;
vector < int > Shop;

void Update(int i) {
	/*	Collect new orders and deliver the currently held items
		if we are in vertex 1.
	*/
	int Nnew;
	cin>>Nnew;
	while (Nnew --) {
		int ID,v;
		cin>>ID>>v;
		Order[v].push_back(i);
		OrderNode[ID] = v;
		Shop.push_back(v);
	}
	if (Vertex == 1) {
		last = i;
		for (auto it : Shop) {
			cnt[it] += 1;
		}
		Shop.clear();
	}
	int Nput;
	cin>>Nput;
	while (Nput --) {
		int v;
		cin>>v;
	}
};

void Move(int V) {
	cout << V << '\n';
	fflush(stdout);
	string Verdict;
	fflush(stdin);
	cin>>Verdict;
	//Assuming it's always OK
	int Nachieve;
	cin>>Nachieve;
	while (Nachieve --) {
		int v;
		cin>>v;
		cnt[OrderNode[v]] -= 1;
	}
};
int main(void) {
	cin>>n>>m;
	while (m --) {
		int u,v,w;
		cin>>u>>v>>w;
		G[u].push_back(v);
		G[v].push_back(u);
		E[u][v] = E[v][u] = w;
	}
	srand(chrono::steady_clock::now().time_since_epoch().count());
	for (int i = 1;i <= n;++i) //Frequency of the customers, not usefull in this approach
		cin>>freq[i];
	int Tmax;
	cin>>Tmax;
	vector < int > Path;
	for (int i = 0;i < Tmax;) {
		Update(i);
		/*	We are selecting a new path only when we are not following any other path.
			Therefore, the number of times we call TryPaths is ~ O(Tmax / AvgLen) where
			AvgLen is the average size of the path we have chosen to follow.
		*/
		if (Path.empty()) {
			TryPaths(Vertex);
			int bestNode;
			if (Vertex != 1) {//Heuristically it's better to return to vertex 1 to deliver current items
				bestNode = 1;
			} else {//Otherwise move to the vertex with the biggest cost calculated using random samplings of paths
				bestNode = max_element(Cost + 1,Cost + 1 + n) - Cost;
			}
			Path = SubPath[bestNode];
			if (abs(Cost[bestNode]) < eps && Vertex == 1) {//Not worth of following this path
				Path.clear();
			}
		}
		/*	Previously we decided to follow path `Path`. In this case we will have to move
			to vertex Path[0]. If it's the last vertex in the path or we transversed the 
			edge, then change `Vertex` accordingly.
		*/
		if (!Path.empty()) {
			Move(Path[0]);
			if (Path.size() == 1 || Path[0] != Path[1])
				Vertex = Path[0];
			Path.erase(Path.begin());//Inefficient but makes O(1) if it's amortized
			++i;
		} else {//Otherwise just stay in the same place
			Move(-1);
			++i;
		}
	}
    return 0;
}