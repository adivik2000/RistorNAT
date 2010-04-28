
#include "stock.h"
#include <QtPlugin>
#include <simpletable.h>

Stock::Stock(QWidget *parent):pluginInterface(parent)
{
    simpleTable *table = new simpleTable(parent);
    table->setTableName("stock");

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(table);

    setLayout(layout);
}

Q_EXPORT_PLUGIN2(stock, Stock);
