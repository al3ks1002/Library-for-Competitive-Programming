#include <stdio.h>
#include <algorithm>
#include <vector>

using namespace std;

// Class that represents an undirected graph where
// you can find the biconnected components (separated by bridges).
class BCCBridgeGraph {
 public:
  explicit BCCBridgeGraph(const int num_vertices) : num_vertices_(num_vertices) {
    discovery_time_.resize(num_vertices + 1);
    low_link_.resize(num_vertices + 1);
    edges_.resize(num_vertices + 1);
    what_component_.resize(num_vertices + 1);
  }

  void AddEdge(const int from, const int to) {
    edges_[from].push_back(to);
    edges_[to].push_back(from);
  }

  void FindBiconnectedComponents() {
    current_count_ = 0;
    fill(discovery_time_.begin(), discovery_time_.end(), 0);
    fill(low_link_.begin(), low_link_.end(), 0);
    fill(what_component_.begin(), what_component_.end(), 0);
    biconnected_components_.clear();
    stack_.clear();
    DFS(1, 0);
  }

  vector<vector<int>> GetBiconnectedComponents() const {
    return biconnected_components_;
  }

  int GetComponent(const int node) const {
    return what_component_[node];
  }

 private:
  void DFS(const int node, const int father) {
    discovery_time_[node] = low_link_[node] = ++current_count_;
    stack_.push_back(node);

    for (auto neighbour : edges_[node]) {
      if (neighbour != father) {
        if (!discovery_time_[neighbour]) {
          DFS(neighbour, node);
          low_link_[node] = min(low_link_[node], low_link_[neighbour]);
        } else {
          low_link_[node] = min(low_link_[node], discovery_time_[neighbour]);
        }
      }
    }

    // The edge father - node is a bridge.
    if (low_link_[node] == discovery_time_[node]) {
      FetchComponent(node);
    }
  }

  void FetchComponent(const int node) {
    vector<int> new_component;
    int components_size = (int)biconnected_components_.size() + 1;
    while (stack_.back() != node) {
      new_component.push_back(stack_.back());
      what_component_[stack_.back()] = components_size;
      stack_.pop_back();
    }
    stack_.pop_back();
    new_component.push_back(node);
    what_component_[node] = components_size;
    biconnected_components_.push_back(new_component);
  }

  const int num_vertices_;
  int current_count_;
  vector<int> discovery_time_;
  vector<int> low_link_;
  vector<int> what_component_;
  vector<vector<int>> edges_;
  vector<vector<int>> biconnected_components_;
  vector<int> stack_;
};

int main() {
  int n, m;
  scanf("%d%d", &n, &m);

  BCCBridgeGraph graph(n);
  for (; m; m--) {
    int x, y;
    scanf("%d%d", &x, &y);
    graph.AddEdge(x, y);
  }

  graph.FindBiconnectedComponents();
  vector<vector<int>> bcc = graph.GetBiconnectedComponents();

  for (auto& comp : bcc) {
    sort(comp.begin(), comp.end());
  }
  sort(bcc.begin(), bcc.end());

  printf("%d\n", (int)bcc.size());
  for (auto& comp : bcc) {
    for (auto x : comp) {
      printf("%d ", x);
    }
    printf("\n");
  }

  return 0;
}
