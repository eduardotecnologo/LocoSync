#include "locosync/client.hpp"
#include <curl/curl.h>
#include <iostream>

namespace locosync {
    // Callback estático para escrita de dados (Seguro contra estouro)    
    static size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) 
    {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }

    std::shared_ptr<Client> Client::create() 
    {
        return std::shared_ptr<Client>(new Client());
    }

    Client::Client() 
    {
        curl_global_init(CURL_GLOBAL_DEFAULT);
    }

    std::future<Response> Client::get(const std::string& url)
    {
        return std::async(std::launch::async, [this, url]()
        {

        })
    }

}