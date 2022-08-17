#include <graphene/chain/dao_account_evaluator.hpp>
#include <graphene/chain/database.hpp>
#include <graphene/chain/exceptions.hpp>
#include <graphene/chain/hardfork.hpp>
#include <graphene/chain/internal_exceptions.hpp>

#include <algorithm>

namespace graphene { namespace chain {

    void_result dao_account_create_evaluator::do_evaluate( operation_type const& op )
    { try {
        database& d = db();
        FC_ASSERT( d.find_object( op.account_id ) );
        FC_ASSERT( fee_paying_account->is_lifetime_member(), "Only Lifetime members may register an DAO account." );
        FC_ASSERT( d.find_object( op.dao_id ) );
                   
        auto &dao_acc_indx = d.get_index_type<dao_account_index>();
        auto current_dao_acc_itr = dao_acc_indx.indices().get<by_dao_and_account_id>().find( boost::make_tuple( op.dao_id, op.account_id ) );
        FC_ASSERT( current_dao_acc_itr == dao_acc_indx.indices().get<by_dao_and_account_id>().end(),
                   "DAO account '${a}' already exists.", ("a", current_dao_acc_itr->id) );

        return void_result();
    } FC_CAPTURE_AND_RETHROW( (op) ) }
    
	object_id_type dao_account_create_evaluator::do_apply( operation_type const& op )
    { try {

        database& d = db();
        
        auto const& new_dao_acc_obj = d.create<dao_account_object>( [&op, &d]( auto &dao_acc_obj )
        {
            dao_acc_obj.account_id = op.account_id;
            dao_acc_obj.dao_id = op.dao_id;
            dao_acc_obj.created = time_point_sec();
        });
        return new_dao_acc_obj.id;
    } FC_CAPTURE_AND_RETHROW( (op) ) }
} }