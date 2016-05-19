#ifndef CERT_H
#define CERT_H

#include "rpcserver.h"
#include "dbwrapper.h"
#include "script/script.h"
#include "serialize.h"
class CWalletTx;
class CTransaction;
class CReserveKey;
class CCoinsViewCache;
class CCoins;
class CBlock;
bool CheckCertInputs(const CTransaction &tx, int op, int nOut, const std::vector<std::vector<unsigned char> > &vvchArgs, const CCoinsViewCache &inputs, bool fJustCheck, int nHeight, const CBlock *block = NULL);
bool DecodeCertTx(const CTransaction& tx, int& op, int& nOut, std::vector<std::vector<unsigned char> >& vvch);
bool DecodeAndParseCertTx(const CTransaction& tx, int& op, int& nOut, std::vector<std::vector<unsigned char> >& vvch);
bool DecodeCertScript(const CScript& script, int& op, std::vector<std::vector<unsigned char> > &vvch);
bool IsCertOp(int op);
int IndexOfCertOutput(const CTransaction& tx);
bool EncryptMessage(const std::vector<unsigned char> &vchPublicKey, const std::vector<unsigned char> &vchMessage, std::string &strCipherText);
bool DecryptMessage(const std::vector<unsigned char> &vchPublicKey, const std::vector<unsigned char> &vchCipherText, std::string &strMessage);
std::string certFromOp(int op);
int GetCertExpirationDepth();
CScript RemoveCertScriptPrefix(const CScript& scriptIn);

class CCert {
public:
	std::vector<unsigned char> vchPubKey;
    std::vector<unsigned char> vchTitle;
    std::vector<unsigned char> vchData;
    uint256 txHash;
    uint64_t nHeight;
	bool bPrivate;
	unsigned char safetyLevel;
    CCert() {
        SetNull();
    }
    CCert(const CTransaction &tx) {
        SetNull();
        UnserializeFromTx(tx);
    }
	void ClearCert()
	{
		vchData.clear();
		vchTitle.clear();
	}
	ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion);

    friend bool operator==(const CCert &a, const CCert &b) {
        return (
        a.vchTitle == b.vchTitle
        && a.vchData == b.vchData
        && a.txHash == b.txHash
        && a.nHeight == b.nHeight
		&& a.vchPubKey == b.vchPubKey
		&& a.bPrivate == b.bPrivate
		&& a.safetyLevel == b.safetyLevel
        );
    }

    CCert operator=(const CCert &b) {
        vchTitle = b.vchTitle;
        vchData = b.vchData;
        txHash = b.txHash;
        nHeight = b.nHeight;
		vchPubKey = b.vchPubKey;
		bPrivate = b.bPrivate;
		safetyLevel = b.safetyLevel;
        return *this;
    }

    friend bool operator!=(const CCert &a, const CCert &b) {
        return !(a == b);
    }

    void SetNull() { safetyLevel = 0; nHeight = 0; txHash.SetNull(); vchPubKey.clear(); bPrivate = false; vchTitle.clear(); vchData.clear();}
    bool IsNull() const { return (safetyLevel == 0 && txHash.IsNull() &&  nHeight == 0 && vchPubKey.empty() && vchData.empty() && vchTitle.empty() && vchPubKey.empty()); }
    bool UnserializeFromTx(const CTransaction &tx);
	bool UnserializeFromData(const std::vector<unsigned char> &vchData);
	const std::vector<unsigned char> Serialize();
};


class CCertDB : public CDBWrapper {
public:
    CCertDB(size_t nCacheSize, bool fMemory, bool fWipe) : CDBWrapper(GetDataDir() / "certificates", nCacheSize, fMemory, fWipe) {}

    bool WriteCert(const std::vector<unsigned char>& name, std::vector<CCert>& vtxPos) {
        return Write(make_pair(std::string("certi"), name), vtxPos);
    }

    bool EraseCert(const std::vector<unsigned char>& name) {
        return Erase(make_pair(std::string("certi"), name));
    }

    bool ReadCert(const std::vector<unsigned char>& name, std::vector<CCert>& vtxPos) {
        return Read(make_pair(std::string("certi"), name), vtxPos);
    }

    bool ExistsCert(const std::vector<unsigned char>& name) {
        return Exists(make_pair(std::string("certi"), name));
    }

    bool ScanCerts(
            const std::vector<unsigned char>& vchName,
            unsigned int nMax,
            std::vector<std::pair<std::vector<unsigned char>, CCert> >& certScan);

    bool ReconstructCertIndex(CBlockIndex *pindexRescan);
};
bool GetTxOfCert(const std::vector<unsigned char> &vchCert,
        CCert& txPos, CTransaction& tx);
void PutToCertList(std::vector<CCert> &certList, CCert& index);
#endif // CERT_H
