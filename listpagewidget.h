#ifndef LISTPAGEWIDGET_H
#define LISTPAGEWIDGET_H

#include <QWidget>

class QTableView;
class QAbstractTableModel;
class SmartFmClient;

class ListPageWidget : public QWidget
{
Q_OBJECT

    QTableView* m_tblView;
    SmartFmClient* m_smartFmClient;
public:
    explicit ListPageWidget(int listId, QString listTitle, QWidget *parent = 0);

signals:

public slots:

private slots:
    void slotItemsModelReady(QAbstractTableModel*);

};

#endif // LISTPAGEWIDGET_H
