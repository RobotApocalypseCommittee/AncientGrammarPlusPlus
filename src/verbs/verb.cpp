#include <algorithm>
#include <vector>
#include <string>
#include <map>

#include "../../include/verbs/verb.h"
#include "../../include/utils.h"
#include "../../include/utf8proc.h"

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

        Verb::Verb(const AllowedFormsMap &allowedForms) : mAllowedForms(allowedForms) {};

        Verb::~Verb() = default;

        std::string
        Verb::calculateAugment(const std::string &stem, const bool uncommonEpsilon, const std::string &preposition) {
            std::string stemCopy = utils::calculateUnicodeNormalization(stem, "NFC");

            bool hasPreposition = !preposition.empty();
            std::string toPrepend;
            if (hasPreposition) {
                utils::CharacterVector prepositionCharacterVector = utils::utf8CharacterVector(preposition);
                 unsigned long long int prepositionLength = prepositionCharacterVector.size();
                if (!(utils::isEqual(preposition, "προ") || utils::isEqual(preposition, "περι")) &&
                        utils::isVowel(utils::characterVectorSubstr(prepositionCharacterVector, prepositionLength-1))) {

                    toPrepend = utils::characterVectorSubstr(prepositionCharacterVector, 0, prepositionLength-1);
                } else {
                    toPrepend = preposition;
                }
                stemCopy = utils::utf8Substr(stemCopy, utils::utf8Length(preposition));
            }

            if (!utils::isVowel(utils::utf8Substr(stemCopy, 0, 1))) {
                if (hasPreposition) {
                    return toPrepend + "ε" + stemCopy;
                } else {
                    return utils::calculateUnicodeNormalization("ἐ", "NFC") + stemCopy;
                }
            }

            std::string noAccentStemCopy = utils::removeAccents(stemCopy);

            bool toReturnSet = false;
            std::string toReturn;
            for (const std::string &start : kAugmentOrder) {
                if (noAccentStemCopy.compare(0, start.size(), start) == 0) {
                    toReturn = calculateBreathing(stemCopy, kAugmentMap.at(start),
                                                  utils::utf8CharacterVector(start).size(),
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

        std::string Verb::calculateBreathing(std::string stem, std::string augment, unsigned long long int length,
                bool hasPreposition) {
            if (hasPreposition) {
                return augment + utils::utf8Substr(stem, length);
            }

            if (ancientgrammar::utils::calculateUnicodeNormalization(stem, "NFD").find("̓") != std::string::npos) {
                // Smooth breathing found
                return ancientgrammar::utils::calculateUnicodeNormalization(
                        augment + "̓" + utils::utf8Substr(stem, length), "NFC");
            } else if (ancientgrammar::utils::calculateUnicodeNormalization(stem, "NFD").find("̔") !=
                       std::string::npos) {
                // Rough breathing found
                return ancientgrammar::utils::calculateUnicodeNormalization(
                        augment + "̔" + utils::utf8Substr(stem, length), "NFC");
            } else {
                throw std::logic_error("VerbComputeError: Could not ascertain breathing!");
            }
        }

        bool Verb::canGetForm(const Tense tense, const Voice voice) const {
            std::vector<Voice> voicesAllowedForTense = mAllowedForms.at(tense);
            return std::find(voicesAllowedForTense.begin(), voicesAllowedForTense.end(),
                    voice) != voicesAllowedForTense.end();
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