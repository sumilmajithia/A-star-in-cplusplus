#include <cstdio>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cfloat>
#include <unordered_set>
#include <cmath>

class PriorityQueueNode {
public:
  unsigned int x;
  unsigned int y;
  double start_distance;
  double distance;
  PriorityQueueNode* parent;
  bool invalid;
  bool visited;
  PriorityQueueNode(unsigned int x, unsigned int y, double start_distance, double distance, PriorityQueueNode* parent, bool invalid, bool visited) :
    x(x), y(y), distance(distance), start_distance(start_distance), parent(parent), invalid(invalid), visited(visited) {}
};

class PriorityQueueCmp {
public:
  bool operator()(const PriorityQueueNode* lhs, const PriorityQueueNode* rhs) {
    return lhs->distance < rhs->distance;
  }
};

int main(int argc, char* argv[]) {
  if (argc == 2) {
    std::ifstream input_file;
    input_file.open(argv[1]);
    if (!input_file) {
      printf("Could not open file %s\n", argv[1]);
      return 0;
    }

    // Read first line to determine the size of the grid
    unsigned int num_rows = 0, num_cols = 0;
    std::string first_line;
    std::getline(input_file, first_line);
    sscanf(first_line.c_str(), "%d %d", &num_rows, &num_cols);
    // Initialilze the grid elements
    std::vector<PriorityQueueNode> grid;
    std::vector<std::string> output_fmt;
    grid.reserve(num_rows);
    output_fmt.reserve(num_rows);
    unsigned int i = 0;
    unsigned int start, end;
    std::string line;
    bool s_found = false, t_found = false;
    while(std::getline(input_file, line)) {
      // Trim whitespaces
      line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
      if (line.empty() || line[0] == '\n')
	continue;
      output_fmt.push_back(line);
      for (unsigned int j = 0; j < line.length() ; j++) {
	auto x = i;
	auto y = j;
	double distance = DBL_MAX;
	auto invalid = line[j] == '1' ? true : false;
	grid.emplace_back(i, j, DBL_MAX, distance, nullptr, invalid, false);
	if (line[j] == 'S') {
	  start = i*num_cols + j;
	  s_found = true;
	}
	else if (line[j] == 'T') {
	  end = i*num_cols + j;
	  t_found = true;
	}	
      }
      i++;
    }

    if (!t_found || !s_found) {
      std::cout << "INPUT GRID SHOULD HAVE AN S AND T" << std::endl;
      return 0;
    }
    grid[start].start_distance = 0;
    std::unordered_set<PriorityQueueNode*> queue;
    queue.insert(&grid[start]);
    PriorityQueueCmp cmp;
    while(queue.size() > 0) {
      auto min_element = std::min_element(queue.begin(), queue.end(), cmp);
      auto* node = *min_element;
      for (int i = -1; i < 2; i++)
	for (int j = -1; j < 2; j++) {
	  if (i == j && i == 0) continue;
	  float distance = 1.0;
	  if (std::abs(i) == std::abs(j)) distance = 1.414;
	  if (node->x + i >=0 && node->x + i < num_rows
	      && node->y + j < num_cols && node->y + j >=0) {
	    auto index = (node->x + i) * num_cols + (node->y + j);
	    if (!grid[index].invalid && !grid[index].visited) {
	      auto x_dist = grid[index].x - grid[end].x;
	      auto y_dist = grid[index].y - grid[end].y;
	      auto start_distance = node->start_distance
		+ distance; 
	      auto heuristic = std::sqrt(x_dist*x_dist + y_dist*y_dist);
	      auto new_distance = start_distance + heuristic;
	     if(start_distance < grid[index].start_distance) {
		grid[index].distance = new_distance;
		grid[index].start_distance = start_distance;
		grid[index].parent = node;
	      }
	      if (queue.count(&grid[index]) == 0)
		queue.insert(&grid[index]);
	    }
	  }
	}
      node->visited = true;
      queue.erase(min_element);
    }
    auto* node = grid[end].parent;
    if (!node) {
      std::cout << "NO PATH FOUND " << std::endl;
      return 0;
    }
    while (node->x * num_cols + node->y != start) {
      output_fmt[node->x][node->y] = '*';
      node = node->parent;
    }
    for (const auto& line : output_fmt)
      std::cout << line << std::endl;
    input_file.close();
  } else {
    printf("Usage : grid_solver input_filename\n");
  }
  return 0;
}
