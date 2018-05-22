#include "tbb/flow_graph.h"
#include "tbb/concurrent_queue.h"
#include <thread>
#include <chrono>
#include <iostream>

using namespace tbb::flow;
typedef int input_type;
typedef int output_type;
typedef tbb::flow::async_node<input_type, output_type> async_node_type;

struct Source {
	const int LIMIT = 100;
	int count;

	Source() {
		count = 0;
	}

	bool operator()(input_type& v) {
		if (count < LIMIT) {
			std::cout << "source:" << count << "\n";
			v = (input_type)count;
			count++;
			return true;
		} else {
			return false;
		}
	}
};

int main() {
    tbb::flow::graph g;

    async_node_type async_consumer( g, unlimited,
    [&] ( input_type input, async_node_type::gateway_type& gateway ) {
        //async_activity.submit( input, &gateway );
    } );

    tbb::flow::source_node<input_type> source( g, Source(), false); 
    tbb::flow::function_node<output_type> sink( g, unlimited, [](const output_type& v) { 
        //consume data from async work
        //v = 0;
        return continue_msg();
    } );

    tbb::flow::make_edge( source, async_consumer );
    tbb::flow::make_edge( async_consumer, sink);
	source.activate();	

    g.wait_for_all();
}
