#include <Util/basic.hpp>
#include <ECS/constants.hpp>

#pragma once


namespace SupDef {

    const long INFINITE_PIERCE = -1;

    enum class DamageType {
        Normal, Fire
    };

    DEFINE_SCLASS_BEGIN(Damage)
        long damage_amount = 0;
        long pierce = INFINITE_PIERCE;
        DamageType damageType = DamageType::Normal;

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

        REFLECT_COMPONENT_BEGIN(ThisType)
            REFLECT_FIELD(damage_amount)
            REFLECT_FIELD(damageType)
            REFLECT_FIELD(pierce)
        REFLECT_COMPONENT_END()
    DEFINE_SCLASS_END
    
    DEFINE_UNIQUE_AND_LIST(Damage, UDamage, Damages);

}
