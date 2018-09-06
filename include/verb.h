#ifndef ANCIENTGRAMMAR_VERB_H
#define ANCIENTGRAMMAR_VERB_H

#include <vector>
#include <string>
#include <map>

namespace ancientgrammar {
    namespace verbs {

        // Defines an enum for verb types (Regular does NOT include Deponent)
        enum class VerbType {
            REGULAR,
            DEPONENT,
            IRREGULAR
        };

        // Defines an enum for verb tenses
        enum class Tense {
            FUTURE,
            PRESENT,
            IMPERFECT,
            AORIST
        };

        // Defines an enum for verb mood
        enum class Mood {
            INDICATIVE,
            SUBJUNCTIVE,
            OPTATIVE,
            IMPERATIVE,
            INFINITIVE
        };

        // Defines an enum for verb voice
        enum class Voice {
            ACTIVE,
            MIDDLE,
            PASSIVE
        };

        const std::vector<Voice> kAllVoices = {Voice::ACTIVE, Voice::MIDDLE, Voice::PASSIVE};
        const std::map<Tense, std::vector<Voice>> kAllFormsAllowed = {
                {Tense::PRESENT, kAllVoices},
                {Tense::AORIST, kAllVoices},
                {Tense::FUTURE, kAllVoices},
                {Tense::IMPERFECT, kAllVoices}
        };

        // Defines an enum for Aorist Type (Weak/1st or Strong/2nd type aorists)
        enum class AoristType {
            WEAK,
            STRONG
        };

        // Interface class for verbs.
        class Verb {
        private:
            const std::map<Tense, std::vector<Voice>> mAllowedForms;

        protected:
            // Defines the order in which augments should be looked for to convert them
            static const std::vector<std::string> kAugmentOrder;
            // Defines the conversion between a string of characters and its augmented form
            static const std::map<std::string, std::string> kAugmentMap;

        public:
            Verb();
            ~Verb();
            //virtual bool canGetForm(Tense tense, Voice voice) = 0;
            //virtual std::string getFiniteForm(Tense tense, Mood mood, Voice voice, int person, bool isPlural, bool autocontract) = 0;
            //virtual std::string getImperative(Tense aspect, Voice voice, bool isPlural, bool autocontract) = 0;
            //virtual std::string getInfinitive(Tense tense, Voice voice, bool autocontract) = 0;
            // virtual Adjective getParticiple(Tense tense, Voice voice) = 0; TODO Make adjective a thing

            static std::string calculateAugment(std::string stem, bool uncommonEpsilon, std::string* preposition);
            static std::string calculateBreathing(std::string stem, std::string augment, size_t length, bool has_prepostion);
            static std::string calculateUnicode(std::string in, std::string mode);
        };

        // A namespace for a variety of helper functions not meant to really be used
        namespace detail {
            // Returns true if the tense can be described by aspect
            bool isTenseAspect(Tense tense);

            // Return true if the mood is finite
            bool isMoodFinite(Mood mood);
        }
    }
}



#endif