//
// Created by Attoa on 06/09/2018.
//

#ifndef ANCIENTGRAMMAR_UTILS_H
#define ANCIENTGRAMMAR_UTILS_H

#include <string>
#include <vector>

namespace ancientgrammar {
    namespace utils {
        typedef long long int length_t;

        typedef std::vector<std::string> CharacterVector;

        enum class ContractType {
            ALPHA,
            EPSILON,
            NONE
        };

        std::string calculateUnicodeNormalization(const std::string &in, const std::string &mode);

        bool isVowel(std::string character);

        std::string removeAccents(const std::string &in, bool oneCharOnly = false);

        bool isEqual(const std::string &s1, const std::string &s2);

        std::string basicChar(std::string in);

        CharacterVector utf8CharacterVector(const std::string &in);

        std::string characterVectorSubstr(CharacterVector &in, unsigned long long int start, length_t length);
        std::string characterVectorSubstr(CharacterVector &in, unsigned long long int start);

        std::string utf8Substr(const std::string &in, unsigned long long int start, length_t length);
        std::string utf8Substr(const std::string &in, unsigned long long int start);

        unsigned long long int utf8Length(const std::string &in);
    }
}


#endif //ANCIENTGRAMMAR_UTILS_H
