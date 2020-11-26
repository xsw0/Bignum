#include "test.h"

namespace TEST
{
    std::default_random_engine &random_engine()
    {
        static std::default_random_engine random_engine;
        return random_engine;
    }
} // namespace TEST
