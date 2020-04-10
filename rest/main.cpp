#include <cpprest/http_listener.h>              // HTTP server
#include <cpprest/json.h>                       // JSON library
#include <cpprest/uri.h>                        // URI library
#include <cpprest/ws_client.h>                  // WebSocket client
#include <cpprest/containerstream.h>            // Async streams backed by STL containers
#include <cpprest/interopstream.h>              // Bridges for integrating Async streams with STL and WinRT streams
#include <cpprest/rawptrstream.h>               // Async streams backed by raw pointer to memory
#include <cpprest/producerconsumerstream.h>     // Async streams for producer consumer scenarios
#include <thread>
// using namespace utility;                    // Common utilities like string conversions
// using namespace web;                        // Common features like URIs.
// using namespace web::http;                  // Common HTTP functionality
// using namespace web::http::client;          // HTTP client features
// using namespace concurrency::streams;       // Asynchronous streams

int main(){


    std::string x("Hello");
    auto t1 = pplx::task<std::string>([&x]() // catch x
    {
        std::cout <<"Entry1 with TID : "<< std::this_thread::get_id()
        <<std::endl;
        sleep(2);  // some dummy work
        return (x + std::string(" World !")); // return a std::string
    })
    .then([](std::string x) // catch the return value of the prevTask or    prevLambda function
    {
        sleep(1); // some dummy work
        std::cout << "Entry2 with TID : " << std::this_thread::get_id()
        <<" : "<< x << std::endl;
    });
    t1.wait();

}
