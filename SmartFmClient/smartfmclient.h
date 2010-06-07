#ifndef SMARTFMCLIENT_H
#define SMARTFMCLIENT_H

#include <QObject>

class QNetworkAccessManager;
class QNetworkReply;
class QAuthenticator;
class QIODevice;
class QAbstractTableModel;
class SmartFmListsModel;
class SmartFmItemsModel;

class SmartFmClient : public QObject
{
    Q_OBJECT
private:
    QNetworkAccessManager *m_networkAccessManager;

    // user account
    QString m_strUserName;
    QString m_strPassword;
    QString m_strApiKey;

    int m_listId;

    // models
    SmartFmListsModel* m_listsModel;
    SmartFmItemsModel* m_itemsModel;

public:
    explicit SmartFmClient(QObject *parent = 0);

    void getListsCreated(QString userName, QString passWord, int page = 1);
    void getItemsInList(int listId, int page = 1);

    void setApiKey(QString apiKey);

signals:
    void signalListsModelReady(QAbstractTableModel*);
    void signalItemsModelReady(QAbstractTableModel*);
    void signalHttpError(int errorCode, QString message);

private slots:
    void slotReplyListsCreated(QNetworkReply*);
    void slotReplyItemsInList(QNetworkReply*);

public slots:
    void slotHttpError(int errorCode, QString message);

    // if authentication is required
    void slotReplyAuthenticationRequired(QNetworkReply * reply, QAuthenticator * authenticator);
};

#endif // SMARTFMCLIENT_H
