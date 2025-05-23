#pragma once

#include <Util/util.hpp>
#include <ECS/field.hpp>
#include <ECS/constants.hpp>


namespace SupDef {

    class Component {
        public:
            virtual ~Component() = default;

            virtual void to_json(json& j) const = 0;
            virtual void from_json(const json& j) = 0;
            virtual std::string getTypeName() const = 0;
            virtual bool isAsset() const { return false; }
            virtual bool isTech() const { return false; }
            
    };

    using UComponent = std::unique_ptr<Component>;

}
