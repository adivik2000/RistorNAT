/** @brief Delete a good from stock
  *
  * @param p_good Good to delete ( ref to basic_good(name) )
  */
CREATE OR REPLACE FUNCTION delete_from_stock(p_good VARCHAR)
    RETURNS VOID AS $$
        DELETE FROM stock WHERE article=$1;
        DELETE FROM stock_log WHERE article=$1;
$$ LANGUAGE SQL;

CREATE OR REPLACE FUNCTION get_div_for_um(p_um1 VARCHAR, p_um2 VARCHAR)
    RETURNS NUMERIC AS $$
    DECLARE
        div NUMERIC;
        um1 RECORD;
        um2 RECORD;
    BEGIN
        SELECT * INTO um1 FROM get_basic_and_mult_for_um(p_um1);
        SELECT * INTO um2 FROM get_basic_and_mult_for_um(p_um2);
        IF um1.base != um2.base THEN
            RAISE EXCEPTION 'No compatible base. Aborting';
        END IF;
        div := um1.multi / um2.multi;
        RETURN div;
    END;
$$ LANGUAGE 'plpgsql';

CREATE OR REPLACE FUNCTION update_stock_qty_changing_um() RETURNS TRIGGER AS $$
    DECLARE
        div NUMERIC;
    BEGIN
        IF OLD.um != NEW.um THEN
            div := get_div_for_um(OLD.um,NEW.um);
            UPDATE stock SET quantity=round(NEW.quantity*div,2) WHERE article=NEW.article;
        END IF;
        RETURN NULL;
    END;
$$ LANGUAGE 'plpgsql';

CREATE TRIGGER update_qty_from_um_in_stock AFTER UPDATE ON stock
    FOR EACH ROW EXECUTE PROCEDURE update_stock_qty_changing_um();

CREATE OR REPLACE FUNCTION update_stock(p_date DATE, p_article VARCHAR, p_quantity NUMERIC,
                                                                          p_um VARCHAR, p_insert BOOLEAN)
    RETURNS VOID AS $$
    DECLARE
        old_stock RECORD;
        old_stock_log RECORD;
        div NUMERIC;
        new_qty NUMERIC;
    BEGIN
        SELECT * INTO old_stock_log FROM stock_log WHERE article=p_article;

        IF FOUND THEN
            div := get_div_for_um(old_stock_log.um, p_um);
            new_qty := round(p_quantity / div,5);
        ELSE
            new_qty := p_quantity;
            SELECT um INTO old_stock_log.um FROM basic_good
                WHERE name=p_article;
        END IF;

        IF p_insert THEN
            INSERT INTO stock_log(article,modifier,stock_date,um) VALUES
                (p_article, new_qty, p_date, old_stock_log.um);
        ELSE
            INSERT INTO stock_log(article,modifier,stock_date,um) VALUES
                (p_article, 0-new_qty, p_date, old_stock_log.um);
        END IF;

        SELECT * INTO old_stock FROM stock WHERE article=p_article;

        div := get_div_for_um(old_stock.um,p_um);
        new_qty := p_quantity / div;

        IF p_insert THEN
            UPDATE stock SET quantity=old_stock.quantity+new_qty
                WHERE article=p_article;
        ELSE
            UPDATE stock SET quantity=old_stock.quantity-new_qty
                WHERE article=p_article;
        END IF;
    END;
$$ LANGUAGE 'plpgsql';

CREATE TYPE stock_report AS
(
    Article VARCHAR,
    qty NUMERIC,
    unit_of_measurement VARCHAR
);


CREATE OR REPLACE FUNCTION report_hist_stock(p_date_from DATE,
                                                                                p_date_to DATE)
    RETURNS SETOF stock_report AS $$
        SELECT article, sum(modifier), um FROM stock_log
            WHERE stock_date>=$1 AND stock_date<=$2
            GROUP BY article, um;
$$ LANGUAGE SQL;
