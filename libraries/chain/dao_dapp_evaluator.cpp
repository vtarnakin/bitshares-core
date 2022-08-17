#include <graphene/chain/dao_dapp_evaluator.hpp>
#include <graphene/chain/database.hpp>
#include <graphene/chain/exceptions.hpp>
#include <graphene/chain/hardfork.hpp>
#include <graphene/chain/internal_exceptions.hpp>

#include <algorithm>

namespace graphene { namespace chain {

    void_result dao_dapp_evaluator::do_evaluate( operation_type const& op )
    { try {
        database& d = db();
        FC_ASSERT( d.find_object( op.account_id ) );
        FC_ASSERT( d.find_object( op.dao_id ) );
        
        for( auto id : op.common_options.whitelist_moderators )
            d.get_object( id );
        for( auto id : op.common_options.blacklist_moderators )
            d.get_object( id );
        
        auto &dao_dapp_indx = d.get_index_type<dao_dapp_index>();
        if( op.name.size() )
        {
            auto current_dao_dapp_itr = dao_dapp_indx.indices().get<by_dao_and_name>().find( boost::make_tuple( op.dao_id, op.name ) );
            FC_ASSERT( current_dao_dapp_itr == dao_dapp_indx.indices().get<by_dao_and_name>().end(),
                       "DAO DApp '${a}' already exists.", ("a", current_dao_acc_itr->id) );
        }
        return void_result();
    } FC_CAPTURE_AND_RETHROW( (op) ) }
    
	object_id_type dao_dapp_evaluator::do_apply( operation_type const& op )
    { try {

        database& d = db();
        
        auto const& new_dao_dapp_obj = d.create<dao_dapp_object>( [&op, &d]( auto &dao_dapp_obj )
        {
            dao_dapp_obj.dao_id  = op.account_id;
            dao_dapp_obj.url     = op.url;
            dao_dapp_obj.created = time_point_sec();
            dao_dapp_obj.options = op.common_options;
        });
        return new_dao_dapp_obj.id;
    } FC_CAPTURE_AND_RETHROW( (op) ) }
} }