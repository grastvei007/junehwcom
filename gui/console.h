#ifndef CONSOLE_H
#define CONSOLE_H

#include <QPlainTextEdit>

class Device;

class Console : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit Console(Device *aDevice, QWidget *parent = nullptr);
    virtual ~Console();


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
