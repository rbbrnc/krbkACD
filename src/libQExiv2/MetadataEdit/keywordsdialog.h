#ifndef KEYWORDSDIALOG_H
#define KEYWORDSDIALOG_H

#include <QDialog>

class QStringListModel;
class QSortFilterProxyModel;
class QExiv2;

namespace Ui {
    class KeywordsDialog;
}

class KeywordsDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit KeywordsDialog(const QString &file, QWidget *parent = 0);
        explicit KeywordsDialog(const QStringList &files, QWidget *parent = 0);
        ~KeywordsDialog();

    private:
        bool init();
        bool loadData(const QString &file);
        bool saveData(const QString &file);
        void disableWidgets();

    public slots:
        virtual void accept();
        virtual void reject();

    private slots:
        void on_addButton_clicked();
        void on_removeButton_clicked();
        void on_keywordListView_doubleClicked(const QModelIndex &index);

private:
        Ui::KeywordsDialog *ui;

        QExiv2 *m_exiv2;
        QStringList m_files;

        QStringListModel *m_model;
        QSortFilterProxyModel *m_proxy;
};

#endif
