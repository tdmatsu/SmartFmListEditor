#include "smartfmlistsmodel.h"

#include <QtCore>

SmartFmListsModel::SmartFmListsModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

/*
    return
        -1: error
        0: reached end of entries
        1: shouldn't happen
        2 or greater: next page number to be read
*/
int SmartFmListsModel::parseXml(QIODevice*data)
{
    int totalResults = 0;
    int startIndex = 0;
    int itemsPerPage = 0;

    QXmlStreamReader xml(data);
    while (!xml.atEnd()){                   // loop in "lists"
        xml.readNext();
        if(xml.isStartElement()){
            if (xml.name().toString() == "error"){
                int errorCode = xml.attributes().value("code").toString().toInt();
                QString message = xml.attributes().value("message").toString();
                emit signalHttpError(errorCode, message);
                return -1;
            }else if(xml.name().toString() == "totalResults"){
                totalResults = xml.readElementText().toInt();
            }else if(xml.name().toString() == "startIndex"){
                startIndex = xml.readElementText().toInt();
                if (startIndex == 0){
                    m_lstEntries.clear();
                }
            }else if(xml.name().toString() == "itemsPerPage"){
                itemsPerPage = xml.readElementText().toInt();
            }else if(xml.name().toString() == "list"){
                parseXml_list(&xml);
            }
        }
    }

    if ((startIndex + itemsPerPage) < totalResults){
        return startIndex / itemsPerPage + 2;
    }else{
        return 0;
    }
}

void SmartFmListsModel::parseXml_list(QXmlStreamReader* xml)
{
    QMap<QString, QString>* item = new QMap<QString, QString>;
    m_lstEntries.append(item);
    m_lstEntries.last()->insert("id", xml->attributes().value("id").toString());
    m_lstEntries.last()->insert("updated_at", xml->attributes().value("updated_at").toString());
    m_lstEntries.last()->insert("published_at", xml->attributes().value("published_at").toString());
    m_lstEntries.last()->insert("created_at", xml->attributes().value("created_at").toString());
    while(!xml->atEnd()){                   // loop in "list"
        xml->readNext();
        if(xml->isStartElement()){
            if(xml->name().toString() == "title"){
                m_lstEntries.last()->insert("title", xml->readElementText());
            }else if(xml->name().toString() == "description"){
                m_lstEntries.last()->insert("description", xml->readElementText());
            }else if(xml->name().toString() == "language"){
                m_lstEntries.last()->insert("language", xml->readElementText());
            }else if(xml->name().toString() == "translation_language"){
                m_lstEntries.last()->insert("translation_language", xml->readElementText());
            }else if(xml->name().toString() == "icon "){
                m_lstEntries.last()->insert("icon_href", xml->attributes().value("href").toString());
            }else if(xml->name().toString() == "square_icon"){
                m_lstEntries.last()->insert("icon_href", xml->attributes().value("href").toString());
            }else if(xml->name().toString() == "items_count"){
                m_lstEntries.last()->insert("items_count", xml->readElementText());
            }else if(xml->name().toString() == "sentences_count"){
                m_lstEntries.last()->insert("sentences_count", xml->readElementText());
            }else if(xml->name().toString() == "users_count"){
                m_lstEntries.last()->insert("users_count", xml->readElementText());
            }else if(xml->name().toString() == "likes_count"){
                m_lstEntries.last()->insert("likes_count", xml->readElementText());
            }else if(xml->name().toString() == "favorites_count"){
                m_lstEntries.last()->insert("favorites_count", xml->readElementText());
            }
        }else if(xml->isEndElement() && xml->name().toString() == "list"){
            break;
        }
    }
}

int SmartFmListsModel::rowCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
    return m_lstEntries.count();
}

int SmartFmListsModel::columnCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
    return 9;
}

/*
0: id (tooltip: description)
1: title
2: language
3: translation_language
4: items_count
5: sentences_count
6: users_count
7: likes_count
8: favorites_count
*/
QVariant SmartFmListsModel::data(const QModelIndex &index, int role /*= Qt::DisplayRole*/) const
{
    if (role == Qt::DisplayRole){
        switch (index.column()){
        case 0: return m_lstEntries.at(index.row())->value("id");
        case 1: return m_lstEntries.at(index.row())->value("title");
        case 2: return m_lstEntries.at(index.row())->value("language");
        case 3: return m_lstEntries.at(index.row())->value("translation_language");
        case 4: return m_lstEntries.at(index.row())->value("items_count");
        case 5: return m_lstEntries.at(index.row())->value("sentences_count");
        case 6: return m_lstEntries.at(index.row())->value("users_count");
        case 7: return m_lstEntries.at(index.row())->value("likes_count");
        case 8: return m_lstEntries.at(index.row())->value("favorites_count");
        }
    }else if (role == Qt::ToolTipRole){
        if (index.column() == 0){
            return m_lstEntries.at(index.row())->value("description");
        }
    }

    return QVariant();
}

QVariant SmartFmListsModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
    if(role == Qt::DisplayRole){
        if (orientation == Qt::Horizontal){
            switch(section){
            case 0: return "id";
            case 1: return "title";
            case 2: return "lang1";
            case 3: return "lang2";
            case 4: return "items";
            case 5: return "sentences";
            case 6: return "users";
            case 7: return "likes";
            case 8: return "favorites";
            default: return QVariant();
            }
        }else{
            return section;
        }
    }else{
        return QVariant();
    }
}
