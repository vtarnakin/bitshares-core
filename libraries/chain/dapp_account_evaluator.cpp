#include <graphene/chain/dapp_account_evaluator.hpp>
#include <graphene/chain/database.hpp>
#include <graphene/chain/exceptions.hpp>
#include <graphene/chain/hardfork.hpp>
#include <graphene/chain/internal_exceptions.hpp>

#include <algorithm>

namespace graphene { namespace chain {

    void_result dapp_account_create_evaluator::do_evaluate( operation_type const& op )
    { try {
        database& d = db();

        FC_ASSERT( fee_paying_account->is_lifetime_member(), "Only Lifetime members may register an DApp account." );
        
        auto &dapp_indx = d.get_index_type<dapp_index>();
        auto current_dapp_itr = dapp_indx.indices().get<by_id>().find( op.dapp_id );
        FC_ASSERT( current_dapp_itr != dapp_indx.indices().get<by_id>().end(),
                   "DApp '${a}' isn't exists.", ("a", op.dapp_id) );
                       
        auto &dapp_acc_indx = d.get_index_type<dapp_account_index>();
        auto current_dapp_acc_itr = dapp_acc_indx.indices().get<by_dapp_and_account_id>().find( boost::make_tuple( op.dapp_id, op.account_id ) );
        FC_ASSERT( current_dapp_acc_itr == dapp_acc_indx.indices().get<by_dapp_and_account_id>().end(),
                   "DApp account '${a}' already exists.", ("a", current_dapp_acc_itr->id) );

        return void_result();
    } FC_CAPTURE_AND_RETHROW( (op) ) }
    
	object_id_type dapp_account_create_evaluator::do_apply( operation_type const& op )
    { try {

        database& d = db();
        
        auto const& new_dapp_acc_obj = d.create<dapp_account_object>( [&op, &d]( auto &dapp_acc_obj )
        {
            dapp_acc_obj.account_id = op.account_id;
            dapp_acc_obj.dapp_id = op.dapp_id;
            dapp_acc_obj.created = time_point_sec( d.head_block_time() );
        });
        return new_dapp_acc_obj.id;
    } FC_CAPTURE_AND_RETHROW( (op) ) }
} }