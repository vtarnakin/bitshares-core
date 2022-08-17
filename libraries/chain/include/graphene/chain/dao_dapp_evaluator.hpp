#pragma once
#include <graphene/chain/evaluator.hpp>
#include <graphene/chain/dao_dapp_object.hpp>

namespace graphene { namespace chain {

    struct dao_dapp_create_evaluator : public evaluator<dao_dapp_create_evaluator>
    {
        using operation_type = dao_dapp_create_operation;

	    void_result do_evaluate(operation_type const&);
	    object_id_type do_apply(operation_type const&);
    };
    /*
    struct dao_dapp_dao_update_evaluator : public evaluator<dao_dapp_dao_update_evaluator>
    {
        using operation_type = dao_dapp_dao_update_operation;

	    void_result do_evaluate(operation_type const&);
	    object_id_type do_apply(operation_type const&);
    };
    
    struct dao_dapp_update_evaluator : public evaluator<dao_dapp_update_evaluator>
    {
        using operation_type = dao_dapp_update_operation;

	    void_result do_evaluate(operation_type const&);
	    object_id_type do_apply(operation_type const&);
    };*/
} } 