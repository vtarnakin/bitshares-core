#pragma once

#include <graphene/protocol/dao.hpp>
#include <graphene/protocol/account.hpp>
#include <graphene/db/generic_index.hpp>
#include <graphene/chain/types.hpp>

#include <boost/multi_index/composite_key.hpp>

namespace graphene { namespace chain {

    struct dao_dapp_object : public graphene::db::abstract_object<dao_dapp_object>
    {
        static constexpr uint8_t space_id = protocol_ids;
        static constexpr uint8_t type_id  = dao_dapp_object_type;
        
        dao_id_type      dao_id;
        string           name;
        string           url;
        time_point_sec   created; 
        dao_dapp_options options;
    };
    
    struct by_dao_id;
    struct by_name;
    struct by_url;
    struct by_created;
    struct by_dao_id_and_name;

    using dao_dapp_object_multi_index_type = multi_index_container<
        dao_dapp_object,
        indexed_by<
            ordered_unique< tag<by_id>, member< object, object_id_type, &object::id > >,
            ordered_unique< 
                tag<by_dao_id>,
                member< dao_dapp_object, dao_id_type, &dao_dapp_object::dao_id >
            >,
            ordered_unique< 
                tag<by_name>,
                member< dao_dapp_object, string, &dao_dapp_object::name >
            >,
            ordered_unique< 
                tag<by_url>,
                member< dao_dapp_object, string, &dao_dapp_object::url >
            >,
            ordered_unique< 
                tag<by_created>,
                member< dao_dapp_object, time_point_sec, &dao_dapp_object::created >
            >,
            ordered_unique< 
                tag<by_dao_id_and_name>,
                composite_key<
                    dao_dapp_object,
                    member< dao_dapp_object, dao_id_type, &dao_dapp_object::dao_id >,
                    member< dao_dapp_object, string, &dao_dapp_object::name >
                >
            >
        >
    >;
    using dao_index = generic_index<dao_dapp_object, dao_dapp_object_multi_index_type>;
} }

MAP_OBJECT_ID_TO_TYPE(graphene::chain::dao_dapp_object)

FC_REFLECT_TYPENAME( graphene::chain::dao_dapp_object)

GRAPHENE_DECLARE_EXTERNAL_SERIALIZATION( graphene::chain::dao_dapp_object )