#pragma once
#include <graphene/chain/evaluator.hpp>
#include <graphene/chain/dapp_account_object.hpp>

namespace graphene { namespace chain {

    struct dapp_account_create_evaluator : public evaluator<dapp_account_create_evaluator>
    {
        using operation_type = dapp_account_create_operation;

	    void_result do_evaluate(operation_type const&);
	    object_id_type do_apply(operation_type const&);
    };
} }