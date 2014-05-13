#include "contactsform.h"
#include "ui_contactsform.h"

#include <QDebug>

#include <QItemSelectionModel>
#include <QSortFilterProxyModel>
#include <QMessageBox>

#include "db.h"
#include "insertdialog.h"

ContactsForm::ContactsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ContactsForm)
{
    ui->setupUi(this);

    m_dbm = new DatabaseManager(this);

    if (!m_dbm->openDB()) {
        qDebug() << "Error open DB:" << m_dbm->lastError().text();
        return;
    }

    // Refresh sorting
    QHeaderView *hw = ui->contactView->horizontalHeader();
    connect(hw, SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)), m_dbm, SLOT(sort(int, Qt::SortOrder)));


    ui->contactView->setModel(m_dbm->model());

    // Default Hide
    ui->contactView->hideColumn(ContactData::Id);
    ui->contactView->hideColumn(ContactData::Category);
    ui->contactView->hideColumn(ContactData::Portrait);

    m_categoryFilter = new QSortFilterProxyModel(this);
    m_categoryFilter->setSourceModel(m_dbm->model());

    // Set sort e filter su colonna
    m_categoryFilter->sort(ContactData::Category, Qt::AscendingOrder);
    m_categoryFilter->setFilterKeyColumn(ContactData::Category);

    ui->contactView->setModel(m_categoryFilter);
    ui->contactView->setSortingEnabled(true);

    ui->contactView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->contactView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_selectionModel = ui->contactView->selectionModel();

    // Fill category comboBox
    QStringList categories;
    categories << "__All__";
    categories << ContactData::categories();
    ui->categoryFilter->addItems(categories);


    ui->contactView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->contactView, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(onViewContextMenuRequested(QPoint)));

    ui->contactView->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->contactView->horizontalHeader(), SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(onHeaderContextMenuRequested(QPoint)));
/*
    connect(m_selectionModel, SIGNAL(currentChanged(QModelIndex, QModelIndex)),
            this, SLOT(onContactChanged(QModelIndex, QModelIndex)));
*/

    // Connect Actions
    connect(ui->actionAddContact, SIGNAL(triggered()), this, SLOT(addContact()));
    connect(ui->addContactButton, SIGNAL(clicked()), this, SLOT(addContact()));
}

ContactsForm::~ContactsForm()
{
    delete ui;
}

// [SLOT]
void ContactsForm::addContact()
{
    ContactData data;
    InsertDialog dlg(&data, this);
    if (dlg.exec() == QDialog::Accepted) {
        m_dbm->addRecord(dlg.data());
    }
}

void ContactsForm::editRecord(int row)
{
    // Check for a valid selection
    if (row < 0) {
        return;
    }

    ContactData data;
    m_dbm->getRecord(row, data);

    InsertDialog dlg(&data, this);
    if (dlg.exec() == QDialog::Accepted) {
        m_dbm->updateRecord(row, dlg.data());
    }
}

/*
void ContactsForm::onContactChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)
    qDebug() << __func__;
}
*/

void ContactsForm::on_delContactButton_clicked()
{
    ContactData data;
    int row = ui->contactView->currentIndex().row();

    m_dbm->getRecord(row, data);

    //QString msg = QString("Vuoi cancellare il contatto\n%1").arg(data.company);
    QString msg = QString("Delete Record?");

    if (QMessageBox::No == QMessageBox::warning(this, "Delete Contact",
                                      msg, QMessageBox::Yes, QMessageBox::No)) {
        return;
    }
    /*bool*/ m_dbm->delRecord(row);
}

void ContactsForm::on_updateButton_clicked()
{
	// Get correct row from model
	QModelIndex proxyIndex = ui->contactView->currentIndex();
	QModelIndex mi = m_categoryFilter->mapToSource(proxyIndex);

    int row = mi.row();
    if (row < 0) {
        return;
    }

    editRecord(row);
}

void ContactsForm::on_categoryFilter_currentIndexChanged(int index)
{
    if (index == 0) {
        m_categoryFilter->setFilterRegExp("");
    } else {
        m_categoryFilter->setFilterRegExp(ui->categoryFilter->itemText(index));
    }
}

QStringList ContactsForm::currentSelection() const
{
    QStringList sel;
    if (m_selectionModel->hasSelection()) {
        QModelIndexList selection = m_selectionModel->selectedRows();
        for (int i = 0; i < selection.count(); i++) {
            int row = selection.at(i).row();
            QModelIndex index = ui->contactView->model()->index(row, 4, QModelIndex());
            sel.append(ui->contactView->model()->data(index).toString());
        }
    }
    return sel;
}

void ContactsForm::onViewContextMenuRequested(QPoint pos)
{
    int row = ui->contactView->indexAt(pos).row();
    ui->actionEditContact->setData(QVariant(row));

    QMenu *menu = new QMenu(this);
    menu->addAction(ui->actionEditContact);
    menu->addAction(ui->actionShowAllColumns);

    menu->popup(ui->contactView->viewport()->mapToGlobal(pos));
}

void ContactsForm::onHeaderContextMenuRequested(QPoint pos)
{
    int column = ui->contactView->horizontalHeader()->logicalIndexAt(pos);
    ui->actionHideColumn->setData(QVariant(column));

    QMenu *menu = new QMenu(this);
    menu->addAction(ui->actionHideColumn);
    menu->popup(ui->contactView->horizontalHeader()->viewport()->mapToGlobal(pos));
}


void ContactsForm::on_actionEditContact_triggered()
{
	// Get correct row from model
	QModelIndex proxyIndex = m_categoryFilter->index(ui->actionEditContact->data().toInt(), ContactData::Category);
	QModelIndex mi = m_categoryFilter->mapToSource(proxyIndex);

    int row = mi.row();
    if (row < 0) {
        return;
    }
    editRecord(row);
}

void ContactsForm::on_actionHideColumn_triggered()
{
    int col = ui->actionHideColumn->data().toInt();
    ui->contactView->hideColumn(col); // hide ID
}

void ContactsForm::on_actionShowAllColumns_triggered()
{
    for (int i=0; i < ui->contactView->model()->columnCount(); i++) {
        if (ui->contactView->isColumnHidden(i)) {
            ui->contactView->showColumn(i);
        }
    }
}

void ContactsForm::setMenu(QMenu *parent)
{
    parent->addAction(ui->actionAddContact);
    parent->addAction(ui->actionShowAllColumns);

    for (int i=0; i < ui->contactView->model()->columnCount(); i++) {
        QAction *act = new QAction(QString("Show Column %1").arg(i), this);
        //m_fieldsMenu.append(menu);
        parent->addAction(act);
    }


}
