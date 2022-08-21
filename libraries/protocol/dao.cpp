#include <graphene/protocol/dao.hpp>
#include <graphene/protocol/account.hpp>
#include <fc/io/raw.hpp>

namespace graphene { namespace protocol {
    
    void dao_options::validate() const
    {
      FC_ASSERT( min_management_members % 2 != 0 );
      FC_ASSERT( min_moderators % 2 != 0 );
      for( auto item : whitelist_authorities )
      { FC_ASSERT( whitelist_authorities.find( item ) == whitelist_authorities.end() ); }
      for( auto item : blacklist_authorities )
      { FC_ASSERT( blacklist_authorities.find( item ) == blacklist_authorities.end() ); }
      for( auto item : whitelist_daos )
      { FC_ASSERT( blacklist_daos.find( item ) == blacklist_daos.end() ); }
      for( auto item : blacklist_daos )
      { FC_ASSERT( whitelist_daos.find( item ) == whitelist_daos.end() ); }
    }
    
    void dao_project_options::validate() const
    {
      for( auto item : whitelist_moderators )
      { FC_ASSERT( whitelist_moderators.find( item ) == whitelist_moderators.end() ); }
      for( auto item : blacklist_moderators )
      { FC_ASSERT( blacklist_moderators.find( item ) == blacklist_moderators.end() ); }
    }
    
    void dao_create_operation::validate() const
    {
      common_options.validate();
      FC_ASSERT( fee.amount >= 0 );
      FC_ASSERT( short_name.size() );
      FC_ASSERT( long_name.size() );
      //FC_ASSERT( is_valid_name( name ) );
    }
    
    share_type dao_create_operation::calculate_fee( fee_parameters_type const& schedule ) const
    {
      auto core_fee_required = schedule.fee; 
      // common_options contains several lists and a string. Charge fees for its size
      core_fee_required += calculate_data_fee( fc::raw::pack_size( *this ), schedule.price_per_kbyte );
      return core_fee_required; 
    }
    
    void dao_account_create_operation::validate() const
    {
      FC_ASSERT( fee.amount >= 0 );
    }
    
    share_type dao_account_create_operation::calculate_fee( fee_parameters_type const& schedule ) const 
    { 
      auto core_fee_required = schedule.fee; 
      core_fee_required += calculate_data_fee( fc::raw::pack_size( *this ), schedule.price_per_kbyte );
      return core_fee_required; 
    }
    
    void dao_update_owner_operation::validate() const
    {
      FC_ASSERT( fee.amount >= 0 );
    }
    
    void dao_update_operation::validate() const
    {
      new_options.validate();
      FC_ASSERT( fee.amount >= 0 );
    }
    
    share_type dao_update_operation::calculate_fee( fee_parameters_type const& schedule ) const 
    { 
      auto core_fee_required = schedule.fee; 
      core_fee_required += calculate_data_fee( fc::raw::pack_size( *this ), schedule.price_per_kbyte );
      return core_fee_required; 
    }
    
    void dao_project_create_operation::validate() const
    {
      common_options.validate();
      FC_ASSERT( fee.amount >= 0 );
      FC_ASSERT( short_name.size() );
      FC_ASSERT( long_name.size() );
      //FC_ASSERT( is_valid_name( name ) );
    }
    
    share_type dao_project_create_operation::calculate_fee( fee_parameters_type const& schedule ) const
    {
      auto core_fee_required = schedule.fee; 
      // common_options contains several lists and a string. Charge fees for its size
      core_fee_required += calculate_data_fee( fc::raw::pack_size( *this ), schedule.price_per_kbyte );
      return core_fee_required; 
    }
} }
GRAPHENE_IMPLEMENT_EXTERNAL_SERIALIZATION( graphene::protocol::dao_options )
GRAPHENE_IMPLEMENT_EXTERNAL_SERIALIZATION( graphene::protocol::dao_project_options )

GRAPHENE_IMPLEMENT_EXTERNAL_SERIALIZATION( graphene::protocol::dao_create_operation::fee_parameters_type )
GRAPHENE_IMPLEMENT_EXTERNAL_SERIALIZATION( graphene::protocol::dao_update_owner_operation::fee_parameters_type )
GRAPHENE_IMPLEMENT_EXTERNAL_SERIALIZATION( graphene::protocol::dao_update_operation::fee_parameters_type )
GRAPHENE_IMPLEMENT_EXTERNAL_SERIALIZATION( graphene::protocol::dao_account_create_operation::fee_parameters_type )
GRAPHENE_IMPLEMENT_EXTERNAL_SERIALIZATION( graphene::protocol::dao_project_create_operation::fee_parameters_type )

GRAPHENE_IMPLEMENT_EXTERNAL_SERIALIZATION( graphene::protocol::dao_create_operation )
GRAPHENE_IMPLEMENT_EXTERNAL_SERIALIZATION( graphene::protocol::dao_update_owner_operation )
GRAPHENE_IMPLEMENT_EXTERNAL_SERIALIZATION( graphene::protocol::dao_update_operation )
GRAPHENE_IMPLEMENT_EXTERNAL_SERIALIZATION( graphene::protocol::dao_account_create_operation )
GRAPHENE_IMPLEMENT_EXTERNAL_SERIALIZATION( graphene::protocol::dao_project_create_operation )