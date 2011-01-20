CREATE OR REPLACE FUNCTION test_Seller_init()
    RETURNS BOOLEAN AS $$
    DECLARE
    BEGIN

        INSERT INTO unit_of_measurement(name,multi) VALUES ('g', 1);
        INSERT INTO unit_of_measurement(name,multi) VALUES ('lt', 1);
        INSERT INTO unit_of_measurement(name,multi) VALUES ('m', 1);
        INSERT INTO unit_of_measurement(name,base,multi) VALUES ('kg','g', 1000);
        INSERT INTO unit_of_measurement(name,base,multi) VALUES ('hg','g', 100);
        INSERT INTO unit_of_measurement(name,base,multi) VALUES ('mg','g', 0.001);
        INSERT INTO unit_of_measurement(name,base,multi) VALUES ('dl','lt', 0.1);
        INSERT INTO unit_of_measurement(name,base,multi) VALUES ('ton','kg', 1000);
        INSERT INTO unit_of_measurement(name,base,multi) VALUES ('m2','m', 1000);
        INSERT INTO unit_of_measurement(name,base,multi) VALUES ('dm2','m2', 1000);

        INSERT INTO supplier(name) VALUES ('Barilla');
        INSERT INTO supplier(name) VALUES ('Giancarlo');
        INSERT INTO supplier(name) VALUES ('Acqua Lete');

        INSERT INTO document_purchasing_cost(document_date,
            document_number, seller,amount) VALUES ('2009-01-01', 1, 'Giancarlo', '100.58');
        INSERT INTO document_purchasing_cost(document_date,
            document_number, seller,amount) VALUES ('2009-02-01', 2, 'Giancarlo', '100.53');
        INSERT INTO document_purchasing_cost(document_date,
            document_number, seller,amount) VALUES ('2009-02-02', 3, 'Giancarlo', '100');

        INSERT INTO document_goods_cost(document_date,
            document_number, seller,amount) VALUES ('2009-01-01', 1, 'Acqua Lete', '100.21');
        INSERT INTO document_goods_cost(document_date,
            document_number, seller,amount) VALUES ('2009-02-01', 2, 'Barilla', '100');
        INSERT INTO document_goods_cost(document_date,
            document_number, seller,amount) VALUES ('2009-02-02', 3, 'Barilla', '100');
        INSERT INTO document_goods_cost(document_date,
           document_number, seller,amount) VALUES ('2009-05-15', 1, 'Barilla', '100');
        INSERT INTO document_goods_cost(document_date,
           document_number, seller,amount) VALUES ('2009-05-15', 2, 'Acqua Lete', '100');
       INSERT INTO document_goods_cost(document_date,
           document_number, seller,amount) VALUES ('2009-06-15', 3, 'Barilla', '100');

        INSERT INTO document_goods_cost(document_date,
            document_number, seller,amount) VALUES ('2010-01-01', 1, 'Acqua Lete', '100.22');
        INSERT INTO document_goods_cost(document_date,
            document_number, seller,amount) VALUES ('2010-02-01', 2, 'Acqua Lete', '100');
        INSERT INTO document_goods_cost(document_date,
           document_number, seller,amount) VALUES ('2010-02-02', 3, 'Barilla', '100.33');
        INSERT INTO document_goods_cost(document_date,
           document_number, seller,amount) VALUES ('2010-02-06', 3, 'Giancarlo', '1000');

        RETURN TRUE;
    END;
$$ LANGUAGE 'plpgsql';

/** @brief Testsuite for Seller
   *
   * Needs to test:
   * *)report_seller_purchasing(p_seller VARCHAR, p_date_from DATE, p_date_to DATE)
   *      => NUMERIC
   * *)report_seller_goods(p_seller VARCHAR, p_date_from DATE, p_date_to DATE)
   *      => NUMERIC
   * *)report_seller_amount(p_seller VARCHAR, p_date_from DATE, p_date_to DATE)
   *      => NUMERIC
   */
CREATE OR REPLACE FUNCTION test_Seller_do()
    RETURNS BOOLEAN AS $$
    DECLARE
        result NUMERIC;
    BEGIN

        SELECT * INTO result FROM report_seller_goods('Giancarlo', '2008-01-01',
            '2010-02-02');
        IF result IS NULL OR result != 0 THEN
            RAISE WARNING 'report_seller_goods test 1 failed, got %, expected 0', result;
            RETURN FALSE;
        END IF;

        SELECT * INTO result FROM report_seller_goods('Giancarlo', '2008-01-01',
            '2010-02-02');
        IF result IS NULL OR result != 0 THEN
            RAISE WARNING 'report_seller_goods test 1 failed, got %, expected 0', result;
            RETURN FALSE;
        END IF;

        SELECT * INTO result FROM report_seller_goods('Barilla', '2009-05-15',
            '2009-06-14');
        IF result IS NULL OR result != 100 THEN
            RAISE WARNING 'report_seller_goods test 2 failed, got %, expected 100', result;
            RETURN FALSE;
        END IF;

        SELECT * INTO result FROM report_seller_goods('Barilla', '2009-05-15',
            '2009-06-15');
        IF result IS NULL OR result != 200 THEN
            RAISE WARNING 'report_seller_goods test 3 failed, got %, expected 200', result;
            RETURN FALSE;
        END IF;

        SELECT * INTO result FROM report_seller_goods('Acqua Lete', '2009-01-01',
            '2009-08-01');
        IF result IS NULL OR result != 200.21 THEN
            RAISE WARNING 'report_seller_goods test 4 failed, got %, expected 200.21', result;
            RETURN FALSE;
        END IF;

        SELECT * INTO result FROM report_seller_goods('NonEsiste', '2009-01-01',
            '2009-08-01');
        IF result IS NULL OR result != 0 THEN
            RAISE WARNING 'report_seller_goods test 5 failed, got %, expected 0', result;
            RETURN FALSE;
        END IF;

        SELECT * INTO result FROM report_seller_purchasing('Giancarlo', '2008-01-01',
            '2008-02-02');
        IF result IS NULL OR result != 0 THEN
            RAISE WARNING 'report_seller_purchasing test 1 failed, got %, expected 0', result;
            RETURN FALSE;
        END IF;

            SELECT * INTO result FROM report_seller_purchasing('Giancarlo', '2009-01-15',
            '2009-02-02');
        IF result IS NULL OR result != 200.53 THEN
            RAISE WARNING 'report_seller_purchasing test 2 failed, got %, expected 200.53', result;
            RETURN FALSE;
        END IF;

        SELECT * INTO result FROM report_seller_purchasing('Giancarlo', '2009-01-01',
            '2009-02-02');
        IF result IS NULL OR result != 301.11 THEN
            RAISE WARNING 'report_seller_purchasing test 3 failed, got %, expected 301.11', result;
            RETURN FALSE;
        END IF;

        SELECT * INTO result FROM report_seller_purchasing('Giancarlo', '2009-02-02',
            '2009-02-02');
        IF result IS NULL OR result != 100 THEN
            RAISE WARNING 'report_seller_purchasing test 4 failed, got %, expected 100', result;
            RETURN FALSE;
        END IF;

        SELECT * INTO result FROM report_seller_amount('Giancarlo', '2005-01-01',
            '2006-01-01');
        IF result IS NULL OR result != 0 THEN
            RAISE WARNING 'report_seller_amount test 1 failed, got %, expected 0', result;
            RETURN FALSE;
        END IF;

        SELECT * INTO result FROM report_seller_amount('Giancarlo', '2009-01-01',
            '2010-05-05');
        IF result IS NULL OR result != 1301.11 THEN
            RAISE WARNING 'report_seller_amount test 2 failed, got %, expected 1301.11', result;
            RETURN FALSE;
        END IF;

        RETURN TRUE;
    END;
$$ LANGUAGE 'plpgsql';

CREATE OR REPLACE FUNCTION test_Seller_clean()
    RETURNS BOOLEAN AS $$
    DECLARE
    BEGIN
        DELETE FROM document_goods_cost;
        DELETE FROM document_purchasing_cost;
        DELETE FROM supplier;
        DELETE FROM unit_of_measurement ;
        RETURN TRUE;
    END;
$$ LANGUAGE 'plpgsql';
