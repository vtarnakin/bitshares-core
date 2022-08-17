#pragma once

#include <graphene/protocol/dao.hpp>
#include <graphene/protocol/account.hpp>
#include <graphene/db/generic_index.hpp>
#include <graphene/chain/types.hpp>

#include <boost/multi_index/composite_key.hpp>

namespace graphene { namespace chain {

    struct dao_account_object : public graphene::db::abstract_object<dao_account_object>
    {
        static constexpr uint8_t space_id = protocol_ids;
        static constexpr uint8_t type_id  = dao_account_object_type;
        
        account_id_type account_id;
        dao_id_type     dao_id;
        time_point_sec  created;
    };
    
    struct by_dao_id;
    struct by_account_id;
    struct by_created;
    struct by_dao_and_account_id;

    using dao_account_object_multi_index_type = multi_index_container<
        dao_account_object,
        indexed_by<
            ordered_unique< tag<by_id>, member< object, object_id_type, &object::id > >,
            ordered_unique< 
                tag<by_dao_id>,
                member< dao_account_object, dao_id_type, &dao_account_object::dao_id >
            >,
            ordered_unique< 
                tag<by_account_id>,
                member< dao_account_object, account_id_type, &dao_account_object::account_id >
            >,
            ordered_unique< 
                tag<by_created>,
                member< dao_account_object, time_point_sec, &dao_account_object::created >
            >,
            ordered_unique< 
                tag<by_dao_and_account_id>,
                composite_key<
                    dao_account_object,
                    member< dao_account_object, dao_id_type, &dao_account_object::dao_id >,
                    member< dao_account_object, account_id_type, &dao_account_object::account_id >
                >
            >
        >
    >;

    using dao_account_index = generic_index<dao_account_object, dao_account_object_multi_index_type>;
} }

MAP_OBJECT_ID_TO_TYPE(graphene::chain::dao_account_object)

FC_REFLECT_TYPENAME( graphene::chain::dao_account_object )

GRAPHENE_DECLARE_EXTERNAL_SERIALIZATION( graphene::chain::dao_account_object )