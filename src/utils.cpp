//
// Created by Attoa on 06/09/2018.
//
#include <string>
#include <vector>

#include "../include/utils.h"
#include "../include/utf8proc.h"

namespace ancientgrammar {
    namespace utils {
        std::string calculateUnicodeNormalization(const std::string &in, const std::string &mode) {
            auto pString = reinterpret_cast<const utf8proc_uint8_t*>(in.c_str());

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

                const utf8proc_int32_t codepointCopy = codepoint;

                utf8proc_uint8_t character = 0;
                auto charSize = (size_t) utf8proc_encode_char(codepointCopy, &character);
                std::string realChar = std::string((const char*) &character).substr(0, charSize);

                // God knows why this is needed, but the above function call seems to somehow alter codepoint;
                codepoint = codepointCopy;

                offset += charSize;

                if ((utf8proc_category(codepoint) == UTF8PROC_CATEGORY_MN) && ((codepoint != 0x0345) || !oneCharOnly)) {
                    continue;
                }

                rebuiltString += realChar;
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

        std::string basicChar(std::string in) {
            return removeAccents(in, true);
        }

        std::vector<std::string> utf8CharacterVector(const std::string &in) {
            std::string combinedString = calculateUnicodeNormalization(in, "NFC");
            auto pCombinedString = (const utf8proc_uint8_t*) combinedString.c_str();

            std::vector<std::string> characterVector;
            size_t offset = 0;
            while (true) {
                utf8proc_int32_t codepoint;
                utf8proc_iterate(pCombinedString + offset, -1, &codepoint);

                //Null, end of string TODO better way (get length of whole string)
                if (codepoint == 0) {
                    break;
                }

                utf8proc_uint8_t character;
                auto charSize = (size_t) utf8proc_encode_char(codepoint, &character);

                offset += charSize;

                std::string realChar = std::string((const char*) &character).substr(0, charSize);
                characterVector.push_back(realChar);
            }

            return characterVector;
        }

        std::string utf8Substr(const std::string &in, int start, length_t length) {
            std::vector<std::string> characterVector = utf8CharacterVector(in);
            std::string reconstructedString;
            if (length >= 0) {
                for (int i = start; i < start + length; i++) {
                    reconstructedString += characterVector[i];
                }
            } else {
                for (int i = start; i < characterVector.size(); i++) {
                    reconstructedString += characterVector[i];
                }
            }

            return reconstructedString;
        }

        std::string utf8Substr(const std::string &in, int start) {
            return utf8Substr(in, start, -1);
        }
    }
}
