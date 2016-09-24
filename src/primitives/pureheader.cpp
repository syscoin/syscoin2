// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "primitives/pureheader.h"

#include "chainparams.h"
#include "hash.h"
#include "utilstrencodings.h"
// SYSCOIN fix setbaseversion to only set base and not chain bits
void CBlockVersion::SetBaseVersion(int32_t nBaseVersion)
{
	int32_t nBaseVersion = GetBaseVersion();
	if(IsAuxpow())
	{
		const int32_t nChainId = Params ().GetConsensus ().nAuxpowChainId;
		nVersion = nBaseVersion | (nChainId * VERSION_CHAIN_START);
		SetAuxpow(true);
	}
}

uint256 CPureBlockHeader::GetHash() const
{
    return SerializeHash(*this);
}