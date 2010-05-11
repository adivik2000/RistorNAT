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
            INSERT INTO stock(article,quantity) VALUES (NEW.name,0);
        ELSE
            DELETE FROM stock WHERE article = OLD.name;
        END IF;
        RETURN NULL;
    END;
$$ LANGUAGE 'plpgsql';

CREATE TRIGGER update_article_in_stock AFTER INSERT OR DELETE ON basic_good
    FOR EACH ROW EXECUTE PROCEDURE article_to_stock();
