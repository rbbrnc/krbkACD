#include "insertdialog.h"
#include "ui_insertdialog.h"

#include <QDebug>
#include <QFileDialog>

InsertDialog::InsertDialog(ContactData *data, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::InsertDialog)
{
	ui->setupUi(this);

    // Fill category comboBox
    QStringList categories = ContactData::categories();
    ui->categoryComboBox->addItems(categories);

    if (data) {
        m_contactData = *data;

        ui->firstName->setText(data->fieldData(ContactData::FirstName).toString());
        ui->lastName->setText(data->fieldData(ContactData::LastName).toString());
        ui->aka->setText(data->fieldData(ContactData::Aka).toString());
        ui->email->setText(data->fieldData(ContactData::EMail).toString());
        ui->webSite->setText(data->fieldData(ContactData::WebSite).toString());

        QString category = data->fieldData(ContactData::Category).toString();
        for (int i = 0; i < categories.size(); i++) {
            if (categories.value(i) ==  category) {
                ui->categoryComboBox->setCurrentIndex(i);
            }
        }

        QPixmap portrait;
        if (portrait.load(data->fieldData(ContactData::Portrait).toString())) {
            ui->portraitButton->setIcon(QIcon(portrait));
        }

        ui->notesPlainTextEdit->setPlainText(data->fieldData(ContactData::Note).toString());
    }
}

InsertDialog::~InsertDialog()
{
	delete ui;
}

ContactData &InsertDialog::data()
{
    return m_contactData;
}

// [SLOT public]
void InsertDialog::reject()
{
	emit rejected();
	QDialog::reject();
}

// [SLOT public]
void InsertDialog::accept()
{
    m_contactData.setFieldData(ContactData::Id, QVariant(-1));
    m_contactData.setFieldData(ContactData::Aka, QVariant(ui->aka->text()));
    m_contactData.setFieldData(ContactData::FirstName, QVariant(ui->firstName->text()));
    m_contactData.setFieldData(ContactData::LastName, QVariant(ui->lastName->text()));
    m_contactData.setFieldData(ContactData::EMail, QVariant(ui->email->text()));
    m_contactData.setFieldData(ContactData::WebSite, QVariant(ui->webSite->text()));
    m_contactData.setFieldData(ContactData::Category, QVariant(ui->categoryComboBox->currentText()));
    m_contactData.setFieldData(ContactData::Note, QVariant(ui->notesPlainTextEdit->toPlainText()));

	QDialog::accept();
}

void InsertDialog::on_portraitButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"));
    QPixmap portrait;
    if (portrait.load(fileName)) {
        ui->portraitButton->setIcon(QIcon(portrait));
        m_contactData.setFieldData(ContactData::Portrait, QVariant(fileName));
    }
}
