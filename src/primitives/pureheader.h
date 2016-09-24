// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2013 The SYSCOIN developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef SYSCOIN_PRIMITIVES_PUREHEADER_H
#define SYSCOIN_PRIMITIVES_PUREHEADER_H

#include "serialize.h"
#include "uint256.h"

/**
 * Encapsulate a block version.  This takes care of building it up
 * from a base version, the modifier flags (like auxpow) and
 * also the auxpow chain ID.
 */
class CBlockVersion
{

private:

    /* Modifiers to the version.  */
    static const int32_t VERSION_AUXPOW = (1 << 8);

    /** Bits above are reserved for the auxpow chain ID.  */
    static const int32_t VERSION_CHAIN_START = (1 << 16);

    // used for chainid and auxpow detection
    int32_t nAuxPowVersion;
	// used for versionbits detection using softfork selection bits
	int32_t nBaseVersion;

public:

    inline CBlockVersion()
    {
        SetNull();
    }

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion) {
        READWRITE(GetFullVersion());
    }

    inline void SetNull()
    {
        nAuxPowVersion = nBaseVersion = 0;
    }

    /**
     * Extract the base version (without modifiers and chain ID).
     * @return The base version./
     */
    inline int32_t GetBaseVersion() const
    {
        return nBaseVersion;
    }

    /**
     * Set the base version (apart from chain ID and auxpow flag) to
     * the one given. This ideally passes in the versionbits from ComputeBlockVersion
     * @param nVersion The base version.
     */
    inline void SetBaseVersion(int32_t nVersion)
    {
        nBaseVersion = nVersion;
    }
    /**
     * Extract the chain ID.
     * @return The chain ID encoded in the version.
     */
    inline int32_t GetChainId() const
    {
        return nAuxPowVersion / VERSION_CHAIN_START;
    }

    /**
     * Set the chain ID.  This is used for the test suite.
     * @param ch The chain ID to set.
     */
    inline void SetChainId(int32_t chainId)
    {
        nAuxPowVersion %= VERSION_CHAIN_START;
        nAuxPowVersion |= chainId * VERSION_CHAIN_START;
    }

    /**
     * Extract the full version.  Used for RPC results and debug prints.
     * @return The full version.
     */
    inline int32_t GetFullVersion() const
    {
        return nAuxPowVersion | nBaseVersion;
    }

    /**
     * Check if the auxpow flag is set in the version.
     * @return True iff this block version is marked as auxpow.
     */
    inline bool IsAuxpow() const
    {
        return nAuxPowVersion & VERSION_AUXPOW;
    }

    /**
     * Set the auxpow flag.  This is used for testing.
     * @param auxpow Whether to mark auxpow as true.
     */
    void SetAuxpow (bool auxpow);

    /**
     * Check whether this is a "legacy" block without chain ID.
     * @return True iff it is.
     */
    inline bool IsLegacy() const
    {
        return GetFullVersion() == 1;
    }

};

/**
 * A block header without auxpow information.  This "intermediate step"
 * in constructing the full header is useful, because it breaks the cyclic
 * dependency between auxpow (referencing a parent block header) and
 * the block header (referencing an auxpow).  The parent block header
 * does not have auxpow itself, so it is a pure header.
 */
class CPureBlockHeader
{
public:
    // header
    static const int32_t CURRENT_VERSION=4;
    CBlockVersion nVersion;
    uint256 hashPrevBlock;
    uint256 hashMerkleRoot;
    uint32_t nTime;
    uint32_t nBits;
    uint32_t nNonce;

    CPureBlockHeader()
    {
        SetNull();
    }

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion) {
        READWRITE(this->nVersion);
        nVersion = this->nVersion.GetBaseVersion();
        READWRITE(hashPrevBlock);
        READWRITE(hashMerkleRoot);
        READWRITE(nTime);
        READWRITE(nBits);
        READWRITE(nNonce);
    }

    void SetNull()
    {
        nVersion.SetNull();
        hashPrevBlock.SetNull();
        hashMerkleRoot.SetNull();
        nTime = 0;
        nBits = 0;
        nNonce = 0;
    }

    bool IsNull() const
    {
        return (nBits == 0);
    }

    uint256 GetHash() const;
    int64_t GetBlockTime() const
    {
        return (int64_t)nTime;
    }
};

#endif // SYSCOIN_PRIMITIVES_PUREHEADER_H