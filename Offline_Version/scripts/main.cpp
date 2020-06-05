#include "bits/stdc++.h"
using namespace std;
#define all(s) s.begin(),s.end()
template < class P , class Q > ostream& operator<<(ostream& stream, pair < P , Q > v){ stream << "(" << v.fi << ',' << v.se << ")"; return stream;}
template < class T > ostream& operator<<(ostream& stream, const vector<T> v){ stream << "[ "; for (int i=0; i<(int)v.size(); i++) stream << v[i] << " "; stream << "]"; return stream;}
template < class T > T smin(T &a,T b) {if (a > b) a = b;return a;}
template < class T > T smax(T &a,T b) {if (a < b) a = b;return a;}
const int N = 512;
vector < int > Path[N][N];
vector < int > UniquePath[N][N];
vector < int > Concat(vector < int > a,vector < int > b) {
	vector < int > c;
	c.reserve(a.size() + b.size());
	c.insert(c.end(),a.begin(),a.end());
	c.insert(c.end(),b.begin(),b.end());
	return c;
}
 
//Function that get bigger as `a` increases and `b` decreases
long double f(long long a,long long b) {
	return 1.0 * a / b;
}
vector < int > Order[N];
int main(void) {
	int n,m;
	cin>>n>>m;
	while (m --) { 
		int u,v,w;
		cin>>u>>v>>w;
		for (int i = 0;i < w;++i) {
			Path[u][v].push_back(v);
			Path[v][u].push_back(u);
		}
	}
	// Run Floyd–Warshall algorithm on the graph in O(N^3)
	for (int k = 1;k <= n;++k)
		for (int i = 1;i <= n;++i)
			for (int j = 1;j <= n;++j)
				if (!Path[i][k].empty() && !Path[k][j].empty() // (i,k) and (k,j) are valid paths
						&&	((i != j && Path[i][j].empty()) //(i,j) does not have a path in between them
					 	||  Path[i][j].size() > Path[i][k].size() + Path[k][j].size())) /*or the path from (i,j) is
											is bigger than the path from (i,k) plus path from path (k,j)*/
					Path[i][j] = Concat(Path[i][k],Path[k][j]);

	int Tmax;
	cin>>Tmax;

	for (int i = 0;i < Tmax;++i) {
		int Nnew;
		cin>>Nnew;
		while (Nnew --) {
			int ID,v;
			cin>>ID>>v;
			Order[v].push_back(i);
		}
	}
	//Last is the last time we visited vertex 1
	int last = 0;
	int Vertex = 1;
	vector < int > Answer;
	//O(N * Tmax)
	for (int i = 0;i < Tmax;) {
		if (Vertex == 1) {
			last = i;
		}
		/*	W.first represent some cost which we want to maximize, the bigger the cost,
			the bigger is the potential reward. The costs are heuristically determined.
			W.se will represent a vertex that will correspond to the Path we will follow
			in the following steps.
		*/
		pair < db , int > W = make_pair(0,1);
		for (int j = 1;j <= n;++j) {
			/*	How many items can we take since the last time we visited vertex 1
				We want this value to be as big as possible. On the other hand, if the
				distance from current vertex `Vertex` to to vertex j is too big, then
				it might not be profitable to go follow this path. The cost is set to
				be Reward / Len(Path). The Reward is calculated only based on vertex j
				with vertices on the path between `Vertex` and j being ignored.
			*/
			int Cnt1 = lower_bound(all(Order[j]),last + 1) - Order[j].begin();
			//Abort if there is no path between `Vertex` and `j`
			if (Path[Vertex][j].empty())
				continue;
			smax(W,make_pair(f(Cnt1,Path[Vertex][j].size()),j));

			/*	Another type of paths would be to go from `Vertex` to vertex 1, delivering
				all the items and then comming back to vertex j. To distinguish between these
				two types of paths, W.se is set to -j in this case
			*/
			int cnt2 = lower_bound(all(Order[j]),i + 1 + Path[Vertex][1].size()) - Order[j].begin();
			smax(W,make_pair(f(cnt2,Path[Vertex][1].size() + Path[1][j].size()),-j));
		}
		//If it's not worth following any path then just stay in the same vertex
		if (!W.fi) {
			Answer.push_back(-1);
			++i;
			continue;
		}
		/*	Depending on the best path to follow, delete the items collected already while
			going through this path
		*/
		if (W.se < 0) {
			for (auto it : Path[Vertex][1]) {
				Answer.push_back(it);
				while (!Order[it].empty() && Order[it][0] <= last)
					Order[it].erase(Order[it].begin());
				++i;
			}
			Vertex = 1;
			last = i;
			W.se *= -1;
		}
		for (auto it : Path[Vertex][W.se]) {
			Answer.push_back(it);
			while (!Order[it].empty() && Order[it][0] <= last)
				Order[it].erase(Order[it].begin());
			++i;
		}
		Vertex = W.se;

	}
	//If we added extra vertices to our path that do not fit in time then let's remove them
	if (Answer.size() > Tmax) {
		Answer.resize(Tmax);
	}

	for (auto it : Answer)
		cout << it << '\n';
    return 0;
}