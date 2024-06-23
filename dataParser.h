//
// Created by one on 13/06/24.
//
#ifndef INPUT_PUTTER_DATAPARSER_H
#define INPUT_PUTTER_DATAPARSER_H
#endif //INPUT_PUTTER_DATAPARSER_H

#include <string>
#include <vector>
#include <regex>
#include <iostream>
namespace parser
{

    struct item_data
            {
        char item_name[64];
        float hundred_gram_cost;
        float item_kilos;

        float per_item_cost;

        float per_litre_cost;
        float item_litres;

        int multi_buy_cost;
        int multi_buy_quantity;
    };

    // helper method for splitting a string by a character, similar to java
    std::vector<std::string> split(const std::string &s, char delimiter) {
        std::vector<std::string> tokens;
        std::string token;
        std::stringstream ss(s);
        while (std::getline(ss, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

    std::vector<item_data> items;

    //parses the input and puts raw data into the vector "items", returns a pointer to this vector
    std::vector<item_data>* parse(const std::string& in)
    {
        std::vector<std::string> args = split(in, '\n');

        //filter out unwanted arguments
        for (int i = 0; i < args.size();) {
            auto arg = args.at(i);
            if (strstr(arg.c_str(), "Product of") != nullptr || arg.empty()) {
                args.erase(args.begin() + i);
                continue;
            }

            i++;
        }

        for (int i = 0; i < args.size(); i++) {
            std::cout << "\narg: '" << args.at(i) + "'";
        }
        return &items;
    }

    //cleans up memory and resets to original state
    void close()
    {
        items.clear();
    }


}