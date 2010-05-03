/** @brief Returns places for dinner
  *
  * @param p_date Date of the dinner
  * @return Integer for places, or NULL if at p_date there isn't a dinner
  */
CREATE OR REPLACE FUNCTION places_for_dinner(p_date DATE)
    RETURNS INTEGER AS $$
        SELECT places FROM dinner WHERE dinner_date=$1;
 $$ LANGUAGE SQL;

CREATE OR REPLACE FUNCTION sold_d_articles (p_dinner_date DATE)
    RETURNS SETOF articles_sold AS $$
      SELECT dsl.article, gfs.description, dsl.quantity_sold, dsl.sell_price
      FROM d_sales AS dsl, good_for_sale AS gfs
      WHERE sales_date=$1 AND gfs.id=dsl.article;
$$ LANGUAGE SQL;

CREATE OR REPLACE FUNCTION dinner_ins_upd(p_date DATE, p_places INTEGER)
    RETURNS VOID AS $$
    DECLARE
    BEGIN

        PERFORM * FROM dinner WHERE dinner_date=p_date;

        IF NOT FOUND THEN
            INSERT INTO dinner("dinner_date","places") VALUES ($1,$2);
            RETURN;
        END IF;

        PERFORM * FROM dinner WHERE dinner_date=p_date AND places=p_places;

        IF NOT FOUND THEN
            UPDATE dinner SET places=p_places WHERE dinner_date=p_date;
        END IF;

        RETURN;

    END;
$$ LANGUAGE 'plpgsql';

-- Insert a selling article in a single dinner
CREATE OR REPLACE FUNCTION single_dinner_sell(p_dinner_date date, p_id_article integer,
  p_quantity integer,  p_sell_price numeric)
    RETURNS BOOLEAN AS $$
    DECLARE
    BEGIN

      PERFORM * FROM d_sales
      WHERE sales_date=p_dinner_date AND article=p_id_article;

      IF FOUND THEN
        UPDATE d_sales SET quantity_sold = p_quantity, sell_price = p_sell_price
        WHERE sales_date = p_dinner_date AND article=p_id_article;
      ELSE
        INSERT INTO d_sales ("sales_date","article","quantity_sold","sell_price")
        VALUES (p_dinner_date,p_id_article,p_quantity,p_sell_price);
      END IF;

      RETURN TRUE;
    END;
$$ LANGUAGE 'plpgsql';

CREATE TRIGGER update_stock_for_dinner AFTER INSERT OR UPDATE OR DELETE ON d_sales
    FOR EACH ROW EXECUTE PROCEDURE update_stock_sell();

CREATE TRIGGER update_last_cost_dinner AFTER INSERT OR UPDATE ON d_sales
    FOR EACH ROW EXECUTE PROCEDURE update_last_cost();
