#pragma once

#include <graphene/protocol/dao.hpp>
#include <graphene/db/generic_index.hpp>
#include <graphene/chain/types.hpp>

#include <boost/multi_index/composite_key.hpp>

namespace graphene { namespace chain {

    struct dao_project_object : public graphene::db::abstract_object<dao_project_object>
    {
      static constexpr uint8_t space_id = protocol_ids;
      static constexpr uint8_t type_id  = dao_project_object_type;
      dao_id_type         dao_id;
      string              short_name;
      string              long_name;
      string              url;
      time_point_sec      created; 
      dao_project_options options;
    };
    
    struct by_dao_id;
    struct by_short_name;
    struct by_long_name;
    struct by_url;
    struct by_created;
    struct by_dao_id_and_short_name;
    struct by_dao_id_and_long_name;

    using dao_project_object_multi_index_type = multi_index_container<
      dao_project_object,
      indexed_by<
        ordered_unique< tag<by_id>, member< object, object_id_type, &object::id > >,
        ordered_non_unique< 
          tag<by_dao_id>,
            member< dao_project_object, dao_id_type, &dao_project_object::dao_id >
        >,
        ordered_non_unique< 
          tag<by_short_name>,
          member< dao_project_object, string, &dao_project_object::short_name >
        >,
        ordered_non_unique< 
          tag<by_long_name>,
          member< dao_project_object, string, &dao_project_object::long_name >
        >,
        ordered_unique< 
          tag<by_url>,
          member< dao_project_object, string, &dao_project_object::url >
        >,
        ordered_non_unique< 
          tag<by_created>,
          member< dao_project_object, time_point_sec, &dao_project_object::created >
        >,
        ordered_unique< 
          tag<by_dao_id_and_short_name>,
          composite_key<
            dao_project_object,
            member< dao_project_object, dao_id_type, &dao_project_object::dao_id >,
            member< dao_project_object, string, &dao_project_object::short_name >
          >
        >,
        ordered_unique< 
          tag<by_dao_id_and_long_name>,
          composite_key<
            dao_project_object,
            member< dao_project_object, dao_id_type, &dao_project_object::dao_id >,
            member< dao_project_object, string, &dao_project_object::long_name >
          >
        >
      >
    >;
    using dao_project_index = generic_index<dao_project_object, dao_project_object_multi_index_type>;
} }
MAP_OBJECT_ID_TO_TYPE( graphene::chain::dao_project_object )
FC_REFLECT_TYPENAME( graphene::chain::dao_project_object)
GRAPHENE_DECLARE_EXTERNAL_SERIALIZATION( graphene::chain::dao_project_object )