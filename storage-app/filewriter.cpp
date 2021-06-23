#include "filewriter.h"

#include <stdexcept>
#include <QDebug>

FileWriter::FileWriter(const QString &path, qint64 initialSize)
    : file(path), stream(&file), initialSize(initialSize) {

    stream.setVersion(QDataStream::Qt_5_0);
    file.open(QIODevice::WriteOnly);

    if (file.size() == 0) {
        file.resize(initialSize);
    }

    file.seek(0);
}

int FileWriter::write(const QByteArray &bytes) {
    int returnValue{};

    returnValue = stream.writeRawData(bytes.data(), bytes.size());

    if (returnValue == -1) {
        throw std::runtime_error("Error writing file!");
    }

    return returnValue;
}

FileWriter::~FileWriter() {
    qDebug() << "Initial file size: " << initialSize;
    qDebug() << "Actual file size: " << file.size();

    if (file.isOpen()) {
        if (initialSize != 0 && initialSize > file.size()) {
            file.remove();
        }
        else if (initialSize != 0) {
            file.resize(initialSize);
        }

        file.close();
    }
}
