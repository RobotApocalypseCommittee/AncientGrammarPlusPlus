//
// Created by Attoa on 06/09/2018.
//
#include <string>

#include "../include/utils.h"
#include "../include/utf8proc.h"

namespace ancientgrammar {
    namespace utils {
        std::string calculateUnicode(const std::string &in, const std::string &mode) {
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
    }
}
