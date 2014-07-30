#ifndef RENAMEDIALOG_H
#define RENAMEDIALOG_H

#include <QDialog>
#include <QStringList>

namespace Ui {
    class RenameDialog;
}

class RenameDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit RenameDialog(const QStringList &files, QWidget *parent = 0);
        ~RenameDialog();

        const QStringList newFileNames() const;

    private:
        int numberOfDigits(int n);
        const QString applyPattern(int type, QString value, int index);
        void updateNames();

    private slots:
        void onRemovePattern();
        void onPatternTypeChanged(int index);
        void onExtensionTypeChanged(int index);

        void on_addPatternButton_clicked();
        void on_fileExtensionEdit_textChanged(const QString &arg1);

        void on_advancedCheckBox_toggled(bool checked);
        void showContextMenu(const QPoint &pos);

    public slots:
        virtual void accept();
        virtual void reject();

    private:
        Ui::RenameDialog *ui;

        QString m_originalPath;

        QStringList m_oldNames; // keep old file names
        QStringList m_oldExt;   // keep old file extensions

        QStringList m_newNames; // for new file names
        QStringList m_newExt;   // for new file extensions

        int m_items; // Number of items to rename
};

#endif
