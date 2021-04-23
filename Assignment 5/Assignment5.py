import mysql.connector
import datetime

CITY = ['Mumbai','Chennai','Kolkata','Pune','Bengaluru','Guwahati','Kochi']
STATE = {'Mumbai':'Maharastra','Chennai':'Tamil Nadu','Kolkata':'West Bengal','Pune':'Maharastra','Bengaluru':'Karnataka','Guwahati':'Assam','Kochi':'Kerala'}
PINCODES = [123456,234567,345678,456789,567890,678901,789012,890123,901234]
CATEGORY = ['GROCR','CONSM','FASHN','MEDCN','VGTBL']
SUBCATEGORY = ['PRS','OFF','MIL']
DAYOFTHEWEEK = ['MON','TUE','WED','THU','FRI','SAT','SUN']
MONTH = ['JAN','FEB','MAR','APR','MAY','JUN','JUL','AUG','SEP','OCT','NOV','DEC']
YEAR = [2015,2016,2017,2018,2019,2020]

def quarter(month):
    if month in [0,1,2]:
        return 1
    elif month in [3,4,5]:
        return 2
    elif month in [6,7,8]:
        return 3
    else:
        return 4
def connect_to_database(host='localhost',user='saketh',password='Saketh@14',database='ADS_Assignment'):
    print('Connecting to database')
    if database is not None:
        mydb = mysql.connector.connect(
            host=host,
            user=user,
            password=password,
            database=database)
    else:
        mydb = mysql.connector.connect(
            host=host,
            user=user,
            password=password)
    return mydb

def create_disk_tables(mycursor):
    print('Creating disk tables')
    create_storedim_disk_query = """
        CREATE TABLE dsk_storedim(
            scode INTEGER,
            sname VARCHAR(30),
            city VARCHAR(30),
            state VARCHAR(20),
            pincode INTEGER,
            PRIMARY KEY (scode)
        );
    """
    mycursor.execute(create_storedim_disk_query)
    create_itemdim_disk_query = """
        CREATE TABLE dsk_itemdim(
            icode INTEGER,
            iname VARCHAR(30),
            category CHAR(5),
            subcategory CHAR(3),
            price INTEGER,
            PRIMARY KEY (icode)
        );
    """
    mycursor.execute(create_itemdim_disk_query)
    create_dateofpurchase_disk_query = """
        CREATE TABLE dsk_dateofpurchase(
            dcode INTEGER,
            purchasedate DATE,
            dayoftheweek CHAR(3),
            month CHAR(3),
            quarter CHAR(2),
            year INTEGER,
            PRIMARY KEY (dcode)
        );
    """
    mycursor.execute(create_dateofpurchase_disk_query)
    create_purchasefact_disk_query = """
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
    """
    mycursor.execute(create_purchasefact_disk_query)


def create_mem_tables(mycursor):
    print("Creating in memory tables")
    create_storedim_mem_query = """
        CREATE TABLE inm_storedim(
            scode INTEGER,
            sname VARCHAR(30),
            city VARCHAR(30),
            state VARCHAR(20),
            pincode INTEGER,
            PRIMARY KEY (scode)
        ) ENGINE=MEMORY;
    """
    mycursor.execute(create_storedim_mem_query)
    create_itemdim_mem_query = """
        CREATE TABLE inm_itemdim(
            icode INTEGER,
            iname VARCHAR(30),
            category CHAR(5),
            subcategory CHAR(3),
            price INTEGER,
            PRIMARY KEY (icode)
        ) ENGINE=MEMORY;
    """
    mycursor.execute(create_itemdim_mem_query)
    create_dateofpurchase_mem_query = """
            CREATE TABLE inm_dateofpurchase(
            dcode INTEGER,
            purchasedate DATE,
            dayoftheweek CHAR(3),
            month CHAR(3),
            quarter CHAR(2),
            year INTEGER,
            PRIMARY KEY (dcode)
        ) ENGINE=MEMORY;
    """
    mycursor.execute(create_dateofpurchase_mem_query)
    create_purchasefact_mem_query = """
        CREATE TABLE inm_purchasefact (
            scode INTEGER,
            icode INTEGER,
            dcode INTEGER,
            qty INTEGER,
            PRIMARY KEY (scode,icode,dcode),
            FOREIGN KEY (scode) REFERENCES inm_storedim(scode),
            FOREIGN KEY (icode) REFERENCES inm_itemdim(icode),
            FOREIGN KEY (dcode) REFERENCES inm_dateofpurchase(dcode)
        ) ENGINE=MEMORY;
    """
    mycursor.execute(create_purchasefact_mem_query)

def create_insert_query(tablename,values):
    table_cols = list(values.keys())
    table_vals = list(values.values())
    query = 'INSERT INTO '+tablename+'(';
    for col in table_cols[:-1]:
        if col in ['month','year']:
            query += '"{}"'.format(col)+', '
        else:
            query += col+', '
    if table_cols[-1] in ['month','year']:
        query += '"{}"'.format(table_cols[-1])+')\n'
    else:
        query += table_cols[-1]+')\n'
    query += 'VALUES ('
    for val in table_vals[:-1]:
        if isinstance(val,str):
            query += "'{}'".format(val)+', '
        else:
            query += str(val)+', '
    if isinstance(table_vals[-1],str):
        query += "'{}'".format(table_vals[-1])+');'
    else:
        query += str(table_vals[-1])+');'
    return query

def create_storedim_data(scode):
    values = {}
    values['scode'] = scode
    values['sname'] = 'sname'+str(scode)
    values['city'] = CITY[scode%len(CITY)]
    values['state'] = STATE[values['city']]
    values['pincode'] = PINCODES[scode%len(PINCODES)]
    return values

def create_itemdim_data(icode):
    values = {}
    values['icode'] = icode
    values['iname'] = 'iname'+str(icode)
    values['category'] = CATEGORY[icode%len(CATEGORY)]
    values['subcategory'] = SUBCATEGORY[icode%len(SUBCATEGORY)]
    values['price'] = (icode^(2*icode+1))
    return values

def create_dateofpurchase_data(dcode):
    values = {}
    values['dcode'] = dcode
    year = YEAR[dcode%len(YEAR)]
    month = dcode%len(MONTH)
    date = (dcode%28)+1
    date_obj = datetime.date(year,month+1,date)
    values['purchasedate'] = '{}'.format(date_obj)
    values['dayoftheweek'] = DAYOFTHEWEEK[date_obj.isoweekday()-1]
    values['month'] = MONTH[month]
    values['quarter'] = 'Q'+str(quarter(month))
    values['year'] = year
    return values

def create_purchasefact_data(scode,icode,dcode):
    values = {}
    values['scode'] = scode
    values['icode'] = icode
    values['dcode'] = dcode
    values['qty'] = scode^icode^dcode+1
    return values

def main():
    # mydb = connect_to_database(host='localhost',user='saketh',password='Saketh@14',database='ADS_Assignment')
    # mycursor = mydb.cursor()
    # create_disk_tables(mycursor)
    # create_mem_tables(mycursor)
    print("Storedim queries")
    # with open('dsk_storedim.txt','w') as f,open('inm_storedim.txt','w') as g:
    #     for scode in range(1000):
    #         values = create_storedim_data(scode)
    #         dsk_query = create_insert_query('dsk_storedim',values)
    #         inm_query = create_insert_query('inm_storedim',values)
    #         f.write(dsk_query+'\n')
    #         g.write(inm_query+'\n')
    # print('Itemdim queries')
    # with open('dsk_itemdim.txt','w') as f,open('inm_itemdim.txt','w') as g:
    #     for icode in range(1000):
    #         values = create_itemdim_data(icode)
    #         dsk_query = create_insert_query('dsk_itemdim',values)
    #         inm_query = create_insert_query('inm_itemdim',values)
    #         f.write(dsk_query+'\n')
    #         g.write(inm_query+'\n')
    # print('Dateofpurchase queries')
    # with open('dsk_dateofpurchase.txt','w') as f,open('inm_dateofpurchase.txt','w') as g:
    #     for dcode in range(1000):
    #         values = create_dateofpurchase_data(dcode)
    #         dsk_query = create_insert_query('dsk_dateofpurchase',values)
    #         inm_query = create_insert_query('inm_dateofpurchase',values)
    #         f.write(dsk_query+'\n')
    #         g.write(inm_query+'\n')
    # print('Purchasefact queries')
    # with open('dsk_purchasefact.txt','w') as f, open('inm_purchasefact.txt','w') as g:
    #     for scode in range(1000):
    #         for icode in range(1000):
    #             for dcode in range(10):
    #                 values = create_purchasefact_data(scode,icode,dcode)
    #                 dsk_query = create_insert_query('dsk_purchasefact',values)
    #                 inm_query = create_insert_query('inm_purchasefact',values)
    #                 f.write(dsk_query+'\n')
    #                 g.write(inm_query+'\n')
    # with open('maria_storedim.txt','w') as f:
    #     for scode in range(1000):
    #         values = create_storedim_data(scode)
    #         query = create_insert_query('maria_storedim',values)
    #         f.write(query+'\n')
    # print('Itemdim queries')
    # with open('maria_itemdim.txt','w') as f:
    #     for icode in range(1000):
    #         values = create_itemdim_data(icode)
    #         query = create_insert_query('maria_itemdim',values)
    #         f.write(query+'\n')
    # print('Dateofpurchase queries')
    # with open('maria_dateofpurchase.txt','w') as f:
    #     for dcode in range(1000):
    #         values = create_dateofpurchase_data(dcode)
    #         query = create_insert_query('maria_dateofpurchase',values)
    #         f.write(query+'\n')
    # print('Purchasefact queries')
    # with open('maria_purchasefact.txt','w') as f:
    #     for scode in range(1000):
    #         for icode in range(1000):
    #             for dcode in range(10):
    #                 values = create_purchasefact_data(scode,icode,dcode)
    #                 query = create_insert_query('maria_purchasefact',values)
    #                 f.write(query+'\n')

    # with open('monet_storedim.txt','w') as f:
    #     for scode in range(1000):
    #         values = create_storedim_data(scode)
    #         query = create_insert_query('monet_storedim',values)
    #         f.write(query+'\n')
    print('Itemdim queries')
    # with open('monet_itemdim.txt','w') as f:
    #     for icode in range(1000):
    #         values = create_itemdim_data(icode)
    #         query = create_insert_query('monet_itemdim',values)
    #         f.write(query+'\n')
    print('Dateofpurchase queries')
    # with open('monet_dateofpurchase.txt','w') as f:
    #     for dcode in range(1000):
    #         values = create_dateofpurchase_data(dcode)
    #         query = create_insert_query('monet_dateofpurchase',values)
    #         f.write(query+'\n')
    print('Purchasefact queries')
    with open('monet_purchasefact.txt','w') as f:
        for scode in range(1000):
            for icode in range(1000):
                for dcode in range(10):
                    values = create_purchasefact_data(scode,icode,dcode)
                    query = create_insert_query('monet_purchasefact',values)
                    f.write(query+'\n')                    

if __name__ == '__main__':
    main()