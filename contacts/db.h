#ifndef DB_H
#define DB_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QFile>
#include <QSqlQuery>
#include <QString>
#include <QAbstractItemModel>
#include <QStringList>

#include "dbcontactsdata.h"

class QSqlQueryModel;
class QSqlTableModel;

class DatabaseManager : public QObject
{
    Q_OBJECT

	public:
		DatabaseManager(QObject *parent = 0);
		~DatabaseManager();

		bool openDB();
		bool deleteDB();

        QAbstractItemModel *model() const;

		bool createTable();
        void addRecord(ContactData &data);
        void updateRecord(int row, ContactData &data);
        void delRecord(int row);
        void getRecord(int row, ContactData &data);

		QSqlError lastError();
    private:
        bool setSqlModel();

    public slots:
        void sort(int column, Qt::SortOrder order);

	private:
        QSqlDatabase m_db;
        QSqlTableModel *m_sqlModel;
        QStringList m_categories;
};

#endif
