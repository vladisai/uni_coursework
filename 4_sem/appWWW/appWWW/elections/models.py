from django.db import models
from django.template.defaultfilters import slugify


class Wojewodztwo(models.Model):
    name = models.CharField(max_length=200)
    voters = models.IntegerField(default=0)
    ballots = models.IntegerField(default=0)
    slug = models.CharField(max_length=200)

    @classmethod
    def create(cls, name):
        w = cls(name=name, slug=slugify(name))
        return w

    def path(self):
        return '/{0}/'.format(self.slug)

    def __str__(self):
        return self.name


class Powiat(models.Model):
    wojewodztwo = models.ForeignKey(Wojewodztwo)
    name = models.CharField(max_length=200)
    ballots = models.IntegerField(default=0)
    voters = models.IntegerField(default=0)
    slug = models.CharField(max_length=200)

    @classmethod
    def create(cls, name, wojewodztwo):
        w = cls(name=name, slug=slugify(name), wojewodztwo=wojewodztwo)
        return w

    def path(self):
        return '/{0}/{1}/'.format(self.wojewodztwo.slug, self.slug)

    def __str__(self):
        return self.name


class Gmina(models.Model):
    powiat = models.ForeignKey(Powiat)
    name = models.CharField(max_length=200)
    code = models.IntegerField()
    ballots = models.IntegerField(default=0)
    voters = models.IntegerField(default=0)
    slug = models.CharField(max_length=200)

    @classmethod
    def create(cls, name, code, powiat):
        w = cls(name=name, slug=slugify(name), powiat=powiat, code=code)
        return w

    def path(self):
        return '/{0}/{1}/{2}/'.format(self.powiat.wojewodztwo.slug, self.powiat.slug, self.slug)

    def __str__(self):
        return self.name


class Candidate(models.Model):
    name = models.CharField(max_length=200)

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
