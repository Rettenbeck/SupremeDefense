#include <Util/basic.hpp>
#include <ECS/constants.hpp>

#pragma once


namespace SupDef {

    enum class DamageType {
        Normal, Fire
    };

    class Damage {
        public:
            long damage_amount = 0;
            DamageType damageType = DamageType::Normal;

            Damage() {}
            Damage(long damage_amount) : damage_amount(damage_amount) {}
            Damage(long damage_amount, DamageType damageType) : damage_amount(damage_amount), damageType(damageType) {}

            void to_json(json& j) const {
                j[SX_DAMAGE_AMOUNT] = damage_amount;
                j[SX_DAMAGE_TYPE  ] = damageType;
            }
            
            void from_json(const json& j) {
                damage_amount = j.at(SX_DAMAGE_AMOUNT).get<long>();
                damageType    = j.at(SX_DAMAGE_TYPE  ).get<DamageType>();
            }
        
    };
    
    using UDamage = std::unique_ptr<Damage>;
    using Damages = std::vector<UDamage>;

}
