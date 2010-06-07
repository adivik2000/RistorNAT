CREATE OR REPLACE FUNCTION report_goodcategory_amount (p_goodcat VARCHAR,
                            p_date_from DATE, p_date_to DATE)
    RETURNS NUMERIC AS $$
    DECLARE
        lunch_amount NUMERIC;
        dinner_amount NUMERIC;
    BEGIN
        SELECT * INTO lunch_amount FROM get_goodcategory_amount(p_goodcat,
            'l_sales',p_date_from,p_date_to);
        SELECT * INTO dinner_amount FROM get_goodcategory_amount(p_goodcat,
            'd_sales',p_date_from,p_date_to);

        RETURN lunch_amount + dinner_amount;
    END;
$$ LANGUAGE 'plpgsql';

CREATE OR REPLACE FUNCTION get_goodcategory_amount (p_goodcat VARCHAR,
                            p_table VARCHAR, p_date_from DATE, p_date_to DATE)
    RETURNS NUMERIC AS $$
    DECLARE
        stmt CHARACTER VARYING(1000);
        amount NUMERIC;
        sales RECORD;
    BEGIN
        stmt := 'SELECT s.quantity_sold, s.sell_price FROM ' || p_table ||
            ' AS s, good_for_sale AS g WHERE s.article=g.id ';
        stmt := stmt || 'AND g.good_category=''' || p_goodcat || '''';
        IF (p_date_from IS NOT NULL) THEN
            stmt := stmt || ' AND sales_date>=''' || p_date_from || '''';
        END IF;
        IF (p_date_to IS NOT NULL) THEN
            stmt := stmt || ' AND sales_date<=''' || p_date_to || '''';
        END IF;

        amount := 0;
        FOR sales IN EXECUTE stmt LOOP
            amount := amount + (sales.quantity_sold * sales.sell_price);
        END LOOP;

        RETURN amount;
    END;
$$ LANGUAGE 'plpgsql';

