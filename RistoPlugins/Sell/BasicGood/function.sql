CREATE OR REPLACE FUNCTION good_average_cost(p_good VARCHAR)
    RETURNS VOID AS $$
    DECLARE
        um_fixed CHARACTER VARYING;
        row_goods RECORD;
        avg_cost NUMERIC;
        cost NUMERIC;
        div NUMERIC;
        count INTEGER;
    BEGIN
        avg_cost := 0;
        count := 0;
        SELECT um INTO um_fixed FROM basic_good WHERE name=p_good;

        FOR row_goods IN SELECT * FROM row_goods_cost WHERE good=p_good LOOP
            SELECT * INTO div FROM get_div_for_um(row_goods.um,um_fixed);
            cost := row_goods.single_price / div;
            avg_cost := avg_cost + cost;
            count := count + 1;
        END LOOP;
        UPDATE basic_good SET average_cost=avg_cost/count WHERE name=p_good;
    END;
$$ LANGUAGE 'plpgsql';

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
