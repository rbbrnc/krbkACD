#ifndef DB_CONTACTS_DATA_H
#define DB_CONTACTS_DATA_H

#include <QVariant>
#include <QString>
#include <QStringList>

class ContactData
{
	public:
        enum FieldIndex {
            Id = 0,
            Category,
            FirstName,
            LastName,
            Aka,
            EMail,
            WebSite,
            Note,
            Portrait
        };

		ContactData();

        static const QString dbPath();
        static const QString dbName();
		static const QString dbTableName();
		static const QString createQuery();
		static const QStringList fields();
		static const QStringList categories();

		QVariant fieldData(int index) const;
        const QString fieldName(int index) const;
		void setFieldData(int index, QVariant data);
		int fieldCount() const;

	public:
        ContactData& operator= (const ContactData &other)
        {
            m_data = other.m_data;
            return *this;
        }

	private:
		const QStringList m_fields;
        QList<QVariant> m_data;
};

#endif
