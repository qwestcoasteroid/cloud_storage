#include "filereader.h"

#include <stdexcept>

FileReader::FileReader(const QString &path)
    : file(path), stream(&file) {

    stream.setVersion(QDataStream::Qt_5_0);
    file.open(QIODevice::ReadOnly);
}

QByteArray FileReader::read(int size) {
    QByteArray result;
    int returnValue{};

    result.reserve(size);

    returnValue = stream.readRawData(result.data(), size);

    if (returnValue == -1) {
        throw std::runtime_error("Error reading file!");
    }

    result.resize(returnValue);

    return result;
}

FileReader::~FileReader() {
    if (file.isOpen()) {
        file.close();
    }
}
