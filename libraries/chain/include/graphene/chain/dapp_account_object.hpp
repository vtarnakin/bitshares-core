#pragma once

#include <graphene/protocol/dapp.hpp>
#include <graphene/protocol/account.hpp>
#include <graphene/db/generic_index.hpp>
#include <graphene/chain/types.hpp>

#include <boost/multi_index/composite_key.hpp>

namespace graphene { namespace chain {

    struct dapp_account_object : public graphene::db::abstract_object<dapp_account_object>
    {
        static constexpr uint8_t space_id = protocol_ids;
        static constexpr uint8_t type_id  = dapp_account_object_type;
        
        account_id_type account_id;
        dapp_id_type    dapp_id;
        time_point_sec  created;
    };
    
    struct by_dapp_id;
    struct by_account_id;
    struct by_created;
    struct by_dapp_and_account_id;

    using dapp_account_object_multi_index_type = multi_index_container<
        dapp_account_object,
        indexed_by<
            ordered_unique< tag<by_id>, member< object, object_id_type, &object::id > >,
            ordered_unique< 
                tag<by_dapp_id>,
                member< dapp_account_object, dapp_id_type, &dapp_account_object::dapp_id >
            >,
            ordered_unique< 
                tag<by_account_id>,
                member< dapp_account_object, account_id_type, &dapp_account_object::account_id >
            >,
            ordered_unique< 
                tag<by_created>,
                member< dapp_account_object, time_point_sec, &dapp_account_object::created >
            >,
            ordered_unique< 
                tag<by_dapp_and_account_id>,
                composite_key<
                    dapp_account_object,
                    member< dapp_account_object, dapp_id_type, &dapp_account_object::dapp_id >,
                    member< dapp_account_object, account_id_type, &dapp_account_object::account_id >
                >
            >
        >
    >;

    using dapp_account_index = generic_index<dapp_account_object, dapp_account_object_multi_index_type>;
} }

MAP_OBJECT_ID_TO_TYPE(graphene::chain::dapp_account_object)

FC_REFLECT_TYPENAME( graphene::chain::dapp_account_object )

GRAPHENE_DECLARE_EXTERNAL_SERIALIZATION( graphene::chain::dapp_account_object )