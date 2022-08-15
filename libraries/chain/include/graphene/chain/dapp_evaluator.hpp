#pragma once
#include <graphene/chain/evaluator.hpp>
#include <graphene/chain/dapp_object.hpp>

namespace graphene { namespace chain {

    struct dapp_create_evaluator : public evaluator<dapp_create_evaluator>
    {
        using operation_type = dapp_create_operation;

	    void_result do_evaluate(operation_type const&);
	    object_id_type do_apply(operation_type const&);
    };
} }