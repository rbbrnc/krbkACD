#ifndef CONTACTSFORM_H
#define CONTACTSFORM_H

#include <QWidget>
#include <QModelIndex>

class QMenu;

namespace Ui {
    class ContactsForm;
}

class DatabaseManager;
class QItemSelectionModel;
class QSortFilterProxyModel;

class ContactsForm : public QWidget
{
    Q_OBJECT

    public:
        explicit ContactsForm(QWidget *parent = 0);
        ~ContactsForm();

        QStringList currentSelection() const;

        void setMenu(QMenu *parent);

    private slots:
        void addContact();
        void editRecord(int row);

        void on_delContactButton_clicked();
//      void onContactChanged(const QModelIndex &current, const QModelIndex &previous);
        void on_updateButton_clicked();
        void on_categoryFilter_currentIndexChanged(int index);

        void onViewContextMenuRequested(QPoint pos);
        void onHeaderContextMenuRequested(QPoint pos);

        void on_actionEditContact_triggered();
        void on_actionHideColumn_triggered();
        void on_actionShowAllColumns_triggered();

private:
        Ui::ContactsForm *ui;
        DatabaseManager *m_dbm;
        QItemSelectionModel *m_selectionModel;
        QSortFilterProxyModel *m_categoryFilter;

        QList<QMenu *> m_fieldsMenu;
};

#endif // CONTACTSFORM_H
