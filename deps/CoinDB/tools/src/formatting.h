///////////////////////////////////////////////////////////////////////////////
//
// formatting.h
//
// Copyright (c) 2013-2014 Eric Lombrozo
//
// All Rights Reserved.
//

#pragma once

#include <Base58Check.h>

#include <CoinQ_script.h>

#include <Vault.h>

// TODO: create separate library
#include <stdutils/stringutils.h>

#include <sstream>

// TODO: Get this from a config file
const unsigned char BASE58_VERSIONS[] = { 0x00, 0x05 };

// Data formatting
inline std::string getAddressFromScript(const bytes_t& script)
{
    using namespace CoinQ::Script;

    payee_t payee = getScriptPubKeyPayee(script);
    if (payee.first == SCRIPT_PUBKEY_PAY_TO_PUBKEY_HASH)
        return toBase58Check(payee.second, BASE58_VERSIONS[0]);
    else if (payee.first == SCRIPT_PUBKEY_PAY_TO_SCRIPT_HASH)
        return toBase58Check(payee.second, BASE58_VERSIONS[1]);
    else
        return "N/A";
}

////////////
// Tables //
////////////

// SigningScripts
inline std::string formattedScriptHeader()
{
    using namespace std;

    stringstream ss;
    ss << " ";
    ss << left  << setw(15) << "account name" << " | "
       << left  << setw(15) << "bin name" << " | "
       << right << setw(5)  << "index" << " | "
       << left  << setw(50) << "script" << " | "
       << left  << setw(36) << "address" << " | "
       << left  << setw(8)  << "status";
    ss << " ";

    size_t header_length = ss.str().size();
    ss << endl;
    for (size_t i = 0; i < header_length; i++) { ss << "="; }
    return ss.str();
}

inline std::string formattedScript(const CoinDB::SigningScriptView& view)
{
    using namespace std;
    using namespace CoinDB;

    stringstream ss;
    ss << " ";
    ss << left  << setw(15) << view.account_name << " | "
       << left  << setw(15) << view.account_bin_name << " | "
       << right << setw(5)  << view.index << " | "
       << left  << setw(50) << uchar_vector(view.txoutscript).getHex() << " | "
       << left  << setw(36) << getAddressFromScript(view.txoutscript) << " | "
       << left  << setw(8)  << SigningScript::getStatusString(view.status);
    ss << " ";
    return ss.str();
}

// TxIns
inline std::string formattedTxInHeader()
{
    using namespace std;

    stringstream ss;
    ss << " ";
    ss << right << setw(5)  << "input" << " | "
       << left  << setw(68) << "outpoint" << " | "
       << right << setw(15) << "value";
    ss << " ";

    size_t header_length = ss.str().size();
    ss << endl;
    for (size_t i = 0; i < header_length; i++) { ss << "="; }
    return ss.str();
}

inline std::string formattedTxIn(const std::shared_ptr<CoinDB::TxIn>& txin)
{
    using namespace std;
    using namespace CoinDB;

    stringstream outpoint;
    outpoint << uchar_vector(txin->outhash()).getHex() << ":" << txin->outindex();

    stringstream ss;
    ss << " ";
    ss << right << setw(5)  << txin->txindex() << " | "
       << left  << setw(68) << outpoint.str() << " | "
       << right << setw(15) << "";
    ss << " ";
    return ss.str();
}

// TxOuts
inline std::string formattedTxOutHeader()
{
    using namespace std;

    stringstream ss;
    ss << " ";
    ss << right << setw(6)  << "output" << " | "
       << right << setw(15) << "value" << " | "
       << left  << setw(50) << "script" << " | "
       << left  << setw(36) << "address" << " | "
       << left  << setw(7)  << "status";
    ss << " ";

    size_t header_length = ss.str().size();
    ss << endl;
    for (size_t i = 0; i < header_length; i++) { ss << "="; }
    return ss.str();
}

inline std::string formattedTxOut(const std::shared_ptr<CoinDB::TxOut>& txout)
{
    using namespace std;
    using namespace CoinDB;

    string status = txout->receiving_account() ? status = TxOut::getStatusString(txout->status()) : "N/A";

    stringstream ss;
    ss << " ";
    ss << right << setw(6)  << txout->txindex() << " | "
       << right << setw(15) << txout->value() << " | "
       << left  << setw(50) << uchar_vector(txout->script()).getHex() << " | "
       << left  << setw(36) << getAddressFromScript(txout->script()) << " | "
       << left  << setw(7)  << status;
    ss << " ";
    return ss.str();
}

inline std::string formattedTxOutViewHeader()
{
    using namespace std;

    stringstream ss;
    ss << " ";
    ss << left  << setw(15) << "account name" << " | "
       << left  << setw(15) << "bin name" << " | "
       << left  << setw(10) << "type" << " | "
       << right << setw(15) << "value" << " | "
       << left  << setw(50) << "script" << " | "
       << left  << setw(36) << "address" << " | "
       << right << setw(13) << "confirmations" << " | "
       << left  << setw(11) << "tx status" << " | "
       << left  << setw(64) << "tx hash";
    ss << " ";

    size_t header_length = ss.str().size();
    ss << endl;
    for (size_t i = 0; i < header_length; i++) { ss << "="; }
    return ss.str();
}

inline std::string formattedTxOutView(const CoinDB::TxOutView& view, unsigned int best_height)
{
    using namespace std;
    using namespace CoinDB;

    bytes_t tx_hash = view.tx_status == Tx::UNSIGNED
        ? view.tx_unsigned_hash : view.tx_hash;

    unsigned int confirmations = view.block_height == 0
        ? 0 : best_height - view.block_height + 1;

    stringstream ss;
    ss << " ";
    ss << left  << setw(15) << view.role_account() << " | "
       << left  << setw(15) << view.role_bin() << " | "
       << left  << setw(10) << TxOut::getRoleString(view.role_flags) << " | "
       << right << setw(15) << view.value << " | "
       << left  << setw(50) << uchar_vector(view.script).getHex() << " | "
       << left  << setw(36) << getAddressFromScript(view.script) << " | "
       << right << setw(13) << confirmations << " | "
       << left  << setw(11) << Tx::getStatusString(view.tx_status) << " | "
       << left  << setw(64) << uchar_vector(tx_hash).getHex();
    ss << " ";
    return ss.str();
}

// Keychains
inline std::string formattedKeychainHeader()
{
    using namespace std;

    stringstream ss;
    ss << " ";
    ss << left  << setw(15) << "keychain name" << " | "
       << left  << setw(7)  << "type" << " | "
       << right << setw(5)  << "id" << " | "
       << left  << setw(40) << "hash";
    ss << " ";

    size_t header_length = ss.str().size();
    ss << endl;
    for (size_t i = 0; i < header_length; i++) { ss << "="; }
    return ss.str();
}

// TODO: KeychainView
inline std::string formattedKeychain(const std::shared_ptr<CoinDB::Keychain>& keychain)
{
    using namespace std;
    using namespace CoinDB;

    stringstream ss;
    ss << " ";
    ss << left  << setw(15) << keychain->name() << " | "
       << left  << setw(7)  << (keychain->isPrivate() ? "PRIVATE" : "PUBLIC") << " | "
       << right << setw(5)  << keychain->id() << " | "
       << left  << setw(40) << uchar_vector(keychain->hash()).getHex();
    ss << " ";
    return ss.str();
}

// Accounts
inline std::string formattedAccountHeader()
{
    using namespace std;

    stringstream ss;
    ss << " ";
    ss << left  << setw(15) << "account name" << " | "
       << right << setw(5)  << "id" << " | "
       << left  << setw(64) << "policy";
    ss << " ";

    size_t header_length = ss.str().size();
    ss << endl;
    for (size_t i = 0; i < header_length; i++) { ss << "="; }
    return ss.str();
}

inline std::string formattedAccount(const CoinDB::AccountInfo& info)
{
    using namespace std;
    using namespace CoinDB;

    stringstream policy;
    policy << info.minsigs() << " of " << stdutils:: delimited_list(info.keychain_names(), ", ");

    stringstream ss;
    ss << " ";
    ss << left  << setw(15) << info.name() << " | "
       << right << setw(5)  << info.id() << " | "
       << left  << setw(64) << policy.str();
    ss << " ";
    return ss.str();
}

