import sqlite3
conn = sqlite3.connect('./local.db') # connect to a database,  create if not existed.
c = conn.cursor() # get a cursor object

c.execute('''insert into people 
          (pid, pname, address)
          values
          ("1", "Nan Qin","1023 Howell Mill Road")''')

c.execute('''insert into people values
          ("2","Charles","2139 Northdrive Avenue")''')

c.execute('''insert into company values
          ("1","Doctier","10th Street")''')

c.execute('''insert into company values
          ("3","Sousys","University Ave")''')

c.execute('''insert into working values
          ("1", "1", "2012-01-10", "CEO", 9000000)''')

c.execute('''insert into working values
          ("1", "2", "2014-01-23", "CTO", 1200000)''')
c.execute('''insert into working values
          ("2", "2", "2014-03-12", "Enginner", 3000)''')
conn.commit() # need to manually commit the changes
c.close() # close the cursor if we are done with it
