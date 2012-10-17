#include <QDebug>

#include "fsManager.h"

FSManager *FSManager::m_instance = 0;

FSManager::FSManager()
{
	m_model = new QFileSystemModel();

	m_path = QDir::current().absolutePath();
	m_currentIndex = m_model->setRootPath(m_path);

	m_model->setFilter(QDir::Files | QDir::System | QDir::NoDot | QDir::Dirs);
	//m_model->setReadOnly(false);
	m_model->setReadOnly(true);
}

FSManager::~FSManager()
{
	delete m_model;
	m_instance = 0;
}

FSManager *FSManager::instance()
{
	if (m_instance == 0) {
		m_instance = new FSManager();
	}

	return m_instance;
}

QFileSystemModel *FSManager::model() const
{
	return m_model;
}

QModelIndex FSManager::currentIndex() const
{
	return m_currentIndex;
}

QModelIndex FSManager::firstRow() const
{
	return m_currentIndex.child(0, 0);
}

QModelIndex FSManager::nextRow(const QModelIndex &current) const
{
	int row = current.row();
	row++;

	return m_model->index(row, current.column(), m_currentIndex);
}

QModelIndex FSManager::nextRow(int current) const
{
	return m_currentIndex.child(++current, 0);
}
QModelIndex FSManager::previousRow(const QModelIndex &current) const
{
	int row = current.row();
	row--;

	return m_model->index(row, current.column(), m_currentIndex);
}

QModelIndex FSManager::previousRow(int current) const
{
	return m_currentIndex.child(--current, 0);
}

void FSManager::changePath(const QString &path)
{
	m_currentIndex = m_model->setRootPath(path);
}

void FSManager::showHiddenFiles(bool show)
{
        if (show) {
                m_model->setFilter(m_model->filter() | QDir::Hidden);
        } else {
                m_model->setFilter(m_model->filter() &~ QDir::Hidden);
        }
}

// Delete files until filelist empty or an error occured
bool FSManager::removeFiles(QModelIndexList &list)
{
        m_model->setReadOnly(false);
        while (!list.isEmpty()) {
                if (m_model->remove(list.first())) {
                        list.removeFirst();
                } else {
			m_model->setReadOnly(true);
                        return false;
		}
	}
	m_model->setReadOnly(true);
	return true;
}
