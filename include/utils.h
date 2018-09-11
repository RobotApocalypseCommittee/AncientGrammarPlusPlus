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
            const std::vector<std::string> kAlphaContractOrder = {
                    "αι", "ᾳ", "α", "ε", "η", "ῃ", "ι", "οι", "ου", "ο", "ω"
            };

            const std::vector<std::string> kEpsilonContractOrder = {
                    "αι", "α", "ει", "ε", "η", "ῃ", "ι", "οι", "ου", "ο", "υ", "ω", "ῳ"
            };

            const std::map<std::string, std::string> kAlphaContractMap {
                    {"αι", "αι"},
                    {"ᾳ", "ᾳ"},
                    {"α", "α"},
                    {"ε", "α"},
                    {"η", "α"},
                    {"ῃ", "ᾳ"},
                    {"ι", "αι"},
                    {"οι", "ῳ"},
                    {"ου", "ω"},
                    {"ο", "ω"},
                    {"ω", "ω"}
            };

            const std::map<std::string, std::string> kEpsilonContractMap {
                    {"αι", "ῃ"},
                    {"α", "η"},
                    {"ει", "ει"},
                    {"ε", "ει"},
                    {"η", "η"},
                    {"ῃ", "ῃ"},
                    {"ι", "ει"},
                    {"οι", "οι"},
                    {"ου", "ου"},
                    {"ο", "ου"},
                    {"υ", "ευ"},
                    {"ω", "ω"},
                    {"ῳ", "ῳ"}
            };
        }
    }
}


#endif //ANCIENTGRAMMAR_UTILS_H
