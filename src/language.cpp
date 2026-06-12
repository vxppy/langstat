#include "languages.h"

const Language get_language_from_ext(std::string_view ext) {
    for(const ExtEntry& entry: ext_map) {
         if(entry.ext == ext) {
             return entry.language;
         }
    }

    return Language::Unknown;
}
