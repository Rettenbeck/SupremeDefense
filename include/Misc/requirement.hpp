#include <Util/basic.hpp>

#pragma once


namespace SupDef {

    class Requirement {
        public:
            virtual ~Requirement() = default;
            
    };
    
    using URequirement = std::unique_ptr<Requirement>;
    using Requirements = std::vector<URequirement>;

}
