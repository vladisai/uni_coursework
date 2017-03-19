from __future__ import unicode_literals

from django.db import models

# Create your models here.
class DriverEntry(models.Model):
    driver_name = models.CharField(max_length = 200)

    def __str__(self):
        return '( ' + self.driver_name + ' )'

class LogSession(models.Model):
    date = models.DateField(auto_now_add = True)
    comment = models.CharField(max_length = 200)

    def __str__(self):
        return '[' + str(self.date) + '] ' + self.comment 

class LogEntry(models.Model):
    time = models.FloatField()
    driver = models.ForeignKey(DriverEntry)
    message = models.CharField(max_length = 200)
    logSession = models.ForeignKey(LogSession)

    def __str__(self):
        return '[' + str(self.time) + ']' + str(self.driver) + ':' + self.message

