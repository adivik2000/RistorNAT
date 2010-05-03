/** @brief
 *
 * Dep: GoodsForSale, Lunch
 */

-- This table contain the places and the date for every dinner.
CREATE TABLE "dinner"
(
  dinner_date DATE NOT NULL,
  places INTEGER NOT NULL,
  PRIMARY KEY (dinner_date)
) WITH (OIDS=TRUE);

-- This table contain the reference of what composed goods are sold in a certain
-- dinner
CREATE TABLE "d_sales"
(
  sales_date DATE REFERENCES dinner(dinner_date)
    MATCH SIMPLE ON UPDATE CASCADE ON DELETE RESTRICT,
  article INTEGER REFERENCES good_for_sale(id)
    MATCH SIMPLE ON UPDATE CASCADE ON DELETE RESTRICT,
  quantity_sold INTEGER NOT NULL,
  sell_price NUMERIC NOT NULL,
  PRIMARY KEY(sales_date,article)
) WITH (OIDS=FALSE);

