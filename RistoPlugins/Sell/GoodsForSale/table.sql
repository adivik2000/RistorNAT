/** @brief GoodsForSale
 *
 * Dep: GoodsCategory, SellCategory, UM
 */
CREATE SEQUENCE "good_for_sale_sequence";

-- This table will contain the articles ready for selling (like pasta all'
-- amatriciana).
-- The article can be composed by one or more basic component.
CREATE TABLE "good_for_sale"
(
  id INTEGER NOT NULL DEFAULT nextval('good_for_sale_sequence'),
  description CHARACTER VARYING(128) NOT NULL,
  good_category CHARACTER VARYING(128) REFERENCES goods_category(description)
    MATCH SIMPLE ON UPDATE CASCADE ON DELETE RESTRICT,
  sell_category CHARACTER VARYING(128) REFERENCES sell_category(description)
    MATCH SIMPLE ON UPDATE CASCADE ON DELETE RESTRICT,
  quantity NUMERIC NOT NULL,
  um CHARACTER VARYING(5) REFERENCES unit_of_measurement(short_name) MATCH SIMPLE
    ON UPDATE CASCADE ON DELETE RESTRICT,
  last_cost NUMERIC,
  PRIMARY KEY(id)
) WITH (OIDS=TRUE);
