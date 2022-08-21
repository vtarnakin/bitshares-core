#include <fc/crypto/aes.hpp>
#include <fc/crypto/base64.hpp>

#include <graphene/seeder/seeder_plugin.hpp>
#include <graphene/protocol/types.hpp>
#include <graphene/utilities/key_conversion.hpp>

#include <graphene/chain/account_object.hpp>
#include <graphene/chain/file_object.hpp>
#include <graphene/chain/order_object.hpp>

namespace graphene { namespace seeder { namespace detail {

    class seeder_plugin_impl
    {
      fc::thread                          _seeder_thread;
      seeder_plugin&                      _plugin;
      protocol::account_id_type _account_id;
      fc::ecc::private_key                _private_key;
public :
      seeder_plugin_impl( seeder_plugin &plugin )
        : _plugin( plugin )
      { } 
      virtual ~seeder_plugin_impl() = default;
      void notify_new_block(protocol::signed_block const&);
      void send_hello();
    };

    void seeder_plugin_impl::notify_new_block( protocol::signed_block const& b )
    {
   
    }


    void seeder_plugin_impl::send_hello()
    {  
      auto &db = _self.database();
   
      protocol::seeder_alive_operation op;
      op.seeder = this->account_id;
      try {
        op.seeder_key = static_cast<chain::account_object const*>(db.find_object(this->account_id))->options.memo_key;
      } catch (std::exception const& e) {
        elog("There is exception in getting seeder key: ${e}", ( "e", e.what() ) ); 
        throw;
      }
   
      auto dyn_props = db.get_dynamic_global_properties();
   
      protocol::signed_transaction tx;
      tx.operations.push_back(op);
      tx.set_reference_block( dyn_props.head_block_id );
      tx.set_expiration( dyn_props.time + fc::seconds(30) );
      tx.validate();
   
      auto _chain_id = db.get_chain_id();
      tx.sign(private_key, _chain_id );
   
      _plugin.p2p_node().broadcast_transaction(tx);

      fc::time_point next_wakeup = fc::time_point::now()  + fc::seconds(this->_self.hello_interval);
      this->seeder_thread.schedule([=](){ this->send_hello();}, next_wakeup);
    }
} // detail

    seeder_plugin::seeder_plugin()
      : my( new detail::seeder_plugin_impl( *this ) )
    { }

    string seeder_plugin::plugin_name() const
    { return "seeder_plugin"; }

    void seeder_plugin::plugin_set_program_options( bpo_d_t &cli, bpo_d_t &cfg )
    {
      cli.add_options() // !!!
          ( "seeder-private-key", bpo_t::value<string>(), 
            "Private key of the account controlling this seeder" )
          ( "seeder", bpo_t::value<string>(), 
            "Account id controlling this seeder" )
          ( "seeder-hello-interval", bpo_t::value<uint32_t>()->default_value( 60 ),
            "Seeder send hello interval in seconds");
      cfg.add( cli );
    }

    void seeder_plugin::plugin_initialize(bpo_vm_t const& options)
    { try {
   
      if( options.count( "seeder" ) ) 
      {
          _my->account_id = fc::variant(options["seeder"].as<std::string>()).as<protocol::account_id_type>(1);
      } else {
          FC_THROW( "missing seeder parameter" );
      }

      if( !options.count( "seeder-private-key" ) )
      { FC_THROW( "Missing seeder-private-key paramenter" ); }

      if ( options.count( "seeder-hello-interval" ) )
      {
          _hello_interval = options["seeder-hello-interval"].as<uint32_t>();
      } else {
          wlog( "Warning! Send hello interval option missing, set to default (60 seconds)" );
          _hello_interval = 60;
      }
   
      ilog( "Hello interval = ${h}", ("h", this->hello_interval) );

      const string key_wif_string = options["seeder-private-key"].as<string>();
      fc::optional<fc::ecc::private_key> private_key = graphene::utilities::wif_to_key(key_wif_string);
      if( !private_key ) 
      { 
          try {
              private_key = fc::variant(key_wif_string).as<fc::ecc::private_key>(1);
          } catch(fc::exception const&) {
              FC_THROW( "Invalid WIF-format seeder private key ${key_string}", ( "key_string", key_wif_string ) );
          }
      }
   
      _my->private_key = *private_key;

      database().applied_block.connect( [&]( protocol::signed_block const& b)
      { _my->notify_new_block(b); });
            
      ilog( "seeder_plugin: plugin_initialize()" );
    } FC_LOG_AND_RETHROW() }

    void seeder_plugin::plugin_startup()
    {
      auto &db = database();
      FC_ASSERT( _my->account_id != chain::account_id_type(), "Cannot find seeder account in database" );
      auto const& seeder_account = db.get( _my->account_id );
      auto public_active_key = seeder_account.active.get_keys();
      FC_ASSERT( public_active_key.size() == 1, "Cannot find active key in db" );
      FC_ASSERT( public_active_key[0] == my->private_key.get_public_key(), "Wrong active private key in config file" );
      fc::time_point next_call = fc::time_point::now()  + fc::seconds(this->hello_interval);
      _my->seeder_thread.schedule([this]() { this->my->send_hello(); }, next_call );
    }
} }