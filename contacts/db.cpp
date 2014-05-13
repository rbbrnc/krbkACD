#include <QVariant>
#include <QDebug>

#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlField>

#include "db.h"
#include "dbcontactsdata.h"

DatabaseManager::DatabaseManager(QObject *parent)
    : QObject(parent)
{
    // Find QSLite driver
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(ContactData::dbPath());

    m_sqlModel = 0;
}

DatabaseManager::~DatabaseManager()
{
}

QSqlError DatabaseManager::lastError()
{
    // If opening database has failed user can ask
    // error description by QSqlError::text()
    return m_db.lastError();
}

QAbstractItemModel *DatabaseManager::model() const
{
	return m_sqlModel;
}

void DatabaseManager::sort(int column, Qt::SortOrder order)
{
    //qDebug() << "Sort by:" << ContactData::fields().at(column) << "order:" << order;
    m_sqlModel->sort(column, order);
}

bool DatabaseManager::setSqlModel()
{

    if (!m_sqlModel) {
        m_sqlModel = new QSqlTableModel(this, m_db);
    }

    const QStringList fields = ContactData::fields();

    m_sqlModel->setTable(ContactData::dbTableName());
    if (m_sqlModel->columnCount() != fields.count()) {
        qDebug() << "WARNING TABLE SIZE MISMATCH!";
    }

    m_sqlModel->setSort(0, Qt::DescendingOrder);

    for (int i = 0; i < fields.count(); i++) {
        m_sqlModel->setHeaderData(i, Qt::Horizontal, fields.at(i));
    }

    m_sqlModel->select();
    return true;
}

// Open databasee
bool DatabaseManager::openDB()
{
    if (m_db.open()) {
        if (m_db.record(ContactData::dbTableName()).isEmpty()) {
            qDebug() << "NO TABLE" << ContactData::dbTableName() << "Present!";
            createTable();
        } else {
            setSqlModel();
        }
        return true;
    }

    qDebug() << "OpenDB failed";
    return false;
}

bool DatabaseManager::deleteDB()
{
   m_db.close();
    return QFile::remove(ContactData::dbPath());
}

bool DatabaseManager::createTable()
{
	bool ret = false;
    if (m_db.isOpen()) {
		QSqlQuery query;
		ret = query.exec(ContactData::createQuery());
        setSqlModel();
        qDebug() << __PRETTY_FUNCTION__ << "ret=" << ret << lastError().text();
	} else {
        qDebug() << "Error DB not opened!" << ret << lastError().text();;
	}

	return ret;
}

void DatabaseManager::addRecord(ContactData &data)
{
    if (!m_db.isOpen()) {
        return;
	}

    //http://www.sqlite.org/autoinc.html
    // NULL = is the keyword for the autoincrement to generate next value
    // http://www.sqlite.org/c3ref/last_insert_rowid.html
    int row = m_sqlModel->rowCount();
    m_sqlModel->insertRows(row, 1);

    // note column 0 is the pk_id
    for (int i = 1; i < data.fieldCount(); i++) {
	    m_sqlModel->setData(m_sqlModel->index(row, i), data.fieldData(i));
	}

    // Commit to the DB
    if (!m_sqlModel->submitAll()) {
        qWarning() << __PRETTY_FUNCTION__ << "submitAll():" << m_sqlModel->lastError().text();
	}

    // Update the model
    m_sqlModel->select();
}

void DatabaseManager::updateRecord(int row, ContactData &data)
{
    if (!m_db.isOpen()) {
        qDebug() << __PRETTY_FUNCTION__ << "DB Closed";
        return;
    }

    QSqlRecord record = m_sqlModel->record(row);

    for (int i = 1; i < data.fieldCount(); i++) {
		record.setValue(i, data.fieldData(i));
	}

	if (!m_sqlModel->setRecord(row, record)) {
        qWarning() << __PRETTY_FUNCTION__ << "setRecord() Error:" << m_sqlModel->lastError().text();
		return;
	}

    if (!m_sqlModel->submitAll()) {
        qWarning() << __PRETTY_FUNCTION__ << "submitAll():" << m_sqlModel->lastError().text();
	}

    m_sqlModel->select();
}

// Delete a record/row from the DB.
void DatabaseManager::delRecord(int row)
{
    if (!m_db.isOpen()) {
        qDebug() << __PRETTY_FUNCTION__ << "DB Closed";
        return;
    }

    m_sqlModel->removeRows(row, 1);
    if (!m_sqlModel->submitAll()) {
        qWarning() << __PRETTY_FUNCTION__ << "submitAll():" << m_sqlModel->lastError().text();
	}

    m_sqlModel->select();
}

// Fetch a record/row from the DB.
void DatabaseManager::getRecord(int row, ContactData &data)
{
    if (!m_db.isOpen()) {
        qDebug() << __PRETTY_FUNCTION__ << "DB Closed";
        return;
    }
    QSqlRecord record = m_sqlModel->record(row);

    for (int i = 1; i < data.fieldCount(); i++) {
		data.setFieldData(i, record.field(i).value());
	}
}
