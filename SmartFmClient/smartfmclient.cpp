/*

//------------------------------------------------------
// SmartFmClient usage example
//------------------------------------------------------

// constructor
    m_smartFmClient = new SmartFmClient(this);
    connect(m_smartFmClient, SIGNAL(signalListsModelReady(QAbstractTableModel*)), this, SLOT(on_listsmodel_ready(QAbstractTableModel*)));
    connect(m_smartFmClient, SIGNAL(signalItemsModelReady(QAbstractTableModel*)), this, SLOT(on_itemsmodel_ready(QAbstractTableModel*)));

// slots

void MainWindow::on_listsmodel_ready(QAbstractTableModel*model)
{
    m_tblLists->setModel(model);
}

void MainWindow::on_itemsmodel_ready(QAbstractTableModel*model)
{
    m_tblItems->setModel(model);
}


// API calls
    // get items in the list
    m_smartFmClient->getItemsInList(50800); // list id

    // get lists created by the user (include private lists)
    m_smartFmClient->getListsCreated("username", "password");

*/


#include "smartfmclient.h"
#include "smartfmlistsmodel.h"
#include "smartfmitemsmodel.h"

#include <QDebug>

#include <QtNetwork>

const QString API_BASE_URL = "http://api.smart.fm/";
const QString API_ITEMS_PER_PAGE = "10";

SmartFmClient::SmartFmClient(QObject *parent)
    : QObject(parent)
{
    m_networkAccessManager = new QNetworkAccessManager(this);

    m_listsModel = new SmartFmListsModel;
    m_itemsModel = new SmartFmItemsModel;
}

void SmartFmClient::slotReplyAuthenticationRequired(QNetworkReply * reply, QAuthenticator * authenticator)
{
    qDebug() << "SmartFmClient::slotReplyAuthenticationRequired";
    authenticator->setUser(m_strUserName);
    authenticator->setPassword(m_strPassword);
}

void SmartFmClient::getListsCreated(QString userName, QString passWord, int page/* = 1*/)
{
    qDebug() << "SmartFmClient::getListsCreated";

    m_strUserName = userName;
    m_strPassword = passWord;

    QString strUrl = API_BASE_URL + "users/" + m_strUserName + "/lists/creator.xml";

    QUrl url(strUrl);

    QList< QPair <QString, QString> > query;
    query.append(QPair<QString, QString>("per_page", API_ITEMS_PER_PAGE));
    query.append(QPair<QString, QString>("page", QString::number(page)));
    query.append(QPair<QString, QString>("include_private", "true"));
    url.setQueryItems(query);

    /*QNetworkReply* reply = */m_networkAccessManager->get(QNetworkRequest(url));
    m_networkAccessManager->disconnect();
    connect(m_listsModel, SIGNAL(signalHttpError(int,QString)), SLOT(slotHttpError(int,QString)));
    connect(m_networkAccessManager, SIGNAL(finished(QNetworkReply*)), SLOT(slotReplyListsCreated(QNetworkReply*)));
    connect(m_networkAccessManager, SIGNAL(authenticationRequired(QNetworkReply*, QAuthenticator*)), SLOT(slotReplyAuthenticationRequired(QNetworkReply*, QAuthenticator*)));
}

void SmartFmClient::slotReplyListsCreated(QNetworkReply* reply)
{
    qDebug() << "SmartFmClient::slotReplyListsCreated";

    int nextPage = m_listsModel->parseXml(reply);
    if (nextPage == -1){
        // do nothing error signal should have been already emitted
    }else if (nextPage > 0){
        qDebug() << "loading next page..." << nextPage;
        getListsCreated(m_strUserName, m_strPassword, nextPage);
    }else{
        emit signalListsModelReady(m_listsModel);
    }
}

void SmartFmClient::slotReplyItemsInList(QNetworkReply* reply)
{
    qDebug() << "SmartFmClient::slotReplyItemsInList";
    int nextPage = ((SmartFmItemsModel*)m_itemsModel)->parseXml(reply);
    if (nextPage > 0){
        qDebug() << "loading next page..." << nextPage;
        getItemsInList(m_listId, nextPage);
    }else{
        emit signalItemsModelReady(m_itemsModel);
    }
}

void SmartFmClient::getItemsInList(int listId, int page/* = 1*/)
{
    qDebug() << "SmartFmClient::getItemsInList listId = " << listId;

    m_listId = listId;

    QString strUrl = API_BASE_URL + "lists/" + QString::number(m_listId) + "/items.xml";

    QUrl url(strUrl);

    QList< QPair <QString, QString> > query;
    query.append(QPair<QString, QString>("page", QString::number(page)));
    query.append(QPair<QString, QString>("per_page", API_ITEMS_PER_PAGE));
    url.setQueryItems(query);

    /*QNetworkReply* reply = */m_networkAccessManager->get(QNetworkRequest(url));
    m_networkAccessManager->disconnect();
    connect(m_networkAccessManager, SIGNAL(finished(QNetworkReply*)), SLOT(slotReplyItemsInList(QNetworkReply*)));
    connect(m_networkAccessManager, SIGNAL(authenticationRequired(QNetworkReply*, QAuthenticator*)), SLOT(slotReplyAuthenticationRequired(QNetworkReply*, QAuthenticator*)));
}

void SmartFmClient::slotHttpError(int errorCode, QString message)
{
    emit signalHttpError(errorCode, message);
}
