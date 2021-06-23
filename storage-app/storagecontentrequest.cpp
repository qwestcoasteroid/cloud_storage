#include "storagecontentrequest.h"

#include "protocol.h"
#include "cipher.h"
#include "storageinfo.h"

#include <QCryptographicHash>

QByteArray decryptData(const QByteArray &data) {
	QDataStream encryptedStream(data);
	encryptedStream.setVersion(QDataStream::Qt_5_0);

    QByteArray result;
    QDataStream resultStream(&result, QIODevice::WriteOnly);
    resultStream.setVersion(QDataStream::Qt_5_0);
	
	while (!encryptedStream.atEnd()) {
        Cipher cipher(StorageInfo::get()->cryptoKey);
		StorageFileStructure fileStructure;
        QByteArray header;
        QString uuid;
	
		encryptedStream >> header >> uuid;
	
        header = cipher.decrypt(header);
	
		QDataStream decryptedStream(header);
		decryptedStream.setVersion(QDataStream::Qt_5_0);

        decryptedStream >> fileStructure.name >> fileStructure.size >> fileStructure.date;

        fileStructure.uuid = uuid;

        resultStream << fileStructure;
	}
	
	return result;
}

void StorageContentRequest::onPacketReady(std::shared_ptr<NetworkPacket> newPacket) {
    if (newPacket->header.id == MessageID::Error) {
        emit error(new QString(QString::fromUtf8(newPacket->data)));
        return;
    }

    emit complete(new QByteArray(decryptData(newPacket->data)));
}

void StorageContentRequest::operator()() {
    auto writer = connection->getWriter();

    writer->write(MessageID::StorageContentRequest);
}

bool operator==(const StorageFileStructure &lhs, const StorageFileStructure &rhs) {
    return lhs.uuid == rhs.uuid;
}

QDataStream &operator>>(QDataStream &stream, StorageFileStructure &file) {
    stream >> file.name >> file.size >> file.date >> file.uuid;
    return stream;
}

QDataStream &operator<<(QDataStream &stream, StorageFileStructure &file) {
    stream << file.name << file.size << file.date << file.uuid;
    return stream;
}
