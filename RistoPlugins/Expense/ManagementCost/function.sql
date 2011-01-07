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

CREATE OR REPLACE FUNCTION update_stocks()
    RETURNS TRIGGER AS $$
    DECLARE
        dc_date DATE;
        value NUMERIC;
    BEGIN
        IF TG_OP = 'INSERT' THEN
            SELECT document_date INTO dc_date FROM document_goods_cost WHERE
                id=NEW.document;

            value := NEW.quantity * NEW.single_price;
            PERFORM * FROM update_stock(dc_date, NEW.good, NEW.quantity, NEW.um,TRUE);
        ELSIF TG_OP = 'UPDATE' THEN
            SELECT document_date INTO dc_date FROM document_goods_cost WHERE
                id=OLD.document;
            value := OLD.quantity * OLD.single_price;
            PERFORM * FROM update_stock(dc_date, OLD.good, OLD.quantity, OLD.um,FALSE);

            SELECT document_date INTO dc_date FROM document_goods_cost WHERE
                id=NEW.document;
            value := NEW.quantity * NEW.single_price;
            PERFORM * FROM update_stock(dc_date, NEW.good, NEW.quantity, NEW.um,TRUE);
        ELSE
            SELECT document_date INTO dc_date FROM document_goods_cost WHERE
                id=OLD.document;
            value := OLD.quantity * OLD.single_price;
            PERFORM * FROM update_stock(dc_date, OLD.good, OLD.quantity, OLD.um,FALSE);
        END IF;
        RETURN NULL;
    END;
$$ LANGUAGE 'plpgsql';

CREATE TRIGGER trigger_row_goods AFTER INSERT OR UPDATE OR DELETE ON row_goods_cost
    FOR EACH ROW EXECUTE PROCEDURE update_stocks();
