#ifndef STARTPAGEWIDGET_H
#define STARTPAGEWIDGET_H

#include <QWidget>

class QLineEdit;
class QCheckBox;
class QDialogButtonBox;
class QStackedLayout;
class QTableView;
class SmartFmClient;

#include <QModelIndex>

class StartPageWidget : public QWidget
{
Q_OBJECT
public:
    explicit StartPageWidget(QWidget *parent = 0);

signals:

public slots:
    void slotHttpError(int errorCode, QString message);

private slots:
    void on_userAccount_accepted();
    void on_listSelection_accepted();
    void on_listsmodel_ready(QAbstractTableModel*);

private:
    QStackedLayout* m_mainLayout;

    QLineEdit* m_editUsername;
    QLineEdit* m_editPassword;
    QLineEdit* m_editApiKey;
    QCheckBox* m_chkAutoLogin;
    QTableView* m_tblLists;

    QDialogButtonBox* m_buttonBoxPage1;
    QDialogButtonBox* m_buttonBoxPage3;

    SmartFmClient* m_smartFmClient;
};

#endif // STARTPAGEWIDGET_H
