#pragma once

#include <graphene/protocol/dao.hpp>
#include <graphene/db/generic_index.hpp>
#include <graphene/chain/types.hpp>

#include <boost/multi_index/composite_key.hpp>

namespace graphene { namespace chain {

    struct dao_object : public graphene::db::abstract_object<dao_object>
    {
      static constexpr uint8_t space_id = protocol_ids;
      static constexpr uint8_t type_id  = dao_object_type;
      account_id_type  owner_account_id;
      string           short_name;
      string           long_name;
      time_point_sec   created; 
      dao_options      options;
    };
    
    struct by_owner_account_id;
    struct by_short_name;
    struct by_long_name;
    struct by_created;

    using dao_object_multi_index_type = multi_index_container<
      dao_object,
      indexed_by<
        ordered_unique< tag<by_id>, member< object, object_id_type, &object::id > >,
        ordered_non_unique< 
          tag<by_owner_account_id>,
          member< dao_object, account_id_type, &dao_object::owner_account_id >
        >,
        ordered_unique< 
          tag<by_short_name>,
          member< dao_object, string, &dao_object::short_name >
        >,
        ordered_unique< 
          tag<by_long_name>,
          member< dao_object, string, &dao_object::long_name >
        >,
        ordered_non_unique< 
          tag<by_created>,
          member< dao_object, time_point_sec, &dao_object::created >
        >
      >
    >;
    using dao_index = generic_index<dao_object, dao_object_multi_index_type>;
} }
MAP_OBJECT_ID_TO_TYPE( graphene::chain::dao_object )
FC_REFLECT_TYPENAME( graphene::chain::dao_object)
GRAPHENE_DECLARE_EXTERNAL_SERIALIZATION( graphene::chain::dao_object )