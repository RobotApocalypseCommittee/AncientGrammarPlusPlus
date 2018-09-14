//
// Created by Attoa on 14/09/2018.
//

#include "../../include/verbs/deponentverb.h"
#include "../../include/utils.h"

namespace ancientgrammar {
    namespace verbs {

        DeponentVerb::DeponentVerb(const std::string &present, const std::string &future, const std::string &aorist,
                                   const std::string &aoristPassive, const std::string &preposition,
                                   bool uncommonEpsilonAugment, const AllowedFormsMap &allowedForms)
                                   : Verb(preposition, uncommonEpsilonAugment) {

            mAllowedForms = allowedForms;

            utils::CharacterVector presentCharacterVector = utils::utf8CharacterVector(present);
            unsigned long long int presentLength = presentCharacterVector.size();
            if (present.empty()) {
                mAllowedForms[Tense::PRESENT] = std::vector<Voice>();
                mAllowedForms[Tense::IMPERFECT] = std::vector<Voice>();
            } else if (utils::isEqual(utils::characterVectorSubstr(presentCharacterVector, presentLength-1), "ω")) {
                mPresent = utils::characterVectorSubstr(presentCharacterVector, 0, presentLength-1);

                std::string secondLastChar = utils::utf8Substr(present, presentLength-2, 1);
                if (utils::isEqual(secondLastChar, "α")) {
                    mContract = utils::ContractType::ALPHA;
                } else if (utils::isEqual(secondLastChar, "ε")) {
                    mContract = utils::ContractType::EPSILON;
                } else {
                    mContract = utils::ContractType::NONE;
                }
            } else if (utils::isEqual(utils::characterVectorSubstr(presentCharacterVector, presentLength-4), "ομαι")) {
                mPresent = utils::characterVectorSubstr(presentCharacterVector, 0, presentLength-4);

                std::vector<Voice> presentForms = mAllowedForms[Tense::PRESENT];
                std::vector<Voice> imperfForms = mAllowedForms[Tense::IMPERFECT];
                
                presentForms.erase(std::remove(presentForms.begin(),
                                                     presentForms.end(), Voice::ACTIVE), presentForms.end());
                imperfForms.erase(std::remove(imperfForms.begin(),
                                                     imperfForms.end(), Voice::ACTIVE), imperfForms.end());

                std::string fifthLastChar = utils::utf8Substr(present, presentLength-5, 1);
                if (utils::isEqual(fifthLastChar, "α")) {
                    mContract = utils::ContractType::ALPHA;
                } else if (utils::isEqual(fifthLastChar, "ε")) {
                    mContract = utils::ContractType::EPSILON;
                } else {
                    mContract = utils::ContractType::NONE;
                }
            } else {
                throw std::logic_error("VerbParseError: Could not recognise present!");
            }

            utils::CharacterVector futureCharacterVector = utils::utf8CharacterVector(future);
            unsigned long long int futureLength = futureCharacterVector.size();
            if (future.empty()) {
                mAllowedForms[Tense::FUTURE] = std::vector<Voice>();
            } else if (utils::isEqual(utils::characterVectorSubstr(futureCharacterVector, futureLength-1), "ω")) {
                mFuture = utils::characterVectorSubstr(futureCharacterVector, 0, futureLength-1);
            } else if (utils::isEqual(utils::characterVectorSubstr(futureCharacterVector, futureLength-4), "ομαι")) {
                mFuture = utils::characterVectorSubstr(futureCharacterVector, 0, futureLength-4);

                std::vector<Voice> futureForms = mAllowedForms[Tense::FUTURE];
                futureForms.erase(std::remove(futureForms.begin(),
                                              futureForms.end(), Voice::ACTIVE), futureForms.end());
            } else {
                throw std::logic_error("VerbParseError: Could not recognise future!");
            }

            // Variable to store whether the aorist passive has been accounted for by the aorist (passive form aorists)
            bool aoristPassiveDone = false;

            std::vector<Voice> aoristAllowedForms = mAllowedForms[Tense::AORIST];

            utils::CharacterVector aoristCharacterVector = utils::utf8CharacterVector(aorist);
            unsigned long long int aoristLength = aoristCharacterVector.size();
            if (aorist.empty()) {
                aoristAllowedForms.erase(std::remove(aoristAllowedForms.begin(),
                                                     aoristAllowedForms.end(), Voice::ACTIVE),
                                         aoristAllowedForms.end());
                aoristAllowedForms.erase(std::remove(aoristAllowedForms.begin(),
                                                     aoristAllowedForms.end(), Voice::MIDDLE),
                                         aoristAllowedForms.end());
            } else if (utils::isEqual(utils::characterVectorSubstr(aoristCharacterVector, aoristLength-1), "α")) {
                mAoristType = AoristType::WEAK;
                mAorist = utils::characterVectorSubstr(aoristCharacterVector, 0, aoristLength-1);
            } else if (utils::isEqual(utils::characterVectorSubstr(aoristCharacterVector, aoristLength-2), "ον")) {
                mAoristType = AoristType::STRONG;
                mAorist = utils::characterVectorSubstr(aoristCharacterVector, 0, aoristLength-2);
            } else if (utils::isEqual(utils::characterVectorSubstr(aoristCharacterVector, aoristLength-4), "αμην")) {
                mAoristType = AoristType::WEAK;
                mAorist = utils::characterVectorSubstr(aoristCharacterVector, 0, aoristLength - 4);

                aoristAllowedForms.erase(std::remove(aoristAllowedForms.begin(),
                                                     aoristAllowedForms.end(), Voice::ACTIVE),
                                         aoristAllowedForms.end());
            } else if (utils::isEqual(utils::characterVectorSubstr(aoristCharacterVector, aoristLength-4), "ομην")) {
                mAoristType = AoristType::STRONG;
                mAorist = utils::characterVectorSubstr(aoristCharacterVector, 0, aoristLength - 4);

                aoristAllowedForms.erase(std::remove(aoristAllowedForms.begin(),
                                                     aoristAllowedForms.end(), Voice::ACTIVE),
                                         aoristAllowedForms.end());
            } else if (utils::isEqual(utils::characterVectorSubstr(aoristCharacterVector, aoristLength-2), "ην")) {
                mAoristType = AoristType::WEAK;
                mAorist = utils::characterVectorSubstr(aoristCharacterVector, 0, aoristLength - 2);

                aoristAllowedForms.erase(std::remove(aoristAllowedForms.begin(),
                                                     aoristAllowedForms.end(), Voice::ACTIVE),
                                         aoristAllowedForms.end());

                aoristAllowedForms.erase(std::remove(aoristAllowedForms.begin(),
                                                     aoristAllowedForms.end(), Voice::MIDDLE),
                                         aoristAllowedForms.end());

                aoristPassiveDone = true;
            } else {
                throw std::logic_error("VerbParseError: Aorist not recognised as a specified type!");
            }

            // This means that there is not just aorist passive left - i.e. it is not a passive form aorist
            // In that case, the passive aorist would have been covered by the aorist section
            utils::CharacterVector aoristPassiveCV = utils::utf8CharacterVector(aoristPassive);
            unsigned long long int aoristPassiveLength = aoristPassiveCV.size();
            if (!aoristPassiveDone) {
                if (aoristPassive.empty()) {
                    aoristAllowedForms.erase(std::remove(aoristAllowedForms.begin(),
                                                         aoristAllowedForms.end(), Voice::PASSIVE),
                                             aoristAllowedForms.end());
                } else if (!utils::isEqual(utils::characterVectorSubstr(aoristPassiveCV, aoristPassiveLength-2), "ην")) {
                    throw std::logic_error("VerbParseError: Aorist passive not recognised!");
                } else {
                    mAoristPassive = utils::characterVectorSubstr(aoristPassiveCV, 0, aoristPassiveLength-2);
                }
            }
        }

        DeponentVerb::~DeponentVerb() = default;
    }
}
