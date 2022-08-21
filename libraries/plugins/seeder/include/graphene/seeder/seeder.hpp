#pragma once

#include <graphene/app/plugin.hpp>

#ifndef SEEDER_SPACE_ID
#define SEEDER_SPACE_ID 8
#endif

namespace graphene { namespace seeder { namespace detail { class seeder_plugin_impl; }

    using bpo_t    = boost::program_options;
    using bpo_d_t  = bpo::options_description;
    using bpo_vm_t = bpo::variables_map;
    
    class seeder_plugin : public graphene::app::plugin
    {
      friend class detail::seeder_plugin_impl;
      std::unique_ptr<detail::seeder_plugin_impl> _my;
      uint32_t _hello_interval;
public :
      seeder_plugin();
      virtual ~seeder_plugin() = default;
      std::string plugin_name() const override;
      virtual void plugin_set_program_options(bpo_d_t&, bpo_d_t&) override;
      virtual void plugin_initialize(bpo_vm_t const&) override;
      virtual void plugin_startup() override;
    };
} }