#include "transferitem.h"
#include "ui_transferitem.h"

TransferItem::TransferItem(const QString &name, QVBoxLayout *layout, QWidget *parent) :
    QWidget(parent), ui(new Ui::TransferItem), layout(layout) {

    ui->setupUi(this);
    ui->textLayout->setAlignment(Qt::AlignLeft);
    ui->optionLayout->setAlignment(Qt::AlignRight);

    layout->insertWidget(layout->count() - 1, this);

    setItemName(name);
    ui->fileNameLabel->setToolTip(name);
    setVisible(true);

    connect(ui->cancelButton, &QPushButton::clicked, this, &TransferItem::forwardCancelation);
}

TransferItem::~TransferItem() {
    delete ui;
}

void TransferItem::setProgress(int value) {
    ui->progressBar->setValue(value);
}

void TransferItem::setItemName(const QString &name) {
    ui->fileNameLabel->setText(name);
    ui->fileNameLabel->setToolTip(name);
}

void TransferItem::forwardCancelation() {
    emit canceled(this);
}
