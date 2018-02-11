// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
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
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Trump’s tough talk may help Pakistan keep top talent February 5, 2018";
    const CScript genesisOutputScript = CScript() << ParseHex("0482fae82061c1400ca141ccb3831ee91bbb054495b75bd13a44c83e6bdff949eb69c76cce3d8957328cec83513f3a1f40d15b4f8a0dcbdd14555bdd3fe45649b4") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
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

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 52631579; // 40 billion divided by 760
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("0x62d4ee455f9850e354f5c85d42fba5852581877e032826dc07c0eb21545fff66");
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); 
        consensus.nPowTargetTimespan = 1.5 * 24 * 60 * 60; // 1.5 days
        consensus.nPowTargetSpacing = 1 * 60;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 6480; // 75% of 8064
        consensus.nMinerConfirmationWindow = 8640; // nPowTargetTimespan / nPowTargetSpacing * 4
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0; // 10/19/2017 @ 7:10am (UTC)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL; // 10/10/2018

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0; // January 28, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL; // January 31st, 2018

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0; // January 28, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL; // January 31st, 2018

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0000000000000000000000000000000000000000000000000000000000");

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
         pchMessageStart[0] = 0xac;
        pchMessageStart[1] = 0x3e;
        pchMessageStart[2] = 0xd3;
        pchMessageStart[3] = 0xfd;
        nDefaultPort = 9947;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1518374156, 722266, 0x1e0ffd00, 1, 760 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();

  /*
        printf("new mainnet genesis merkle root: %s\n", genesis.hashMerkleRoot.ToString().c_str());
        printf("new mainnet genesis nonce: %d\n", genesis.nNonce);
        printf("new mainnet genesis nBits: %d\n", genesis.nBits);
        printf("new mainnet genesis nTime: %d\n", genesis.nTime);
        printf("new mainnet genesis hash: %s\n", consensus.hashGenesisBlock.ToString().c_str());
*/
        assert(consensus.hashGenesisBlock == uint256S("0x62d4ee455f9850e354f5c85d42fba5852581877e032826dc07c0eb21545fff66"));
        assert(genesis.hashMerkleRoot == uint256S("0x0943c43bae650f17b8e0f16e46bdb4da40a20f35a31dc6b6d952d04da2d255f9"));


        // Note that of those with the service bits flag, most only support a subset of possible options
        vSeeds.push_back(CDNSSeedData("dnsseed.berycoin.com", "dnsseed.berycoin.com", true));
        vSeeds.push_back(CDNSSeedData("seed1.berycoin.com", "seed1.berycoin.com", true));
        vSeeds.push_back(CDNSSeedData("seed2.berycoin.com", "seed2.berycoin.com", true));


        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,25);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,20);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,50);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,216);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x05)(0x86)(0xc2)(0x2e).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x05)(0x86)(0xdc)(0xf1).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

	checkpointData = (CCheckpointData) {
		boost::assign::map_list_of
			(0, uint256S("0x62d4ee455f9850e354f5c85d42fba5852581877e032826dc07c0eb21545fff66")),
			0, // * UNIX timestamp of last checkpoint block
			0,   // * total number of transactions between genesis and last checkpoint
			//   (the tx=... number in the SetBestChain debug.log lines)
			5500.0     // * estimated number of transactions per day after checkpoint
	};

    }
};
static CMainParams mainParams;

/**
 * Testnet (v4)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 52631579;
        consensus.nMajorityEnforceBlockUpgrade = 51;
        consensus.nMajorityRejectBlockOutdated = 75;
        consensus.nMajorityWindow = 100;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("0xc362aad8c1f4323f7d8ca29d6763874ec9122b55262bfbb02905ceb2adec55b5");
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 3.5 * 24 * 60 * 60; // 3.5 days
        consensus.nPowTargetSpacing = 2.5 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0; // January 1, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL; // January 31st, 2018

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0; // January 1, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL; // January 31st, 2018

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x");

        pchMessageStart[0] = 0xac;
        pchMessageStart[1] = 0xb2;
        pchMessageStart[2] = 0xd5;
        pchMessageStart[3] = 0x2c;
        nDefaultPort = 19947;
        nPruneAfterHeight = 1000;


        genesis = CreateGenesisBlock(1518375437, 3738177, 0x1e0aff00, 1, 760 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();


        assert(consensus.hashGenesisBlock == uint256S("0xc362aad8c1f4323f7d8ca29d6763874ec9122b55262bfbb02905ceb2adec55b5"));
        assert(genesis.hashMerkleRoot == uint256S("0x0943c43bae650f17b8e0f16e46bdb4da40a20f35a31dc6b6d952d04da2d255f9"));

        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,11);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,106);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,58);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,229);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x05)(0x37)(0x82)(0xBF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x05)(0x37)(0x84)(0xA4).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;


        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            ( 0, uint256S("0xc362aad8c1f4323f7d8ca29d6763874ec9122b55262bfbb02905ceb2adec55b5")),
            0,
            0,
            576
        };

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
        consensus.nSubsidyHalvingInterval = 150;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = -1; // BIP34 has not necessarily activated on regtest
        consensus.BIP34Hash = uint256();
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 3.5 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 2.5 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        pchMessageStart[0] = 0xaa;
        pchMessageStart[1] = 0xbd;
        pchMessageStart[2] = 0xaf;
        pchMessageStart[3] = 0xd1;
        nDefaultPort = 19447;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1518377271, 328258, 0x1e0a1f44, 1, 760 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();


        assert(consensus.hashGenesisBlock == uint256S("0x22a9444f2bd4b8c3551467fc369f5cfe6c2fa29dd94500f435fb5f7e492b1beb"));
        assert(genesis.hashMerkleRoot == uint256S("0x0943c43bae650f17b8e0f16e46bdb4da40a20f35a31dc6b6d952d04da2d255f9"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true; 
        fTestnetToBeDeprecatedFieldRPC = false;

	checkpointData = (CCheckpointData){
		boost::assign::map_list_of
			( 0, uint256S("0x22a9444f2bd4b8c3551467fc369f5cfe6c2fa29dd94500f435fb5f7e492b1beb")),
			0,
			0,
			0
	};
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,113);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,123);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,58);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,202);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x05)(0x39)(0x81)(0xAB).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x05)(0x39)(0x85)(0x2C).convert_to_container<std::vector<unsigned char> >();
    }

    void UpdateBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
    {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
            return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
            return testNetParams;
    else if (chain == CBaseChainParams::REGTEST)
            return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

void UpdateRegtestBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    regTestParams.UpdateBIP9Parameters(d, nStartTime, nTimeout);
}
 
