//
// Created by Attoa on 10/09/2018.
//

#include <iostream>
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
    }
}