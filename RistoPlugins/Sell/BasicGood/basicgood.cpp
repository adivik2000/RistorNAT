#include <QtPlugin>
#include "basicgood.h"
#include "simpletable.h"

basicGood::basicGood(QWidget *parent):pluginInterface(parent)
{
    simpleTable *table = new simpleTable(parent);
    table->setTableName("basic_good");
    table->setColumnWidth(0,300);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(table);

    setLayout(layout);
}

Q_EXPORT_PLUGIN2(basicgood, basicGood);
