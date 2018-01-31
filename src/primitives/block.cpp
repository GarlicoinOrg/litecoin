// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "primitives/block.h"

#include "hash.h"
#include "tinyformat.h"
#include "utilstrencodings.h"
#include "crypto/common.h"
#include "crypto/scrypt.h"

uint256 CBlockHeader::GetHash() const
{
    return SerializeHash(*this);
}

uint256 CBlockHeader::GetPoWHash() const
{
    uint256 thash;
    unsigned char Nfactor;
    const unsigned char minNfactor = 10;
    const unsigned char maxNfactor = 20;

    // epoch times of chain start and current block time
    int64_t nChainStartTime = 1515925970;

    // n-factor will change every this interval is hit
    int64_t nChangeInterval = 36288000; // 420 days

    if (GetBlockTime() <= nChainStartTime) {
        Nfactor = minNfactor;
    } else {
        int64_t s = GetBlockTime() - nChainStartTime;
        int n = s/nChangeInterval + 10;

        if (n < 0) n = 0;
        unsigned char tempN = (unsigned char) n;
        Nfactor = std::min(std::max(tempN, minNfactor), maxNfactor);
    }
    scrypt_N_1_1_256(BEGIN(nVersion), BEGIN(thash), Nfactor);
    return thash;
}

std::string CBlock::ToString() const
{
    std::stringstream s;
    s << strprintf("CBlock(hash=%s, ver=0x%08x, hashPrevBlock=%s, hashMerkleRoot=%s, nTime=%u, nBits=%08x, nNonce=%u, vtx=%u)\n",
        GetHash().ToString(),
        nVersion,
        hashPrevBlock.ToString(),
        hashMerkleRoot.ToString(),
        nTime, nBits, nNonce,
        vtx.size());
    for (const auto& tx : vtx) {
        s << "  " << tx->ToString() << "\n";
    }
    return s.str();
}
