#pragma once
#include <graphene/chain/evaluator.hpp>
#include <graphene/chain/dao_object.hpp>

namespace graphene { namespace chain {

    struct dao_create_evaluator : public evaluator<dao_create_evaluator>
    {
        using operation_type = dao_create_operation;

	    void_result do_evaluate(operation_type const&);
	    object_id_type do_apply(operation_type const&);
    };
    
    struct dao_update_owner_evaluator : public evaluator<dao_update_owner_evaluator>
    {
        using operation_type = dao_update_owner_operation;

	    void_result do_evaluate(operation_type const&);
	    void_result do_apply(operation_type const&);
        dao_object const* dao_to_update = nullptr;
    };
    
    struct dao_update_evaluator : public evaluator<dao_update_evaluator>
    {
        using operation_type = dao_update_operation;

	    void_result do_evaluate(operation_type const&);
	    void_result do_apply(operation_type const&);
        dao_object const* dao_to_update = nullptr;
    };
} }