
//  curl -d '{"key1":10, "key2":20}' -H "Content-Type: application/json" -X POST http://127.0.0.1:8080/interface

#include <iostream>
#include <exception>
#include "json.h"
#include "httplib.h"

httplib::Server gHttpServer;

nlohmann::json jsonAPI(nlohmann::json &rParametersIn) {
    nlohmann::json lResponse;
    for (auto&[key, value]: rParametersIn.items()) {
        //std::cout << key << " : " << value << std::endl; //debug
        if (key == "key1") {
            lResponse["key1"] = (uint64_t) value + 1;
        }
        if (key == "key2") {
            lResponse["key2"] = (uint64_t) value + 1;
        }
    }
    return lResponse;
}

void getKey() {
    std::cout << "Press q + return to exit." << std::endl;
    char lInput = '0';
    while (lInput != 'q') {
        std::cin >> lInput;
    }
    std::cout << "Terminating server." << std::endl;
    gHttpServer.stop();
}

int main() {
    std::cout << "Starting the C++ JSON API" << std::endl;

    gHttpServer.Post("/interface", [](const httplib::Request &rRequest, httplib::Response &rResponce) {

        //std::cout << rRequest.body << std::endl; //Debug
        bool lIsJson = false;
        for (auto &rHeader: rRequest.headers) {
            //std::cout << rHeader.first << std::endl; //Debug
            //std::cout << rHeader.second << std::endl; //Debug
            if (strcasecmp(rHeader.first.c_str(), "content-type") == 0 &&
                strcasecmp(rHeader.second.c_str(), "application/json") == 0) {
                lIsJson = true;
                break;
            }
        }

        if (lIsJson) {
            nlohmann::json lParsedJson;
            try {
                lParsedJson = nlohmann::json::parse(rRequest.body);
            } catch (std::exception &e) {
                nlohmann::json lResponse;
                lResponse["status"] = "NOK";
                lResponse["why"] = e.what();
                rResponce.set_content(lResponse.dump(), "application/json");
                return;
            }
            auto lResponse = jsonAPI(lParsedJson);
            if (lResponse.empty()) {
                lResponse["status"] = "NOK";
                lResponse["why"] = "JSON data sent to server not correct.";
            } else {
                lResponse["status"] = "OK";
            }
            rResponce.set_content(lResponse.dump(), "application/json");

        } else {
            nlohmann::json lResponse;
            lResponse["status"] = "NOK";
            lResponse["why"] = "application type not json";
            rResponce.set_content(lResponse.dump(), "application/json");
        }

    });

    std::thread t(getKey);
    t.detach();

    try {
        auto lSuccess = gHttpServer.listen("0.0.0.0", 8080);
        if (!lSuccess) {
            std::cout << "Server Failed starting." << std::endl;
            return EXIT_FAILURE;
        }
    } catch (std::exception &e) {
        std::cout << "Exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}