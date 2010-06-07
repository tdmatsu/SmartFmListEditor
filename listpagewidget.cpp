#include "listpagewidget.h"

#include <QtGui>

#include "SmartFmClient/smartfmclient.h"

ListPageWidget::ListPageWidget(int listId, QString listTitle, QWidget *parent) :
    QWidget(parent)
{
    m_tblView = new QTableView(this);
    m_tblView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tblView->setSelectionMode(QAbstractItemView::SingleSelection);
    QVBoxLayout* vlayout = new QVBoxLayout(this);
    vlayout->addWidget(m_tblView);

    setLayout(vlayout);

    // Smart.fm client
    m_smartFmClient = new SmartFmClient(this);
    connect(m_smartFmClient,SIGNAL(signalItemsModelReady(QAbstractTableModel*)),this,SLOT(slotItemsModelReady(QAbstractTableModel*)));

    m_smartFmClient->getItemsInList(listId);
}

void ListPageWidget::slotItemsModelReady(QAbstractTableModel*model)
{
    m_tblView->setModel(model);
    m_tblView->resizeRowsToContents();
    m_tblView->resizeColumnsToContents();
}
