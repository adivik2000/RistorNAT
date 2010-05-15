CREATE OR REPLACE FUNCTION delete_from_stock(p_good VARCHAR)
    RETURNS VOID AS $$
        DELETE FROM stock WHERE article=$1;
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
