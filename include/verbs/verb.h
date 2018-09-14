#ifndef ANCIENTGRAMMAR_VERB_H
#define ANCIENTGRAMMAR_VERB_H

#include <vector>
#include <string>
#include <map>

#include "../json.h"

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
        // Code for mapping between the enum values and their name
        const std::string tenseNames[4] = {"FUTURE", "PRESENT", "IMPERFECT", "AORIST"};

        // Defines an enum for verb mood
        enum class Mood {
            INDICATIVE,
            SUBJUNCTIVE,
            OPTATIVE,
            IMPERATIVE,
            INFINITIVE
        };
        // Code for mapping between the enum values and their name
        const std::string moodNames[5] = {"INDICATIVE", "SUBJUNCTIVE", "OPTATIVE", "IMPERATIVE", "INFINITIVE"};

        // Defines an enum for verb voice
        enum class Voice {
            ACTIVE,
            MIDDLE,
            PASSIVE
        };
        // Code for mapping between the enum values and their name
        const std::string voiceNames[3] = {"ACTIVE", "MIDDLE", "PASSIVE"};

        // Defines an enum for Aorist Type (Weak/1st or Strong/2nd type aorists)
        enum class AoristType {
            WEAK,
            STRONG
        };
        // Code for mapping between the enum values and their name
        const std::string aoristTypeNames[2] = {"WEAK", "STRONG"};

        typedef std::map<Tense, std::vector<Voice>> AllowedFormsMap;

        // Defines the constant values for allowed forms. If within the tense a voice is allowed, then that combo of
        // voice and tense are allowed
        const std::vector<Voice> kAllVoices = {Voice::ACTIVE, Voice::MIDDLE, Voice::PASSIVE};
        const AllowedFormsMap kAllFormsAllowed = {
                {Tense::PRESENT,   kAllVoices},
                {Tense::AORIST,    kAllVoices},
                {Tense::FUTURE,    kAllVoices},
                {Tense::IMPERFECT, kAllVoices}
        };

        // Base class for verbs.
        class Verb {
        protected:
            // Defines the order in which augments should be looked for to convert them
            static const std::vector<std::string> kAugmentOrder;
            // Defines the conversion between a string of characters and its augmented form
            static const std::map<std::string, std::string> kAugmentMap;

            // Defines which forms are allowed with this specific verb
            AllowedFormsMap mAllowedForms;

            // Declares some common variables that are constant and need to be set from all base classes
            // As subclasses cannot directly initialise base const members
            const std::string mPreposition;
            const bool mUncommonEpsilonAugment;

            bool canGetForm(Tense tense, Voice voice) const;

            // Calculates and applies the correct augment for a stem and returns the new stem.
            // Uncommon epsilon augment refers to the few (but not unique) verbs which,
            // starting with ε, instead of the temporal augment lengthening it to η, it instead
            // becomes ει (like εχω).
            // Preposition is an optional argument to tell where in the string to apply the augment - if this is set,
            // BOTH stem and preposition should contain the preposition.
            static std::string calculateAugment(const std::string &stem, bool uncommonEpsilon,
                                                const std::string &preposition);

            // Chooses and applies the correct breathing to use based on stem, augment, length of augment, and
            static std::string calculateBreathing(std::string stem, std::string augment, unsigned long long int length,
                                                  bool hasPreposition);

            // TODO undo commenting out of virtuals
            explicit Verb(const std::string &preposition = "", bool uncommonEpsilonAugment = false);
            ~Verb();
        public:
            virtual std::string getFiniteForm(Tense tense, Mood mood, Voice voice, int person, bool isPlural,
                    bool autocontract=true) const = 0;
            virtual std::string getImperative(Tense aspect, Voice voice, bool isPlural,
                    bool autocontract=true) const = 0;
            virtual std::string getInfinitive(Tense tense, Voice voice,
                    bool autocontract=true) const = 0;
            //virtual Adjective getParticiple(Tense tense, Voice voice) const = 0; TODO Make adjective a thing
        };

        /* todo after irreg implemented
        // Function to get the correct verb type
        Verb& getVerb(const std::string &present = "", const std::string &future = "",
                     const std::string &aorist = "", const std::string &aoristPassive = "",
                     const std::string &preposition = "", bool uncommonEpsilonAugment = false,
                     const AllowedFormsMap &allowedForms = kAllFormsAllowed);
        */

        // A namespace for a variety of helper functions not meant to really be used
        namespace detail {
            // Returns true if the tense can be described by aspect
            bool isTenseAspect(Tense tense);

            // Return true if the mood is finite
            bool isMoodFinite(Mood mood);

            // REALLY LONG json string for verb tables. Please ignore
            const nlohmann::json kVerbTable =  nlohmann::json::parse("{\n"
                                               "    \"PRESENT\":{\n"
                                               "        \"INDICATIVE\":{\n"
                                               "            \"ACTIVE\":{\n"
                                               "                \"1\":{\n"
                                               "                    \"False\":\"ω\",\n"
                                               "                    \"True\":\"ομεν\"\n"
                                               "                },\n"
                                               "                \"2\":{\n"
                                               "                    \"False\":\"εις\",\n"
                                               "                    \"True\":\"ετε\"\n"
                                               "                },\n"
                                               "                \"3\":{\n"
                                               "                    \"False\":\"ει\",\n"
                                               "                    \"True\":\"ουσι\"\n"
                                               "                }\n"
                                               "            },\n"
                                               "            \"MIDDLE\":{\n"
                                               "                \"1\":{\n"
                                               "                    \"False\":\"ομαι\",\n"
                                               "                    \"True\":\"ομεθα\"\n"
                                               "                },\n"
                                               "                \"2\":{\n"
                                               "                    \"False\":\"ῃ\",\n"
                                               "                    \"True\":\"εσθε\"\n"
                                               "                },\n"
                                               "                \"3\":{\n"
                                               "                    \"False\":\"εται\",\n"
                                               "                    \"True\":\"ονται\"\n"
                                               "                }\n"
                                               "            },\n"
                                               "            \"PASSIVE\":{\n"
                                               "                \"1\":{\n"
                                               "                    \"False\":\"ομαι\",\n"
                                               "                    \"True\":\"ομεθα\"\n"
                                               "                },\n"
                                               "                \"2\":{\n"
                                               "                    \"False\":\"ει\",\n"
                                               "                    \"True\":\"εσθε\"\n"
                                               "                },\n"
                                               "                \"3\":{\n"
                                               "                    \"False\":\"εται\",\n"
                                               "                    \"True\":\"ονται\"\n"
                                               "                }\n"
                                               "            }\n"
                                               "        },\n"
                                               "        \"SUBJUNCTIVE\":{\n"
                                               "            \"ACTIVE\":{\n"
                                               "                \"1\":{\n"
                                               "                    \"False\":\"ω\",\n"
                                               "                    \"True\":\"ωμεν\"\n"
                                               "                },\n"
                                               "                \"2\":{\n"
                                               "                    \"False\":\"ῃς\",\n"
                                               "                    \"True\":\"ητε\"\n"
                                               "                },\n"
                                               "                \"3\":{\n"
                                               "                    \"False\":\"ῃ\",\n"
                                               "                    \"True\":\"ωσι\"\n"
                                               "                }\n"
                                               "            },\n"
                                               "            \"MIDDLE\":{\n"
                                               "                \"1\":{\n"
                                               "                    \"False\":\"ωμαι\",\n"
                                               "                    \"True\":\"ωμεθα\"\n"
                                               "                },\n"
                                               "                \"2\":{\n"
                                               "                    \"False\":\"ῃ\",\n"
                                               "                    \"True\":\"ησθε\"\n"
                                               "                },\n"
                                               "                \"3\":{\n"
                                               "                    \"False\":\"ηται\",\n"
                                               "                    \"True\":\"ωνται\"\n"
                                               "                }\n"
                                               "            },\n"
                                               "            \"PASSIVE\":{\n"
                                               "                \"1\":{\n"
                                               "                    \"False\":\"ωμαι\",\n"
                                               "                    \"True\":\"ωμεθα\"\n"
                                               "                },\n"
                                               "                \"2\":{\n"
                                               "                    \"False\":\"ῃ\",\n"
                                               "                    \"True\":\"ησθε\"\n"
                                               "                },\n"
                                               "                \"3\":{\n"
                                               "                    \"False\":\"ηται\",\n"
                                               "                    \"True\":\"ωνται\"\n"
                                               "                }\n"
                                               "            }\n"
                                               "        },\n"
                                               "        \"OPTATIVE\":{\n"
                                               "            \"ACTIVE\":{\n"
                                               "                \"1\":{\n"
                                               "                    \"False\":{\n"
                                               "                        \"False\":\"οιμι\",\n"
                                               "                        \"True\":\"οιην\"\n"
                                               "                    },\n"
                                               "                    \"True\":\"οιμεν\"\n"
                                               "                },\n"
                                               "                \"2\":{\n"
                                               "                    \"False\":{\n"
                                               "                        \"False\":\"οις\",\n"
                                               "                        \"True\":\"οιης\"\n"
                                               "                    },\n"
                                               "                    \"True\":\"οιτε\"\n"
                                               "                },\n"
                                               "                \"3\":{\n"
                                               "                    \"False\":{\n"
                                               "                        \"False\":\"οι\",\n"
                                               "                        \"True\":\"οιη\"\n"
                                               "                    },\n"
                                               "                    \"True\":\"οιεν\"\n"
                                               "                }\n"
                                               "            },\n"
                                               "            \"MIDDLE\":{\n"
                                               "                \"1\":{\n"
                                               "                    \"False\":\"οιμην\",\n"
                                               "                    \"True\":\"οιμεθα\"\n"
                                               "                },\n"
                                               "                \"2\":{\n"
                                               "                    \"False\":\"οιο\",\n"
                                               "                    \"True\":\"οισθε\"\n"
                                               "                },\n"
                                               "                \"3\":{\n"
                                               "                    \"False\":\"οιτο\",\n"
                                               "                    \"True\":\"οιντο\"\n"
                                               "                }\n"
                                               "            },\n"
                                               "            \"PASSIVE\":{\n"
                                               "                \"1\":{\n"
                                               "                    \"False\":\"οιμην\",\n"
                                               "                    \"True\":\"οιμεθα\"\n"
                                               "                },\n"
                                               "                \"2\":{\n"
                                               "                    \"False\":\"οιο\",\n"
                                               "                    \"True\":\"οισθε\"\n"
                                               "                },\n"
                                               "                \"3\":{\n"
                                               "                    \"False\":\"οιτο\",\n"
                                               "                    \"True\":\"οιντο\"\n"
                                               "                }\n"
                                               "            }\n"
                                               "        },\n"
                                               "        \"IMPERATIVE\":{\n"
                                               "            \"ACTIVE\":{\n"
                                               "                \"2\":{\n"
                                               "                    \"False\":\"ε\",\n"
                                               "                    \"True\":\"ετε\"\n"
                                               "                }\n"
                                               "            },\n"
                                               "            \"MIDDLE\":{\n"
                                               "                \"2\":{\n"
                                               "                    \"False\":\"ου\",\n"
                                               "                    \"True\":\"εσθε\"\n"
                                               "                }\n"
                                               "            },\n"
                                               "            \"PASSIVE\":{\n"
                                               "                \"2\":{\n"
                                               "                    \"False\":\"ου\",\n"
                                               "                    \"True\":\"εσθε\"\n"
                                               "                }\n"
                                               "            }\n"
                                               "        },\n"
                                               "        \"INFINITIVE\":{\n"
                                               "            \"ACTIVE\":\"ειν\",\n"
                                               "            \"MIDDLE\":\"εσθαι\",\n"
                                               "            \"PASSIVE\":\"εσθαι\"\n"
                                               "        }\n"
                                               "    },\n"
                                               "    \"IMPERFECT\":{\n"
                                               "        \"INDICATIVE\":{\n"
                                               "            \"ACTIVE\":{\n"
                                               "                \"1\":{\n"
                                               "                    \"False\":\"ον\",\n"
                                               "                    \"True\":\"ομεν\"\n"
                                               "                },\n"
                                               "                \"2\":{\n"
                                               "                    \"False\":\"ες\",\n"
                                               "                    \"True\":\"ετε\"\n"
                                               "                },\n"
                                               "                \"3\":{\n"
                                               "                    \"False\":\"ε\",\n"
                                               "                    \"True\":\"ον\"\n"
                                               "                }\n"
                                               "            },\n"
                                               "            \"MIDDLE\":{\n"
                                               "                \"1\":{\n"
                                               "                    \"False\":\"ομην\",\n"
                                               "                    \"True\":\"ομεθα\"\n"
                                               "                },\n"
                                               "                \"2\":{\n"
                                               "                    \"False\":\"ου\",\n"
                                               "                    \"True\":\"εσθε\"\n"
                                               "                },\n"
                                               "                \"3\":{\n"
                                               "                    \"False\":\"ετο\",\n"
                                               "                    \"True\":\"οντο\"\n"
                                               "                }\n"
                                               "            },\n"
                                               "            \"PASSIVE\":{\n"
                                               "                \"1\":{\n"
                                               "                    \"False\":\"ομην\",\n"
                                               "                    \"True\":\"ομεθα\"\n"
                                               "                },\n"
                                               "                \"2\":{\n"
                                               "                    \"False\":\"ου\",\n"
                                               "                    \"True\":\"εσθε\"\n"
                                               "                },\n"
                                               "                \"3\":{\n"
                                               "                    \"False\":\"ετο\",\n"
                                               "                    \"True\":\"οντο\"\n"
                                               "                }\n"
                                               "            }\n"
                                               "        }\n"
                                               "    },\n"
                                               "    \"AORIST\":{\n"
                                               "        \"INDICATIVE\":{\n"
                                               "            \"ACTIVE\":{\n"
                                               "                \"WEAK\":{\n"
                                               "                    \"1\":{\n"
                                               "                        \"False\":\"α\",\n"
                                               "                        \"True\":\"αμεν\"\n"
                                               "                    },\n"
                                               "                    \"2\":{\n"
                                               "                        \"False\":\"ας\",\n"
                                               "                        \"True\":\"ατε\"\n"
                                               "                    },\n"
                                               "                    \"3\":{\n"
                                               "                        \"False\":\"ε\",\n"
                                               "                        \"True\":\"αν\"\n"
                                               "                    }\n"
                                               "                },\n"
                                               "                \"STRONG\":{\n"
                                               "                    \"1\":{\n"
                                               "                        \"False\":\"ον\",\n"
                                               "                        \"True\":\"ομεν\"\n"
                                               "                    },\n"
                                               "                    \"2\":{\n"
                                               "                        \"False\":\"ες\",\n"
                                               "                        \"True\":\"ετε\"\n"
                                               "                    },\n"
                                               "                    \"3\":{\n"
                                               "                        \"False\":\"ε\",\n"
                                               "                        \"True\":\"ον\"\n"
                                               "                    }\n"
                                               "                }\n"
                                               "            },\n"
                                               "            \"MIDDLE\":{\n"
                                               "                \"WEAK\":{\n"
                                               "                    \"1\":{\n"
                                               "                        \"False\":\"αμεν\",\n"
                                               "                        \"True\":\"αμεθα\"\n"
                                               "                    },\n"
                                               "                    \"2\":{\n"
                                               "                        \"False\":\"ω\",\n"
                                               "                        \"True\":\"ασθε\"\n"
                                               "                    },\n"
                                               "                    \"3\":{\n"
                                               "                        \"False\":\"ατο\",\n"
                                               "                        \"True\":\"αντο\"\n"
                                               "                    }\n"
                                               "                },\n"
                                               "                \"STRONG\":{\n"
                                               "                    \"1\":{\n"
                                               "                        \"False\":\"ομην\",\n"
                                               "                        \"True\":\"ομεθα\"\n"
                                               "                    },\n"
                                               "                    \"2\":{\n"
                                               "                        \"False\":\"ου\",\n"
                                               "                        \"True\":\"εσθε\"\n"
                                               "                    },\n"
                                               "                    \"3\":{\n"
                                               "                        \"False\":\"ετο\",\n"
                                               "                        \"True\":\"οντο\"\n"
                                               "                    }\n"
                                               "                }\n"
                                               "            },\n"
                                               "            \"PASSIVE\":{\n"
                                               "                \"WEAK\":{\n"
                                               "                    \"1\":{\n"
                                               "                        \"False\":\"ην\",\n"
                                               "                        \"True\":\"ης\"\n"
                                               "                    },\n"
                                               "                    \"2\":{\n"
                                               "                        \"False\":\"ης\",\n"
                                               "                        \"True\":\"ητε\"\n"
                                               "                    },\n"
                                               "                    \"3\":{\n"
                                               "                        \"False\":\"η\",\n"
                                               "                        \"True\":\"ησαν\"\n"
                                               "                    }\n"
                                               "                }\n"
                                               "            }\n"
                                               "        },\n"
                                               "        \"SUBJUNCTIVE\":{\n"
                                               "            \"ACTIVE\":{\n"
                                               "                \"WEAK\":{\n"
                                               "                    \"1\":{\n"
                                               "                        \"False\":\"ω\",\n"
                                               "                        \"True\":\"ωμεν\"\n"
                                               "                    },\n"
                                               "                    \"2\":{\n"
                                               "                        \"False\":\"ῃς\",\n"
                                               "                        \"True\":\"ητε\"\n"
                                               "                    },\n"
                                               "                    \"3\":{\n"
                                               "                        \"False\":\"ῃ\",\n"
                                               "                        \"True\":\"ωσι\"\n"
                                               "                    }\n"
                                               "                },\n"
                                               "                \"STRONG\":{\n"
                                               "                    \"1\":{\n"
                                               "                        \"False\":\"ω\",\n"
                                               "                        \"True\":\"ωμεν\"\n"
                                               "                    },\n"
                                               "                    \"2\":{\n"
                                               "                        \"False\":\"ῃς\",\n"
                                               "                        \"True\":\"ητε\"\n"
                                               "                    },\n"
                                               "                    \"3\":{\n"
                                               "                        \"False\":\"ῃ\",\n"
                                               "                        \"True\":\"ωσι\"\n"
                                               "                    }\n"
                                               "                }\n"
                                               "            },\n"
                                               "            \"MIDDLE\":{\n"
                                               "                \"WEAK\":{\n"
                                               "                    \"1\":{\n"
                                               "                        \"False\":\"ωμαι\",\n"
                                               "                        \"True\":\"ωμεθα\"\n"
                                               "                    },\n"
                                               "                    \"2\":{\n"
                                               "                        \"False\":\"ῃ\",\n"
                                               "                        \"True\":\"ησθε\"\n"
                                               "                    },\n"
                                               "                    \"3\":{\n"
                                               "                        \"False\":\"ηται\",\n"
                                               "                        \"True\":\"ωνται\"\n"
                                               "                    }\n"
                                               "                },\n"
                                               "                \"STRONG\":{\n"
                                               "                    \"1\":{\n"
                                               "                        \"False\":\"ωμαι\",\n"
                                               "                        \"True\":\"ωμεθα\"\n"
                                               "                    },\n"
                                               "                    \"2\":{\n"
                                               "                        \"False\":\"ῃ\",\n"
                                               "                        \"True\":\"ησθε\"\n"
                                               "                    },\n"
                                               "                    \"3\":{\n"
                                               "                        \"False\":\"ηται\",\n"
                                               "                        \"True\":\"ωνται\"\n"
                                               "                    }\n"
                                               "                }\n"
                                               "            },\n"
                                               "            \"PASSIVE\":{\n"
                                               "                \"WEAK\":{\n"
                                               "                    \"1\":{\n"
                                               "                        \"False\":\"ω\",\n"
                                               "                        \"True\":\"ωμεν\"\n"
                                               "                    },\n"
                                               "                    \"2\":{\n"
                                               "                        \"False\":\"ῃς\",\n"
                                               "                        \"True\":\"ητε\"\n"
                                               "                    },\n"
                                               "                    \"3\":{\n"
                                               "                        \"False\":\"ῃ\",\n"
                                               "                        \"True\":\"ωσι\"\n"
                                               "                    }\n"
                                               "                }\n"
                                               "            }\n"
                                               "        },\n"
                                               "        \"OPTATIVE\":{\n"
                                               "            \"ACTIVE\":{\n"
                                               "                \"WEAK\":{\n"
                                               "                    \"1\":{\n"
                                               "                        \"False\":\"αιμι\",\n"
                                               "                        \"True\":\"αιμεν\"\n"
                                               "                    },\n"
                                               "                    \"2\":{\n"
                                               "                        \"False\":\"ειας\",\n"
                                               "                        \"True\":\"αιτε\"\n"
                                               "                    },\n"
                                               "                    \"3\":{\n"
                                               "                        \"False\":\"ειε\",\n"
                                               "                        \"True\":\"ειαν\"\n"
                                               "                    }\n"
                                               "                },\n"
                                               "                \"STRONG\":{\n"
                                               "                    \"1\":{\n"
                                               "                        \"False\":\"οιμι\",\n"
                                               "                        \"True\":\"οιμεν\"\n"
                                               "                    },\n"
                                               "                    \"2\":{\n"
                                               "                        \"False\":\"οις\",\n"
                                               "                        \"True\":\"οιτε\"\n"
                                               "                    },\n"
                                               "                    \"3\":{\n"
                                               "                        \"False\":\"οι\",\n"
                                               "                        \"True\":\"οιεν\"\n"
                                               "                    }\n"
                                               "                }\n"
                                               "            },\n"
                                               "            \"MIDDLE\":{\n"
                                               "                \"WEAK\":{\n"
                                               "                    \"1\":{\n"
                                               "                        \"False\":\"αιμην\",\n"
                                               "                        \"True\":\"αιμεθα\"\n"
                                               "                    },\n"
                                               "                    \"2\":{\n"
                                               "                        \"False\":\"αιο\",\n"
                                               "                        \"True\":\"αισθε\"\n"
                                               "                    },\n"
                                               "                    \"3\":{\n"
                                               "                        \"False\":\"αιτο\",\n"
                                               "                        \"True\":\"αιντο\"\n"
                                               "                    }\n"
                                               "                },\n"
                                               "                \"STRONG\":{\n"
                                               "                    \"1\":{\n"
                                               "                        \"False\":\"οιμην\",\n"
                                               "                        \"True\":\"οιμεθα\"\n"
                                               "                    },\n"
                                               "                    \"2\":{\n"
                                               "                        \"False\":\"οιο\",\n"
                                               "                        \"True\":\"οισθε\"\n"
                                               "                    },\n"
                                               "                    \"3\":{\n"
                                               "                        \"False\":\"οιτο\",\n"
                                               "                        \"True\":\"οιντο\"\n"
                                               "                    }\n"
                                               "                }\n"
                                               "            },\n"
                                               "            \"PASSIVE\":{\n"
                                               "                \"WEAK\":{\n"
                                               "                    \"1\":{\n"
                                               "                        \"False\":\"ειην\",\n"
                                               "                        \"True\":\"ειμεν\"\n"
                                               "                    },\n"
                                               "                    \"2\":{\n"
                                               "                        \"False\":\"ειης\",\n"
                                               "                        \"True\":\"ειτε\"\n"
                                               "                    },\n"
                                               "                    \"3\":{\n"
                                               "                        \"False\":\"ειη\",\n"
                                               "                        \"True\":\"ειεν\"\n"
                                               "                    }\n"
                                               "                }\n"
                                               "            }\n"
                                               "        },\n"
                                               "        \"IMPERATIVE\":{\n"
                                               "            \"ACTIVE\":{\n"
                                               "                \"WEAK\":{\n"
                                               "                    \"2\":{\n"
                                               "                        \"False\":\"ον\",\n"
                                               "                        \"True\":\"ατε\"\n"
                                               "                    }\n"
                                               "                },\n"
                                               "                \"STRONG\":{\n"
                                               "                    \"2\":{\n"
                                               "                        \"False\":\"ε\",\n"
                                               "                        \"True\":\"ετε\"\n"
                                               "                    }\n"
                                               "                }\n"
                                               "            },\n"
                                               "            \"MIDDLE\":{\n"
                                               "                \"WEAK\":{\n"
                                               "                    \"2\":{\n"
                                               "                        \"False\":\"αι\",\n"
                                               "                        \"True\":\"ασθε\"\n"
                                               "                    }\n"
                                               "                },\n"
                                               "                \"STRONG\":{\n"
                                               "                    \"2\":{\n"
                                               "                        \"False\":\"ου\",\n"
                                               "                        \"True\":\"εσθε\"\n"
                                               "                    }\n"
                                               "                }\n"
                                               "            },\n"
                                               "            \"PASSIVE\":{\n"
                                               "                \"WEAK\":{\n"
                                               "                    \"2\":{\n"
                                               "                        \"False\":\"ητι\",\n"
                                               "                        \"True\":\"ητε\"\n"
                                               "                    }\n"
                                               "                }\n"
                                               "            }\n"
                                               "        },\n"
                                               "        \"INFINITIVE\":{\n"
                                               "            \"ACTIVE\":{\n"
                                               "                \"WEAK\":\"αι\",\n"
                                               "                \"STRONG\":\"ειν\"\n"
                                               "            },\n"
                                               "            \"MIDDLE\":{\n"
                                               "                \"WEAK\":\"ασθαι\",\n"
                                               "                \"STRONG\":\"εσθαι\"\n"
                                               "            },\n"
                                               "            \"PASSIVE\":{\n"
                                               "                \"WEAK\":\"ηναι\"\n"
                                               "            }\n"
                                               "        }\n"
                                               "    }\n"
                                               "}");
        }
    }
}


#endif