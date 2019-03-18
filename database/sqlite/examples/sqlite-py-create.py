import sqlite3
conn = sqlite3.connect('./local.db') # connect to a database,  create if not existed.
c = conn.cursor() # get a cursor object
# Entity: people, company, relationship: job, since, salary many to many
# primary key should have explicit NOT NULL, otherwise it can be null.
c.execute('''create table if not exists people
                (pid text NOT NULL, 
                pname text NOT NULL, 
                address text,
                primary key (pid))''')
c.execute('''create table if not exists company
                (cid text NOT NULL, 
                cname text NOT NULL, 
                address text,
                primary key (cid))''')
c.execute('''create table if not exists working
                (pid text NOT NULL, 
                cid text NOT NULL,
                since text,
                job text,
                salary integer,
                primary key (pid, cid),
                FOREIGN KEY (pid) REFERENCES people (pid),
                FOREIGN KEY (cid) REFERENCES company (cid))''')


conn.commit() # need to manually commit the changes
c.close() # close the cursor if we are done with it
