#ifndef INSERT_DIALOG_H
#define INSERT_DIALOG_H

#include "qglobal.h"
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#else
#include <QtGui>
#endif

#include <QDialog>

#include "dbcontactsdata.h"

namespace Ui {
    class InsertDialog;
}

class InsertDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit InsertDialog(ContactData *data, QWidget *parent = 0);
        ~InsertDialog();

        ContactData &data();

    public slots:
        virtual void accept();
        virtual void reject();

    private slots:
        void on_portraitButton_clicked();

    private:
        Ui::InsertDialog *ui;
        ContactData m_contactData;
};

#endif
