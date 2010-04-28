/** @brief
 *
 * Dep: GoodsForSale, ArticleComponent
 */

-- This table contain the places and the date for every lunch.
CREATE TABLE "lunch"
(
  lunch_date DATE NOT NULL,
  places INTEGER NOT NULL,
  PRIMARY KEY (lunch_date)
) WITH (OIDS=FALSE);

CREATE TYPE articles_sold AS
(
  Article_Number INTEGER,
  Article VARCHAR,
  Quantity INTEGER,
  Sell_price NUMERIC
);

-- This table contain the reference of what composed goods are sold in a certain
-- lunch
CREATE TABLE "l_sales"
(
  sales_date DATE REFERENCES lunch(lunch_date)
    MATCH SIMPLE ON UPDATE CASCADE ON DELETE RESTRICT,
  article INTEGER REFERENCES good_for_sale(id)
    MATCH SIMPLE ON UPDATE CASCADE ON DELETE RESTRICT,
  quantity_sold INTEGER NOT NULL,
  sell_price NUMERIC NOT NULL,
  PRIMARY KEY(sales_date,article)
) WITH (OIDS=FALSE);

-- Insert a selling article in a single lunch
CREATE OR REPLACE FUNCTION single_lunch_sell(p_lunch_date date, p_id_article integer,
  p_quantity integer,  p_sell_price numeric)
    RETURNS BOOLEAN AS $$
    DECLARE
    BEGIN

      PERFORM * FROM l_sales
      WHERE sales_date=p_lunch_date AND article=p_id_article;

      IF FOUND THEN
        UPDATE l_sales SET quantity_sold = p_quantity, sell_price = p_sell_price
        WHERE sales_date = p_lunch_date AND article=p_id_article;
      ELSE
        INSERT INTO l_sales ("sales_date","article","quantity_sold","sell_price")
        VALUES (p_lunch_date,p_id_article,p_quantity,p_sell_price);
      END IF;

      RETURN TRUE;
    END;
$$ LANGUAGE 'plpgsql';

CREATE OR REPLACE FUNCTION lunch_ins_upd(p_date DATE, p_places INTEGER)
    RETURNS VOID AS $$
    DECLARE
    BEGIN

        PERFORM * FROM lunch WHERE lunch_date=p_date;

        IF NOT FOUND THEN
            INSERT INTO lunch("lunch_date","places") VALUES ($1,$2);
            RETURN;
        END IF;

        PERFORM * FROM lunch WHERE lunch_date=p_date AND places=p_places;

        IF NOT FOUND THEN
            UPDATE lunch SET places=p_places WHERE lunch_date=p_date;
        END IF;

        RETURN;

    END;
$$ LANGUAGE 'plpgsql';

CREATE OR REPLACE FUNCTION places_for_lunch(p_date DATE)
    RETURNS INTEGER AS $$
        SELECT places FROM lunch WHERE lunch_date=$1;
 $$ LANGUAGE SQL;


CREATE OR REPLACE FUNCTION sold_l_articles (p_lunch_date DATE)
    RETURNS SETOF articles_sold AS $$
      SELECT lsl.article, gfs.description, lsl.quantity_sold, lsl.sell_price
      FROM l_sales AS lsl, good_for_sale AS gfs
      WHERE sales_date=$1 AND gfs.id=lsl.article;
$$ LANGUAGE SQL;

CREATE OR REPLACE FUNCTION update_stock_sell()
    RETURNS TRIGGER AS $$
    DECLARE
        component RECORD;
    BEGIN

        IF TG_OP = 'INSERT' THEN
            FOR component IN SELECT * FROM article_made_up(NEW.article) LOOP
                PERFORM * FROM update_stock(component.quantity,
                    component.basic_component, component.um,FALSE);
            END LOOP;
        ELSIF TG_OP = 'UPDATE' THEN
            FOR component IN SELECT * FROM article_made_up(OLD.article) LOOP
                PERFORM * FROM update_stock(component.quantity,
                    component.basic_component, component.um,TRUE);
            END LOOP;
            FOR component IN SELECT * FROM article_made_up(NEW.article) LOOP
                PERFORM * FROM update_stock(component.quantity,
                    component.basic_component, component.um,FALSE);
            END LOOP;
        ELSE
            FOR component IN SELECT * FROM article_made_up(OLD.article) LOOP
                PERFORM * FROM update_stock(component.quantity,
                    component.basic_component, component.um,TRUE);
            END LOOP;
        END IF;
        RETURN NULL;
    END;
$$ LANGUAGE 'plpgsql';


CREATE OR REPLACE FUNCTION update_last_cost()
    RETURNS TRIGGER AS $$
    DECLARE
    BEGIN
        UPDATE good_for_sale SET last_cost = NEW.sell_price
            WHERE id = NEW.article;

        RETURN NULL;
    END;
$$ LANGUAGE 'plpgsql';

CREATE TRIGGER update_stock_for_lunch AFTER INSERT OR UPDATE OR DELETE ON l_sales
    FOR EACH ROW EXECUTE PROCEDURE update_stock_sell();

CREATE TRIGGER update_last_cost_lunch AFTER INSERT OR UPDATE ON l_sales
    FOR EACH ROW EXECUTE PROCEDURE update_last_cost();

