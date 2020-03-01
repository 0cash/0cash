// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "key_io.h"
#include "main.h"
#include "crypto/equihash.h"

#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, const uint256& nNonce, const std::vector<unsigned char>& nSolution, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    // To create a genesis block for a new chain which is Overwintered:
    //   txNew.nVersion = OVERWINTER_TX_VERSION
    //   txNew.fOverwintered = true
    //   txNew.nVersionGroupId = OVERWINTER_VERSION_GROUP_ID
    //   txNew.nExpiryHeight = <default value>
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nSolution = nSolution;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = genesis.BuildMerkleTree();
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database (and is in any case of zero value).
 *
 * >>> from pyblake2 import blake2s
 * >>> '0cash' + blake2s(b'0cash - decentralized electronic cash. BTC#616634 - 00000000000000000007113e79edb30157cf555e802f715bc78631a507e1fdd4').hexdigest()
 */
static CBlock CreateGenesisBlock(uint32_t nTime, const uint256& nNonce, const std::vector<unsigned char>& nSolution, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "0cashcb05b56932e0e7a51331b4701c80271c3ead3c5ebdf0999f5eeefb967b48eb0a";
    const CScript genesisOutputScript = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nSolution, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

const arith_uint256 maxUint = UintToArith256(uint256S("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"));


class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        strCurrencyUnits = "ZCH";
        bip44CoinType = 177; // As registered in https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        consensus.fCoinbaseMustBeProtected = true;
        consensus.nSubsidySlowStartInterval = 0;
        consensus.nSubsidyHalvingInterval = 840000;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 4000;
        consensus.powLimit = uint256S("0007ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowAveragingWindow = 13;
        assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow);
        consensus.nPowMaxAdjustDown = 34;
        consensus.nPowMaxAdjustUp = 34;
        consensus.nPowTargetSpacing = 2.5 * 60;
        consensus.nPowAllowMinDifficultyBlocksAfterHeight = boost::none;
        consensus.vUpgrades[Consensus::BASE_SPROUT].nProtocolVersion = 170002;
        consensus.vUpgrades[Consensus::BASE_SPROUT].nActivationHeight =
            Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nProtocolVersion = 170002;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight =
            Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_OVERWINTER].nProtocolVersion = 770006;
        consensus.vUpgrades[Consensus::UPGRADE_OVERWINTER].nActivationHeight = 1;
        consensus.vUpgrades[Consensus::UPGRADE_SAPLING].nProtocolVersion = 770006;
        consensus.vUpgrades[Consensus::UPGRADE_SAPLING].nActivationHeight = 1;
        consensus.vUpgrades[Consensus::UPGRADE_BLOSSOM].nProtocolVersion = 770009;
        consensus.vUpgrades[Consensus::UPGRADE_BLOSSOM].nActivationHeight =
            Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;

        // The best chain should have at least this much work.
        // consensus.nMinimumChainWork = uint256S("0x00000000000000000000000000000000000000000000000000a95cc5099213e3");

        /**
         * The message start string should be awesome! ⓩ❤
         */
        pchMessageStart[0] = 0x24;
        pchMessageStart[1] = 0xe9;
        pchMessageStart[2] = 0x27;
        pchMessageStart[3] = 0x64;
        vAlertPubKey = ParseHex("04d5212ed0303c64db1840e799d31953eb362fd71d8e742dccd9aa78c4713d6d26b44974b44e2ac71aa38b06ef60c020207b85d270e4bdf8c797f3216f969960dc");
        nDefaultPort = 1847;
        nMaxTipAge = 24 * 60 * 60;
        nPruneAfterHeight = 100000;
        eh_epoch_1 = eh200_9;
        eh_epoch_2 = eh144_5;
        eh_epoch_1_endblock = 104;
        eh_epoch_2_startblock = 104;

        vRollingCheckpointStartHeight = 5;

        genesis = CreateGenesisBlock(
            1581246595,
            uint256S("0x0000000000000000000000000000000000000000000000000000000000001642"),
            ParseHex(""),
            0x1f07ffff, 4, 0);
        consensus.hashGenesisBlock = genesis.GetHash();

        assert(consensus.hashGenesisBlock == uint256S("0x0006a5d89fc283f12bed00b35248cfdcd2962e876d2c7112bb28b8596a033e71")); //incremented by 1 making 2
        assert(genesis.hashMerkleRoot == uint256S("0xcfa9d7248178109dad5034590b861057b6c658cbb210b903c8c449fc1432e009"));

        // guarantees the first 2 characters, when base58 encoded, are "t1"
        base58Prefixes[PUBKEY_ADDRESS]     = {0x1C,0xB8};
        // guarantees the first 2 characters, when base58 encoded, are "t3"
        base58Prefixes[SCRIPT_ADDRESS]     = {0x1C,0xBD};
        // the first character, when base58 encoded, is "5" or "K" or "L" (as in Bitcoin)
        base58Prefixes[SECRET_KEY]         = {0x80};
        // do not rely on these BIP32 prefixes; they are not specified and may change
        base58Prefixes[EXT_PUBLIC_KEY]     = {0x04,0x88,0xB2,0x1E};
        base58Prefixes[EXT_SECRET_KEY]     = {0x04,0x88,0xAD,0xE4};
        // guarantees the first 2 characters, when base58 encoded, are "zc"
        base58Prefixes[ZCPAYMENT_ADDRRESS] = {0x16,0x9A};
        // guarantees the first 4 characters, when base58 encoded, are "ZiVK"
        base58Prefixes[ZCVIEWING_KEY]      = {0xA8,0xAB,0xD3};
        // guarantees the first 2 characters, when base58 encoded, are "SK"
        base58Prefixes[ZCSPENDING_KEY]     = {0xAB,0x36};

        bech32HRPs[SAPLING_PAYMENT_ADDRESS]      = "zs";
        bech32HRPs[SAPLING_FULL_VIEWING_KEY]     = "zviews";
        bech32HRPs[SAPLING_INCOMING_VIEWING_KEY] = "zivks";
        bech32HRPs[SAPLING_EXTENDED_SPEND_KEY]   = "secret-extended-key-main";

	vFixedSeeds.clear();
        vSeeds.clear();

        vSeeds.push_back(CDNSSeedData("0cashnode1", "0cashnode1.xyz"));
	vSeeds.push_back(CDNSSeedData("0cashnode2", "0cashnode2.xyz"));
	vSeeds.push_back(CDNSSeedData("0cashnode3", "0cashnode3.xyz"));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            ( 0, uint256S("0x0006a5d89fc283f12bed00b35248cfdcd2962e876d2c7112bb28b8596a033e71")),

            1581246595,     // * UNIX timestamp of last checkpoint block
            0,         // * total number of transactions between genesis and last checkpoint
                            //   (the tx=... number in the SetBestChain debug.log lines)
            0  // * estimated number of transactions per day after checkpoint
                            //   total number of tx / (checkpoint block height / (24 * 24))
        };

        // Hardcoded fallback value for the Sprout shielded value pool balance
        // for nodes that have not reindexed since the introduction of monitoring
        // in #2795.
        // nSproutValuePoolCheckpointHeight = 520633;
        // nSproutValuePoolCheckpointBalance = 22145062442933;
        fZIP209Enabled = true;
        // hashSproutValuePoolCheckpointBlock = uint256S("0000000000c7b46b6bc04b4cbf87d8bb08722aebd51232619b214f7273f8460e");

        // Community Fee script expects a vector of 2-of-3 multisig addresses
        vCommunityFeeAddress = {
            "t3gHuqS6FnewvKRPZFXFmhfbPb9Nvfbsrg6",
            "t3SyzgMMrV3EePKnr2wiw65nBHbmN8GzjvA",
            "t3RqH7JhqEB2ERULekR5WRSuxzJiGPEjqKT",
            "t3curSWoCnLqhgJTEcnX1qfW6wEsGMEYnGi",
            "t3ZxeGaz5PJQXQwFKoSesPxgeqXjyPBoa5h",
            "t3beRemdNB9EmGr5bsCeZe8iQk4jgAXWzZs",
            "t3gQpEyktxxj6yxGUaeLjfzP1JGwSAuuL7o",
            "t3gX34RAqpxVSnFcm4Ak3D776kNJjkHKsWG",
            "t3MS6XVB2e8FRppvzYfEHUjuYnUSh4QJvb3",
            "t3UJW9S2yXH6HhDzAxqxR8BZCrfM9Vd8po6"
        };
        vCommunityFeeStartHeight = 2;
        vCommunityFeeLastHeight = 1000000;
        assert(vCommunityFeeAddress.size() <= GetLastCommunityFeeBlockHeight());
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        strCurrencyUnits = "TZB";
        bip44CoinType = 1;
        consensus.fCoinbaseMustBeProtected = true;
        consensus.nSubsidySlowStartInterval = 0;
        consensus.nSubsidyHalvingInterval = 840000;
        consensus.nMajorityEnforceBlockUpgrade = 51;
        consensus.nMajorityRejectBlockOutdated = 75;
        consensus.nMajorityWindow = 400;
        consensus.powLimit = uint256S("07ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowAveragingWindow = 13;
        assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow);
        consensus.nPowMaxAdjustDown = 34;
        consensus.nPowMaxAdjustUp = 34;
        consensus.nPowTargetSpacing = 2.5 * 60;
        consensus.nPowAllowMinDifficultyBlocksAfterHeight = boost::none;
        consensus.vUpgrades[Consensus::BASE_SPROUT].nProtocolVersion = 170002;
        consensus.vUpgrades[Consensus::BASE_SPROUT].nActivationHeight =
            Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nProtocolVersion = 170002;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight =
            Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_OVERWINTER].nProtocolVersion = 770006;
        consensus.vUpgrades[Consensus::UPGRADE_OVERWINTER].nActivationHeight = 1500;
        consensus.vUpgrades[Consensus::UPGRADE_SAPLING].nProtocolVersion = 770006;
        consensus.vUpgrades[Consensus::UPGRADE_SAPLING].nActivationHeight = 1500;
        consensus.vUpgrades[Consensus::UPGRADE_BLOSSOM].nProtocolVersion = 770008;
        consensus.vUpgrades[Consensus::UPGRADE_BLOSSOM].nActivationHeight =
            Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000005000");

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0x1a;
        pchMessageStart[2] = 0xf9;
        pchMessageStart[3] = 0xbf;
        vAlertPubKey = ParseHex("048679fb891b15d0cada9692047fd0ae26ad8bfb83fabddbb50334ee5bc0683294deb410be20513c5af6e7b9cec717ade82b27080ee6ef9a245c36a795ab044bb3");
        nDefaultPort = 11847;
        nMaxTipAge = 24 * 60 * 60;
        nPruneAfterHeight = 1000;
        eh_epoch_1 = eh200_9;
        eh_epoch_2 = eh144_5;
        eh_epoch_1_endblock = 1210;
        eh_epoch_2_startblock = 1200;

        futureBlockTimeWindows = boost::assign::map_list_of
            ( 0, 2 * 60 ) // originally 2 hours
            ( 13999, 30 ) // 30 minutes
            ( 14000, 5 ); // 5 minutes

        vRollingCheckpointStartHeight = 14000;

        genesis = CreateGenesisBlock(
            1479443947,
            uint256S("0x0000000000000000000000000000000000000000000000000000000000000013"),
            ParseHex("002b24e10a5d2ab32b053a20ca6ebed779be1d935b1500eeea5c87aec684c6f934196fdfca6539de0cf1141544bffc5c0d1d4bab815fb5d8c2b195ccdf0755599ee492b9d98e3b79a178949f45485ad80dba38ec0461102adaa369b757ebb2bf8d75b5f67a341d666406d862a102c69800f20a7075be360a7eb2d315d78e4ce32c741f3baf7bf3e1e651976f734f367b1f126f62503b34d06d6e99b3659b2a47f5cfcf71c87e24e5023151d4af87454e7638a19b846350dd5fbc53e4ce1cce2597992b36cbcae0c24717e412c8df9ddca3e90c7629bd8c157c66d8906486943cf78e24d55dd4152f45eff49acf9fb9fddef81f2ee55892b38db940c404eaacf819588b83f0f761f1ba5b31a0ea1f8f4c5210638bbb59a2d8ddff9535f546b42a7eac5f3ee87616a075bddc3118b7f2c041f4b1e8dbcd11eea95835403066b5bb50cd23122dcb12166d75aafcfc1ca8f30580b4d48a5aa305657a06b4b650ed4633f2fa496235082feff65f70e19871f41b70632b53e57ddf38c207d631e5a56fa50bb71150f99427f73d82a439a5f70dfc7d8bbfc39d330ca7924527a5deb8950b9fa7020cfde5e07b84546e96764519ef6dd3fdc3a974abd342bdc7e4ee76bc11d5519541015afba1a0517fd347196aa326b0905a5916b83515c16f8f13105479c29f1eff3bc024ddbb07dcc672247cedc0d4ba32332ead0f13c58f50170642e16e076c34f5e75e3e8f5ac7f5238d67564fd385efecf972b0abf939a99bc7ef8f3a21cac21d2168706bbad3f4af66bb01cf61cfbc352a23797b62dcb5480bf2b7b277af233f5ce42a144d47119a89e1d114fa0bec2f13475b6b1df907bc3a429f1771afa3857bf16bfca3f76a5df14da62dc157fff4225bda73c3cfefa989edc24673bf932a024593da4c38b1a4628dd77ad919f4f7b7fb76976e696db69c89016ab30d9aa2d509f78d913d00ca9ac881aa759fc019b8c5e3eac6fddb4e0f044595e10d4997e29c79800f77cf1d97583d534db0f2726cba3739e7371eeffa2aca12b0d290ac45f44973f32f7675a5b49c94c4b608da2926555d16b7eb3670e12345a63f88797e5a5e21252c2c9463d7896001031a81bac0354336b35c5a10c93d9ae3054f6f6e4492f7c1f09a9d75034d5d0b220a9bb231e583659d5b6923a4e879326194de5c9805a02cb648508a8f9b6cd26dc17d322a478c1c599e1ec3adf2da6ce7a7e3a073b55cf30cf6b124f7700409abe14af8c60ab178579623916f165dbfd26f37056bf33c34f3af30939e1277376e4c5cba339f36381a05ef6481db033fb4c07a19e8655f8b12f9ab3c602e127b4ab1ee48e1c6a91382b54ed36ef9bb21b3bfa80a9107864dcb594dcad250e402b312607e648639631a3d1aeb17cfe3370202720ca8a46db15af92e8b46062b5bd035b24c35a592e5620d632faf1bf19a86df179fe52dd4cdbecd3cb7a336ca7489e4d1dc9433f1163c89d88c5eac36fc562496dc7583fe67c559c9a71cf89e9a0a59d5a14764926852d44a88d2ddb361d612ec06f9de874473eaf1d36b3a41911ac072b7826e6acea3d8425dc271833dba2ec17d1a270e49becbf21330ba2f0edc4b05f4df01623f3c82246ae23ea2c022434ef09611aa19ba35c3ecbad965af3ad9bc6c9b0d3b059c239ffbf9272d0150c151b4510d659cbd0e4a9c32945c612681b70ee4dcbeefeacde630b127115fd9af16cef4afefe611c9dfcc63e6833bf4dab79a7e1ae3f70321429557ab9da48bf93647830b5eb5780f23476d3d4d06a39ae532da5b2f30f151587eb5df19ec1acf099e1ac506e071eb52c3c3cc88ccf6622b2913acf07f1b772b5012e39173211e51773f3eb42d667fff1d902c5c87bd507837b3fd993e70ac9706a0"),
            0x2007ffff, 4, 0);
        consensus.hashGenesisBlock = genesis.GetHash();

        assert(consensus.hashGenesisBlock == uint256S("0x" + consensus.hashGenesisBlock.ToString()));

        vFixedSeeds.clear();
        vSeeds.clear();
        //vSeeds.push_back(CDNSSeedData("rotorproject.org", "test-dnsseed.rotorproject.org")); // Zclassic

        // guarantees the first 2 characters, when base58 encoded, are "tm"
        base58Prefixes[PUBKEY_ADDRESS]     = {0x1D,0x25};
        // guarantees the first 2 characters, when base58 encoded, are "t2"
        base58Prefixes[SCRIPT_ADDRESS]     = {0x1C,0xBA};
        // the first character, when base58 encoded, is "9" or "c" (as in Bitcoin)
        base58Prefixes[SECRET_KEY]         = {0xEF};
        // do not rely on these BIP32 prefixes; they are not specified and may change
        base58Prefixes[EXT_PUBLIC_KEY]     = {0x04,0x35,0x87,0xCF};
        base58Prefixes[EXT_SECRET_KEY]     = {0x04,0x35,0x83,0x94};
        // guarantees the first 2 characters, when base58 encoded, are "zt"
        base58Prefixes[ZCPAYMENT_ADDRRESS] = {0x16,0xB6};
        // guarantees the first 4 characters, when base58 encoded, are "ZiVt"
        base58Prefixes[ZCVIEWING_KEY]      = {0xA8,0xAC,0x0C};
        // guarantees the first 2 characters, when base58 encoded, are "ST"
        base58Prefixes[ZCSPENDING_KEY]     = {0xAC,0x08};

        bech32HRPs[SAPLING_PAYMENT_ADDRESS]      = "ztestsapling";
        bech32HRPs[SAPLING_FULL_VIEWING_KEY]     = "zviewtestsapling";
        bech32HRPs[SAPLING_INCOMING_VIEWING_KEY] = "zivktestsapling";
        bech32HRPs[SAPLING_EXTENDED_SPEND_KEY]   = "secret-extended-key-test";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;


        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            ( 0, consensus.hashGenesisBlock),
            genesis.nTime,
            0,
            0
        };

        // Hardcoded fallback value for the Sprout shielded value pool balance
        // for nodes that have not reindexed since the introduction of monitoring
        // in #2795.
        // nSproutValuePoolCheckpointHeight = 440329;
        // nSproutValuePoolCheckpointBalance = 40000029096803;
        fZIP209Enabled = true;
        // hashSproutValuePoolCheckpointBlock = uint256S("000a95d08ba5dcbabe881fc6471d11807bcca7df5f1795c99f3ec4580db4279b");

        // Founders reward script expects a vector of 2-of-3 multisig addresses
        vCommunityFeeAddress = {
            "t2FHbHM9rKKHZe74HRBNozwNdRsExug8tCw",
            "t29tM6DkMPSVp9R3g7UjZjvsobKhsbsRqFL",
            "t2K2KixLVJo19phPJMv9ApSiFmxQCSQUvc9",
            "t2AWJcGVUMWFC8A9KC3PL7qoCb1vxSzxbJP",
            "t26p8FyjHmhqZ6duzhRFLCQcExh1TuCD1sC",
            "t27x5n41uRNF3tJkb3Lg1CMomUjTNZwtUfm",
            "t2VhRQJ9xeVkVVk7ic21CtDePKmHnrDyF8Z",
            "t27hL1iAsTHBPWrdc1qYGSSTc3pTyBqohd4",
            "t2RqLYWG8Eo4hopDsn1m8GUoAWtjZQEPE9s",
            "t2V1osVDkcwYFL4PF9qG8t9Ez1XRVMAkAb6"
        };
        vCommunityFeeStartHeight = 1500;
        vCommunityFeeLastHeight = 1400000;
        assert(vCommunityFeeAddress.size() <= GetLastCommunityFeeBlockHeight());
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        strCurrencyUnits = "REG";
        bip44CoinType = 1;
        consensus.fCoinbaseMustBeProtected = false;
        consensus.nSubsidySlowStartInterval = 0;
        consensus.nSubsidyHalvingInterval = 150;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.powLimit = uint256S("0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f");
        consensus.nPowAveragingWindow = 13;
        assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow);
        consensus.nPowMaxAdjustDown = 0; // Turn off adjustment down
        consensus.nPowMaxAdjustUp = 0; // Turn off adjustment up
        consensus.nPowTargetSpacing = 2.5 * 60;
        consensus.nPowAllowMinDifficultyBlocksAfterHeight = 0;
        consensus.vUpgrades[Consensus::BASE_SPROUT].nProtocolVersion = 170002;
        consensus.vUpgrades[Consensus::BASE_SPROUT].nActivationHeight =
            Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nProtocolVersion = 170002;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight =
            Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_OVERWINTER].nProtocolVersion = 770006;
        consensus.vUpgrades[Consensus::UPGRADE_OVERWINTER].nActivationHeight =
            Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_SAPLING].nProtocolVersion = 170006;
        consensus.vUpgrades[Consensus::UPGRADE_SAPLING].nActivationHeight =
            Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_BLOSSOM].nProtocolVersion = 170008;
        consensus.vUpgrades[Consensus::UPGRADE_BLOSSOM].nActivationHeight =
            Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        pchMessageStart[0] = 0xaa;
        pchMessageStart[1] = 0xe8;
        pchMessageStart[2] = 0x3f;
        pchMessageStart[3] = 0x5f;
        nDefaultPort = 11847;
        nMaxTipAge = 24 * 60 * 60;
        //assert(consensus.hashGenesisBlock == uint256S("0x0575f78ee8dc057deee78ef691876e3be29833aaee5e189bb0459c087451305a"));
        nPruneAfterHeight = 1000;
        eh_epoch_1 = eh48_5;
        eh_epoch_2 = eh48_5;
        eh_epoch_1_endblock = 1;
        eh_epoch_2_startblock = 1;

        futureBlockTimeWindows = boost::assign::map_list_of
            ( 0, 2 * 60 ) // originally 2 hours
            ( 159300, 30 ) // 30 minutes
            ( 364400, 5 ); // 5 minutes

        vRollingCheckpointStartHeight = 364400;

        genesis = CreateGenesisBlock(
            1482971059,
            uint256S("0x0000000000000000000000000000000000000000000000000000000000000009"),
            ParseHex("05ffd6ad016271ade20cfce093959c3addb2079629f9f123c52ef920caa316531af5af3f"),
            0x200f0f0f, 4, 0);
        consensus.hashGenesisBlock = genesis.GetHash();
        //assert(consensus.hashGenesisBlock == uint256S("0x029f11d80ef9765602235e1bc9727e3eb6ba20839319f761fee920d63401e327"));
        //assert(genesis.hashMerkleRoot == uint256S("0xc4eaa58879081de3c24a7b117ed2b28300e7ec4c4c1dff1d3f1268b7857a4ddb"));

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.
        //vSeeds.push_back(CDNSSeedData("rotorproject.org", "test-dnsseed.rotorproject.org")); // Zclassic

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("0x0575f78ee8dc057deee78ef691876e3be29833aaee5e189bb0459c087451305a")),
            0,
            0,
            0
        };

                // These prefixes are the same as the testnet prefixes
        base58Prefixes[PUBKEY_ADDRESS]     = {0x1D,0x25};
        base58Prefixes[SCRIPT_ADDRESS]     = {0x1C,0xBA};
        base58Prefixes[SECRET_KEY]         = {0xEF};
        // do not rely on these BIP32 prefixes; they are not specified and may change
        base58Prefixes[EXT_PUBLIC_KEY]     = {0x04,0x35,0x87,0xCF};
        base58Prefixes[EXT_SECRET_KEY]     = {0x04,0x35,0x83,0x94};
        base58Prefixes[ZCPAYMENT_ADDRRESS] = {0x16,0xB6};
        base58Prefixes[ZCVIEWING_KEY]      = {0xA8,0xAC,0x0C};
        base58Prefixes[ZCSPENDING_KEY]     = {0xAC,0x08};

        bech32HRPs[SAPLING_PAYMENT_ADDRESS]      = "zregtestsapling";
        bech32HRPs[SAPLING_FULL_VIEWING_KEY]     = "zviewregtestsapling";
        bech32HRPs[SAPLING_INCOMING_VIEWING_KEY] = "zivkregtestsapling";
        bech32HRPs[SAPLING_EXTENDED_SPEND_KEY]   = "secret-extended-key-regtest";

        // Founders reward script expects a vector of 2-of-3 multisig addresses
        vCommunityFeeAddress = {
            "t2FHbHM9rKKHZe74HRBNozwNdRsExug8tCw",
            "t29tM6DkMPSVp9R3g7UjZjvsobKhsbsRqFL",
            "t2K2KixLVJo19phPJMv9ApSiFmxQCSQUvc9",
            "t2AWJcGVUMWFC8A9KC3PL7qoCb1vxSzxbJP",
            "t26p8FyjHmhqZ6duzhRFLCQcExh1TuCD1sC",
            "t27x5n41uRNF3tJkb3Lg1CMomUjTNZwtUfm",
            "t2VhRQJ9xeVkVVk7ic21CtDePKmHnrDyF8Z",
            "t27hL1iAsTHBPWrdc1qYGSSTc3pTyBqohd4",
            "t2RqLYWG8Eo4hopDsn1m8GUoAWtjZQEPE9s",
            "t2V1osVDkcwYFL4PF9qG8t9Ez1XRVMAkAb6"
        };
        vCommunityFeeStartHeight = 200;
        vCommunityFeeLastHeight = 1400000;
        assert(vCommunityFeeAddress.size() <= GetLastCommunityFeeBlockHeight());
    }

    void UpdateNetworkUpgradeParameters(Consensus::UpgradeIndex idx, int nActivationHeight)
    {
        assert(idx > Consensus::BASE_SPROUT && idx < Consensus::MAX_NETWORK_UPGRADES);
        consensus.vUpgrades[idx].nActivationHeight = nActivationHeight;
    }

    void SetRegTestZIP209Enabled() {
        fZIP209Enabled = true;
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams &Params(CBaseChainParams::Network network) {
    switch (network) {
        case CBaseChainParams::MAIN:
            return mainParams;
        case CBaseChainParams::TESTNET:
            return testNetParams;
        case CBaseChainParams::REGTEST:
            return regTestParams;
        default:
            assert(false && "Unimplemented network");
            return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network) {
    SelectBaseParams(network);
    pCurrentParams = &Params(network);

    // Some python qa rpc tests need to enforce the coinbase consensus rule
    if (network == CBaseChainParams::REGTEST && mapArgs.count("-regtestprotectcoinbase")) {
        regTestParams.SetRegTestCoinbaseMustBeProtected();
    }

    // When a developer is debugging turnstile violations in regtest mode, enable ZIP209
    if (network == CBaseChainParams::REGTEST && mapArgs.count("-developersetpoolsizezero")) {
        regTestParams.SetRegTestZIP209Enabled();
    }
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}

// Index variable i ranges from 0 - (vCommunityFeeAddress.size()-1)
std::string CChainParams::GetCommunityFeeAddressAtHeight(int nHeight) const {
    int maxHeight = GetLastCommunityFeeBlockHeight();
    assert(nHeight > 0 && nHeight <= maxHeight);

    size_t addressChangeInterval = (maxHeight + vCommunityFeeAddress.size()) / vCommunityFeeAddress.size();
    size_t i = nHeight / addressChangeInterval;
    return vCommunityFeeAddress[i];
}

// Block height must be >0 and <=last founders reward block height
// The founders reward address is expected to be a multisig (P2SH) address
CScript CChainParams::GetCommunityFeeScriptAtHeight(int nHeight) const {
    assert(nHeight > 0 && nHeight <= GetLastCommunityFeeBlockHeight());

    CTxDestination address = DecodeDestination(GetCommunityFeeAddressAtHeight(nHeight).c_str());
    assert(IsValidDestination(address));
    assert(boost::get<CScriptID>(&address) != nullptr);
    CScriptID scriptID = boost::get<CScriptID>(address); // address is a boost variant
    CScript script = CScript() << OP_HASH160 << ToByteVector(scriptID) << OP_EQUAL;
    return script;
}

std::string CChainParams::GetCommunityFeeAddressAtIndex(int i) const {
    assert(i >= 0 && i < vCommunityFeeAddress.size());
    return vCommunityFeeAddress[i];
}

void UpdateNetworkUpgradeParameters(Consensus::UpgradeIndex idx, int nActivationHeight)
{
    regTestParams.UpdateNetworkUpgradeParameters(idx, nActivationHeight);
}

int validEHparameterList(EHparameters *ehparams, unsigned long blockheight, const CChainParams& params){
    //if in overlap period, there will be two valid solutions, else 1.
    //The upcoming version of EH is preferred so will always be first element
    //returns number of elements in list
    if(blockheight >= params.eh_epoch_2_start() && blockheight > params.eh_epoch_1_end()){
        ehparams[0] = params.eh_epoch_2_params();
        return 1;
    }
    if(blockheight < params.eh_epoch_2_start()){
        ehparams[0] = params.eh_epoch_1_params();
        return 1;
    }
    ehparams[0] = params.eh_epoch_2_params();
    ehparams[1] = params.eh_epoch_1_params();
    return 2;
}

bool checkEHParamaters(int solSize, int height, const CChainParams& params) {
    // Block will be validated prior to mining, and will have a zero length
    // equihash solution. These need to be let through.
    if (height == 0) {
        return true;
    }

    //allocate on-stack space for parameters list
    EHparameters ehparams[MAX_EH_PARAM_LIST_LEN];
    int listlength = validEHparameterList(ehparams, height, params);
    for(int i = 0; i < listlength; i++){
        LogPrint("pow", "checkEHParamaters height: %d n:%d k:%d solsize: %d \n", 
            height, ehparams[i].n, ehparams[i].k, ehparams[i].nSolSize);
        if (ehparams[i].nSolSize == solSize)
            return true;
    }

    return false;
}

int CChainParams::GetFutureBlockTimeWindow(int height) const {
    return 300;
}
