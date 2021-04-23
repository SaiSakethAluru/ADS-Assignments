CREATE TABLE dsk_storedim(
    scode INTEGER,
    sname VARCHAR(30),
    city VARCHAR(30),
    state VARCHAR(20),
    pincode INTEGER,
    PRIMARY KEY (scode)
);

CREATE TABLE dsk_itemdim(
    icode INTEGER,
    iname VARCHAR(30),
    category CHAR(5),
    subcategory CHAR(3),
    price INTEGER,
    PRIMARY KEY (icode)
);

CREATE TABLE dsk_dateofpurchase(
    dcode INTEGER,
    purchasedate DATE,
    dayoftheweek CHAR(3),
    month CHAR(3),
    quarter CHAR(2),
    year INTEGER,
    PRIMARY KEY (dcode)
);

CREATE TABLE dsk_purchasefact (
    scode INTEGER,
    icode INTEGER,
    dcode INTEGER,
    qty INTEGER,
    PRIMARY KEY (scode,icode,dcode),
    FOREIGN KEY (scode) REFERENCES dsk_storedim(scode),
    FOREIGN KEY (icode) REFERENCES dsk_itemdim(icode),
    FOREIGN KEY (dcode) REFERENCES dsk_dateofpurchase(dcode)
);