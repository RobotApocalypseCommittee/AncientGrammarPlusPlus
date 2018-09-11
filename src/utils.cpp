//
// Created by Attoa on 06/09/2018.
//
#include <string>
#include <vector>
#include <iostream>

#include "../include/json.h"
#include "../include/utils.h"
#include "../include/utf8proc.h"

namespace ancientgrammar {
    namespace utils {
        std::string calculateUnicodeNormalization(const std::string &in, const std::string &mode) {
            auto pString = (const utf8proc_uint8_t*) in.c_str();

            utf8proc_uint8_t* pOutString;
            if (mode == "NFC") {
                pOutString = utf8proc_NFC(pString);
            } else {
                pOutString = utf8proc_NFD(pString);
            }

            std::string retString = std::string((const char*) pOutString);
            free(pOutString);

            return retString;
        }

        CharacterVector utf8CharacterVector(const std::string &in) {
            std::string combinedString = calculateUnicodeNormalization(in, "NFC");
            auto pCombinedString = (const utf8proc_uint8_t*) combinedString.c_str();

            CharacterVector characterVector;
            size_t offset = 0;
            while (true) {
                utf8proc_int32_t codepoint;
                utf8proc_iterate(pCombinedString + offset, -1, &codepoint);

                //Null, end of string TODO better way (get length of whole string)
                if (codepoint == 0) {
                    break;
                }

                utf8proc_uint8_t character[4];
                auto charSize = (size_t) utf8proc_encode_char(codepoint, character);

                offset += charSize;

                std::string realChar = std::string((const char*) character, charSize);
                characterVector.push_back(realChar);
            }

            return characterVector;
        }

        std::string characterVectorSubstr(CharacterVector &in, unsigned long long int start, length_t length) {
            std::string reconstructedString;
            if (length >= 0) {
                for (unsigned long long int i = start; i < start + length; i++) {
                    reconstructedString += in[(size_t) i];
                }
            } else {
                for (unsigned long long int i = start; i < in.size(); i++) {
                    reconstructedString += in[(size_t) i];
                }
            }

            return reconstructedString;
        }

        std::string characterVectorSubstr(CharacterVector &in, unsigned long long int start) {
            return characterVectorSubstr(in, start, -1);
        }

        std::string utf8Substr(const std::string &in, unsigned long long int start, length_t length) {
            CharacterVector characterVector = utf8CharacterVector(in);
            return characterVectorSubstr(characterVector, start, length);
        }

        std::string utf8Substr(const std::string &in, unsigned long long int start) {
            return utf8Substr(in, start, -1);
        }

        unsigned long long int utf8Length(const std::string &in) {
            return utf8CharacterVector(in).size();
        }

        bool isVowel(const std::string character) {
            return std::string("αειορυωη").find(basicChar(character)) != std::string::npos;
        }

        std::string removeAccents(const std::string &in, const bool oneCharOnly) {
            std::string decomposedString = calculateUnicodeNormalization(in, "NFD");
            auto pDecomposedString = (const utf8proc_uint8_t*) decomposedString.c_str();

            size_t offset = 0;
            std::string rebuiltString;
            while (true) {
                utf8proc_int32_t codepoint;
                utf8proc_iterate(pDecomposedString + offset, -1, &codepoint);

                //Null, end of string TODO better way (get length of whole string)
                if (codepoint == 0) {
                    break;
                }

                utf8proc_uint8_t character[4];
                auto charSize = (size_t) utf8proc_encode_char(codepoint, character);

                offset += charSize;

                if ((utf8proc_category(codepoint) == UTF8PROC_CATEGORY_MN) && ((codepoint != 0x0345) || oneCharOnly)) {
                    continue;
                }

                rebuiltString.append((const char*) character, charSize);
            }

            if (!oneCharOnly) {
                return calculateUnicodeNormalization(rebuiltString, "NFC");
            } else {
                return rebuiltString;
            }
        }

        bool isEqual(const std::string &s1, const std::string &s2) {
            return removeAccents(s1) == removeAccents(s2);
        }

        std::string basicChar(const std::string &in) {
            return removeAccents(in, true);
        }

        std::string calculateContraction(const std::string &stem, const std::string &ending, ContractType contractType,
                                         bool spuriousEi) {
            CharacterVector stemCharacterVector = utf8CharacterVector(stem);
            unsigned long long int stemLength = stemCharacterVector.size();

            CharacterVector endingCharacterVector = utf8CharacterVector(ending);

            std::string noAccentEnding = removeAccents(ending);

            if (contractType == ContractType::ALPHA) {
                if (isEqual(characterVectorSubstr(endingCharacterVector, 0, 2), "ει")) {
                    if (spuriousEi) {
                        return characterVectorSubstr(stemCharacterVector, 0, stemLength-1) + ending;
                    } else {  // I have no known examples of this TODO
                        return characterVectorSubstr(stemCharacterVector, 0, stemLength-1) + "ᾳ" +
                                characterVectorSubstr(endingCharacterVector, 2);
                    }
                }


                for (const std::string &endingStart : detail::kAlphaContractOrder) {
                    if (noAccentEnding.compare(0, endingStart.size(), endingStart) == 0) {
                        return characterVectorSubstr(stemCharacterVector, 0, stemLength-1) +
                                detail::kAlphaContractMap.at(endingStart) +
                                characterVectorSubstr(endingCharacterVector, utf8Length(endingStart));
                    }
                }
            } else if (contractType == ContractType::EPSILON) {
                for (const std::string &endingStart : detail::kEpsilonContractOrder) {
                    if (noAccentEnding.compare(0, endingStart.size(), endingStart) == 0) {
                        return characterVectorSubstr(stemCharacterVector, 0, stemLength-1) +
                               detail::kEpsilonContractMap.at(endingStart) +
                               characterVectorSubstr(endingCharacterVector, utf8Length(endingStart));
                    }
                }
            }

            return stem + ending;
        }
    }
}
