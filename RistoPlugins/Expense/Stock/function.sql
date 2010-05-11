CREATE OR REPLACE FUNCTION delete_from_stock(p_good VARCHAR)
    RETURNS VOID AS $$
        DELETE FROM stock WHERE article=$1;
$$ LANGUAGE SQL;
