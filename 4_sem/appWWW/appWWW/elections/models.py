from django.db import models

class Wojewodztwo(models.Model):
    name = models.CharField(max_length = 200)

    def __str__(self):
        return self.name

class Powiat(models.Model):
    wojewodztwo = models.ForeignKey(Wojewodztwo)
    name = models.CharField(max_length = 200)

    def __str__(self):
        return self.name

class Gmina(models.Model):
    powiat = models.ForeignKey(Powiat)
    name = models.CharField(max_length = 200)
    code = models.IntegerField()

    def __str__(self):
        return self.name

class Candidate(models.Model):
    name = models.CharField(max_length = 200)

    def __str__(self):
        return self.name

class Vote(models.Model):
    wojewodztwo = models.ForeignKey(Wojewodztwo)
    gmina = models.ForeignKey(Gmina)
    powiat = models.ForeignKey(Powiat)
    candidate = models.ForeignKey(Candidate)
    votes = models.IntegerField()

    def __str__(self):
        return 'In {0}, {1}, {2}, {3} people voted for {4}'.format(self.wojewodztwo, 
                self.gmina,
                self.powiat, 
                self.votes, 
                self.candidate)

