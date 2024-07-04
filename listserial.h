#ifndef LISTSERIAL_H_
#define LISTSERIAL_H_

#include "plugininterface.h"
#include <QDialog>
#include <QList>
#include <QSerialPortInfo>

#define PLUGIN_NAME    "List Serial"
#define PLUGIN_VERSION "0.0.2"

class ListSerial : public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.quardCRT.PluginInterface" FILE "./plugininterface/plugininterface.json")
    Q_INTERFACES(PluginInterface)

public:
    ListSerial() : m_action(nullptr) {}
    virtual ~ListSerial() {}

    int init(QMap<QString, QString> params, QWidget *parent);

    void setLanguage(const QLocale &language,QApplication *app);
    void retranslateUi();
    QString name() { return PLUGIN_NAME; }
    QString version() { return PLUGIN_VERSION; }

    QMap<QString,void *> metaObject() {
        QMap<QString,void *> ret;
        ret.insert("QAction", (void *)m_action);
        return ret;
    }

    QMenu *terminalContextMenu(QString selectedText, QString workingDirectory, QMenu *parentMenu) {Q_UNUSED(selectedText);Q_UNUSED(workingDirectory);Q_UNUSED(parentMenu); return nullptr;}
    QList<QAction *> terminalContextAction(QString selectedText, QString workingDirectory, QMenu *parentMenu) {Q_UNUSED(selectedText);Q_UNUSED(workingDirectory);Q_UNUSED(parentMenu); return QList<QAction *>();}

    class ListSerialDialog: public QDialog
    {
    public:
        ListSerialDialog(QWidget *parent = nullptr);
        void setSerialPortInfo(const QList<QSerialPortInfo> &serialPortInfoList);
        void setCloseText(const QString &text);
        void setRefreshText(const QString &text);
        void setHeaderLabels(const QStringList &labels);

    private:
        QString m_closeText = "Close";
        QString m_refreshText = "Refresh";
        QStringList m_headerLabels = QStringList() << "Port" << 
            "Description" << "Manufacturer" << "Serial Number" << 
            "Location" << "Vendor ID" << "Product ID";
        QPushButton *buttonClose;
        QPushButton *buttonRefresh;
    };

private:
    QAction *m_action;
};

#endif /* LISTSERIAL_H_ */
