/********************************************************************************
** Form generated from reading UI file 'socialmetadatadialog.ui'
**
** Created: Thu Jul 4 13:10:53 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SOCIALMETADATADIALOG_H
#define UI_SOCIALMETADATADIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_SocialMetadataDialog
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *label;
    QLineEdit *lineEdit;

    void setupUi(QDialog *SocialMetadataDialog)
    {
        if (SocialMetadataDialog->objectName().isEmpty())
            SocialMetadataDialog->setObjectName(QString::fromUtf8("SocialMetadataDialog"));
        SocialMetadataDialog->resize(703, 529);
        buttonBox = new QDialogButtonBox(SocialMetadataDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(350, 480, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        label = new QLabel(SocialMetadataDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(30, 30, 57, 15));
        label->setTextFormat(Qt::RichText);
        label->setOpenExternalLinks(true);
        lineEdit = new QLineEdit(SocialMetadataDialog);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(100, 20, 113, 23));

        retranslateUi(SocialMetadataDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), SocialMetadataDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), SocialMetadataDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(SocialMetadataDialog);
    } // setupUi

    void retranslateUi(QDialog *SocialMetadataDialog)
    {
        SocialMetadataDialog->setWindowTitle(QApplication::translate("SocialMetadataDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("SocialMetadataDialog", "Album:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SocialMetadataDialog: public Ui_SocialMetadataDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SOCIALMETADATADIALOG_H
