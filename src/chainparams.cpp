// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Copyright (c) 2014-2017 The Dash Core developers
// Copyright (c) 2017-2018 The Nyx Core developers
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

static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "audaces fortuna juvat - 2018-04-20";
    const CScript genesisOutputScript = CScript() << ParseHex("0424e63bd76b81e25f2d0f945b0c743b2cc6283bc7603d577c76201203c7391b9277c38fa8fe7ec8bd2a29576c062e691070637d4a47ca650adce63f2547d49207") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}


class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 262800; // Note: actual number of blocks per calendar year with DGW v3 is ~200700 (for example 449750 - 249050)
        consensus.nMasternodePaymentsStartBlock = 100000; // not true, but it's ok as long as it's less then nMasternodePaymentsIncreaseBlock
        consensus.nMasternodePaymentsIncreaseBlock = 158000; // actual historical value
        consensus.nMasternodePaymentsIncreasePeriod = 576*30; // 17280 - actual historical value
        consensus.nInstantSendKeepLock = 24;
        consensus.nBudgetPaymentsStartBlock = 999000000; // future release
        consensus.nBudgetPaymentsCycleBlocks = 99900; // future release
        consensus.nBudgetPaymentsWindowBlocks = 100;
        consensus.nBudgetProposalEstablishingTime = 60*60*24;
        consensus.nSuperblockStartBlock = 999000001; // future release
        consensus.nSuperblockCycle = 99900; // future release
        consensus.nGovernanceMinQuorum = 10;
        consensus.nGovernanceFilterElements = 20000;
        consensus.nMasternodeMinimumConfirmations = 15;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = 1;
        consensus.BIP34Hash = uint256S("0x000000c3ea1c9c294de36aeed116bc57a245fc00361e2a1b4240f53e4da35881");
        consensus.powLimit = uint256S("00000fffff000000000000000000000000000000000000000000000000000000");
        consensus.nPowTargetTimespan = 60 * 60; // Nyx: 60 minutes 30 blocks
        consensus.nPowTargetSpacing = 2 * 60; // Nyx: 2 minutes
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nPowKGWHeight = 1000;
        consensus.nPowDGWHeight = 1000;
        consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1486252800; // Feb 5th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1517788800; // Feb 5th, 2018

        // Deployment of DIP0001
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = 1525176000; // Tue, 01 May 2018 12:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = 1556712000; // Wed, 01 May 2019 12:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nWindowSize = 600;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nThreshold = 480; // 80% of 600

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00"); // 52000

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x000000000067f916f4333284fca23ed3067efc875af63a0cc8342d8704b99248"); // 55254

        pchMessageStart[0] = 0xaf;
        pchMessageStart[1] = 0x3c;
        pchMessageStart[2] = 0x7b;
        pchMessageStart[3] = 0xcc;
        vAlertPubKey = ParseHex("041d33468b8753a0df3b8ddbd0b64edf0e7e2bbc4996ff7d7ceb32283496d72b40421eff54bf4a223f0af838253114ed27f1661a35086870e7693b52ff040ac641");
        nDefaultPort = 3483;
        nMaxTipAge = 6 * 60 * 60; // ~144 blocks behind -> 2 x fork detection time, was 24 * 60 * 60 in bitcoin
        nDelayGetHeadersTime = 24 * 60 * 60;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1524278894, 444985, 0x1e0ffff0, 1, 100 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        //assert(consensus.hashGenesisBlock == uint256S("00000ccd2ae6e7a1d3f9bf2d33e40ec1794e884dec5f956769c099647597afa5"));
        //assert(genesis.hashMerkleRoot == uint256S(""));
        
        if (consensus.hashGenesisBlock != uint256S("b0b13e119bfd6d5adb504e0ce3aba8b8425ba0d428d9963d52fde781c8061f65")) {
            printf("Searching for genesis block ...\n");
            genesis = CreateGenesisBlock(1524278894, 444985, 0x1e0ffff0, 1, 100 * COIN);
            consensus.hashGenesisBlock = genesis.GetHash();
            printf("\ngenesishash:\n");
            printf(consensus.hashGenesisBlock.ToString().c_str());
            printf("\nmerkleroot:\n");            
            printf(genesis.hashMerkleRoot.ToString().c_str());
        }
        else
           printf("good");



        //vSeeds.push_back(CDNSSeedData("node_nl", "node1.nyxcoin.org"));	

        // Nyx addresses start with 'N'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,45);
        // Nyx script addresses start with '9'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,53);
        // Nyx private keys start with 'x' or 'y'
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,139);
        // Nyx BIP32 pubkeys start with 'xpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        // Nyx BIP32 prvkeys start with 'xprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

        // Nyx BIP44 coin type is '5'
        nExtCoinType = 5;

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 60*60; // fulfilled requests expire in 1 hour
        strSporkPubKey = "04f44658e6f5fbfe63e1db3914aafb5116eb1bec6435493bae2b6afe329eadcd9598f0bdc4de24d20ea125d23d2a1eb509f63a8eda6e1d3f625af2501801ae27fa";

        checkpointData = (CCheckpointData){
            boost::assign::map_list_of
            ( 175, uint256S("0x0000008324b22109a4cd33aa15181a81e13b0ed6b4724fd4b0b9355637a65058"))
            ( 5175, uint256S("0x000000000002b5b8886acc2c0613f871e1eff192c68bde64fded61a337652065"))
            ( 25175, uint256S("0x000000000323b956e72290f25c3f3b0c29e4282b35902406a2a89e097efe94c8"))
            ( 55254, uint256S("0x000000000067f916f4333284fca23ed3067efc875af63a0cc8342d8704b99248")),				
            1524117018, // * UNIX timestamp of last checkpoint block
            72743,    	// * total number of transactions between genesis and last checkpoint(the tx=... number in the SetBestChain debug.log lines)
	        1000 		// * estimated number of transactions per day after checkpoint
        };
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
        consensus.nSubsidyHalvingInterval = 262800; // Note: actual number of blocks per calendar year with DGW v3 is ~200700
        consensus.nMasternodePaymentsStartBlock = 1; // not true, but it's ok as long as it's less then nMasternodePaymentsIncreaseBlock
        consensus.nMasternodePaymentsIncreaseBlock = 2; // actual historical value
        consensus.nMasternodePaymentsIncreasePeriod = 3; //actual historical value
        consensus.nInstantSendKeepLock = 24;
        consensus.nBudgetPaymentsStartBlock = 2100000000; // actual historical value
        consensus.nBudgetPaymentsCycleBlocks = 21600;
        consensus.nBudgetPaymentsWindowBlocks = 100;
        consensus.nBudgetProposalEstablishingTime = 60*60*24;
        consensus.nSuperblockStartBlock = 2100000000; // actual historical value
        consensus.nSuperblockCycle = 21600;
        consensus.nGovernanceMinQuorum = 10;
        consensus.nGovernanceFilterElements = 20000;
        consensus.nMasternodeMinimumConfirmations = 1;
        consensus.nMajorityEnforceBlockUpgrade = 51;
        consensus.nMajorityRejectBlockOutdated = 75;
        consensus.nMajorityWindow = 100;
        consensus.BIP34Height = -1;
        consensus.BIP34Hash = uint256S("0x00");
        consensus.powLimit = uint256S("00000fffff000000000000000000000000000000000000000000000000000000");
        consensus.nPowTargetTimespan = 60 * 60; // Nyx: 60 minutes
        consensus.nPowTargetSpacing = 2 * 60; // Nyx: 2 minutes
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nPowKGWHeight = 1000; // nPowKGWHeight >= nPowDGWHeight means "no KGW"
        consensus.nPowDGWHeight = 1000;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1506556800; // September 28th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1538092800; // September 28th, 2018

        // Deployment of DIP0001
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = 1505692800; // Sep 18th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = 1537228800; // Sep 18th, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nWindowSize = 100;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nThreshold = 50; // 50% of 100

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00"); // 0

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00"); // 0

        pchMessageStart[0] = 0x3b;
        pchMessageStart[1] = 0x1f;
        pchMessageStart[2] = 0xbd;
        pchMessageStart[3] = 0x2c;
        vAlertPubKey = ParseHex("0451c826df9385f36dfd8e23bde24b9fab40aac24521dc1358abd8af6582202fd2faf1b42607f4157039f5e9cd4998e8b74e15819dc601c2c5d748f93683baa4ea");
        nDefaultPort = 4332;
        nMaxTipAge = 0x7fffffff; // allow mining on top of old blocks for testnet
        nDelayGetHeadersTime = 24 * 60 * 60;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1524278894, 444985, 0x1e0ffff0, 1, 100 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        //assert(consensus.hashGenesisBlock == uint256S("b0b13e119bfd6d5adb504e0ce3aba8b8425ba0d428d9963d52fde781c8061f65"));
        //assert(genesis.hashMerkleRoot == uint256S("48923b5ffc1876766148fc375c55ac0f2bdae005b61b0c1103566ca8cc2a4a71"));


         if (consensus.hashGenesisBlock != uint256S("b0b13e119bfd6d5adb504e0ce3aba8b8425ba0d428d9963d52fde781c8061f65")) {
            printf("Searching for genesis block ...\n");
            genesis = CreateGenesisBlock(1524278894, 444985, 0x1e0ffff0, 1, 100 * COIN);
            consensus.hashGenesisBlock = genesis.GetHash();
            printf("\ngenesishash:\n");
            printf(consensus.hashGenesisBlock.ToString().c_str());
            printf("\nmerkleroot:\n");
            printf(genesis.hashMerkleRoot.ToString().c_str());
        }
        else
           printf("good");



        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("node_nl", "node1.nyxcoin.org"));
        vSeeds.push_back(CDNSSeedData("node_ny", "node2.nyxcoin.org"));

        // Testnet Nyx addresses start with 'y'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,140);
        // Testnet Nyx script addresses start with '8' or '9'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,19);
        // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        // Testnet Nyx BIP32 pubkeys start with 'tpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        // Testnet Nyx BIP32 prvkeys start with 'tprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        // Testnet Nyx BIP44 coin type is '1' (All coin's testnet default)
        nExtCoinType = 1;

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes
        strSporkPubKey = "04297f4c53d758c61fee82495915439e58e7b43c3a0356062a418a3e58b28bdf78381b133867e5193907fb76db7da180f531ab441c4bce873d13600660bcd27c9b";

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            (        0, uint256S("0x00000d72d411dc83f7f09f88c7d57c77d3dc061eec14806a958c051902602636")),
            1517054400, // * UNIX timestamp of last checkpoint block
                     0, // * total number of transactions between genesis and last checkpoint
                        //   (the tx=... number in the SetBestChain debug.log lines)
                   500  // * estimated number of transactions per day after checkpoint
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
        consensus.nMasternodePaymentsStartBlock = 240;
        consensus.nMasternodePaymentsIncreaseBlock = 350;
        consensus.nMasternodePaymentsIncreasePeriod = 10;
        consensus.nInstantSendKeepLock = 6;
        consensus.nBudgetPaymentsStartBlock = 1000;
        consensus.nBudgetPaymentsCycleBlocks = 50;
        consensus.nBudgetPaymentsWindowBlocks = 10;
        consensus.nBudgetProposalEstablishingTime = 60*20;
        consensus.nSuperblockStartBlock = 1500;
        consensus.nSuperblockCycle = 10;
        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 100;
        consensus.nMasternodeMinimumConfirmations = 1;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = -1; // BIP34 has not necessarily activated on regtest
        consensus.BIP34Hash = uint256();
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 60 * 60; // Nyx: 60 minutes
        consensus.nPowTargetSpacing = 2 * 60; // Nyx: 2 minutes
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nPowKGWHeight = 1000; // same as mainnet
        consensus.nPowDGWHeight = 1000; // same as mainnet
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0xb2;
        pchMessageStart[1] = 0xc3;
        pchMessageStart[2] = 0xd4;
        pchMessageStart[3] = 0xf5;
        nMaxTipAge = 6 * 60 * 60; // ~144 blocks behind -> 2 x fork detection time, was 24 * 60 * 60 in bitcoin
        nDelayGetHeadersTime = 0; // never delay GETHEADERS in regtests
        nDefaultPort = 4334;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1524278894, 444985, 0x1e0ffff0, 1, 100 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        //assert(consensus.hashGenesisBlock == uint256S("0x00000d72d411dc83f7f09f88c7d57c77d3dc061eec14806a958c051902602636"));
        //assert(genesis.hashMerkleRoot == uint256S("0xbb3755d2f47f9f30081b068778b2c607fa86359d74056801f7d40c2f1f697375"));

        if (consensus.hashGenesisBlock != uint256S("b0b13e119bfd6d5adb504e0ce3aba8b8425ba0d428d9963d52fde781c8061f65")) {
            printf("Searching for genesis block ...\n");
            genesis = CreateGenesisBlock(1524278894, 444985, 0x1e0ffff0, 1, 100 * COIN);
            consensus.hashGenesisBlock = genesis.GetHash();
            printf("\ngenesishash:\n");
            printf(consensus.hashGenesisBlock.ToString().c_str());
            printf("\nmerkleroot:\n");
            printf(genesis.hashMerkleRoot.ToString().c_str());
        }
        else
           printf("good");

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes

        checkpointData = (CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("0x00000d72d411dc83f7f09f88c7d57c77d3dc061eec14806a958c051902602636")),
            1517054400,
            0,
            0
        };
        // Regtest Nyx addresses start with 'y'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,140);
        // Regtest Nyx script addresses start with '8' or '9'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,19);
        // Regtest private keys start with '9' or 'c' (Bitcoin defaults)
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        // Regtest Nyx BIP32 pubkeys start with 'tpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        // Regtest Nyx BIP32 prvkeys start with 'tprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        // Regtest Nyx BIP44 coin type is '1' (All coin's testnet default)
        nExtCoinType = 1;
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
