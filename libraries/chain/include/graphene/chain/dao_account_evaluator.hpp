#pragma once
#include <graphene/chain/evaluator.hpp>
#include <graphene/chain/dao_account_object.hpp>

namespace graphene { namespace chain {

    struct dao_account_create_evaluator : public evaluator<dao_account_create_evaluator>
    {
        using operation_type = dao_account_create_operation;

	    void_result do_evaluate(operation_type const&);
	    object_id_type do_apply(operation_type const&);
    };
    
    struct dao_account_update_owner_evaluator : public evaluator<dao_account_update_owner_evaluator>
    {
        using operation_type = dao_account_update_owner_operation;

	    void_result do_evaluate(operation_type const&);
	    object_id_type do_apply(operation_type const&);
        dao_account_object const* dao_account_to_update = nullptr;
    };
} }