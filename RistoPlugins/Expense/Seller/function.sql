CREATE OR REPLACE FUNCTION report_seller_amount(p_date_from DATE,p_date_to DATE,
                                                p_seller VARCHAR)
    RETURNS NUMERIC AS $$
        SELECT sum(amount) FROM document_purchasing_cost
            WHERE document_date>=$1 AND document_date<=$2
            AND seller=$3 GROUP BY seller;
$$ LANGUAGE SQL;
