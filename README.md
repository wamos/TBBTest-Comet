## Goal
Test async_node of Intel TBB flow graph on SDSC's Comet Cluster

## Source Files
**async_basic.cpp**: It declares async_node in the very first few lines of the source code.
```C++
line 10 typedef tbb::flow::async_node<input_type, output_type> async_node_type;
```
**async_thread.cpp**: It's a fully functional async_node controlling a seperated thread. The seperated thread is tied with a gateway to return the result to the flow graph.


## Environment Setup

0. The offocial Github repo of Intel TBB provides a through tutorial on how to use CMake to compile TBB on Linux. [Comiple TBB with CMake on Linux](https://github.com/01org/tbb/tree/tbb_2018/cmake#source-code-integration-of-intel-tbb-to-the-sub-string-finder-sample-linux-os). We basically followed their instructions to write our own CMakeLists.txt file.

1. Check out https://github.com/01org/tbb to a directory ${DIR}, make sure you check out the TBB_2018 branch, which is the current stable release.  `git clone https://github.com/01org/tbb.git`

2. Check out https://github.com/wamos/TBBTest-Comet to the same directory ${DIR}. This repo is our code to test async_node of TBB flow graph.  `git clone https://github.com/wamos/TBBTest-Comet.git`

3. Create a directory seperated from our source code. `mkdir ${DIR}/TBBTest-Comet/build; cd ${DIR}/TBBTest-Comet/build` 

4. Load CMake module on Comet `load module cmake`

5. Specify TBB ROOT for CMake `cmake -DTBB_ROOT=${DIR}/tbb ..` 

6. Run make in the build directory `make`

## Expected Errors
```CMake
/${DIR}/TBBTest-Comet/async_basic.cpp:10:20: error: 'async_node' in namespace 'tbb::flow' does not name a template type
 typedef tbb::flow::async_node<input_type, output_type> async_node_type;
                    ^
/${DIR}/TBBTest-Comet/async_basic.cpp: In function 'int main()':
/${DIR}/TBBTest-Comet/async_basic.cpp:35:5: error: 'async_node_type' was not declared in this scope
     async_node_type async_consumer( g, unlimited,
     ^
/${DIR}/TBBTest-Comet/async_basic.cpp:38:7: error: expected primary-expression before ')' token
     } );
       ^
/${DIR}/TBBTest-Comet/async_basic.cpp:47:35: error: 'async_consumer' was not declared in this scope
     tbb::flow::make_edge( source, async_consumer );
                                   ^
make[2]: *** [CMakeFiles/async_basic.dir/async_basic.cpp.o] Error 1
make[1]: *** [CMakeFiles/async_basic.dir/all] Error 2
make: *** [all] Error 2

```



