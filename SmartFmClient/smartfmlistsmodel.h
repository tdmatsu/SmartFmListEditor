#ifndef SMARTFMLISTSMODEL_H
#define SMARTFMLISTSMODEL_H

#include <QAbstractTableModel>

#include <QMap>
class QIODevice;
class QXmlStreamReader;

class SmartFmListsModel : public QAbstractTableModel
{
Q_OBJECT

    QList <QMap<QString, QString>*> m_lstEntries;

    void parseXml_list(QXmlStreamReader* xml);

public:
    explicit SmartFmListsModel(QObject *parent = 0);

    int parseXml(QIODevice*data);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

signals:
    void signalHttpError(int errorCode, QString message);

public slots:

};

#endif // SMARTFMLISTSMODEL_H
