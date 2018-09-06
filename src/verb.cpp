#include "../include/verb.h"
#include "../include/utils.h"
#include "../include/utf8proc.h"

#include <iostream>
#include <cstring>

namespace ancientgrammar {
    namespace verbs {

        const std::vector<std::string> Verb::kAugmentOrder = {
                u8"ᾳ", u8"αι", u8"ει", u8"αυ", u8"8ευ", u8"α", u8"ι", u8"ο", u8"οι", u8"υ", u8"η", u8"ω"
        };
        const std::map<std::string, std::string> Verb::kAugmentMap = {
                {u8"ᾳ", u8"η\u0345"},
                {u8"αι", u8"η\u0345"},
                {u8"ει", u8"η\u0345"},
                {u8"αυ", u8"ηυ"},
                {u8"ευ", u8"ηυ"},
                {u8"α", u8"η"},
                {u8"ι", u8"ι"},
                {u8"ο", u8"ω"},
                {u8"οι", u8"ω\u0345"},
                {u8"υ", u8"υ"},
                {u8"η", u8"η"},
                {u8"ω", u8"ω"}
        };

        Verb::Verb() : mAllowedForms(kAllFormsAllowed) {};
        Verb::~Verb() = default;

        std::string Verb::calculateAugment(const std::string stem, const bool uncommonEpsilon, const std::string* preposition) {
            std::string stemCopy = stem;
            std::string noAccentStemCopy = utils::removeAccents(stemCopy);

            bool hasPreposition = preposition != nullptr;
            std::string toPrepend;
            if (hasPreposition) {
                if (!(utils::isEqual(*preposition, u8"προ") || utils::isEqual(*preposition, u8"περι")) &&
                      utils::isVowel(std::string(1, preposition->back()))) {
                    toPrepend = preposition->substr(0, preposition->length()-1);
                } else {
                    toPrepend = *preposition;
                }

                stemCopy = stemCopy.substr(preposition->length());
            }

            if (!ancientgrammar::utils::isVowel(std::string(1, preposition->front()))) {
                if (hasPreposition) {
                    return toPrepend + u8"ε" + stemCopy;
                } else {
                    return ancientgrammar::utils::calculateUnicodeNormalization(u8"ε\u0313", "NFC") + stemCopy;
                }
            }

            bool toReturnSet = false;
            std::string toReturn;
            for (const std::string& start : kAugmentOrder) {
                if (noAccentStemCopy.compare(0, start.size(), start) == 0) {
                    std::string decomposedString = utils::calculateUnicodeNormalization(start, "NFD");
                    auto pDecomposedString = (const utf8proc_uint8_t*) decomposedString.c_str();

                    // Because of niceness, offset is also the length in bytes
                    size_t offset = 0;
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
                    }

                    toReturn = calculateBreathing(decomposedString, kAugmentMap.at(start), offset, hasPreposition);
                    toReturnSet = true;

                    break;
                }
            }

            if (noAccentStemCopy.front() == *u8"ε" && !toReturnSet) {
                utf8proc_int32_t codepoint;
                utf8proc_iterate((const utf8proc_uint8_t*)u8"ε", -1, &codepoint);
                utf8proc_uint8_t character;
                auto charSize = (size_t) utf8proc_encode_char(codepoint, &character);

                if (uncommonEpsilon) {
                    toReturn = calculateBreathing(stemCopy, u8"ει", charSize, hasPreposition);
                } else {
                    toReturn = calculateBreathing(stemCopy, u8"η", charSize, hasPreposition);
                }
            }

            if (hasPreposition) {
                return toPrepend + toReturn;
            } else {
                return toReturn;
            }
        }

        std::string Verb::calculateBreathing(std::string stem, std::string augment, size_t length, bool hasPreposition) {
            if (hasPreposition) {
                return augment + stem.substr(length);
            }

            if (ancientgrammar::utils::calculateUnicodeNormalization(stem, "NFD").find(u8"\u0313") != std::string::npos) {
                // Smooth breathing found
                return ancientgrammar::utils::calculateUnicodeNormalization(augment + u8"\u0313" + stem.substr(length), "NFC");
            } else if (ancientgrammar::utils::calculateUnicodeNormalization(stem, "NFD").find(u8"\u0314") != std::string::npos) {
                // Rough breathing found
                return ancientgrammar::utils::calculateUnicodeNormalization(augment + u8"\u0314" + stem.substr(length), "NFC");
            } else {
                // TODO actually throw error
                return "Oh no";
            }
        }


        namespace detail {
            bool isTenseAspect(Tense tense) {
                return (tense == Tense::PRESENT) || (tense == Tense::AORIST);
            }

            bool isMoodFinite(Mood mood) {
                return (mood == Mood::INDICATIVE) || (mood == Mood::SUBJUNCTIVE) || (mood == Mood::OPTATIVE);
            }
        }

    }
}