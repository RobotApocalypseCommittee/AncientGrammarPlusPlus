//
// Created by Attoa on 06/09/2018.
//

#ifdef _WIN32
#include <Windows.h>
#endif

#include "../include/verbs/verb.h"
#include "../include/verbs/regularverb.h"
#include "../include/utils.h"
#include "../include/utf8proc.h"
#include "../include/json.h"

#include <iostream>
#include <cstring>

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    setvbuf(stdout, nullptr, _IOFBF, 1000);
#endif
    //std::cout << "Test: " << ancientgrammar::verbs::Verb::calculateBreathing(std::string(u8"ἀγγελλω"), std::string(u8"η"), 1, false) << std::endl;
    //std::cout << "Test: " << utf8proc_NFD(reinterpret_cast<const utf8proc_uint8_t*>(std::string(u8"ἀγγελλω").c_str())) << std::endl;

    auto stem = std::string("ἀγγελλω");
    auto augment = std::string("η");

    if (ancientgrammar::utils::calculateUnicodeNormalization(stem, "NFD").find("̓") != std::string::npos) {
        //std::cout << "Found" << std::endl;
        //std::cout << ancientgrammar::utils::calculateUnicodeNormalization(stem, "NFC") << std::endl;
    }

    //std::cout << ancientgrammar::verbs::Verb::calculateBreathing(stem, augment, 3, false) << std::endl;

    /*
    size_t offset = 0;
    while (true) {
        utf8proc_int32_t codepoint;
        utf8proc_iterate(((const utf8proc_uint8_t*) u8"ἀγγελλω") + offset, -1, &codepoint);

        if (codepoint == 0) {
            break;
        }

        utf8proc_uint8_t character;
        auto charSize = (size_t) utf8proc_encode_char(codepoint, &character);

        std::string realChar = std::string((const char*) &character).substr(0, charSize);
        std::cout << realChar << ": ";
        std::cout << strlen(realChar.c_str()) << std::endl;

        offset += charSize;
    }*/

    std::string prep("παρα");

    std::cout << ancientgrammar::utils::removeAccents("ᾤκεον", false) << std::endl;

    ancientgrammar::verbs::RegularVerb bang("τιμαω");

    std::cout << bang.getFiniteForm(ancientgrammar::verbs::Tense::PRESENT, ancientgrammar::verbs::Mood::OPTATIVE,
            ancientgrammar::verbs::Voice::ACTIVE, 2, false) << std::endl;
    
    //std::cout << ancientgrammar::verbs::detail::kVerbTable << std::endl;

    return 0;
}
