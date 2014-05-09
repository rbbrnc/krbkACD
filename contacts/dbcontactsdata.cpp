#include <QDir>
#include <QDebug>

#include "dbcontactsdata.h"

struct TableDef {
    QString dbName;
    QString tableName;

    struct FieldDef {
        QString fieldName;
        QString sqlType;
    } fields[10];
};

static struct TableDef tableDef = {
    "contactsdb.sqlite",    // .dbName
    "contacts",             // .tableName
    {
        /* .fields {name, sql_type) */
        { "id",        "INTEGER PRIMARY KEY" },
        { "category",  "VARCHAR(128)" },
        { "firstname", "VARCHAR(128)" },
        { "lastname",  "VARCHAR(128)" },
        { "aka",       "VARCHAR(128)" },
        { "email",     "VARCHAR(255)" },
        { "website",   "VARCHAR(255)" },
        { "notes",     "TEXT"},
        { "portrait",  "TEXT"},
        { "",""}
    }
};

ContactData::ContactData()
    : m_fields(ContactData::fields())
{
    // Init Data Fields
    for (int i = 0; i < m_fields.count(); i++) {
        m_data.append(QVariant());
    }
}

// [Static member]
const QString ContactData::dbName()
{
    return tableDef.dbName;
}

// [Static member]
const QString ContactData::dbTableName()
{
    return tableDef.tableName;
}

// [Static member]
const QString ContactData::dbPath()
{
#if 0
    // NOTE: We have to store database file into user home folder in Linux
    QString path(QDir::home().path());
    path.append(QDir::separator()).append(tableDef.dbName);
    return path;
#else
   return QDir::toNativeSeparators(tableDef.dbName);
#endif
}

// [Static member]
const QString ContactData::createQuery()
{
    QString query = QString("CREATE TABLE IF NOT EXISTS %1 (").arg(tableDef.tableName);

    int i = 0;
    do {
        if (i > 0) {
            query += ", ";
        }
        query += tableDef.fields[i].fieldName + " " + tableDef.fields[i].sqlType;
        i++;
    } while (!tableDef.fields[i].fieldName.isEmpty());
    query += ")";

    //qDebug() << "QUERY=" << query;

    return query;
}

// [Static member]
const QStringList ContactData::fields()
{
    QStringList flds;

    int i = 0;
    do {
        flds << tableDef.fields[i].fieldName;
        i++;
    } while (!tableDef.fields[i].fieldName.isEmpty());

    return flds;
}

// [Static member]
const QStringList ContactData::categories()
{
	QStringList categories;
	categories << "Friends"
               << "Work"
			   << "SocNet"
			   << "Other";

    categories.sort();

	return categories;
}

const QString ContactData::fieldName(int index) const
{
    return m_fields.at(index);
}

int ContactData::fieldCount() const
{
    return m_fields.count();
}

QVariant ContactData::fieldData(int index) const
{
    if (index < m_data.count()) {
        return m_data[index];
    } else {
        qWarning() << "index" << index << "out-of-range!";
    }

    return QVariant();
}

void ContactData::setFieldData(int index, QVariant data)
{
    if (index < m_data.count()) {
        m_data[index] = data;
    } else {
        qWarning() << "index" << index << "out-of-range! for data:" << data.toString();
    }
}
