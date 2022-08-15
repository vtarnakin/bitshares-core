#pragma once

#include <graphene/protocol/dapp.hpp>
#include <graphene/protocol/account.hpp>
#include <graphene/db/generic_index.hpp>
#include <graphene/chain/types.hpp>

#include <boost/multi_index/composite_key.hpp>

namespace graphene { namespace chain {

    struct dapp_object : public graphene::db::abstract_object<dapp_object>
    {
        static constexpr uint8_t space_id = protocol_ids;
        static constexpr uint8_t type_id  = dapp_object_type;
        
        account_id_type owner_account_id;
        string          name;
        time_point_sec  created;
        dapp_options    options;
    };
    
    struct by_owner_account_id;
    struct by_name;
    struct by_created;

    using dapp_object_multi_index_type = multi_index_container<
        dapp_object,
        indexed_by<
            ordered_unique< tag<by_id>, member< object, object_id_type, &object::id > >,
            ordered_unique< 
                tag<by_owner_account_id>,
                member< dapp_object, account_id_type, &dapp_object::owner_account_id >
            >,
            ordered_unique< 
                tag<by_name>,
                member< dapp_object, string, &dapp_object::name >
            >,
            ordered_unique< 
                tag<by_created>,
                member< dapp_object, time_point_sec, &dapp_object::created >
            >
        >
    >;
    using dapp_index = generic_index<dapp_object, dapp_object_multi_index_type>;
} }

MAP_OBJECT_ID_TO_TYPE(graphene::chain::dapp_object)

FC_REFLECT_TYPENAME( graphene::chain::dapp_object)

GRAPHENE_DECLARE_EXTERNAL_SERIALIZATION( graphene::chain::dapp_object )