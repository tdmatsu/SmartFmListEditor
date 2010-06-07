#include "startpagewidget.h"

#include <QDebug>
#include <QtGui>

#include "SmartFmClient/smartfmclient.h"
#include "listpagewidget.h"

const QString SETTINGS_CREDENTIALS_USERNAME = "credentials/username";
const QString SETTINGS_CREDENTIALS_PASSWORD = "credentials/password";
const QString SETTINGS_CREDENTIALS_APIKEY = "credentials/apikey";
const QString SETTINGS_CREDENTIALS_SAVEINFO = "credentials/saveinfo";

StartPageWidget::StartPageWidget(QWidget *parent) :
    QWidget(parent)
{
    QApplication::setStyle(new QCleanlooksStyle());

    QWidget* widgetPage1 = new QWidget(this);
    {
        // password input page
        QLabel* lblUser = new QLabel("User name", widgetPage1);
        QLabel* lblPass = new QLabel("Password", widgetPage1);
        QLabel* lblApiKey = new QLabel("API Key", widgetPage1);
        m_editUsername = new QLineEdit(widgetPage1);
        m_editPassword = new QLineEdit(widgetPage1);
        m_editPassword->setEchoMode(QLineEdit::Password);
        m_editApiKey = new QLineEdit(widgetPage1);
        m_chkAutoLogin = new QCheckBox("Remember login info", widgetPage1);
        QFormLayout* formLayoutPage1 = new QFormLayout(widgetPage1);
        QPushButton* btnOk = new QPushButton("OK", widgetPage1);
        btnOk->setDefault(true);
        QPushButton* btnCancel = new QPushButton("Cancel", widgetPage1);
        m_buttonBoxPage1 = new QDialogButtonBox(widgetPage1);
        m_buttonBoxPage1->addButton(btnOk, QDialogButtonBox::AcceptRole);
        m_buttonBoxPage1->addButton(btnCancel, QDialogButtonBox::RejectRole);
        formLayoutPage1->addRow(lblUser, m_editUsername);
        formLayoutPage1->addRow(lblPass, m_editPassword);
        formLayoutPage1->addRow(lblApiKey, m_editApiKey);
        formLayoutPage1->addRow(m_chkAutoLogin);
        formLayoutPage1->addRow(m_buttonBoxPage1);
        widgetPage1->setLayout(formLayoutPage1);
    }

    QWidget* widgetPage2 = new QWidget(this);
    {
        QLabel* lblTest = new QLabel("Loading lists...", widgetPage2);
        QVBoxLayout* vBoxLayoutPage2 = new QVBoxLayout(widgetPage2);
        vBoxLayoutPage2->addWidget(lblTest);
        vBoxLayoutPage2->addStretch();
        widgetPage2->setLayout(vBoxLayoutPage2);
    }

    QWidget* widgetPage3 = new QWidget(this);
    {
        QLabel* lblListTitle = new QLabel("Select list", widgetPage3);
        m_tblLists = new QTableView(widgetPage3);
        m_tblLists->setSelectionBehavior(QAbstractItemView::SelectRows);
        m_tblLists->setSelectionMode(QAbstractItemView::SingleSelection);
        QVBoxLayout* vBoxLayoutPage3 = new QVBoxLayout(widgetPage3);
        QPushButton* btnOk = new QPushButton("OK", widgetPage3);
        btnOk->setDefault(true);
        QPushButton* btnCancel = new QPushButton("Cancel", widgetPage3);
        m_buttonBoxPage3 = new QDialogButtonBox(widgetPage3);
        m_buttonBoxPage3->addButton(btnOk, QDialogButtonBox::AcceptRole);
        m_buttonBoxPage3->addButton(btnCancel, QDialogButtonBox::RejectRole);

        // add controls to the layout
        vBoxLayoutPage3->addWidget(lblListTitle);
        vBoxLayoutPage3->addWidget(m_tblLists);
        vBoxLayoutPage3->addStretch();
        vBoxLayoutPage3->addWidget(m_buttonBoxPage3);
        widgetPage3->setLayout(vBoxLayoutPage3);
    }

    // set the main layout (stacked)
    m_mainLayout = new QStackedLayout;
    m_mainLayout->addWidget(widgetPage1);
    m_mainLayout->addWidget(widgetPage2);
    m_mainLayout->addWidget(widgetPage3);
    setLayout(m_mainLayout);

    // load settings
    QSettings settings;
    m_editUsername->setText(settings.value(SETTINGS_CREDENTIALS_USERNAME).toString());
    m_editPassword->setText(settings.value(SETTINGS_CREDENTIALS_PASSWORD).toString());
    m_editApiKey->setText(settings.value(SETTINGS_CREDENTIALS_APIKEY).toString());
    m_chkAutoLogin->setChecked(settings.value(SETTINGS_CREDENTIALS_SAVEINFO).toString() == "true");

    // connections
    connect(m_buttonBoxPage1, SIGNAL(accepted()), this, SLOT(on_userAccount_accepted()));
    connect(m_buttonBoxPage1, SIGNAL(rejected()), this, SLOT(close()));
    connect(m_buttonBoxPage3, SIGNAL(accepted()), this, SLOT(on_listSelection_accepted()));
    connect(m_buttonBoxPage3, SIGNAL(rejected()), this, SLOT(close()));

    // Smart.fm client
    m_smartFmClient = new SmartFmClient(this);
    connect(m_smartFmClient, SIGNAL(signalListsModelReady(QAbstractTableModel*)), this, SLOT(on_listsmodel_ready(QAbstractTableModel*)));
    connect(m_smartFmClient, SIGNAL(signalHttpError(int,QString)), this, SLOT(slotHttpError(int,QString)));
}

void StartPageWidget::on_userAccount_accepted()
{
    QString errMsg;
    if(m_editUsername->text().isEmpty()){
        errMsg = "Please set user name!!";
        m_editUsername->setFocus();
    }else if(m_editPassword->text().isEmpty()){
        errMsg = "Please set password!!";
        m_editPassword->setFocus();
    }else if(m_editApiKey->text().isEmpty()){
        errMsg = "Please set API Key!!";
        m_editApiKey->setFocus();
    }

    if(!errMsg.isEmpty()){
        QMessageBox msg(this);
        msg.setText(errMsg);
        msg.exec();
        return;
    }

    QSettings settings;
    if(m_chkAutoLogin->isChecked()){
        // save settings
        settings.setValue(SETTINGS_CREDENTIALS_USERNAME, m_editUsername->text());
        settings.setValue(SETTINGS_CREDENTIALS_PASSWORD, m_editPassword->text());
        settings.setValue(SETTINGS_CREDENTIALS_APIKEY, m_editApiKey->text());
        settings.setValue(SETTINGS_CREDENTIALS_SAVEINFO, m_chkAutoLogin->isChecked()?"true":"false");
    }else{
        // clear saved settings
        settings.setValue(SETTINGS_CREDENTIALS_USERNAME, "");
        settings.setValue(SETTINGS_CREDENTIALS_PASSWORD, "");
        settings.setValue(SETTINGS_CREDENTIALS_APIKEY, "");
        settings.setValue(SETTINGS_CREDENTIALS_SAVEINFO, "false");
    }

    m_smartFmClient->getListsCreated(m_editUsername->text(), m_editPassword->text());

    m_mainLayout->setCurrentIndex(1);
}

void StartPageWidget::on_listSelection_accepted()
{
    if (m_tblLists->selectionModel()->selectedIndexes().isEmpty()){
        QMessageBox msg(this);
        msg.setText("Please select list.");
        msg.exec();
        return;
    }
    int listId = m_tblLists->selectionModel()->selectedRows(0).at(0).data(Qt::DisplayRole).toInt();
    QString listTitle = m_tblLists->selectionModel()->selectedRows(1).at(0).data(Qt::DisplayRole).toString();

    qDebug() << m_tblLists->selectionModel()->selectedRows(0).at(0).data(Qt::DisplayRole).toString();   // id
    qDebug() << m_tblLists->selectionModel()->selectedRows(1).at(0).data(Qt::DisplayRole).toString();   // title
    qDebug() << m_tblLists->selectionModel()->selectedRows(2).at(0).data(Qt::DisplayRole).toString();   // lang1

    ListPageWidget* listPage = new ListPageWidget(listId, listTitle);
    listPage->show();
}

void StartPageWidget::on_listsmodel_ready(QAbstractTableModel*model)
{
    m_tblLists->setModel(model);
    m_tblLists->resizeRowsToContents();
    m_tblLists->resizeColumnsToContents();
    m_mainLayout->setCurrentIndex(2);
}

void StartPageWidget::slotHttpError(int errorCode, QString message)
{
    if (errorCode == 401){
        QMessageBox msg(this);
        msg.setText("Authentication failed!!");
        msg.exec();
        m_mainLayout->setCurrentIndex(0);
    }else{
        QMessageBox msg(this);
        msg.setText("Unknown error!!");
        msg.exec();
    }
}
