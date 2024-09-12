
#include "listserial.h"

#include <QApplication>
#include <QMap>
#include <QSerialPortInfo>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QHeaderView>
#include <QTranslator>
#include <QClipboard>
#include <QDebug>

ListSerial::ListSerialDialog::ListSerialDialog(QWidget *parent): QDialog(parent) {
    resize(1000, 600);

    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);
    QTableWidget *table = new QTableWidget(this);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->horizontalHeader()->setSectionsClickable(false);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    table->setContextMenuPolicy(Qt::ActionsContextMenu);
    QAction *copyAction = new QAction(tr("Copy"), table);
    table->addAction(copyAction);
    connect(copyAction, &QAction::triggered, [&,table]() {
        QItemSelectionModel *selection = table->selectionModel();
        QModelIndexList indexes = selection->selectedIndexes();

        if (indexes.size() < 1)
            return;

        QString selected_text;
        for (int i = 0; i < indexes.size(); ++i) {
            QModelIndex index = indexes.at(i);
            QString text = table->model()->data(index).toString();
            selected_text.append(text);
            if (i < indexes.size() - 1)
                selected_text.append("\t");
        }

        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(selected_text);
    });
    layout->addWidget(table);
    buttonRefresh= new QPushButton(m_refreshText, this);
    layout->addWidget(buttonRefresh);
    connect(buttonRefresh, &QPushButton::clicked, this, [&](void) {
        QList<QSerialPortInfo> serialPortInfoList = QSerialPortInfo::availablePorts();
        setSerialPortInfo(serialPortInfoList);
    });
    buttonClose = new QPushButton(m_closeText, this);
    layout->addWidget(buttonClose);
    connect(buttonClose, &QPushButton::clicked, this, &QDialog::accept);
}

void ListSerial::ListSerialDialog::setSerialPortInfo(const QList<QSerialPortInfo> &serialPortInfoList) {
    QTableWidget *table = findChild<QTableWidget *>();
    table->clearContents();
    table->setColumnCount(7);
    table->setHorizontalHeaderLabels(m_headerLabels);
    table->setRowCount(serialPortInfoList.size());
    for (int i = 0; i < serialPortInfoList.size(); i++) {
        const QSerialPortInfo &info = serialPortInfoList.at(i);
        table->setItem(i, 0, new QTableWidgetItem(info.portName()));
        table->item(i, 0)->setToolTip(info.portName());
        table->setItem(i, 1, new QTableWidgetItem(info.description()));
        table->item(i, 1)->setToolTip(info.description());
        table->setItem(i, 2, new QTableWidgetItem(info.manufacturer()));
        table->item(i, 2)->setToolTip(info.manufacturer());
        table->setItem(i, 3, new QTableWidgetItem(info.serialNumber()));
        table->item(i, 3)->setToolTip(info.serialNumber());
        table->setItem(i, 4, new QTableWidgetItem(info.systemLocation()));
        table->item(i, 4)->setToolTip(info.systemLocation());
        if(info.hasVendorIdentifier()) {
            table->setItem(i, 5, new QTableWidgetItem(QString::number(info.vendorIdentifier(), 16)));
            table->item(i, 5)->setToolTip(QString::number(info.vendorIdentifier(), 16));
        }
        if(info.hasProductIdentifier()) {
            table->setItem(i, 6, new QTableWidgetItem(QString::number(info.productIdentifier(), 16)));
            table->item(i, 6)->setToolTip(QString::number(info.productIdentifier(), 16));
        }
    }
}

void ListSerial::ListSerialDialog::setCloseText(const QString &text) {
    m_closeText = text;
    buttonClose->setText(m_closeText);
}

void ListSerial::ListSerialDialog::setRefreshText(const QString &text) {
    m_refreshText = text;
    buttonRefresh->setText(m_refreshText);
}

void ListSerial::ListSerialDialog::setHeaderLabels(const QStringList &labels) {
    m_headerLabels = labels;
    QTableWidget *table = findChild<QTableWidget *>();
    table->setHorizontalHeaderLabels(m_headerLabels);
}

int ListSerial::init(QMap<QString, QString> params, QWidget *parent)
{
    m_action = new QAction(tr("List Serial"), parent);
    connect(m_action, &QAction::triggered, [parent](){
        QList<QSerialPortInfo> serialPortInfoList = QSerialPortInfo::availablePorts();
        ListSerialDialog dialog(parent);
        dialog.setWindowTitle(tr("List Serial Ports"));
        dialog.setCloseText(tr("Close"));
        dialog.setRefreshText(tr("Refresh"));
        dialog.setHeaderLabels(QStringList() << tr("Port") << 
            tr("Description") << tr("Manufacturer") << tr("Serial Number") << 
            tr("Location") << tr("Vendor ID") << tr("Product ID"));
        dialog.setSerialPortInfo(serialPortInfoList);
        dialog.exec();
    });
    Q_UNUSED(params);

    return 0;
}


void ListSerial::setLanguage(const QLocale &language,QApplication *app) {
    static QTranslator *qtTranslator = nullptr;
    if(qtTranslator == nullptr) {
        qtTranslator = new QTranslator(app);
    } else {
        app->removeTranslator(qtTranslator);
        delete qtTranslator;
        qtTranslator = new QTranslator(app);
    }
    switch(language.language()) {
    case QLocale::Chinese:
        if(qtTranslator->load(":/lang/listserial_zh_CN.qm"))
            app->installTranslator(qtTranslator);
        break;
    default:
    case QLocale::English:
        if(qtTranslator->load(":/lang/listserial_en_US.qm"))
            app->installTranslator(qtTranslator);
        break;
    }
}

void ListSerial::retranslateUi() {
    m_action->setText(tr("List Serial"));
}
