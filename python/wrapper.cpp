//
// Created by Attoa on 06/09/2018.
//

#include <boost/python.hpp>

#include "../include/verb.h"

BOOST_PYTHON_MODULE(AncientGrammar) {
    using namespace boost::python;
    using namespace ancientgrammar::verbs;
    class_<Verb>("Verb")
            .def("calculate_breathing", &Verb::calculateBreathing);
}
