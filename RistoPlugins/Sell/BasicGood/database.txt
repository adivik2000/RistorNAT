/** @brief
 *
 */

-- This table will contain the description and the code of the basic things
-- used to make advanced goods that can be selled (like pasta, prosciutto,
-- pomodori pelati..)
CREATE TABLE "basic_good"
(
  name CHARACTER VARYING(128) NOT NULL,
  PRIMARY KEY(name)
) WITH (OIDS=FALSE);

CREATE OR REPLACE FUNCTION article_to_stock()
    RETURNS TRIGGER AS $$
    DECLARE
    BEGIN
        IF TG_OP = 'INSERT' THEN
            INSERT INTO stock(article,quantity) VALUES (NEW.description,0);
        ELSE
            DELETE FROM stock WHERE article = OLD.description;
        END IF;
        RETURN NULL;
    END;
$$ LANGUAGE 'plpgsql';

CREATE TRIGGER update_article_in_stock AFTER INSERT OR DELETE ON basic_good
    FOR EACH ROW EXECUTE PROCEDURE article_to_stock();
