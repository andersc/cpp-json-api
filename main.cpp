
//  curl -d '{"key1":10, "key2":20}' -H "Content-Type: application/json" -X POST http://127.0.0.1:8080/interface

#include <iostream>
#include "json.h"
#include "httplib.h"
#include <string>

int main() {
    std::cout << "Hello, World!" << std::endl;

    httplib::Server svr;

    svr.Post("/interface", [](const httplib::Request &rRequest, httplib::Response &rResponce) {

        std::cout << rRequest.body << std::endl;

        auto lParsedJson = nlohmann::json::parse(rRequest.body);

        nlohmann::json lResponse;

        for (auto& [key, value] : lParsedJson.items()) {
            std::cout << key << " : " << value << "\n";
            if (key.compare("key1")) {
                lResponse["key1"] = (uint64_t)value + 1;
            }
            if (key.compare("key2")) {
                lResponse["key2"] = (uint64_t)value + 1;
            }

        }

        rResponce.set_content(lResponse.dump(), "application/json");
    });

    svr.listen("0.0.0.0", 8080);

    return 0;
}