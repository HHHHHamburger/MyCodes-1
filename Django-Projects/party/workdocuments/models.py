# -*- coding:gb18030 -*-
from django.db import models
from django.utils import timezone
import datetime

from notifications.models import Notification


FILE_TYPES=(
		(u'�����ĵ�',u'�����ĵ�'),
		(u'�����ĵ�',u'�����ĵ�'),
		)

class Document(models.Model):
	"""
	class Document defines a document with a `filename`,`pub_date`,
	`file_path`,`author`
	"""
	filename=models.CharField(u'�ļ���',max_length=200)
	pub_date=models.DateTimeField(u'����ʱ��')
	file_path=models.FileField(u'�ļ�·��',upload_to='workdocuments/media/uploadFiles',default=None)
	author=models.CharField(u'����',max_length=100)
	filetype=models.CharField(u'����',choices=FILE_TYPES,max_length=10,default=u'�����ĵ�')
	def __unicode__(self):
		return self.filename

	def was_published_in_recent_14_days(self):
		now=timezone.now()
		return now-datetime.timedelta(days=14)<=self.pub_date<=now
	was_published_in_recent_14_days.admin_order_field='pub_date'
	was_published_in_recent_14_days.boolean=True
	was_published_in_recent_14_days.short_description=u"�������"	


class AdditionalDocument(Document):
	"""
	Inherits from Document,`notification` is required.
	"""
	notification_set=models.ManyToManyField(Notification)
