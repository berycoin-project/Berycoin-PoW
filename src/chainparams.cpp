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
        consensus.nSubsidyHalvingInterval = 200000000; // 40 billion divided by 200
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("0xff23a3590cc6e308f1817283cfa807793aae41d6a1de9f30ed15b7e8361d7005");
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); 
        consensus.nPowTargetTimespan = 1.5 * 24 * 60 * 60; // 1.5 days
        consensus.nPowTargetSpacing = 1 * 60;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 6480; // 75% of 8064
        consensus.nMinerConfirmationWindow = 8640; // nPowTargetTimespan / nPowTargetSpacing * 4
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0; // 1/1/1970
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 7258118400; // January 1st, 2200

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0; // 1/1/1970
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 7258118400; // January 1st, 2200

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0; // 1/1/1970
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 7258118400; // January 1st, 2200

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

        genesis = CreateGenesisBlock(1522822110, 621185, 0x1e0ffd00, 1, 200 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();

  /*
        printf("new mainnet genesis merkle root: %s\n", genesis.hashMerkleRoot.ToString().c_str());
        printf("new mainnet genesis nonce: %d\n", genesis.nNonce);
        printf("new mainnet genesis nBits: %d\n", genesis.nBits);
        printf("new mainnet genesis nTime: %d\n", genesis.nTime);
        printf("new mainnet genesis hash: %s\n", consensus.hashGenesisBlock.ToString().c_str());
*/
        assert(consensus.hashGenesisBlock == uint256S("0xff23a3590cc6e308f1817283cfa807793aae41d6a1de9f30ed15b7e8361d7005"));
        assert(genesis.hashMerkleRoot == uint256S("0x15f721c969cb8b4115363024d64ddffc57f7167e4dcbc9cfb89d12f19b59f4e4"));


        // Note that of those with the service bits flag, most only support a subset of possible options
        vSeeds.push_back(CDNSSeedData("dnsseed.berycoin.com", "dnsseed.berycoin.com", true));
        vSeeds.push_back(CDNSSeedData("dnsseed.berycoin.org", "dnsseed.berycoin.org", true));
        vSeeds.push_back(CDNSSeedData("bago.resteemexposure.com", "bago.resteemexposure.com", true));
	vSeeds.push_back(CDNSSeedData("bilal.myfxstore.com", "bilal.myfxstore.com", true));


        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,25);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,20);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,51);
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
			(  0, uint256S("0xff23a3590cc6e308f1817283cfa807793aae41d6a1de9f30ed15b7e8361d7005"))
			( 4700, uint256S("822f46e6c93fafc4aa340b07bf684843346fac32674b2b4920ae271d67f1b722"))
			( 8200, uint256S("975f7aaf5c0676e457a05bea3e4a2a269b79481411ab9c9aa568a6d5e21a4ce4"))
			(24916, uint256S("4b5c69fc33b030e1b74e0eb747ba90d4335b56e3cc10ec0c42d3ea230948465b")),
			1525273669, // * UNIX timestamp of last checkpoint block
			26906,   // * total number of transactions between genesis and last checkpoint
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
        consensus.nSubsidyHalvingInterval = 200000000;
        consensus.nMajorityEnforceBlockUpgrade = 51;
        consensus.nMajorityRejectBlockOutdated = 75;
        consensus.nMajorityWindow = 100;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("0xfa211189d78247c5173828cdf035a808a69d74294022d0d5d170d707544d7ba8");
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 3.5 * 24 * 60 * 60; // 3.5 days
        consensus.nPowTargetSpacing = 2.5 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0; // 1/1/1970
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 7258118400; // January 1st, 2200

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0; // 1/1/1970
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 7258118400; // January 1st, 2200

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0; // 1/1/1970
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 7258118400; // January 1st, 2200

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x");

        pchMessageStart[0] = 0xac;
        pchMessageStart[1] = 0xb2;
        pchMessageStart[2] = 0xd5;
        pchMessageStart[3] = 0x2c;
        nDefaultPort = 19947;
        nPruneAfterHeight = 1000;


        genesis = CreateGenesisBlock(1522822929, 186618, 0x1e0aff00, 1, 200 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();


        assert(consensus.hashGenesisBlock == uint256S("0xfa211189d78247c5173828cdf035a808a69d74294022d0d5d170d707544d7ba8"));
        assert(genesis.hashMerkleRoot == uint256S("0x15f721c969cb8b4115363024d64ddffc57f7167e4dcbc9cfb89d12f19b59f4e4"));

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
            ( 0, uint256S("0xfa211189d78247c5173828cdf035a808a69d74294022d0d5d170d707544d7ba8")),
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

        genesis = CreateGenesisBlock(1522823194, 5551720, 0x1e0a1f44, 1, 200 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();


        assert(consensus.hashGenesisBlock == uint256S("0x3ba0bdc07f1acbd0453d2976a5faa10c10e7b14c93b83fad0d99b3b8c8c7d731"));
        assert(genesis.hashMerkleRoot == uint256S("0x15f721c969cb8b4115363024d64ddffc57f7167e4dcbc9cfb89d12f19b59f4e4"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true; 
        fTestnetToBeDeprecatedFieldRPC = false;

	checkpointData = (CCheckpointData){
		boost::assign::map_list_of
			( 0, uint256S("0x3ba0bdc07f1acbd0453d2976a5faa10c10e7b14c93b83fad0d99b3b8c8c7d731")),
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
 
