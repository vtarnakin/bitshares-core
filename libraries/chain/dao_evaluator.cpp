#include <graphene/chain/dao_evaluator.hpp>
#include <graphene/chain/database.hpp>
#include <graphene/chain/exceptions.hpp>
#include <graphene/chain/hardfork.hpp>
#include <graphene/chain/internal_exceptions.hpp>

#include <algorithm>

namespace graphene { namespace chain {

    void_result dao_create_evaluator::do_evaluate( operation_type const& op )
    { try {
        database& d = db();
        
        FC_ASSERT( d.find_object( op.owner_account_id ) );
        FC_ASSERT( fee_paying_account->is_lifetime_member(), "Only Lifetime members may register an DAO." );
        
        for( auto id : op.common_options.whitelist_authorities )
            d.get_object( id );
        for( auto id : op.common_options.blacklist_authorities )
            d.get_object( id );
        for( auto id : op.common_options.whitelist_daos )
            d.get_object( id );
        for( auto id : op.common_options.blacklist_daos )
            d.get_object( id );
      
        auto &dao_indx = d.get_index_type<dao_index>();
        if( op.name.size() )
        {
            auto current_dao_itr = dao_indx.indices().get<by_name>().find( op.name );
            FC_ASSERT( current_dao_itr == dao_indx.indices().get<by_name>().end(),
                       "DAO '${a}' already exists.", ("a", op.name) );
        }
        return void_result();
    } FC_CAPTURE_AND_RETHROW( (op) ) }
    
	object_id_type dao_create_evaluator::do_apply( operation_type const& op )
    { try {

        database& d = db();
        
        auto const& new_dao_obj = d.create<dao_object>( [&op, &d]( auto &dao_obj )
        {
            dao_obj.owner_account_id = op.owner_account_id;
            dao_obj.name             = op.name;
            dao_obj.created          = time_point_sec(); //head_block_time
            dao_obj.options          = op.common_options;
            d.create<dao_account_object>( [&dao_obj]( dao_account_object &dao_account_obj )
            {
                dao_account_obj.account_id = dao_obj.owner_account_id;
                dao_account_obj.dao_id     = dao_obj.id;
                dao_account_obj.created    = dao_obj.created;
            });
        });
        return new_dao_obj.id;
    } FC_CAPTURE_AND_RETHROW( (op) ) }
    
    void_result dao_update_owner_evaluator::do_evaluate( operation_type const& op )
    { try {
        database& d = db();
        FC_ASSERT( d.find_object( op.new_owner_account_id ) );
        dao_object const& dao = op.dao_id_to_update( d );
        dao_to_update = &dao;
        FC_ASSERT( op.owner_account_id == dao.owner_account_id,
                   "Incorrect owner for DAO! (${op.owner_account_id} != ${dao.owner_account_id})",
                   ("op.owner_account_id", op.owner_account_id)("dao.owner_account_id", dao.owner_account_id) );
        return void_result();
    } FC_CAPTURE_AND_RETHROW( (op) ) }
    
    void_result dao_update_owner_evaluator::do_apply( operation_type const& op )
    { try {
        database& d = db();
        d.modify( *dao_to_update, [&op]( dao_object &dao_obj ) 
        { dao_obj.owner_account_id = op.new_owner_account_id; });
        return void_result();
    } FC_CAPTURE_AND_RETHROW( (op) ) }
    
    void_result dao_update_evaluator::do_evaluate( operation_type const& op )
    { try {
        database& d = db();
        dao_object const& dao = op.dao_id_to_update( d );
        dao_to_update = &dao;
        FC_ASSERT( op.owner_account_id == dao.owner_account_id,
                   "Incorrect owner for DAO! (${op.owner_account_id} != ${dao.owner_account_id})",
                   ("op.owner_account_id", op.owner_account_id)("dao.owner_account_id", dao.owner_account_id) );
        return void_result();
    } FC_CAPTURE_AND_RETHROW( (op) ) }
    
    void_result dao_update_evaluator::do_apply( operation_type const& op )
    { try {
        database& d = db();
        d.modify( *dao_to_update, [&op]( dao_object &dao_obj ) 
        { dao_obj.options   = op.new_options; });
        return void_result();
    } FC_CAPTURE_AND_RETHROW( (op) ) }
    /*
    void_result dao_update_evaluator::do_evaluate( operation_type const& op )
    { try {
        database& d = db();
        
    } FC_CAPTURE_AND_RETHROW( (op) ) }
    
    void_result dao_update_evaluator::do_apply( operation_type const& op )
    { try {
        database& d = db();
        
    } FC_CAPTURE_AND_RETHROW( (op) ) }*/
} }