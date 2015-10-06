# -*- coding:gb18030 -*-
from django.db import models
from django.utils import timezone
import datetime
from ckeditor.fields import RichTextField


class Notification(models.Model):
	"""
	class Notification defines a notification with a `title`,`pub_date`,
	`content`,`files`.
	note: notification to `additional_files is a many-to-many relationship. 
	"""
	title=models.CharField(u'����',max_length=200)
	pub_date=models.DateTimeField(u'����ʱ��',default=timezone.now)
	content=RichTextField(u'����')
	
	def __unicode__(self):
		return self.title

	def was_published_in_recent_14_days(self):
		now=timezone.now()
		return now-datetime.timedelta(days=14)<=self.pub_date<=now
	was_published_in_recent_14_days.admin_order_field='pub_date'
	was_published_in_recent_14_days.boolean=True
	was_published_in_recent_14_days.short_description=u"�������"

	def short_content(self):
		response=self.content[:70]
		if len(self.content)>70:
			response+='...'
		return response
	short_content.short_description=u"��Ҫ����"
