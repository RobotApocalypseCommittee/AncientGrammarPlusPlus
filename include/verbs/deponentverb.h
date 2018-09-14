//
// Created by Attoa on 14/09/2018.
//

#ifndef ANCIENTGRAMMAR_DEPONENTVERB_H
#define ANCIENTGRAMMAR_DEPONENTVERB_H

#include "regularverb.h"

namespace ancientgrammar {
    namespace verbs {
        class DeponentVerb : virtual public RegularVerb {
        public:
            explicit DeponentVerb(const std::string &present = "", const std::string &future = "",
                                 const std::string &aorist = "", const std::string &aoristPassive = "",
                                 const std::string &preposition = "", bool uncommonEpsilonAugment = false,
                                 const AllowedFormsMap &allowedForms = kAllFormsAllowed);

            ~DeponentVerb();
        };
    }
}


#endif //ANCIENTGRAMMAR_DEPONENTVERB_H
