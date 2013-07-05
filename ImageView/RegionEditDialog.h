#ifndef REGION_EDIT_DIALOG_H
#define REGION_EDIT_DIALOG_H

#include <QDialog>

namespace Ui {
	class RegionEditDialog;
}

#include "RegionGraphicsItem.h"

class RegionEditDialog : public QDialog
{
	Q_OBJECT

	public:
		RegionEditDialog(RegionGraphicsItem *item, QWidget *parent = 0);
		~RegionEditDialog();

	public slots:
		virtual void accept();
		virtual void reject();

	private:
		Ui::RegionEditDialog *ui;
		RegionGraphicsItem *m_item;
};

#endif
