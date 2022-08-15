#include <graphene/protocol/dapp.hpp>
#include <graphene/protocol/account.hpp>
#include <fc/io/raw.hpp>

namespace graphene { namespace protocol {
    
    void dapp_options::validate() const
    {
        for( auto item : whitelist_authorities )
        { FC_ASSERT( whitelist_authorities.find( item ) == whitelist_authorities.end() ); }
        for( auto item : blacklist_authorities )
        { FC_ASSERT( blacklist_authorities.find( item ) == blacklist_authorities.end() ); }
        for( auto item : whitelist_dapps )
        { FC_ASSERT( blacklist_dapps.find( item ) == blacklist_dapps.end() ); }
        for( auto item : blacklist_dapps )
        { FC_ASSERT( whitelist_dapps.find( item ) == whitelist_dapps.end() ); }
    }
    
    void dapp_create_operation::validate() const
    {
        common_options.validate();
        FC_ASSERT( fee.amount >= 0 );
        FC_ASSERT( is_valid_name( name ) );
    }
    
    share_type dapp_create_operation::calculate_fee( fee_parameters_type const& schedule ) const
    {
        auto core_fee_required = schedule.fee; 
        // common_options contains several lists and a string. Charge fees for its size
        core_fee_required += calculate_data_fee( fc::raw::pack_size( *this ), schedule.price_per_kbyte );
        return core_fee_required; 
    }
    
    void dapp_account_create_operation::validate() const
    {
        FC_ASSERT( fee.amount >= 0 );
    }
    
    share_type dapp_account_create_operation::calculate_fee( fee_parameters_type const& schedule ) const 
    { 
        auto core_fee_required = schedule.fee; 
        core_fee_required += calculate_data_fee( fc::raw::pack_size( *this ), schedule.price_per_kbyte );
        return core_fee_required; 
    }
} }

GRAPHENE_IMPLEMENT_EXTERNAL_SERIALIZATION( graphene::protocol::dapp_options )
GRAPHENE_IMPLEMENT_EXTERNAL_SERIALIZATION( graphene::protocol::dapp_create_operation::fee_parameters_type )
GRAPHENE_IMPLEMENT_EXTERNAL_SERIALIZATION( graphene::protocol::dapp_account_create_operation::fee_parameters_type )
GRAPHENE_IMPLEMENT_EXTERNAL_SERIALIZATION( graphene::protocol::dapp_create_operation )
GRAPHENE_IMPLEMENT_EXTERNAL_SERIALIZATION( graphene::protocol::dapp_account_create_operation )