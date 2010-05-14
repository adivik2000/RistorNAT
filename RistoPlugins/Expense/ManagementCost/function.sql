CREATE OR REPLACE FUNCTION set_amount_document_purchasing(p_document INTEGER,
        p_amount NUMERIC) RETURNS VOID AS $$
        UPDATE document_purchasing_cost SET amount=$2 WHERE id = $1;
$$ LANGUAGE SQL;

CREATE OR REPLACE FUNCTION set_amount_document_goods(p_document INTEGER,
        p_amount NUMERIC) RETURNS VOID AS $$
        UPDATE document_goods_cost SET amount=$2 WHERE id = $1;
$$ LANGUAGE SQL;

CREATE OR REPLACE FUNCTION get_amount_document_purchasing(p_document INTEGER)
    RETURNS NUMERIC AS $$
        SELECT amount FROM document_purchasing_cost WHERE id = $1;
$$ LANGUAGE SQL;

CREATE OR REPLACE FUNCTION get_amount_document_goods(p_document INTEGER)
    RETURNS NUMERIC AS $$
        SELECT amount FROM document_goods_cost WHERE id = $1;
$$ LANGUAGE SQL;

CREATE OR REPLACE FUNCTION delete_document_purchasing(p_doc INTEGER)
    RETURNS VOID AS $$
        DELETE FROM row_purchasing_cost WHERE document=$1;
        DELETE FROM document_purchasing_cost WHERE id=$1;
$$ LANGUAGE SQL;

CREATE OR REPLACE FUNCTION delete_document_goods(p_doc INTEGER)
    RETURNS VOID AS $$
        DELETE FROM row_goods_cost WHERE document=$1;
        DELETE FROM document_goods_cost WHERE id=$1;
$$ LANGUAGE SQL;

CREATE OR REPLACE FUNCTION document_purchasing_ins_upd(p_date DATE,
                                p_number INTEGER, p_seller CHARACTER VARYING)
    RETURNS INTEGER AS $$
        DECLARE
        ret INTEGER = -1;
        BEGIN

        SELECT id INTO ret FROM document_purchasing_cost
            WHERE document_date=p_date AND document_number=p_number AND
                seller=p_seller;

        IF NOT FOUND THEN
            INSERT INTO document_purchasing_cost("id","document_date","document_number",
                    "seller") VALUES (DEFAULT,$1,$2,$3) RETURNING id INTO ret;
        END IF;

        RETURN ret;
     END;
$$ LANGUAGE 'plpgsql';

CREATE OR REPLACE FUNCTION document_goods_ins_upd(p_date DATE,
                                p_number INTEGER, p_seller CHARACTER VARYING)
    RETURNS INTEGER AS $$
        DECLARE
        ret INTEGER = -1;
        BEGIN

        SELECT id INTO ret FROM document_goods_cost
            WHERE document_date=p_date AND document_number=p_number AND
                seller=p_seller;

        IF NOT FOUND THEN
            INSERT INTO document_goods_cost("id","document_date","document_number",
                    "seller") VALUES (DEFAULT,$1,$2,$3) RETURNING id INTO ret;
        END IF;

        RETURN ret;
     END;
$$ LANGUAGE 'plpgsql';

CREATE OR REPLACE FUNCTION update_stock(p_quantity NUMERIC, p_article VARCHAR,
                                        p_um VARCHAR, p_insert BOOLEAN)
    RETURNS VOID AS $$
    DECLARE
        tot_qty NUMERIC;
        multiplicator NUMERIC;
        basic VARCHAR;
    BEGIN
        SELECT multi INTO multiplicator FROM unit_of_measurement
            WHERE short_name=p_um;
        SELECT base INTO basic FROM unit_of_measurement
            WHERE short_name=p_um;
        SELECT quantity INTO tot_qty FROM stock WHERE article=p_article;

        IF p_insert THEN
            UPDATE stock SET quantity=tot_qty+(p_quantity*multiplicator), um=basic
                WHERE article=p_article;
        ELSE
            UPDATE stock SET quantity=tot_qty-(p_quantity*multiplicator)
                WHERE article=p_article;
        END IF;
    END;
$$ LANGUAGE 'plpgsql';

CREATE OR REPLACE FUNCTION update_cost(p_article VARCHAR, p_single_price NUMERIC,
                                       p_insert BOOLEAN)
    RETURNS VOID AS $$
    DECLARE
        old_price NUMERIC;
        new_price NUMERIC;
    BEGIN
        SELECT average_cost INTO old_price FROM basic_good
            WHERE name=p_article;
        IF old_price IS NULL THEN
            old_price := 0;
        END IF;
        IF p_insert THEN
            new_price := (old_price + p_single_price)/2;
        ELSE
            new_price := ((old_price*2)-p_single_price)/2;
        END IF;
        UPDATE basic_good SET average_cost=new_price WHERE name=p_article;
    END;
$$ LANGUAGE 'plpgsql';

CREATE OR REPLACE FUNCTION update_goods_stocks_and_cost()
    RETURNS TRIGGER AS $$
    DECLARE
        tot_amount NUMERIC;
    BEGIN
        IF TG_OP = 'INSERT' THEN
            PERFORM * FROM update_stock(NEW.quantity, NEW.good, NEW.um,TRUE);
            PERFORM * FROM update_cost(NEW.good,NEW.single_price,TRUE);
        ELSIF TG_OP = 'UPDATE' THEN
            PERFORM * FROM update_stock(OLD.quantity, OLD.good, OLD.um,FALSE);
            PERFORM * FROM update_cost(OLD.good,OLD.single_price,FALSE);
            PERFORM * FROM update_stock(NEW.quantity, NEW.good, NEW.um,TRUE);
            PERFORM * FROM update_cost(NEW.good,NEW.single_price,TRUE);
        ELSE
            PERFORM * FROM update_stock(OLD.quantity, OLD.good, OLD.um,FALSE);
            PERFORM * FROM update_cost(OLD.good,OLD.single_price,FALSE);
        END IF;
        RETURN NULL;
    END;
$$ LANGUAGE 'plpgsql';

CREATE TRIGGER trigger_row_goods AFTER INSERT OR UPDATE OR DELETE ON row_goods_cost
    FOR EACH ROW EXECUTE PROCEDURE update_goods_stocks_and_cost();
