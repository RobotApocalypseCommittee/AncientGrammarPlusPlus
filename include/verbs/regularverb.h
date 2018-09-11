//
// Created by Attoa on 10/09/2018.
//

#ifndef ANCIENTGRAMMAR_REGULARVERB_H
#define ANCIENTGRAMMAR_REGULARVERB_H

#include <string>
#include <map>
#include <vector>

#include "verb.h"
#include "../utils.h"

namespace ancientgrammar {
    namespace verbs {
        class RegularVerb : public Verb {
        protected:
            std::string mPresent;
            std::string mFuture;
            std::string mAorist;
            std::string mAoristPassive;

            AoristType mAoristType;
            utils::ContractType mContract;

            const std::string mPreposition;

            const bool mUncommonEpsilonAugment;

            std::string getStem(Tense tense, Mood mood, Voice voice) const;
        public:
            explicit RegularVerb(const std::string &present = "", const std::string &future = "",
                        const std::string &aorist = "", const std::string &aoristPassive = "",
                        const std::string &preposition = "", bool uncommonEpsilonAugment = false,
                        const AllowedFormsMap &allowedForms = kAllFormsAllowed);

            ~RegularVerb();

            virtual std::string getFiniteForm(Tense tense, Mood mood, Voice voice, int person, bool isPlural,
                                              bool autocontract=true) const override;

            virtual std::string getImperative(Tense aspect, Voice voice, bool isPlural, bool autocontract=true) const override;

            virtual std::string getInfinitive(Tense tense, Voice voice, bool autocontract=true) const override;
        };
    }
}


#endif //ANCIENTGRAMMAR_REGULARVERB_H
