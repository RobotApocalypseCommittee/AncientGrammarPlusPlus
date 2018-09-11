//
// Created by Attoa on 06/09/2018.
//

#include <boost/python.hpp>

#include "../include/verbs/regularverb.h"
#include "../include/verbs/verb.h"
#include "../include/utils.h"
#include "../include/json.h"

std::string getVT() {
    return ancientgrammar::verbs::detail::kVerbTable.dump(4);
}

BOOST_PYTHON_FUNCTION_OVERLOADS(regularVerbConstructorOverloads, RegularVerb::RegularVerb, 0, 7)

BOOST_PYTHON_MODULE(AncientGrammar) {
    using namespace boost::python;
    using namespace ancientgrammar::verbs;
    using namespace ancientgrammar::utils;
    class_<RegularVerb>("RegularVerb")
            .def(init<std::string>())
            .def(init<std::string, std::string>())
            .def(init<std::string, std::string, std::string>())
            .def(init<std::string, std::string, std::string, std::string>())
            .def(init<std::string, std::string, std::string, std::string, std::string>())
            .def(init<std::string, std::string, std::string, std::string, std::string, bool>())
            //.def(init<std::string, std::string, std::string, std::string, std::string, bool, AllowedFormsMap>())
            .def("get_finite_form", &RegularVerb::getFiniteForm);

    enum_<Tense>("Tense")
        .value("FUTURE", Tense::FUTURE)
        .value("PRESENT", Tense::PRESENT)
        .value("IMPERFECT", Tense::IMPERFECT)
        .value("AORIST", Tense::AORIST);

    enum_<Mood>("Mood")
        .value("INDICATIVE", Mood::INDICATIVE)
        .value("SUBJUNCTIVE", Mood::SUBJUNCTIVE)
        .value("OPTATIVE", Mood::OPTATIVE)
        .value("IMPERATIVE", Mood::IMPERATIVE)
        .value("INFINITIVE", Mood::INFINITIVE);

    enum_<Voice>("Voice")
        .value("ACTIVE", Voice::ACTIVE)
        .value("MIDDLE", Voice::MIDDLE)
        .value("PASSIVE", Voice::PASSIVE);

    def("remove_accents", &removeAccents);

    def("get_verb_table", &getVT);
}
