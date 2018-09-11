//
// Created by Attoa on 06/09/2018.
//

#ifndef ANCIENTGRAMMAR_UTILS_H
#define ANCIENTGRAMMAR_UTILS_H

#include <string>
#include <vector>

#include "json.h"

namespace ancientgrammar {
    namespace utils {
        typedef long long int length_t;

        typedef std::vector<std::string> CharacterVector;

        // Unicode things
        std::string calculateUnicodeNormalization(const std::string &in, const std::string &mode);

        CharacterVector utf8CharacterVector(const std::string &in);

        std::string characterVectorSubstr(CharacterVector &in, unsigned long long int start, length_t length);
        std::string characterVectorSubstr(CharacterVector &in, unsigned long long int start);

        std::string utf8Substr(const std::string &in, unsigned long long int start, length_t length);
        std::string utf8Substr(const std::string &in, unsigned long long int start);

        unsigned long long int utf8Length(const std::string &in);


        // Real things
        enum class ContractType {
            ALPHA,
            EPSILON,
            NONE
        };

        bool isVowel(std::string character);

        std::string removeAccents(const std::string &in, bool oneCharOnly = false);

        bool isEqual(const std::string &s1, const std::string &s2);

        std::string basicChar(const std::string &in);

        std::string calculateContraction(const std::string &stem, const std::string &ending, ContractType contractType, bool spuriousEi=false);

        namespace detail {
            const nlohmann::json kContractionTable = nlohmann::json::parse("{\n"
                                                                     "    \"ALPHA\":{\n"
                                                                     "        \"αι\":\"αι\",\n"
                                                                     "        \"ᾳ\":\"ᾳ\",\n"
                                                                     "        \"α\":\"α\",\n"
                                                                     "        \"ε\":\"α\",\n"
                                                                     "        \"η\":\"α\",\n"
                                                                     "        \"ῃ\":\"ᾳ\",\n"
                                                                     "        \"ι\":\"αι\",\n"
                                                                     "        \"οι\":\"ῳ\",\n"
                                                                     "        \"ου\":\"ω\",\n"
                                                                     "        \"ο\":\"ω\",\n"
                                                                     "        \"ω\":\"ω\"\n"
                                                                     "    },\n"
                                                                     "    \"EPSILON\":{\n"
                                                                     "        \"αι\":\"ῃ\",\n"
                                                                     "        \"α\":\"η\",\n"
                                                                     "        \"ει\":\"ει\",\n"
                                                                     "        \"ε\":\"ει\",\n"
                                                                     "        \"η\":\"η\",\n"
                                                                     "        \"ῃ\":\"ῃ\",\n"
                                                                     "        \"ι\":\"ει\",\n"
                                                                     "        \"οι\":\"οι\",\n"
                                                                     "        \"ου\":\"ου\",\n"
                                                                     "        \"ο\":\"ου\",\n"
                                                                     "        \"υ\":\"ευ\",\n"
                                                                     "        \"ω\":\"ω\",\n"
                                                                     "        \"ῳ\":\"ῳ\"\n"
                                                                     "    }\n"
                                                                     "}");
        }
    }
}


#endif //ANCIENTGRAMMAR_UTILS_H
