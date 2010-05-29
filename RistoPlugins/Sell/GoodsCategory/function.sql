CREATE OR REPLACE FUNCTION report_goodscategory_amount(p_category VARCHAR, p_date_from DATE,
                                                      p_date_to DATE)
    RETURNS NUMERIC AS $$
    DECLARE
        stmt CHARACTER VARYING(1000);
        amount NUMERIC;
    BEGIN
        RETURN -1;
    END;
$$ LANGUAGE 'plpgsql';
