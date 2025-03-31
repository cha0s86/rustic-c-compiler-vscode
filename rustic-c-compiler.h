#pragma once
#include <string>

union pools {

    struct charpool {
        public:
            std::string charpool[128][64];
    };

    struct keywordpool {
        public:
            std::string keywordpool[128][64];
    };

    struct compiledobject {
        public:
            std::string compiledstring[128][64];
    };
};
