#ifndef REGION_EDIT_DIALOG_H
#define REGION_EDIT_DIALOG_H

#include <QDialog>

namespace Ui {
	class RegionEditDialog;
}

#include "XmpRegion.h"

class RegionEditDialog : public QDialog
{
	Q_OBJECT

	public:
		RegionEditDialog(XmpRegion region, QWidget *parent = 0);
		~RegionEditDialog();

	private:
		Ui::RegionEditDialog *ui;
};

#endif
