#include <QStringListModel>
#include <QSortFilterProxyModel>

#include "keywordsdialog.h"
#include "ui_keywordsdialog.h"

#include "QExiv2.h"

KeywordsDialog::KeywordsDialog(const QStringList &files, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KeywordsDialog),
    m_exiv2(0),
    m_files(files)
{
    if (!init()) {
        disableWidgets();
    }
}

KeywordsDialog::KeywordsDialog(const QString &file, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KeywordsDialog),
    m_exiv2(0),
    m_files(file)
{
    if (!init()) {
        disableWidgets();
    }
}

KeywordsDialog::~KeywordsDialog()
{
    if (m_exiv2) {
        delete m_exiv2;
    }
    delete ui;
}

void KeywordsDialog::disableWidgets()
{
    ui->addButton->setEnabled(false);
    ui->removeButton->setEnabled(false);
}

bool KeywordsDialog::init()
{
    ui->setupUi(this);

    if (m_files.isEmpty()) {
        return false;
    }

    // Init model end proxy
    m_model = new QStringListModel(this);
    m_proxy = new QSortFilterProxyModel(this);

    m_exiv2 = new QExiv2();
    if (m_files.isEmpty()) {
        return false;
    } else if (m_files.count() == 1) {
        // Single File Selection
        if (m_exiv2->load(m_files.first())) {
            m_model->setStringList(m_exiv2->xmpTagStringBag("Xmp.dc.subject", true));
        } else {
            return false;
        }
    } else {
        // Multi File Selection
        m_model->setStringList(QStringList());
    }

    m_proxy->setSourceModel(m_model);

    connect(ui->keyword, SIGNAL(textChanged(QString)),
            m_proxy, SLOT(setFilterFixedString(QString)));

    m_proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->keywordListView->setModel(m_proxy);

    return true;
}

bool KeywordsDialog::loadData(const QString &file)
{
    return m_exiv2->load(file);
}

bool KeywordsDialog::saveData(const QString &file)
{
    Q_UNUSED(file)
    if (!m_exiv2->isValid()) {
        return false;
    }

    m_exiv2->setXmpTagStringBag("Xmp.dc.subject", m_model->stringList());
    if (m_exiv2->save()) {
        return true;
    }
    return false;
}

// [SLOT private]
// Add a keyword to the list
void KeywordsDialog::on_addButton_clicked()
{
    QString str = ui->keyword->text();
    if (str.isEmpty())
        return;

    str = str.simplified();

    m_model->insertRows(m_model->rowCount(), 1);
    m_model->setData(m_model->index(m_model->rowCount() - 1), str);
    ui->keyword->clear();
}

// [SLOT private]
// Remove a keyword from the list
void KeywordsDialog::on_removeButton_clicked()
{
    QModelIndex idx = ui->keywordListView->currentIndex();
    m_model->removeRow(idx.row());
}

// [SLOT private]
void KeywordsDialog::on_keywordListView_doubleClicked(const QModelIndex &index)
{
    ui->keyword->setText(index.data().toString());
}

// [SLOT public]
void KeywordsDialog::reject()
{
    emit rejected();
    QDialog::reject();
}

// [SLOT public]
void KeywordsDialog::accept()
{
    if (m_files.isEmpty()) {
        reject();
    } else if (m_files.count() == 1) {
        if (!saveData(m_files.first())) {
            qWarning() << __PRETTY_FUNCTION__ << "Error Saving Metadata:" << m_files.first();
            reject();
        }
    } else {
        for (int i = 0; i < m_files.count(); i++) {
            if (!loadData(m_files.at(i))) {
                qWarning() << __PRETTY_FUNCTION__ << "Error Loading Metadata:" << m_files.at(i);
            } else {
                saveData(m_files.at(i));
            }
        }
    }

    QDialog::accept();
}
