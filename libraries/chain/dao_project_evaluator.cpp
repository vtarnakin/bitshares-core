#include <graphene/chain/dao_project_evaluator.hpp>
#include <graphene/chain/database.hpp>
#include <graphene/chain/exceptions.hpp>
#include <graphene/chain/hardfork.hpp>
#include <graphene/chain/internal_exceptions.hpp>

#include <algorithm>

namespace graphene { namespace chain {

    void_result dao_project_create_evaluator::do_evaluate( operation_type const& op )
    { try {
      database& d = db();
      FC_ASSERT( d.find_object( op.account_id ) );
      auto const* obj = d.find_object( op.dao_id );
      FC_ASSERT( obj );
      auto const* dao_obj = dynamic_cast<dao_object const*>( obj );
      FC_ASSERT( dao_obj->owner_account_id == op.account_id, "DAO account '${a}' isn't owner.", ("a", op.account_id) );
        
      for( auto id : op.common_options.whitelist_moderators )
          d.get_object( id );
      for( auto id : op.common_options.blacklist_moderators )
          d.get_object( id );
        
      auto &dao_project_indx = d.get_index_type<dao_project_index>();
      if( op.short_name.size() )
      {
          auto current_dao_project_itr = dao_project_indx.indices().get<by_dao_id_and_short_name>().find( boost::make_tuple( op.dao_id, op.short_name ) );
          FC_ASSERT( current_dao_project_itr == dao_project_indx.indices().get<by_dao_id_and_short_name>().end(),
                     "DAO project '${a}' already exists.", ("a", current_dao_project_itr->id) );
      }
      if( op.long_name.size() )
      {
          auto current_dao_project_itr = dao_project_indx.indices().get<by_dao_id_and_long_name>().find( boost::make_tuple( op.dao_id, op.long_name ) );
          FC_ASSERT( current_dao_project_itr == dao_project_indx.indices().get<by_dao_id_and_long_name>().end(),
                     "DAO project '${a}' already exists.", ("a", current_dao_project_itr->id) );
      }
      return void_result();
    } FC_CAPTURE_AND_RETHROW( (op) ) }
    
	object_id_type dao_project_create_evaluator::do_apply( operation_type const& op )
    { try {
      database& d = db();
      auto const& new_dao_project_obj = d.create<dao_project_object>( [&op, &d]( auto &dao_project_obj )
      {
        dao_project_obj.dao_id     = op.dao_id;
        dao_project_obj.short_name = op.short_name;
        dao_project_obj.long_name  = op.long_name;
        dao_project_obj.url        = op.url;
        dao_project_obj.created    = time_point_sec();
        dao_project_obj.options    = op.common_options;
      });
      return new_dao_project_obj.id;
    } FC_CAPTURE_AND_RETHROW( (op) ) }
} }