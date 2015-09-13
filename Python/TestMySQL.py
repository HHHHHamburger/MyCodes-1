# -*- coding: gbk -*-

import MySQLdb
from random import random
from time import clock

SIZE1 = 10000
SIZE2 = 100
DB_ENGINE = 'InnoDB' # InnoDB Memory MyISAM
print '������������', SIZE1 * SIZE2
print '�������棺', DB_ENGINE

t1 = clock()

sqls = ['INSERT INTO test (value) VALUES (%s)' % '),('.join([`random()` for i in xrange(SIZE1)]) for j in xrange(SIZE2)]
t2 = clock()

try:
  con = MySQLdb.connect(host='localhost',user='root', passwd='Dyc19930418', db='pythonsql',port=3306)
  t3 = clock()
  cu = con.cursor()
except:
	pass

con.autocommit(True)

try:
  cu.execute('DROP TABLE test')
except:
  pass

cu.execute('''CREATE TABLE test (
`id` INT AUTO_INCREMENT PRIMARY KEY,
`value` REAL)
ENGINE = %s''' % DB_ENGINE)
t4 = clock()

for sql in sqls:
  cu.execute(sql)
t5 = clock()

cu.execute('SELECT COUNT(*) FROM test WHERE value < 0.1')
print '����%d��С��0.1�������' % cu.fetchone()[0]
t6 = clock()

cu.execute('SELECT * FROM test WHERE value > 0.9')
print '����%d������0.9�������' % len(cu.fetchall())
t7 = clock()

cu.execute('UPDATE test SET value = value + 0.1 WHERE value > 0.4 AND value < 0.5')
t8 = clock()

cu.execute('DELETE FROM test WHERE value > 0.5 AND value < 0.6')
t9 = clock()

cu.close()
con.close()
t10 = clock()

con = MySQLdb.connect(host='localhost',user='root', passwd='Dyc19930418', db='pythonsql',port=3306)
t11 = clock()
con.close()

print '�����������', t2 - t1
print '�����������ݿ⣺', t3 - t2
print '�ٴ��������ݿ⣺', t11 - t10
print '��ʼ�����ݿ⣺', t4 - t3
print '���룺', t5 - t4
print 'ѡ��COUNT��', t6 - t5
print 'ѡ��', t7 - t6
print '���£�', t8 - t7
print 'ɾ����', t9 - t8
print '�ر����ӣ�', t10 - t9
print '��ʱ�䣺', t10 - t1
