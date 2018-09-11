//
// Created by Attoa on 10/09/2018.
//

#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include <algorithm>

#include "../../include/verbs/regularverb.h"
#include "../../include/utils.h"

namespace ancientgrammar {
    namespace verbs {
        RegularVerb::RegularVerb(const std::string &present, const std::string &future, const std::string &aorist,
                                 const std::string &aoristPassive, const std::string &preposition,
                                 const bool uncommonEpsilonAugment,
                                 const AllowedFormsMap &allowedForms)
                : Verb(allowedForms), mPreposition(preposition),
                  mUncommonEpsilonAugment(uncommonEpsilonAugment) {

            utils::CharacterVector presentCharacterVector = utils::utf8CharacterVector(present);
            unsigned long long int presentLength = presentCharacterVector.size();
            if (present.empty()) {
                mAllowedForms[Tense::PRESENT] = std::vector<Voice>();
                mAllowedForms[Tense::IMPERFECT] = std::vector<Voice>();
            } else if (!utils::isEqual(utils::characterVectorSubstr(presentCharacterVector, presentLength-1), "ω")) {
                throw std::logic_error("VerbParseError: Could not recognise present!");
            } else {
                mPresent = utils::characterVectorSubstr(presentCharacterVector, 0, presentLength-1);

                std::string secondLastChar = utils::utf8Substr(present, presentLength-2, 1);
                if (utils::isEqual(secondLastChar, "α")) {
                    mContract = utils::ContractType::ALPHA;
                } else if (utils::isEqual(secondLastChar, "ε")) {
                    mContract = utils::ContractType::EPSILON;
                } else {
                    mContract = utils::ContractType::NONE;
                }
            }

            utils::CharacterVector futureCharacterVector = utils::utf8CharacterVector(future);
            unsigned long long int futureLength = futureCharacterVector.size();
            if (future.empty()) {
                mAllowedForms[Tense::FUTURE] = std::vector<Voice>();
            } else if (!utils::isEqual(utils::characterVectorSubstr(futureCharacterVector, futureLength-1), "ω")) {
                throw std::logic_error("VerbParseError: Could not recognise future!");
            } else {
                mFuture = utils::characterVectorSubstr(futureCharacterVector, 0, futureLength-1);
            }

            std::vector<Voice> aoristAllowedForms = mAllowedForms[Tense::AORIST];

            utils::CharacterVector aoristCharacterVector = utils::utf8CharacterVector(aorist);
            unsigned long long int aoristLength = aoristCharacterVector.size();
            if (aorist.empty()) {
                aoristAllowedForms.erase(std::remove(aoristAllowedForms.begin(),
                        aoristAllowedForms.end(), Voice::ACTIVE), aoristAllowedForms.end());
                aoristAllowedForms.erase(std::remove(aoristAllowedForms.begin(),
                        aoristAllowedForms.end(), Voice::MIDDLE), aoristAllowedForms.end());
            } else if (utils::isEqual(utils::characterVectorSubstr(aoristCharacterVector, aoristLength-1), "α")) {
                mAoristType = AoristType::WEAK;
                mAorist = utils::characterVectorSubstr(aoristCharacterVector, 0, aoristLength-1);
            } else if (utils::isEqual(utils::characterVectorSubstr(aoristCharacterVector, aoristLength-2), "ον")) {
                mAoristType = AoristType::STRONG;
                mAorist = utils::characterVectorSubstr(aoristCharacterVector, 0, aoristLength-2);
            } else {
                throw std::logic_error("VerbParseError: Aorist not recognised as a specified type!");
            }

            utils::CharacterVector aoristPassiveCV = utils::utf8CharacterVector(aoristPassive);
            unsigned long long int aoristPassiveLength = aoristPassiveCV.size();
            if (aoristPassive.empty()) {
                aoristAllowedForms.erase(std::remove(aoristAllowedForms.begin(),
                        aoristAllowedForms.end(), Voice::PASSIVE), aoristAllowedForms.end());
            } else if (!utils::isEqual(utils::characterVectorSubstr(aoristPassiveCV, aoristPassiveLength-2), "ην")) {
                throw std::logic_error("VerbParseError: Aorist passive not recognised!");
            } else {
                mAoristPassive = utils::characterVectorSubstr(aoristPassiveCV, 0, aoristPassiveLength-2);
            }

        }

        RegularVerb::~RegularVerb() = default;

        std::string RegularVerb::getStem(Tense tense, Mood mood, Voice voice) const {
            switch (tense) {
                case Tense::PRESENT: return mPresent;
                case Tense::FUTURE: return voice == Voice::PASSIVE ? mAoristPassive + "ησ" : mFuture;
                case Tense::IMPERFECT: {
                    std::string stem = mPresent;
                    if (mood == Mood::INDICATIVE) {
                        stem = calculateAugment(stem, mUncommonEpsilonAugment, mPreposition);
                    }
                    return stem;
                }
                default: { // Aorist
                    std::string stem;
                    if (voice == Voice::PASSIVE) {
                        stem = mAoristPassive;
                    } else {
                        stem = mAorist;
                    }

                    if (mood == Mood::INDICATIVE) {
                        stem = calculateAugment(stem, mUncommonEpsilonAugment, mPreposition);
                    }

                    return stem;
                }
            }
        }

        std::string RegularVerb::getFiniteForm(Tense tense, Mood mood, Voice voice, int person, bool isPlural, //NOLINT
                                               bool autocontract) const {
            if (!detail::isMoodFinite(mood)) {
                throw std::logic_error("VerbComputeError: The passed mood is not finite!");
            }

            if (!canGetForm(tense, voice)) {
                throw std::logic_error("VerbComputeError: That form of the verb either does not exist, or was not supplied!");
            }

            const std::string stem = getStem(tense, mood, voice);

            tense = tense == Tense::FUTURE ? Tense::PRESENT : tense;

            AoristType aoristType = voice == Voice::PASSIVE ? AoristType::WEAK : mAoristType;

            std::string ending;
            if (tense == Tense::AORIST) {
                ending = detail::kVerbTable["AORIST"][moodNames[(int)mood]][voiceNames[(int)voice]]
                            [aoristTypeNames[(int) aoristType]][std::to_string(person)][isPlural ? "True" : "False"]
                            .get<std::string>();
            } else if (tense == Tense::PRESENT && mood == Mood::OPTATIVE && voice == Voice::ACTIVE && !isPlural) {
                // Handles the dodgy optative
                ending = detail::kVerbTable["PRESENT"]["OPTATIVE"]["ACTIVE"][std::to_string(person)]["False"]
                            [mContract != utils::ContractType::NONE ? "True" : "False"].get<std::string>();
            } else {
                ending = detail::kVerbTable[tenseNames[(int)tense]][moodNames[(int)mood]][voiceNames[(int)voice]]
                        [std::to_string(person)][isPlural ? "True" : "False"].get<std::string>();
            }

            utils::CharacterVector stemCharacterVector = utils::utf8CharacterVector(stem);
            unsigned long long int stemLength = stemCharacterVector.size();
            if (autocontract && utils::isVowel(utils::characterVectorSubstr(stemCharacterVector, stemLength-1))) {
                return utils::calculateContraction(stem, ending, mContract);
            } else {
                return stem + ending;
            }
        }

        std::string RegularVerb::getImperative(Tense aspect, Voice voice, bool isPlural, bool autocontract) const { //NOLINT
            if (!detail::isTenseAspect(aspect)) {
                throw std::logic_error("VerbComputeError: The passed tense does not have an imperative!");
            }

            if (!canGetForm(aspect, voice)) {
                throw std::logic_error("VerbComputeError: That form of the verb either does not exist, or was not supplied!");
            }

            AoristType aoristType = voice == Voice::PASSIVE ? AoristType::WEAK : mAoristType;

            std::string stem = getStem(aspect, Mood::IMPERATIVE, voice);

            std::string ending;
            if (aspect == Tense::PRESENT) {
                ending = detail::kVerbTable[tenseNames[(int)aspect]]["IMPERATIVE"][voiceNames[(int)voice]]
                            ["2"][isPlural ? "True" : "False"].get<std::string>();
            } else {  // Aorist
                ending = detail::kVerbTable[tenseNames[(int)aspect]]["IMPERATIVE"][voiceNames[(int)voice]]
                             [aoristTypeNames[(int)aoristType]]["2"][isPlural ? "True" : "False"].get<std::string>();
            }

            if (autocontract) {
                return utils::calculateContraction(stem, ending, mContract);
            } else {
                return stem + ending;
            }
        }

        std::string RegularVerb::getInfinitive(Tense tense, Voice voice, bool autocontract) const { //NOLINT
            if (tense == Tense::IMPERFECT) {
                throw std::logic_error("VerbComputeError: There is no such thing as an imperfect infinitive!");
            }

            if (!canGetForm(tense, voice)) {
                throw std::logic_error("VerbComputeError: That form of the verb either does not exist, or was not supplied!");
            }

            bool spuriousEi = tense == Tense::PRESENT && voice == Voice::ACTIVE;

            AoristType aoristType = voice == Voice::PASSIVE ? AoristType::WEAK : mAoristType;

            std::string stem = getStem(tense, Mood::INFINITIVE, voice);

            // This is done after so only the endings get affected
            tense = tense == Tense::FUTURE ? Tense::PRESENT : tense;

            std::string ending;
            if (tense == Tense::AORIST) {
                ending = detail::kVerbTable[tenseNames[(int)tense]]["INFINITIVE"][voiceNames[(int)voice]]
                            [aoristTypeNames[(int)aoristType]].get<std::string>();
            } else {
                ending = detail::kVerbTable[tenseNames[(int)tense]]["INFINITIVE"][voiceNames[(int)voice]]
                            .get<std::string>();
            }

            if (autocontract) {
                return utils::calculateContraction(stem, ending, mContract, spuriousEi);
            } else {
                return stem + ending;
            }
        }
    }
}