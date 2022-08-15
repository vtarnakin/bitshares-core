#pragma once

#include <graphene/protocol/base.hpp>
#include <graphene/protocol/asset.hpp>

namespace graphene { namespace protocol {
    
    struct dapp_options
    {
        uint16_t owner_permissions = 0;//DEFAULT_DAPP_OWNER_PERMISSION;
        uint16_t flags = 0;
        flat_set<account_id_type> whitelist_authorities;
        flat_set<account_id_type> blacklist_authorities;
        flat_set<dapp_id_type> whitelist_dapps;
        flat_set<dapp_id_type> blacklist_dapps;
        string description;
        extensions_type extensions;  ///< Unused. Reserved for future use.
        void validate() const;
    };
    
    struct dapp_create_operation : public base_operation
    {
        struct fee_parameters_type { 
            uint64_t fee = GRAPHENE_BLOCKCHAIN_PRECISION;
            uint32_t price_per_kbyte = 10;
        };

        asset fee;
        string name;
        account_id_type owner_account_id;
        dapp_options common_options;
        extensions_type extensions;  ///< Unused. Reserved for future use.
        account_id_type fee_payer() const { return owner_account_id; };
        void validate() const;
        share_type calculate_fee(fee_parameters_type const&) const;
    };
    
    struct dapp_account_create_operation : public base_operation
    {
        struct fee_parameters_type { 
           uint64_t fee = GRAPHENE_BLOCKCHAIN_PRECISION;
           uint32_t price_per_kbyte = GRAPHENE_BLOCKCHAIN_PRECISION;
        };

        asset fee;
        account_id_type account_id;
        dapp_id_type dapp_id;
        time_point_sec created;
        extensions_type extensions;  ///< Unused. Reserved for future use.
        account_id_type fee_payer() const { return account_id; };
        void validate() const;
        share_type calculate_fee(fee_parameters_type const&) const;
    };
} }

FC_REFLECT( graphene::protocol::dapp_options,
            (owner_permissions)
            (flags)
            (whitelist_authorities)
            (blacklist_authorities)
            (whitelist_dapps)
            (blacklist_dapps)
            (description)
            (extensions)
          )

FC_REFLECT( graphene::protocol::dapp_create_operation::fee_parameters_type,
            (fee)(price_per_kbyte) )
FC_REFLECT( graphene::protocol::dapp_account_create_operation::fee_parameters_type,
            (fee)(price_per_kbyte) )

FC_REFLECT( graphene::protocol::dapp_create_operation,
            (fee)
            (name)
            (owner_account_id)
            (common_options)
            (extensions)
          )
          
FC_REFLECT( graphene::protocol::dapp_account_create_operation,
            (fee)
            (account_id)
            (dapp_id)
            (created)
            (extensions)
          )

GRAPHENE_DECLARE_EXTERNAL_SERIALIZATION( graphene::protocol::dapp_options )

GRAPHENE_DECLARE_EXTERNAL_SERIALIZATION( graphene::protocol::dapp_create_operation::fee_parameters_type )
GRAPHENE_DECLARE_EXTERNAL_SERIALIZATION( graphene::protocol::dapp_account_create_operation::fee_parameters_type )

GRAPHENE_DECLARE_EXTERNAL_SERIALIZATION( graphene::protocol::dapp_create_operation )
GRAPHENE_DECLARE_EXTERNAL_SERIALIZATION( graphene::protocol::dapp_account_create_operation )