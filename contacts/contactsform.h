#ifndef CONTACTSFORM_H
#define CONTACTSFORM_H

#include <QWidget>
#include <QModelIndex>

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

    private slots:
        void editRecord(int row);

        void on_addContactButton_clicked();
        void on_delContactButton_clicked();
//      void onContactChanged(const QModelIndex &current, const QModelIndex &previous);
        void on_updateButton_clicked();
        void on_categoryFilter_currentIndexChanged(int index);
        void onViewContextMenuRequested(QPoint pos);
        void on_actionEditContact_triggered();

    private:
        Ui::ContactsForm *ui;
        DatabaseManager *m_dbm;
        QItemSelectionModel *m_selectionModel;
        QSortFilterProxyModel *m_categoryFilter;
};

#endif // CONTACTSFORM_H
