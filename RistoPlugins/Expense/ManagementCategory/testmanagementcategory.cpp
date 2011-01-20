#include "testingsql.h"
#include <QStandardItemModel>
#include <QHash>
#include <simplequery.h>

/**
  * @brief Test management category functions
  *
  * The function tested are: report_management_category and
  * report_management_category_detailed.
  */
class TestManagementCategory: public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void report_management_category_test();
    void report_management_category_detailed_test();
    void cleanupTestCase();
private:
    void insertCategories();
    void insertSuppliers();
    void insertDocuments();
    void insertRows();
    void populateAskReply();

    test_t testSimple;
    test_t testComplex;

    TestingSQL *referenceSimple;
    TestingSQL *referenceComplex;
};

void TestManagementCategory::populateAskReply()
{
    QStandardItemModel *res_simpletest = new QStandardItemModel(1,1,this);

    set_data(res_simpletest,0,0,QVariant(100.0));

    QList<QVariant> param;
    param << QVariant(QDate(2010,1,1)) << QVariant(QDate(2010,1,1)) <<
            QVariant("Category1");

    testSimple.insert(param,res_simpletest);
    param.clear();

    // Test Complex

    QStandardItemModel *res_comptest = new QStandardItemModel(2,4,this);
    QList<QVariant> res;

    res << QVariant(QDate(2010,1,1)) << QVariant("Supplier1") << QVariant(1) <<
            QVariant(100.0);
    insert_in_model(res_comptest,res,0);
    res.clear();

    res << QVariant(QDate(2009,1,1)) << QVariant("Supplier1") << QVariant(3) <<
            QVariant(300.0);
    insert_in_model(res_comptest,res,1);
    res.clear();

    param << QVariant(QDate(2009,1,1)) << QVariant(QDate(2010,1,1)) <<
            QVariant("Category1");

    testComplex.insert(param,res_comptest);
}

void TestManagementCategory::insertCategories()
{
    QList<QVariant> categories;
    categories << QVariant("Category1") << QVariant("Category2");

    foreach (QVariant category, categories) {
        referenceSimple->insertInTable("cost_category","description",category);
    }
}

void TestManagementCategory::insertSuppliers()
{
    QList<QVariant> suppliers;
    suppliers << QVariant("Supplier1") << QVariant("Supplier2");

    foreach (QVariant supplier, suppliers)
        referenceSimple->insertInTable("supplier","name",supplier);
}

void TestManagementCategory::insertDocuments()
{
    const int n = 3;
    QStringList columns;
    columns << "id" << "document_date" << "document_number" << "seller";

    QList<QVariant> row[n];
    row[0] << QVariant(1) << QVariant(QDate(2010,1,1)) << QVariant(1) << QVariant("Supplier1");
    row[1] << QVariant(2) << QVariant(QDate(2009,1,1)) << QVariant(2) << QVariant("Supplier2");
    row[2] << QVariant(3) << QVariant(QDate(2009,1,1)) << QVariant(3) << QVariant("Supplier1");

    for (int i=0; i<n; i++)
        referenceSimple->insertInTable("document_purchasing_cost",columns,row[i]);
}

void TestManagementCategory::insertRows()
{
    const int n = 3;
    QStringList columns;
    columns << "id" << "document" << "category" << "price";

    QList<QVariant> row[n];
    row[0] << QVariant(1) << QVariant(1) << QVariant("Category1") << QVariant(100.0);
    row[1] << QVariant(2) << QVariant(2) << QVariant("Category2") << QVariant(200.0);
    row[2] << QVariant(3) << QVariant(3) << QVariant("Category1") << QVariant(300.0);

    for (int i=0; i<n; i++)
        referenceSimple->insertInTable("row_purchasing_cost",columns,row[i]);
}

void TestManagementCategory::initTestCase()
{
    referenceSimple = new TestingSQL("report_management_category", this);
    referenceComplex= new TestingSQL("report_management_category_detailed",this);
    referenceSimple->initDatabase();
    insertCategories();
    insertSuppliers();
    insertDocuments();
    insertRows();

    populateAskReply();
}

void TestManagementCategory::report_management_category_test()
{
    referenceSimple->do_test(testSimple);
}

void TestManagementCategory::report_management_category_detailed_test()
{
    referenceComplex->do_test(testComplex);
}

void TestManagementCategory::cleanupTestCase()
{
    referenceSimple->cleanDatabase();
}

QTEST_MAIN(TestManagementCategory)
#include "testmanagementcategory.moc"
