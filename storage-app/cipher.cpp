#include "cipher.h"

#include "cryptopp850/filters.h"
#include "cryptopp850/osrng.h"
#include "cryptopp850/simple.h"

Cipher::Cipher(const QByteArray &key) :
    key(CryptoPP::AES::MAX_KEYLENGTH),
    iv(CryptoPP::AES::BLOCKSIZE) {

    this->key.Assign(reinterpret_cast<const unsigned char *>(key.data()),
                     CryptoPP::AES::MAX_KEYLENGTH);
    iv.Assign(reinterpret_cast<const unsigned char *>(
                  key.data() + CryptoPP::AES::MAX_KEYLENGTH),
              CryptoPP::AES::BLOCKSIZE);

    encryption.SetKeyWithIV(this->key, this->key.size(), iv, iv.size());
    decryption.SetKeyWithIV(this->key, this->key.size(), iv, iv.size());
}

QByteArray Cipher::encrypt(const QByteArray &bytes) {
    int blocksAmount = bytes.size() / CryptoPP::AES::BLOCKSIZE;

    if (bytes.size() % CryptoPP::AES::BLOCKSIZE != 0) {
        ++blocksAmount;
    }

    QByteArray result(blocksAmount * CryptoPP::AES::BLOCKSIZE, 0);

    CryptoPP::ArraySource source(
                reinterpret_cast<const unsigned char *>(bytes.data()),
                bytes.size(), true,
                new CryptoPP::StreamTransformationFilter(
                    encryption, new CryptoPP::ArraySink(
                        reinterpret_cast<unsigned char *>(result.data()),
                        result.size()), CryptoPP::BlockPaddingSchemeDef::ZEROS_PADDING));

    return result;
}

QByteArray Cipher::decrypt(const QByteArray &bytes) {
    QByteArray result(bytes.size(), 0);

    CryptoPP::ArraySource source(
                reinterpret_cast<const unsigned char *>(bytes.data()),
                bytes.size(), true,
                new CryptoPP::StreamTransformationFilter(
                    decryption, new CryptoPP::ArraySink(
                        reinterpret_cast<unsigned char *>(result.data()),
                        result.size()), CryptoPP::BlockPaddingSchemeDef::ZEROS_PADDING));

    return result;
}

QByteArray Cipher::generateKey() {
    QByteArray result(kDefaultKeySize, 0);

    CryptoPP::AutoSeededRandomPool randomPool;

    randomPool.GenerateBlock(reinterpret_cast<unsigned char *>(result.data()),
                             result.size());

    return result;
}
