#ifndef UPLOADFILEREQUEST_H
#define UPLOADFILEREQUEST_H

#include "irequest.h"

#include "cipher.h"
#include "filereader.h"
#include "storageinfo.h"

class UploadFileRequest : public IRequest {
    Q_OBJECT

public:
    UploadFileRequest(const QString &path, qint64 size) :
        path(path), size(size), cipher(StorageInfo::get()->cryptoKey),
        fileReader(path) {}

signals:
    void progress(int value);

public slots:
    void onPacketReady(std::shared_ptr<NetworkPacket> newPacket) override;
    void stop();

protected:
    void operator()() override;

private:
    QString path;
    qint64 size;
    qint64 sent{};
    //bool isStopped{ false };
    Cipher cipher;
    FileReader fileReader;
};

#endif // UPLOADFILEREQUEST_H
