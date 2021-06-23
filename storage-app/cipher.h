#ifndef CIPHER_H
#define CIPHER_H

#if !defined(CRYPTOPP_IMPORTS) && !defined(CRYPTOPP_EXPORTS) && !defined(CRYPTOPP_DEFAULT_NO_DLL)
# ifdef CRYPTOPP_CONFIG_H
#  error To use the DLL version of Crypto++, this file must be included before any other Crypto++ header files.
# endif
# define CRYPTOPP_IMPORTS
#endif

//#include "cryptopp850/dll.h"

#include <QByteArray>

#include "cryptopp850/modes.h"
#include "cryptopp850/secblock.h"
#include "cryptopp850/aes.h"

constexpr int kDefaultKeySize =
        CryptoPP::AES::MAX_KEYLENGTH +
        CryptoPP::AES::BLOCKSIZE;

class Cipher {
public:
    Cipher(const QByteArray &key);

    QByteArray encrypt(const QByteArray &bytes);
    QByteArray decrypt(const QByteArray &bytes);

    static QByteArray generateKey();

private:
    CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption encryption;
    CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption decryption;
    CryptoPP::SecByteBlock key;
    CryptoPP::SecByteBlock iv;
};

#endif // CIPHER_H
