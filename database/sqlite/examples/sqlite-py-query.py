import sqlite3
conn = sqlite3.connect('./local.db') # connect to a database,  create if not existed.
c = conn.cursor() # get a cursor object


c.execute('select * from people where pid="1"')
for row in c:
    print(row)
c.execute('select cname, address from company')
for row in c:
    print(row)
c.execute('select * from working where salary > 10000')
for row in c:
    print(row)
c.execute('select * from people JOIN working ON people.pid = working.pid')
for row in c:
    print(row)
c.close() # close the cursor if we are done with it
