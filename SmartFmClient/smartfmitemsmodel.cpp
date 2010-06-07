#include "smartfmitemsmodel.h"

#include <QtCore>

SmartFmItemsModel::SmartFmItemsModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

/*
    return
        0: reached end of entries
        1: shouldn't happen
        2 or greater: next page number to be read
*/
int SmartFmItemsModel::parseXml(QIODevice*data)
{
    int totalResults = 0;
    int startIndex = 0;
    int itemsPerPage = 0;

    QXmlStreamReader xml(data);
    while (!xml.atEnd()){                   // loop in "vocabulary"
        xml.readNext();
        if(xml.isStartElement()){
            if(xml.name().toString() == "totalResults"){
                totalResults = xml.readElementText().toInt();
            }else if(xml.name().toString() == "startIndex"){
                startIndex = xml.readElementText().toInt();
                if(startIndex == 0){
                    m_lstEntries.clear();
                }
            }else if(xml.name().toString() == "itemsPerPage"){
                itemsPerPage = xml.readElementText().toInt();
            }else if(xml.name().toString() == "items"){
                while(!xml.atEnd()){        // loop in "items"
                    xml.readNext();
                    if(xml.isStartElement()){
                        if(xml.name().toString() == "item"){
                            parseXml_item(&xml);
                        }
                    }else if(xml.isEndElement() && xml.name().toString() == "items"){
                        break;
                    }
                }
            }
        }
    }

    if ((startIndex + itemsPerPage) < totalResults){
        return startIndex / itemsPerPage + 2;
    }else{
        return 0;
    }
}

void SmartFmItemsModel::parseXml_item(QXmlStreamReader* xml)
{
    QMap<QString, QString>* item = new QMap<QString, QString>;
    m_lstEntries.append(item);
    m_lstEntries.last()->insert("id", xml->attributes().value("id").toString());
    while(!xml->atEnd()){                   // loop in "item"
        xml->readNext();
        if(xml->isStartElement()){
            if(xml->name().toString() == "cue"){
                m_lstEntries.last()->insert("cue_language", xml->attributes().value("language").toString());
                m_lstEntries.last()->insert("cue_part_of_speech", xml->attributes().value("part_of_speech").toString());
                while(!xml->atEnd()){                   // loop in "cue"
                    xml->readNext();
                    if(xml->isStartElement()){
                        if(xml->name().toString() == "text"){
                            m_lstEntries.last()->insert("cue_text", xml->readElementText());
                        }
                    }else if(xml->isEndElement() && xml->name().toString() == "cue"){
                        break;
                    }
                }
            }else if(xml->name().toString() == "response" && xml->attributes().value("type").toString() == "meaning"){
                m_lstEntries.last()->insert("response_language", xml->attributes().value("language").toString());
                while(!xml->atEnd()){                   // loop in "response"
                    xml->readNext();
                    if(xml->isStartElement()){
                        if(xml->name().toString() == "text"){
                            m_lstEntries.last()->insert("response_text", xml->readElementText());
                        }
                    }else if(xml->isEndElement() && xml->name().toString() == "response"){
                        break;
                    }
                }
            }
        }else if(xml->isEndElement() && xml->name().toString() == "item"){
            break;
        }
    }
}

int SmartFmItemsModel::rowCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
    return m_lstEntries.count();
}

int SmartFmItemsModel::columnCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
    return 6;
}

/*
0: id
1: cue_text
2: cue_language
3: cue_part_of_speech
4: response_language
5: response_text
*/
QVariant SmartFmItemsModel::data(const QModelIndex &index, int role /*= Qt::DisplayRole*/) const
{
    if (role == Qt::DisplayRole){
        switch (index.column()){
        case 0: return m_lstEntries.at(index.row())->value("id");
        case 1: return m_lstEntries.at(index.row())->value("cue_text");
        case 2: return m_lstEntries.at(index.row())->value("cue_language");
        case 3: return m_lstEntries.at(index.row())->value("cue_part_of_speech");
        case 4: return m_lstEntries.at(index.row())->value("response_language");
        case 5: return m_lstEntries.at(index.row())->value("response_text");
        }
    }

    return QVariant();
}
