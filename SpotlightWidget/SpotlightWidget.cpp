#include <QStyle>
#include "SpotlightWidget.h"

SpotlightWidget::SpotlightWidget(QWidget *parent)
	: QLineEdit(parent)
{
	glass = new QLabel(this);
	QPixmap gpixmap(":/magnify.png");
	glass->setPixmap(gpixmap);
	glass->setStyleSheet("QLabel {border: none; padding-left: 5px; padding-top: 5px;}");
	glass->setCursor(Qt::ArrowCursor);

	clearButton = new QToolButton(this);
	QPixmap pixmap(":/clear_button.png");
	clearButton->setIcon(QIcon(pixmap));
	clearButton->setIconSize(pixmap.size());
	clearButton->setCursor(Qt::ArrowCursor);
	clearButton->setStyleSheet("QToolButton {border: none; padding: 0px;}");
	clearButton->hide();

	connect(clearButton, SIGNAL(clicked()), this, SLOT(clear()));
	connect(this, SIGNAL(textChanged(const QString&)), this, SLOT(updateCloseButton(const QString&)));

	int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);

	setStyleSheet(QString("QLineEdit {padding-right: %1px; padding-left: %2px;}")
			.arg(clearButton->sizeHint().width() + frameWidth + 1)
			.arg(glass->sizeHint().width()));

	QSize msz = minimumSizeHint();
	setMinimumSize(qMax(msz.width(), clearButton->sizeHint().height() + frameWidth * 2 + 2),
			qMax(msz.height(), clearButton->sizeHint().height() + frameWidth * 2 + 2));
}

void SpotlightWidget::resizeEvent(QResizeEvent *)
{
	QSize sz = clearButton->sizeHint();
	int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
	clearButton->move(rect().right() - frameWidth - sz.width(), (rect().bottom() + 1 - sz.height())/2);
}

void SpotlightWidget::updateCloseButton(const QString& text)
{
	clearButton->setVisible(!text.isEmpty());
}

