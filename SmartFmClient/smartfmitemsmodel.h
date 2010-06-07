#ifndef SMARTFMITEMSMODEL_H
#define SMARTFMITEMSMODEL_H

#include <QAbstractTableModel>

#include <QMap>
class QIODevice;
class QXmlStreamReader;

class SmartFmItemsModel : public QAbstractTableModel
{
Q_OBJECT

    QList <QMap<QString, QString>*> m_lstEntries;

    void parseXml_item(QXmlStreamReader* xml);

public:
    explicit SmartFmItemsModel(QObject *parent = 0);

    int parseXml(QIODevice*data);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

signals:

public slots:

};

#endif // SMARTFMITEMSMODEL_H
