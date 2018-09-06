//
// Created by Attoa on 06/09/2018.
//
#include <string>

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

                utf8proc_uint8_t character;
                auto charSize = (size_t) utf8proc_encode_char(codepoint, &character);

                offset += charSize;

                if ((utf8proc_category(codepoint) == UTF8PROC_CATEGORY_MN) && ((codepoint != 0x0345) || !oneCharOnly)) {
                    continue;
                }

                std::string realChar = std::string((const char*) &character).substr(0, charSize);
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
    }
}
