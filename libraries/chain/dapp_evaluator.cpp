#include <graphene/chain/dapp_evaluator.hpp>
#include <graphene/chain/database.hpp>
#include <graphene/chain/exceptions.hpp>
#include <graphene/chain/hardfork.hpp>
#include <graphene/chain/internal_exceptions.hpp>

#include <algorithm>

namespace graphene { namespace chain {

    void_result dapp_create_evaluator::do_evaluate( operation_type const& op )
    { try {
        database& d = db();

        FC_ASSERT( fee_paying_account->is_lifetime_member(), "Only Lifetime members may register an DApp." );
        
        for( auto id : op.common_options.whitelist_authorities )
            d.get_object( id );
        for( auto id : op.common_options.blacklist_authorities )
            d.get_object( id );
        for( auto id : op.common_options.whitelist_dapps )
            d.get_object( id );
        for( auto id : op.common_options.blacklist_dapps )
            d.get_object( id );
      
        auto &dapp_indx = d.get_index_type<dapp_index>();
        if( op.name.size() )
        {
            auto current_dapp_itr = dapp_indx.indices().get<by_name>().find( op.name );
            FC_ASSERT( current_dapp_itr == dapp_indx.indices().get<by_name>().end(),
                       "DApp '${a}' already exists.", ("a", op.name) );
        }
        return void_result();
    } FC_CAPTURE_AND_RETHROW( (op) ) }
    
	object_id_type dapp_create_evaluator::do_apply( operation_type const& op )
    { try {

        database& d = db();
        
        auto const& new_dapp_obj = d.create<dapp_object>( [&op, &d]( auto &dapp_obj )
        {
            dapp_obj.owner_account_id = op.owner_account_id;
            dapp_obj.name = op.name;
            dapp_obj.created = time_point_sec( d.head_block_time() );
            dapp_obj.options = op.common_options;
            d.create<dapp_account_object>( [&dapp_obj]( dapp_account_object &dapp_account_obj )
            {
                dapp_account_obj.account_id = dapp_obj.owner_account_id;
                dapp_account_obj.dapp_id = dapp_obj.id;
                dapp_account_obj.created = dapp_obj.created;
            });
        });
        return new_dapp_obj.id;
    } FC_CAPTURE_AND_RETHROW( (op) ) }
} }