//generate.h 次窗口头文件
#pragma once

#include <QWidget>
#include <QStandardItemModel>
#include <QItemDelegate>
#include "ui_generate.h"

class generate : public QWidget
{
	Q_OBJECT private:
	QStandardItemModel *model;
	QStringList labels;

public:
	generate(QWidget *parent = Q_NULLPTR);
	~generate();

private:
	Ui::generate ui;

private slots:
	void addButton();
	void openButton();
	void generateButton();
	void deleteButton();
	void backButton();
};

//只读委托
class ReadOnlyDelegate : public QItemDelegate
{
public:
	ReadOnlyDelegate(QWidget* parent = NULL) :QItemDelegate(parent){
	
	}

	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override //final
	{
		Q_UNUSED(parent)
		Q_UNUSED(option)
		Q_UNUSED(index)
		return NULL;
	}
};
