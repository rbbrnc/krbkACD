#include <QtGui>

#include "QExiv2.h"
#include "MetadataTreeModel.h"

class Window : public QWidget
{
//	Q_OBJECT

	public:
		Window(const QString &file, QWidget *parent = 0);

	private:
		QLabel *nameLabel;
		QLabel *addressLabel;
		QLabel *ageLabel;
		QLineEdit *nameEdit;
		QTextEdit *addressEdit;
		QSpinBox *ageSpinBox;
		QPushButton *nextButton;

		QTreeView *treeView;
		MetadataTreeModel *m_model;
};

Window::Window(const QString &file, QWidget *parent) : QWidget(parent)
{
	treeView = new QTreeView();
	QExiv2 *exiv2 = new QExiv2();
	if (exiv2->load(file)) {
		m_model = new MetadataTreeModel(exiv2, parent);
		treeView->setUniformRowHeights(true);
		treeView->setModel(m_model);
		treeView->hideColumn(7); // hide key column
	}

	nameLabel = new QLabel(tr("Na&me:"));
	nameEdit = new QLineEdit();
	addressLabel = new QLabel(tr("&Address:"));
	addressEdit = new QTextEdit();
	ageLabel = new QLabel(tr("A&ge (in years):"));
	ageSpinBox = new QSpinBox();
	nextButton = new QPushButton(tr("&Next"));

	nameLabel->setBuddy(nameEdit);
	addressLabel->setBuddy(addressEdit);
	ageLabel->setBuddy(ageSpinBox);

	QGridLayout *layout = new QGridLayout();
	layout->addWidget(treeView, 0, 0, 1, 1);
	layout->addWidget(nameLabel, 0, 0, 1, 1);
	layout->addWidget(nameEdit, 0, 2, 1, 1);
	layout->addWidget(addressLabel, 1, 0, 1, 1);
	layout->addWidget(addressEdit, 1, 1, 2, 1);
	layout->addWidget(nextButton, 1, 2, 1, 1);
	layout->addWidget(ageLabel, 3, 0, 1, 1);
	layout->addWidget(ageSpinBox, 3, 1, 1, 1);
	setLayout(layout);

	setWindowTitle(tr("Metadata Tree Model"));
}

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QString file(argv[1]);

#if 0
	QExiv2 *exiv2 = new QExiv2();
	if (exiv2->load(file)) {
		MetadataTreeModel model(exiv2);
		QTreeView view;
		view.setUniformRowHeights(true);
		view.setModel(&model);
		view.setWindowTitle(QObject::tr("Metadata Tree Model"));

		view.hideColumn(7); // hide key column

		view.show();
		return app.exec();
	}
	return -1;
#else
	Window w(file);
	w.show();
	return app.exec();
#endif
}
