#include "tbb/flow_graph.h"
#include "tbb/concurrent_queue.h"
#include <thread>
#include <chrono>
#include <iostream>

using namespace tbb::flow;
typedef int input_type;
typedef int output_type;
typedef tbb::flow::async_node<input_type, output_type> async_node_type;

class AsyncActivity {
public:
    typedef async_node_type::gateway_type gateway_type;

    struct work_type {
        input_type input;
        gateway_type* gateway;
    };
	
	
	AsyncActivity():
		stop_thread(false),
		service_thread(&AsyncActivity::mainLoop, this){}

	~AsyncActivity() {
		stop_thread=true;
		if(service_thread.joinable()){
        	service_thread.join();
		}
		std::cout<< "Thread join" << "\n";
	}

    void submit( input_type i, gateway_type* gateway ) {
        work_type w = {i, gateway};
        gateway->reserve_wait();
        my_work_queue.push(w);
    }

private:

	void mainLoop(){
		while(!stop_thread) {
			work_type w;
			while( my_work_queue.try_pop(w) ) {
				output_type result = do_work( w.input );
                //send the result back to the graph
                w.gateway->try_put( result );
                // signal that work is done
                w.gateway->release_wait();
            }
        }
	}

    output_type do_work( input_type& v ) {
        std::cout <<"async:"<< v << "\n";
        v = v+1;
        std::this_thread::sleep_for(std::chrono::milliseconds(3));
        return output_type(v);
    }

    tbb::concurrent_queue<work_type> my_work_queue;
    std::thread service_thread;
	bool stop_thread;
};

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
    AsyncActivity async_activity;
    tbb::flow::graph g;

    async_node_type async_consumer( g, unlimited,
    [&] ( input_type input, async_node_type::gateway_type& gateway ) {
        async_activity.submit( input, &gateway );
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
