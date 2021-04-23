CREATE TABLE monet_storedim(
scode INTEGER,
sname VARCHAR(30),
city VARCHAR(30),
state VARCHAR(20),
pincode INTEGER
) ENGINE=COLUMNSTORE;

CREATE TABLE monet_itemdim(
icode INTEGER,
iname VARCHAR(30),
category CHAR(5),
subcategory CHAR(3),
price INTEGER
) ENGINE=COLUMNSTORE;

CREATE TABLE monet_dateofpurchase(
dcode INTEGER,
purchasedate DATE,
dayoftheweek CHAR(3),
month CHAR(3),
quarter CHAR(2),
year INTEGER
) ENGINE=COLUMNSTORE;

CREATE TABLE monet_purchasefact (
scode INTEGER,
icode INTEGER,
dcode INTEGER,
qty INTEGER
) ENGINE=COLUMNSTORE;