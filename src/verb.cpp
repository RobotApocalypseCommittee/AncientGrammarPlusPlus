#include "../include/verb.h"
#include "../include/utils.h"

#include <iostream>

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

        std::string Verb::calculateAugment(std::string stem, bool uncommonEpsilon, std::string *preposition) {
            bool hasPreposition = preposition != nullptr;
            if (hasPreposition) {

            }
        }

        std::string Verb::calculateBreathing(std::string stem, std::string augment, size_t length, bool hasPreposition) {
            if (hasPreposition) {
                return augment + stem.substr(length);
            }

            if (ancientgrammar::utils::calculateUnicode(stem, "NFD").find(u8"\u0313") != std::string::npos) {
                // Smooth breathing found
                return ancientgrammar::utils::calculateUnicode(augment + u8"\u0313" + stem.substr(length), "NFC");
            } else if (ancientgrammar::utils::calculateUnicode(stem, "NFD").find(u8"\u0314") != std::string::npos) {
                // Rough breathing found
                return ancientgrammar::utils::calculateUnicode(augment + u8"\u0314" + stem.substr(length), "NFC");
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