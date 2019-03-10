#ifndef CONSOLE_H
#define CONSOLE_H

#include <QPlainTextEdit>

class Device;

class Console : public QPlainTextEdit
{
    Q_OBJECT
public:
    Console(QWidget *parent=nullptr);
    Console(Device *aDevice, QWidget *parent = nullptr);
    virtual ~Console();

    void setData(QString aInfo, QByteArray aData);

public slots:
    void setInData(QByteArray aData);
    void setOutData(QByteArray aData);

signals:
    void closed();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void putData(QByteArray aData);

private:
    Device *mDevice;
};

#endif // CONSOLE_H
