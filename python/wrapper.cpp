//
// Created by Attoa on 06/09/2018.
//

#include <boost/python.hpp>

#include "../include/verbs/verb.h"
#include "../include/utils.h"
#include "../include/json.h"

std::string calculateAugment(std::string stem, bool uncommonEpsilon, const std::string preposition = "") {
    return ancientgrammar::verbs::Verb::calculateAugment(stem, uncommonEpsilon, &preposition);
}

std::string getVT() {
    return ancientgrammar::verbs::detail::kVerbTable.dump(4);
}

BOOST_PYTHON_MODULE(AncientGrammar) {
    using namespace boost::python;
    using namespace ancientgrammar::verbs;
    using namespace ancientgrammar::utils;
    class_<Verb>("Verb")
            .def("calculate_breathing", &Verb::calculateBreathing)
            .def("calculate_augment", &calculateAugment)
            .def("calculate_augment", &Verb::calculateAugment);

    def("remove_accents", &removeAccents);

    def("get_verb_table", &getVT);
}
