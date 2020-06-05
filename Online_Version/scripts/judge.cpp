#include <cstdio>
#include "../common/gen/random_graph_generator.hpp"
#include "../common/gen/output_undirected_graph.hpp"
#include "../common/gen/graph_relabeling.hpp"
#include "../common/lib/agent.hpp"
#include "../common/lib/order.hpp"
#include "../common/lib/state.hpp"
#include "../common/gen/output_frequency.hpp"

int main(int argc, char** argv) {
  if(argc != 3) {
    fprintf(stderr, "Usage: %s <case-type> <seed>\n", argv[0]);
    return 1;
  }

  int type = static_cast<int>(strtol(argv[1], nullptr, 10));
  uint_fast64_t seed = strtoull(argv[2], nullptr, 10);
  random_number rnd(seed);

  int N, M; graph<edge> G;
  switch(type) {
  case 1:
    N = rnd.next_int(MIN_N, MAX_N);
    M = rnd.next_int((N*3 + 1) / 2, N*2);
    G = generate_random_graph(N, M, rnd);
    break;
  case 2:
    N = MAX_N;
    M = rnd.next_int((N*3 + 1) / 2, N*2);
    G = generate_random_graph(N, M, rnd);
    break;
  default:
    fprintf(stderr, "Error: case-type is invalid\n");
    return 1;
  }

  G = graph_relabeling(G, rnd);
  output_undirected_graph(G, rnd, stdout);
  output_frequency(G, stdout);

  const int max_step = TIME_LIMIT;
  fprintf(stdout, "%d\n", max_step);
  fflush(stdout);
			
  std::vector<std::pair<double, double>> prob_segment;
  double med = rnd.next_double() * 0.95;
  prob_segment.emplace_back(0.0 , 0.0);
  prob_segment.emplace_back(med , 1.0);
  prob_segment.emplace_back(0.95, 0.0);
  prob_segment.emplace_back(1.1 , 0.0);

  unsigned long long int seed_for_state = rnd.next_ulong();
  state st(G, max_step, prob_segment, seed_for_state);

  bool valid = true;
  for(int i=0; i<max_step; i++) {
    valid &= st.increment_step();
    if(!valid) break;
  }

  if(valid) fprintf(stderr, "Accepted: %" PRId64 "\n", st.calculate_score());
  else fprintf(stderr, "Error: your program did not get Accepted\n");
  return 0;
}
