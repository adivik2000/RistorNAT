CREATE OR REPLACE FUNCTION good_average_cost(p_good VARCHAR,
                                    p_date_from DATE, p_date_to DATE)
    RETURNS NUMERIC AS $$
    DECLARE
        um_fixed CHARACTER VARYING;
        row_goods RECORD;
        avg_cost NUMERIC;
        cost NUMERIC;
        div NUMERIC;
        count INTEGER;
        stmt VARCHAR(2000);
    BEGIN
        avg_cost := 0;
        count := 0;
        SELECT um INTO um_fixed FROM basic_good WHERE name=p_good;

        IF p_date_from IS NOT NULL OR p_date_to IS NOT NULL THEN
            stmt := 'SELECT rg.single_price,rg.um FROM row_goods_cost ';
            stmt := stmt || 'AS rg, document_goods_cost AS dg WHERE good=''';
            stmt := stmt || p_good || '''';
            stmt := stmt || ' AND rg.document=dg.id ';
        ELSE
            stmt := 'SELECT single_price,um FROM row_goods_cost WHERE good=''';
            stmt := stmt || p_good || ''' ';
        END IF;

        IF p_date_from IS NOT NULL THEN
            stmt := stmt || 'AND dg.document_date>=''' || p_date_from || ''' ';
        END IF;

        IF p_date_to IS NOT NULL THEN
            stmt := stmt || 'AND dg.document_date<=''' || p_date_to || ''' ';
        END IF;

        FOR row_goods IN EXECUTE stmt LOOP
            SELECT * INTO div FROM get_div_for_um(row_goods.um,um_fixed);
            cost := row_goods.single_price / div;
            avg_cost := avg_cost + cost;
            count := count + 1;
        END LOOP;
        IF count = 0 THEN
            RETURN 0;
        ELSE
            RETURN avg_cost/count;
        END IF;
    END;
$$ LANGUAGE 'plpgsql';

CREATE OR REPLACE FUNCTION good_average_cost(p_good VARCHAR)
    RETURNS NUMERIC AS $$
        SELECT * from good_average_cost($1,NULL,NULL);
$$ LANGUAGE SQL;

CREATE OR REPLACE FUNCTION set_good_average_cost(p_good VARCHAR)
    RETURNS VOID AS $$
        UPDATE basic_good SET
            average_cost=(SELECT * FROM good_average_cost($1,NULL,NULL))
                WHERE name=$1;
$$ LANGUAGE SQL;

CREATE OR REPLACE FUNCTION set_good_average_cost(p_good VARCHAR,
                                    p_date_from DATE, p_date_to DATE)
    RETURNS VOID AS $$
        UPDATE basic_good SET
            average_cost=(SELECT * FROM good_average_cost($1,$2,$3))
                WHERE name=$1;
$$ LANGUAGE SQL;

CREATE OR REPLACE FUNCTION check_good_in_doc(p_good VARCHAR)
    RETURNS BOOLEAN AS $$
    DECLARE
    BEGIN
        PERFORM * FROM row_goods_cost WHERE good=p_good;
        IF FOUND THEN
            RETURN TRUE;
        ELSE
            RETURN FALSE;
        END IF;
    END;
$$ LANGUAGE 'plpgsql';

CREATE OR REPLACE FUNCTION article_to_stock()
    RETURNS TRIGGER AS $$
    DECLARE
    BEGIN
        IF TG_OP = 'INSERT' THEN
            INSERT INTO stock(article,quantity,um) VALUES (NEW.name,0,NEW.um);
        ELSE
            DELETE FROM stock WHERE article = OLD.name;
        END IF;
        RETURN NULL;
    END;
$$ LANGUAGE 'plpgsql';

CREATE TRIGGER update_article_in_stock AFTER INSERT OR DELETE ON basic_good
    FOR EACH ROW EXECUTE PROCEDURE article_to_stock();
