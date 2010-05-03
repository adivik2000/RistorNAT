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
