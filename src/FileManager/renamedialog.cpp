#include "renamedialog.h"
#include "ui_renamedialog.h"

#include <QUuid>
#include <QFileInfo>
#include <QMenu>
#include <QDebug>

static const QStringList g_patternTypes = {
    "Text",
    "UUID",
    "FileName",
    "Counter"
};

enum RenamePatternType {
    Text_Pattern = 0,
    UUID_Pattern,
    FileName_Pattern,
    Counter_Pattern
};

static const QStringList g_extensionTypes = {
    "Original",
    "Lower Case",
    "Upper Case",
    "Text"
};

enum ExtPatternType {
    Ext_Original = 0, //
    Ext_LowerCase,    // Original lower case
    Ext_UpperCase,    // Original upper case
    Ext_Text          // Free text
};

///////////////////////////////////////////////////////////////////////////////

RenameDialog::RenameDialog(const QStringList &files, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RenameDialog)
{
    ui->setupUi(this);

    m_items = files.count();

    if (m_items > 1) {
        // Multiple File
        ui->singleFileFrame->setVisible(false);
        ui->advancedCheckBox->setChecked(true);
        ui->advancedCheckBox->setVisible(false);
    } else {
        // Sigle File
        ui->multipleFileFrame->setVisible(false);
        ui->advancedCheckBox->setChecked(false);
        ui->advancedOptionsFrame->setVisible(false);
        ui->oldNameLabel->setText(files.first());
        ui->newNameLineEdit->setText(files.first());
    }

    ui->fileExtensionEdit->hide();
    adjustSize();


    // Set file path, names and extensions
    QFileInfo fi(files.first());
    m_originalPath = fi.absolutePath() + '/';
    ui->filePath->setText(m_originalPath);

    m_oldNames.reserve(m_items);
    m_oldExt.reserve(m_items);
    m_newExt.reserve(m_items);
    m_newNames.reserve(m_items);

    for (int i = 0; i < m_items; i++) {
        fi = QFileInfo(files.at(i));

        m_oldNames << fi.completeBaseName();
        m_newNames << fi.completeBaseName();

        if (fi.suffix().isEmpty()) {
            m_oldExt << "";
            m_newExt << "";
        } else {
            m_oldExt << "." + fi.suffix();
            m_newExt << "." + fi.suffix();
        }

        ui->oldList->addItem(m_oldNames.at(i) + m_oldExt.at(i));
        ui->newList->addItem(m_newNames.at(i) + m_newExt.at(i));
    }

    ui->patternTypeComboBox->insertItems(0, g_patternTypes);
    ui->fileExtensionComboBox->insertItems(0, g_extensionTypes);

    // Connect the signals here to avoid triggering on insertItems.
    connect(ui->patternTypeComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &RenameDialog::onPatternTypeChanged);

    connect(ui->fileExtensionComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &RenameDialog::onExtensionTypeChanged);

    // Context Menu for remove patterns
    ui->patternList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->patternList, &QListWidget::customContextMenuRequested, this, &RenameDialog::showContextMenu);
    connect(ui->actionRemovePattern, &QAction::triggered, this, &RenameDialog::onRemovePattern);
}

RenameDialog::~RenameDialog()
{
    delete ui;
}

void RenameDialog::showContextMenu(const QPoint &pos)
{
    QMenu contextMenu("menu", this);
    contextMenu.addAction(ui->actionRemovePattern);
    contextMenu.exec(ui->patternList->mapToGlobal(pos));
}

void RenameDialog::on_addPatternButton_clicked()
{
    // Add new pattern to list.
    int patternType = ui->patternTypeComboBox->currentIndex();

    // Don't add pattern with empty text (not for FileName_Pattern)
    if (patternType != FileName_Pattern) {
        if (ui->patternValue->text().isEmpty()) {
            return;
        }
    }

    QString patternResult = g_patternTypes.at(patternType) + ":" + ui->patternValue->text();

    /*QListWidgetItem *item =*/ new QListWidgetItem(patternResult, ui->patternList, patternType);

    if (ui->patternList->count() == 1) {
        // 1st pattern overwrite default old names
        for (int j = 0; j < m_items; j++) {
            m_newNames[j] = applyPattern(patternType, ui->patternValue->text(), j);
        }
    } else {
        // Append new pattern
        for (int j = 0; j < m_items; j++) {
            m_newNames[j] += applyPattern(patternType, ui->patternValue->text(), j);
        }
    }
    updateNames();
}

void RenameDialog::onRemovePattern()
{
    // If already empty return
    if (!ui->patternList->count()) {
        return;
    }

    // Delete selected pattern item
    delete ui->patternList->takeItem(ui->patternList->currentRow());

    // With remaining patterns (if any) ...
    if (ui->patternList->count() > 0) {
        // Remove pattern from new names is a two step action:
        // 1. Reset new names
        for (int i = 0; i < m_items; i++) {
            m_newNames[i].clear();
        }

        // 2. Re-apply all rename patterns
        for (int i = 0; i < ui->patternList->count(); i++) {
            QString patternText = ui->patternList->item(i)->text();
            QString re = g_patternTypes.at(ui->patternList->item(i)->type()) + ":";
            patternText.remove(0, re.size());

            for (int j = 0; j < m_items; j++) {
                m_newNames[j] += applyPattern(ui->patternList->item(i)->type(), patternText, j);
            }
        }
    }
    // Update list widget
    updateNames();
}

int RenameDialog::numberOfDigits(int n)
{
    int nc = 1;
    while (n / 10) {
        nc++;
    }
    return nc;
}

void RenameDialog::updateNames()
{
    // If pattern list is empty restore old names.
    if (!ui->patternList->count()) {
        m_newNames = m_oldNames;
    }

    if (m_items > 1) {
        for (int j = 0; j < m_items; j++) {
            ui->newList->item(j)->setText(m_newNames.at(j) + m_newExt.at(j));
        }
    } else {
        ui->newNameLineEdit->setText(m_newNames.first() + m_newExt.first());
    }
}

void RenameDialog::on_fileExtensionEdit_textChanged(const QString &arg1)
{
    for (int i = 0; i < m_items; i++) {
        m_newExt.replace(i, arg1);
    }
    updateNames();
}

// [public]
const QStringList RenameDialog::newFileNames() const
{
    QStringList sl;

    for (int i = 0; i < m_items; i++) {
        sl << m_newNames.at(i) + m_newExt.at(i);
    }
    return sl;
}

// [SLOT public]
void RenameDialog::reject()
{
    emit rejected();
    QDialog::reject();
}

// [SLOT public]
void RenameDialog::accept()
{
    // Lists consistency check.
    if (m_newNames.isEmpty() || (m_newNames.count() != m_items)) {
        reject();
        return;
    }

    if (m_items == 1) {
        // For single file get the whole new name from lineEdit.
        m_newNames[0] = ui->newNameLineEdit->text();
        m_newExt[0].clear();
    }

    // Reject if the old name is equal to the new.
    if ((m_newNames == m_oldNames) && (m_newExt == m_oldExt)) {
        reject();
        return;
    }

    QDialog::accept();
}

const QString RenameDialog::applyPattern(int type, QString value, int index)
{
    QString s;

    switch (type) {
    case Text_Pattern:
    case UUID_Pattern:
        s = value;
        break;
    case FileName_Pattern:
        s = m_oldNames.at(index); // original name
        break;
    case Counter_Pattern:
        s = QString("%1").arg(index, value.toInt(), 10, QLatin1Char('0'));
        break;
    default:
        break;
    }

    return s;
}

void RenameDialog::onPatternTypeChanged(int index)
{
    QString s;

    ui->patternValue->clear();

    switch (index) {
    case Text_Pattern:
        ui->patternValueDesc->setText("Free Text");
        ui->patternValue->setEnabled(true);
        break;

    case UUID_Pattern:
        s = QUuid::createUuid().toString().toUpper();
        s.remove('{');
        s.remove('}');
        s.remove('-');
        ui->patternValueDesc->clear();
        ui->patternValue->setText(s);
        ui->patternValue->setEnabled(false);
        break;

    case Counter_Pattern:
        ui->patternValueDesc->setText("Number of digit");
        ui->patternValue->setEnabled(true);
        ui->patternValue->setText(QString::number(numberOfDigits(m_items)));
        break;

    case FileName_Pattern:
        ui->patternValueDesc->clear();
        ui->patternValue->setEnabled(false);
        break;

    default:
        break;
    }
}

void RenameDialog::onExtensionTypeChanged(int index)
{
    switch (index) {
    case Ext_Original:
        ui->fileExtensionEdit->hide();
        m_newExt = m_oldExt;
        break;

    case Ext_LowerCase:
        ui->fileExtensionEdit->hide();
        for (int i = 0; i < m_items; i++) {
            m_newExt.replace(i, m_oldExt.at(i).toLower());
        }
        break;

    case Ext_UpperCase:
        ui->fileExtensionEdit->hide();
        for (int i = 0; i < m_items; i++) {
            m_newExt.replace(i, m_oldExt.at(i).toUpper());
        }
        break;

    case Ext_Text:
        ui->fileExtensionEdit->show();
        break;

    default:
        return;
    }

    updateNames();
}

void RenameDialog::on_advancedCheckBox_toggled(bool checked)
{
    ui->advancedOptionsFrame->setVisible(checked);
    ui->newNameLineEdit->setReadOnly(checked);

    adjustSize();
}
