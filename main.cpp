#include <algorithm>
#include <vector>
#include <string>

//For testing memory
#include <malloc_count-0.7.1/malloc_count.h>
#include <malloc_count-0.7.1/stack_count.h>

#include <polylla.hpp>


int main(int argc, char **argv) {

	std::string node_file = std::string(argv[1]);
	std::string graph_file = std::string(argv[2]);
	std::string output = std::string(argv[3]);
	Polylla mesh(node_file, graph_file);
	mesh.print_OFF(output + ".off");
	mesh.print_time(output + ".json");
	printf("our peak memory allocation: %lld\n",
		(long long)malloc_count_peak());
	return 0;
}
