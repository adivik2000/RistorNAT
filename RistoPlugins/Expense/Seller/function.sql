CREATE OR REPLACE FUNCTION report_seller_purchasing(p_seller VARCHAR, p_date_from DATE,
                                                    p_date_to DATE)
    RETURNS NUMERIC AS $$
    DECLARE
        stmt CHARACTER VARYING(1000);
        amount NUMERIC;
    BEGIN
        stmt := 'SELECT sum(amount) FROM document_purchasing_cost ';
        stmt := stmt || ' WHERE seller=''' || p_seller || '''';

        IF (p_date_from IS NOT NULL) THEN
            stmt := stmt || ' AND document_date>=''' || p_date_from || '''';
        END IF;
        IF (p_date_to IS NOT NULL) THEN
            stmt := stmt || ' AND document_date<=''' || p_date_to || '''';
        END IF;

        stmt := stmt || ' GROUP BY seller';

        EXECUTE stmt INTO amount;
        RETURN amount;
    END;
$$ LANGUAGE 'plpgsql';

CREATE OR REPLACE FUNCTION report_seller_goods(p_seller VARCHAR, p_date_from DATE,
                                               p_date_to DATE)
    RETURNS NUMERIC AS $$
    DECLARE
        stmt CHARACTER VARYING(1000);
        amount NUMERIC;
    BEGIN
        stmt := 'SELECT sum(amount) FROM document_goods_cost ';
        stmt := stmt || ' WHERE seller=''' || p_seller || '''';

        IF (p_date_from IS NOT NULL) THEN
            stmt := stmt || ' AND document_date>=''' || p_date_from || '''';
        END IF;
        IF (p_date_to IS NOT NULL) THEN
            stmt := stmt || ' AND document_date<=''' || p_date_to || '''';
        END IF;

        stmt := stmt || ' GROUP BY seller';

        EXECUTE stmt INTO amount;
        RETURN amount;
    END;
$$ LANGUAGE 'plpgsql';


CREATE OR REPLACE FUNCTION report_seller_amount(p_seller VARCHAR, p_date_from DATE,
                                                p_date_to DATE)
    RETURNS NUMERIC AS $$
    DECLARE
        amount_dpc NUMERIC;
        amount_dgc NUMERIC;
    BEGIN
        SELECT * INTO amount_dpc FROM report_seller_purchasing(p_seller,p_date_from,p_date_to);
        SELECT * INTO amount_dgc FROM report_seller_goods(p_seller,p_date_from,p_date_to);

        RETURN amount_dpc + amount_dgc;
    END;
$$ LANGUAGE 'plpgsql';
