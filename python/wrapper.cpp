//
// Created by Attoa on 06/09/2018.
//

#include <boost/python.hpp>

#include "../include/verb.h"
#include "../include/utils.h"

BOOST_PYTHON_MODULE(AncientGrammar) {
    using namespace boost::python;
    using namespace ancientgrammar::verbs;
    using namespace ancientgrammar::utils;
    class_<Verb>("Verb")
            .def("calculate_breathing", &Verb::calculateBreathing)
            .def("calculate_augment", &Verb::calculateAugment);

    def("remove_accents", &removeAccents);
}
