/*#pragma once

#include <graphene/protocol/dao.hpp>
#include <graphene/db/generic_index.hpp>
#include <graphene/chain/types.hpp>

#include <boost/multi_index/composite_key.hpp>

namespace graphene { namespace chain {

    struct dao_file_object : public graphene::db::abstract_object<dao_object>
    {
        static constexpr uint8_t space_id = protocol_ids;
        static constexpr uint8_t type_id  = dao_object_type;
        
        dao_account_id_type dao_account_id;
        dao_dapp_id_type    dao_dapp_id;
        string              url;
        time_point_sec      created; 
        dao_file_options    options;
    };
    
    struct by_dao_account_id;
    struct by_url;
    struct by_created;

    using dao_file_object_multi_index_type = multi_index_container<
        dao_file_object,
        indexed_by<
            ordered_unique< tag<by_id>, member< object, object_id_type, &object::id > >,
            ordered_unique< 
                tag<by_dao_account_id>,
                member< dao_file_object, dao_account_id_type, &dao_file_object::dao_account_id >
            >,
            ordered_unique< 
                tag<by_name>,
                member< dao_file_object, string, &dao_file_object::url >
            >,
            ordered_unique< 
                tag<by_created>,
                member< dao_file_object, time_point_sec, &dao_file_object::created >
            >
        >
    >;
    using dao_index = generic_index<dao_file_object, dao_file_object_multi_index_type>;
} }

MAP_OBJECT_ID_TO_TYPE(graphene::chain::dao_file_object)

FC_REFLECT_TYPENAME( graphene::chain::dao_file_object)

GRAPHENE_DECLARE_EXTERNAL_SERIALIZATION( graphene::chain::dao_file_object )*/