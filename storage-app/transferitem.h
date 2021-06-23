#ifndef TRANSFERITEM_H
#define TRANSFERITEM_H

#include <QWidget>
#include <QBoxLayout>

namespace Ui { class TransferItem; }

class TransferItem : public QWidget {
    Q_OBJECT

public:
    explicit TransferItem(const QString &name, QVBoxLayout *layout, QWidget *parent = nullptr);
    ~TransferItem();

    void setItemName(const QString &name);

public slots:
    void setProgress(int progress);
    void forwardCancelation();

signals:
    void canceled(TransferItem *item);

private:
    Ui::TransferItem *ui;
    QVBoxLayout *layout;
};

#endif // TRANSFERITEM_H
