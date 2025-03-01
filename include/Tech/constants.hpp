#include <Util/basic.hpp>

#pragma once


namespace SupDef {
    
    using TechID = uint32_t;
    using TechIDs = std::vector<TechID>;
    const TechID NO_TECH_ID = 0;

    const std::string ST_ID                                 = "id"                                  ;
    const std::string ST_DATA                               = "data"                                ;
    const std::string ST_TECHS                              = "techs"                               ;
    const std::string ST_OWNERS                             = "owners"                              ;
    const std::string ST_ASSIGNEES                          = "assignees"                           ;
    const std::string ST_NEXT_TECH_ID                       = "next_tech_id"                        ;
    const std::string ST_TECH_TYPE                          = "tech_type"                           ;
    const std::string ST_SOURCES                            = "sources"                             ;
    const std::string ST_DEPENDENTS                         = "dependents"                          ;
    const std::string ST_DEPENDENCY_TYPE                    = "dependency_type"                     ;

}
