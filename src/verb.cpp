#include <iostream>
#include <cstring>

#include "../include/verb.h"
#include "../include/utils.h"
#include "../include/utf8proc.h"

namespace ancientgrammar {
    namespace verbs {

        const std::vector<std::string> Verb::kAugmentOrder = {
                "ᾳ", "αι", "ει", "αυ", "ευ", "α", "ι", "οι", "ο", "υ", "η", "ω"
        };
        const std::map<std::string, std::string> Verb::kAugmentMap = {
                {"ᾳ",  "ῃ"},
                {"αι", "ῃ"},
                {"ει", "ῃ"},
                {"αυ", "ηυ"},
                {"ευ", "ηυ"},
                {"α",  "η"},
                {"ι",  "ι"},
                {"οι", "ῳ"},
                {"ο",  "ω"},
                {"υ",  "υ"},
                {"η",  "η"},
                {"ω",  "ω"}
        };

        Verb::Verb() : mAllowedForms(kAllFormsAllowed) {};

        Verb::~Verb() = default;

        std::string
        Verb::calculateAugment(const std::string &stem, const bool uncommonEpsilon, const std::string *preposition) {
            std::string stemCopy = utils::calculateUnicodeNormalization(stem, "NFC");
            std::string noAccentStemCopy = utils::removeAccents(stemCopy);

            bool hasPreposition = preposition != nullptr;
            std::string toPrepend;
            if (hasPreposition) {
                if (!(utils::isEqual(*preposition, "προ") || utils::isEqual(*preposition, "περι")) &&
                    utils::isVowel(utils::utf8Substr(*preposition,
                                                     (int) utils::utf8CharacterVector(*preposition).size() - 1))) {
                    toPrepend = utils::utf8Substr(*preposition, 0, utils::utf8CharacterVector(*preposition).size() - 1);
                } else {
                    toPrepend = *preposition;
                }

                int start = (int) utils::utf8CharacterVector(*preposition).size();
                stemCopy = utils::utf8Substr(stemCopy, start);
                //stemCopy = stemCopy.substr(preposition->length());
            }

            if (!utils::isVowel(utils::utf8Substr(stemCopy, 0, 1))) {
                if (hasPreposition) {
                    return toPrepend + "ε" + stemCopy;
                } else {
                    return "ἐ" + stemCopy;
                }
            }

            bool toReturnSet = false;
            std::string toReturn;
            for (const std::string &start : kAugmentOrder) {
                if (noAccentStemCopy.compare(0, start.size(), start) == 0) {
                    toReturn = calculateBreathing(stemCopy, kAugmentMap.at(start),
                                                  (int) utils::utf8CharacterVector(start).size(),
                                                  hasPreposition);
                    toReturnSet = true;

                    break;
                }
            }

            if (utils::isEqual(utils::utf8Substr(noAccentStemCopy, 0, 1), "ε") && !toReturnSet) {
                if (uncommonEpsilon) {
                    toReturn = calculateBreathing(stemCopy, "ει", 1, hasPreposition);
                } else {
                    toReturn = calculateBreathing(stemCopy, "η", 1, hasPreposition);
                }
            }

            if (hasPreposition) {
                return toPrepend + toReturn;
            } else {
                return toReturn;
            }
        }

        std::string
        Verb::calculateBreathing(std::string stem, std::string augment, int length, bool hasPreposition) {
            if (hasPreposition) {
                return augment + utils::utf8Substr(stem, length);
            }

            if (ancientgrammar::utils::calculateUnicodeNormalization(stem, "NFD").find("̓") !=
                std::string::npos) {
                // Smooth breathing found
                return ancientgrammar::utils::calculateUnicodeNormalization(
                        augment + "̓" + utils::utf8Substr(stem, length), "NFC");
            } else if (ancientgrammar::utils::calculateUnicodeNormalization(stem, "NFD").find("̔") !=
                       std::string::npos) {
                // Rough breathing found
                return ancientgrammar::utils::calculateUnicodeNormalization(
                        augment + "̔" + utils::utf8Substr(stem, length), "NFC");
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