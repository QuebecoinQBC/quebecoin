// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>
#include <consensus/merkle.h>
#include <primitives/pureheader.h>

#include <tinyformat.h>
#include <util.h>
#include <utilstrencodings.h>

#include <arith_uint256.h>

#include <assert.h>
#include <memory>

#include <chainparamsseeds.h>

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
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
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
    const char* pszTimestamp = "LaPresse 22/Mar/2014 Le Québec a les moyens de devenir un pays, reconnaît Couillard";
    const CScript genesisOutputScript = CScript() << ParseHex("04e941763c7750969e751bee1ffbe96a651a0feb131db046546c219ea40bff40b95077dc9ba1c05af991588772d8daabbda57386c068fb9bc7477c5e28702d5eb9") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
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

        /*** Quebecoin Additional Chainparams ***/
        consensus.nAveragingInterval = 10; // number of blocks to take the timespan of

        consensus.nStartAuxPow = 1; // Allow AuxPow blocks from this height
        consensus.nAuxpowChainId = 0x006C;
        consensus.fStrictChainId = false;
        consensus.nMaxAdjustDown = 4; // 4% adjustment down
        consensus.nMaxAdjustUp = 4; // 4% adjustment up
        consensus.nBlockSequentialAlgoMaxCount = 3;

        consensus.BIP16Height = 1;
        consensus.BIP34Height = 1;
        consensus.BIP34Hash = uint256S("0xcb41589c918fba1beccca8bc6b34b2b928b4f9888595d7664afd6ec60a576291");
        consensus.BIP65Height = 100; // 2ca9968704301897b956f7e326375413be505509489c06aee2b16fe73805481e
        consensus.BIP66Height = 100; // 2ca9968704301897b956f7e326375413be505509489c06aee2b16fe73805481e
        consensus.powLimit = ArithToUint256(~arith_uint256(0) >> 20);
        consensus.nPowTargetTimespan = 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 20; // Current value
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000017f7255b87a700"); // 282240

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x21b02eff716f9bedcd5fd71f640f2bdf2836a05d309f92e000191a0e872ab8bf"); // 282240

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xd4;
        pchMessageStart[1] = 0xde;
        pchMessageStart[2] = 0x9c;
        pchMessageStart[3] = 0xf2;
        nDefaultPort = 10889;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1557769755, 1674793, 0x1e0fffff, 2, 1000 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x000008c2d57759af6462352ee9f4923d97401cb599a9318e6595a2a74c26ea74"));
        assert(genesis.hashMerkleRoot == uint256S("0xa18eed5e3b349091c7a13f1ecafbbd98f7bd8e3106d9ba0f6b087b7749ce6386"));

        // Note that of those which support the service bits prefix, most only support a subset of
        // possible options.
        // This is fine at runtime as we'll fall back to using them as a oneshot if they dont support the
        // service bits we want, but we should get them updated to support all service bits wanted by any
        // release ASAP to avoid it where possible.
        //vSeeds.emplace_back("seed1.quebecoin..ca");
        //vSeeds.emplace_back("seed2.quebecoin..ca");
        vSeeds.emplace_back("qbcdnsseed.beyondcoin.io"); // Kristian Kramer (Tech1k)

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,58);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,186);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        bech32_hrp = "qb";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = {
            {
                {       0, uint256S("0x000008c2d57759af6462352ee9f4923d97401cb599a9318e6595a2a74c26ea74")},
                {    8064, uint256S("0xaa2957547af2ec9226cb5e67b07554484e29ad22372ee23b2f55760ccbc65d91")},
                {   16128, uint256S("0x8e1a3208ccefb4036129211004f8e6e5f8dacf4e0278c8478fcaa829b3b19acb")},
                {   24192, uint256S("0x000000170e7660bd19bdeeddb1b923b960715e39237f652252456d38f8f2ea95")},
                {   32256, uint256S("0x0000000cde354fe7bdc6be485b1f2dddeb29b1440fc502da13b254333b041d67")},
                {   40320, uint256S("0x00000006d856a879d9849fe82f5557a7b18e38bf0157bbc700da33626bfea205")},
                {   48384, uint256S("0xc7f820c16128745817cf7c339ad8427c54f6cb9b3027c11c96761f2d2ed3ff7b")},
                {   56448, uint256S("0x7dc523e52d74865768e51711baf239f3230ef728c317498bc58232ca3bc4dccf")},
                {   64512, uint256S("0xe10909730b89c78670c94707b25c1babcb99dd9f78ff5554a62c03149f0d18e4")},
                {   72576, uint256S("0xc7faa8be9faa90dd2f7d9aeebe2b8cbc91a369009bb465e973e175cb94c49a3d")},
                {   80640, uint256S("0x000000062413a8ca39513e40b2fa9273836cb9a946074527836aabd735ad82d6")},
                {   88704, uint256S("0x4b746106f2a3f43697ff6102e62f02691cacfa1da51f4de68531bbf2f038c5c4")},
                {   96768, uint256S("0xf05053f6ff35e88157763dc8f14c01a72420916d056175b6f7b95dad7b2602eb")},
                {  104832, uint256S("0xa82a593c5ace14972ca8e0b46066c015ba1a557f3a78aad8e40bddc868725769")},
                {  112896, uint256S("0xd5e80ed522e82cbe74d2c31307e3177f71fbc22a6d0f440bc58c205c3f4faeda")},
                {  120960, uint256S("0xc83fd8f0ae084df670d8a07bb39f953419e7f71f89e2b1cbfd64a8ed4e240670")},
                {  129024‬, uint256S("0x7d810094306fe4feca4ce0f31ffee60566277b9afde570f4640fd55c44283c94")},
                {  137088‬, uint256S("0x52d3099b27917e210ec625341a835f2cb5bc7b16a8a7a13aeb8c2df2665858cd")},
                {  145152‬, uint256S("0x17769bdb9746468fed3aa8d4a43e7c2f50f03063b875d1176bd1aab182805374")},
                {  153216‬, uint256S("0x2c672fc3085c2264869cc91f0893fd9d9c4fe43c50c44234b374f8515b0929cc")},
                {  161280‬, uint256S("0x8e1a3208ccefb4036129211004f8e6e5f8dacf4e0278c8478fcaa829b3b19acb")},
                {  169344, uint256S("0x0000000000008991640aa1799621c1c56016d8b2cbf0db0dc11d601a0d1af5af")},
                {  177408‬, uint256S("0x3b28c0415492733d60f8756304af7ac06436b3f15d5a6be1644ba35c3a0eae4c")},
                {  185472, uint256S("0x079cdc1467c8efeb0c7029368243eccb128e5a1c9f0d9a1a7a790de36ea28e9d")},
                {  193536, uint256S("0x6ee3585911cf99771096298e2ff877c70eefb67b4606e5442db655448870f86b")},
                {  201600‬, uint256S("0x3a0dfb79581a17c86e2b63489941b00355eb8cd3827b04c971d15d8886eb285c")},
                {  209664‬, uint256S("0x0000000000001370fbda36e724a0594cc891a1504574a9d374f5f74bbbbdc583")},
                {  217728‬, uint256S("0x0000000000031b24cdb9bbad97cb19e56a28bd8b465ebeebf8e61dcaf171f115")},
                {  225792‬, uint256S("0x4c050950ead5586acc2c8b8a83bb5983ba764929e964d859b0aad0f15c0f7384")},
                {  233856‬‬, uint256S("0xeb813c0f69ca3262ae0949f3834718102300201d65399f5001a6e72d77bcf37f")},
                {  ‬241920‬, uint256S("0x00000000000038a535c31f0032f405f20d548cad41b456de114fb258cf6ebbb0")},
                {  249984‬, uint256S("0x000000102c9a1b0e6b492888415a265e1fb082c18b223c9c34b5ed2b4758e992")},
                {  ‬258048, uint256S("0xa912a30990231eda7fdcf381c2c5cb0f4f66d2061108556bf5773869563a36f3")},
                {  266112‬‬, uint256S("0x90992ce16f83a2c2bbef42bfc12e2296051e038ce65a9e76dc5fb543ad58cf6d")},
                {  ‬274176‬, uint256S("0xdd4ecc6dc00cb51b641a55b37f63b8119222d9d549ad4285e23d359a902e8dbe")},
                {  282240‬, uint256S("0x21b02eff716f9bedcd5fd71f640f2bdf2836a05d309f92e000191a0e872ab8bf")},
                //{  290304, uint256S("")},
                
            }
        };

        chainTxData = ChainTxData{
            // Data as of block 21b02eff716f9bedcd5fd71f640f2bdf2836a05d309f92e000191a0e872ab8bf (height 282240).
            1573685970, // * UNIX timestamp of last known number of transactions
            380561,    // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
            0.03        // * estimated number of transactions per second after that timestamp
        };
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";

        consensus.nAveragingInterval = 10; // number of blocks to take the timespan of
        consensus.nStartAuxPow = 150;
        consensus.nAuxpowChainId = 0x005A;
        consensus.fStrictChainId = false;

        consensus.nMaxAdjustDown = 4; // 4% adjustment down
        consensus.nMaxAdjustUp= 4; // 4% adjustment up

        consensus.nSubsidyHalvingInterval = 967680;
        consensus.BIP16Height = 1;
        consensus.BIP34Height = 1;
        consensus.BIP34Hash = uint256S("0x0000d23adc28e33bc05f4bee57c873ae0aab584a6a436e75ac0ed40396f6d86b");
        consensus.BIP65Height = 641; // ff983c72147a81ac5b8ebfc68b62b39358cac4b8eb5518242e87f499b71c6a51
        consensus.BIP66Height = 641; // ff983c72147a81ac5b8ebfc68b62b39358cac4b8eb5518242e87f499b71c6a51
        consensus.powLimit = ArithToUint256(~arith_uint256(0) >> 16);
        consensus.nPowTargetTimespan = 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 20;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1504224000; // September 1st, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1535760000; // September 1st, 2018

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1506816000; // October 1st, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1538352000; // October 1st, 2018

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x000000c00310153e400312a5f10a51c14abd4d4456ad92a8efcb516c7c00fde5"); // 26607

        pchMessageStart[0] = 0x01;
        pchMessageStart[1] = 0xf5;
        pchMessageStart[2] = 0x55;
        pchMessageStart[3] = 0xa4;
        nDefaultPort = 20889;
        nPruneAfterHeight = 1000;

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis = CreateGenesisBlock(1392876393, 416875379, 0x1e0fffff, 2, 1000 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        //assert(consensus.hashGenesisBlock == uint256S("0x0000017ce2a79c8bddafbbe47c004aa92b20678c354b34085f62b762084b9788"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.emplace_back("testseed1.quebecoin..ca");
        //vSeeds.emplace_back("myriadtestseed1.cryptapus.org"); // cryptapus

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,88);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,188);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "tq";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;


        checkpointData = {
            {
                {     0, uint256S("0000017ce2a79c8bddafbbe47c004aa92b20678c354b34085f62b762084b9788")},
                {   800, uint256S("00000071942cef6d87635a92f106d5b1935b1314538af80922c766487afd8b22")},
                { 26607, uint256S("000000c00310153e400312a5f10a51c14abd4d4456ad92a8efcb516c7c00fde5")},
            }
        };

        chainTxData = ChainTxData{
            // Data as of block 000000c00310153e400312a5f10a51c14abd4d4456ad92a8efcb516c7c00fde5 (height 26607)
            1549032928,
            26673,
            0.02
        };

    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";

        /*** Quebecoin Additional Chainparams ***/
        consensus.nAveragingInterval = 10; // number of blocks to take the timespan of

        consensus.nStartAuxPow = 150;
        consensus.nAuxpowChainId = 0x005A;
        consensus.fStrictChainId = false;

        consensus.nMaxAdjustDown = 4; // 4% adjustment down
        consensus.nMaxAdjustUp = 4; // 4% adjustment up

        consensus.nSubsidyHalvingInterval = 150;
        consensus.BIP16Height = 0; // always enforce P2SH BIP16 on regtest
        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 20;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nDefaultPort = 18444;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1296688602, 4, 0x207fffff, 2, 1000 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        //assert(consensus.hashGenesisBlock == uint256S("0x63b92987ddc93808aa33dddc80b3e52948bdfffaf2420bf4cd9c5137b54ea37c"));
        //assert(genesis.hashMerkleRoot == uint256S("0x3f75db3c18e92f46c21530dc1222e1fddf4ccebbf88e289a6c9dc787fd6469da"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = {
            {
                {0, uint256S("63b92987ddc93808aa33dddc80b3e52948bdfffaf2420bf4cd9c5137b54ea37c")},
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "bcrt";
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}
