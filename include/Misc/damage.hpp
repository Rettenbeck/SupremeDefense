#include <Util/basic.hpp>
#include <ECS/constants.hpp>

#pragma once


namespace SupDef {

    const long INFINITE_PIERCE = -1;

    enum class DamageType {
        Normal, Fire
    };

    class Damage {
        public:
            long damage_amount = 0;
            long pierce = INFINITE_PIERCE;
            DamageType damageType = DamageType::Normal;

            Damage() {}
            Damage(long damage_amount) : damage_amount(damage_amount) {}
            Damage(long damage_amount, DamageType damageType) : damage_amount(damage_amount), damageType(damageType) {}
            
            Damage(long damage_amount, long pierce, DamageType damageType)
            : damage_amount(damage_amount), pierce(pierce), damageType(damageType) {}

            std::unique_ptr<Damage> clone() {
                auto clone = std::make_unique<Damage>();
                json j;
                to_json(j);
                clone->from_json(j);
                return std::move(clone);
            }

            void to_json(json& j) const {
                j[SX_DAMAGE_AMOUNT] = damage_amount;
                j[SX_DAMAGE_TYPE  ] = damageType;
                j[SX_PIERCE       ] = pierce;
            }
            
            void from_json(const json& j) {
                damage_amount = j.at(SX_DAMAGE_AMOUNT).get<long>();
                damageType    = j.at(SX_DAMAGE_TYPE  ).get<DamageType>();
                pierce        = j.at(SX_PIERCE       ).get<long>();
            }
        
    };
    
    using UDamage = std::unique_ptr<Damage>;
    using Damages = std::vector<UDamage>;

}
