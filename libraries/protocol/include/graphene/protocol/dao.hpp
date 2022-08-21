#pragma once

#include <graphene/protocol/account.hpp>

namespace graphene { namespace protocol {
    
    struct dao_options
    {
      uint16_t owner_permissions = 0;//DEFAULT_DAO_OWNER_PERMISSION;
      uint16_t flags = 0;
      uint16_t min_moderators = 3;//DEFAULT_DAO_MIN_MODERATORS;
      uint16_t min_management_members = 3;//DEFAULT_DAO_MIN_MANAGEMENT_MEMBERS;
      flat_set<account_id_type> whitelist_authorities;
      flat_set<account_id_type> blacklist_authorities;
      flat_set<dao_id_type> whitelist_daos;
      flat_set<dao_id_type> blacklist_daos;
      string description;
      extensions_type extensions;  ///< Unused. Reserved for future use.
      void validate() const;
    };
    
    struct dao_project_options
    {
      uint16_t project_permissions = 0;//DEFAULT_DAO_DAPP_PERMISSION;
      uint16_t flags = 0;
      flat_set<account_id_type> whitelist_moderators;
      flat_set<account_id_type> blacklist_moderators;
      string description;
      extensions_type extensions;  ///< Unused. Reserved for future use.
      void validate() const;
    };
    
    struct dao_create_operation : public base_operation
    {
      struct fee_parameters_type 
      { 
        uint64_t fee = GRAPHENE_BLOCKCHAIN_PRECISION;
        uint32_t price_per_kbyte = 10;
      };
      asset           fee;
      account_id_type owner_account_id;
      string          short_name;
      string          long_name;
      dao_options     common_options;
      extensions_type extensions;  ///< Unused. Reserved for future use.
      account_id_type fee_payer() const { return owner_account_id; }
      void validate() const;
      share_type calculate_fee(fee_parameters_type const&) const;
    };
    
    struct dao_update_owner_operation : public base_operation
    {
      struct fee_parameters_type 
      { uint64_t fee = GRAPHENE_BLOCKCHAIN_PRECISION; };
      asset           fee;
      account_id_type owner_account_id;
      dao_id_type     dao_id_to_update;
      account_id_type new_owner_account_id;
      extensions_type extensions; ///< Unused. Reserved for future use.
      account_id_type fee_payer() const { return owner_account_id; }
      void validate() const;
      void get_required_owner_authorities( flat_set<account_id_type> &a ) const
      { a.insert( owner_account_id ); }
    };
    
    struct dao_update_operation : public base_operation
    {
      struct fee_parameters_type 
      {
        uint64_t fee = GRAPHENE_BLOCKCHAIN_PRECISION;
        uint32_t price_per_kbyte = 10;
      };
      asset            fee;
      account_id_type  owner_account_id;
      dao_id_type      dao_id_to_update;
      string           new_short_name;
      string           new_long_name;
      dao_options      new_options;
      extensions_type  extensions; ///< Unused. Reserved for future use.
      account_id_type fee_payer() const { return owner_account_id; }
      void validate() const;
      share_type calculate_fee(fee_parameters_type const&) const;
    };
    
    
    struct dao_account_create_operation : public base_operation
    {
      struct fee_parameters_type 
      { 
        uint64_t fee = GRAPHENE_BLOCKCHAIN_PRECISION;
        uint32_t price_per_kbyte = GRAPHENE_BLOCKCHAIN_PRECISION;
      };
      asset           fee;
      account_id_type account_id;
      dao_id_type     dao_id;
      extensions_type extensions;  ///< Unused. Reserved for future use.
      account_id_type fee_payer() const { return account_id; }
      void validate() const;
      share_type calculate_fee(fee_parameters_type const&) const;
    };
    
    struct dao_account_update_owner_operation : public base_operation
    {
      struct fee_parameters_type 
      { uint64_t fee = GRAPHENE_BLOCKCHAIN_PRECISION; };
      asset fee;
      dao_account_id_type dao_account_id_to_update;
      account_id_type     account_id;
      account_id_type     new_account_id;
      extensions_type     extensions;  ///< Unused. Reserved for future use.
      account_id_type fee_payer() const { return account_id; }
      void validate() const { }
      void get_required_owner_authorities( flat_set<account_id_type> &a ) const
      { a.insert( account_id ); }
    };
    
    struct dao_project_create_operation : public base_operation
    {
      struct fee_parameters_type 
      { 
        uint64_t fee = GRAPHENE_BLOCKCHAIN_PRECISION;
        uint32_t price_per_kbyte = 10;
      };
      asset               fee;
      account_id_type     account_id;
      dao_id_type         dao_id;
      string              short_name;
      string              long_name;
      string              url;
      dao_project_options common_options;
      extensions_type     extensions;  ///< Unused. Reserved for future use.
      account_id_type fee_payer() const { return account_id; }
      void validate() const;
      share_type calculate_fee(fee_parameters_type const&) const;
    };
} }

FC_REFLECT( graphene::protocol::dao_options,
            (owner_permissions)
            (flags)
            (min_moderators)
            (min_management_members)
            (whitelist_authorities)
            (blacklist_authorities)
            (whitelist_daos)
            (blacklist_daos)
            (description)
            (extensions)
          )
          
FC_REFLECT( graphene::protocol::dao_project_options,
            (project_permissions)
            (flags)
            (whitelist_moderators)
            (blacklist_moderators)
            (description)
            (extensions)
          )

FC_REFLECT( graphene::protocol::dao_create_operation::fee_parameters_type,
            (fee)(price_per_kbyte) )
FC_REFLECT( graphene::protocol::dao_update_owner_operation::fee_parameters_type, (fee) )
FC_REFLECT( graphene::protocol::dao_update_operation::fee_parameters_type,
            (fee)(price_per_kbyte) )
FC_REFLECT( graphene::protocol::dao_account_create_operation::fee_parameters_type,
            (fee)(price_per_kbyte) )
FC_REFLECT( graphene::protocol::dao_project_create_operation::fee_parameters_type,
            (fee)(price_per_kbyte) )

FC_REFLECT( graphene::protocol::dao_create_operation,
            (fee)
            (owner_account_id)
            (short_name)
            (long_name)
            (common_options)
            (extensions)
          )
          
FC_REFLECT( graphene::protocol::dao_update_owner_operation,
            (fee)
            (owner_account_id)
            (dao_id_to_update)
            (new_owner_account_id)
            (extensions)
          )
          
FC_REFLECT( graphene::protocol::dao_update_operation,
            (fee)
            (owner_account_id)
            (dao_id_to_update)
            (new_short_name)
            (new_long_name)
            (new_options)
            (extensions)
          )
          
FC_REFLECT( graphene::protocol::dao_account_create_operation,
            (fee)
            (account_id)
            (dao_id)
            (extensions)
          )
  
FC_REFLECT( graphene::protocol::dao_project_create_operation,
            (fee)
            (account_id)
            (dao_id)
            (short_name)
            (long_name)
            (url)
            (common_options)
            (extensions)
          )

GRAPHENE_DECLARE_EXTERNAL_SERIALIZATION( graphene::protocol::dao_options )
GRAPHENE_DECLARE_EXTERNAL_SERIALIZATION( graphene::protocol::dao_project_options )

GRAPHENE_DECLARE_EXTERNAL_SERIALIZATION( graphene::protocol::dao_create_operation::fee_parameters_type )
GRAPHENE_DECLARE_EXTERNAL_SERIALIZATION( graphene::protocol::dao_update_owner_operation::fee_parameters_type )
GRAPHENE_DECLARE_EXTERNAL_SERIALIZATION( graphene::protocol::dao_update_operation::fee_parameters_type )
GRAPHENE_DECLARE_EXTERNAL_SERIALIZATION( graphene::protocol::dao_account_create_operation::fee_parameters_type )
GRAPHENE_DECLARE_EXTERNAL_SERIALIZATION( graphene::protocol::dao_project_create_operation::fee_parameters_type )

GRAPHENE_DECLARE_EXTERNAL_SERIALIZATION( graphene::protocol::dao_create_operation )
GRAPHENE_DECLARE_EXTERNAL_SERIALIZATION( graphene::protocol::dao_update_owner_operation )
GRAPHENE_DECLARE_EXTERNAL_SERIALIZATION( graphene::protocol::dao_update_operation )
GRAPHENE_DECLARE_EXTERNAL_SERIALIZATION( graphene::protocol::dao_account_create_operation )
GRAPHENE_DECLARE_EXTERNAL_SERIALIZATION( graphene::protocol::dao_project_create_operation )