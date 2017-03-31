from django.db import models
from django.db.models import *
from django.template.defaultfilters import slugify

from django.conf import settings


class Region(models.Model):
    name = models.CharField(max_length=200)
    voters = models.IntegerField(default=0)
    ballots = models.IntegerField(default=0)
    slug = models.CharField(max_length=200)

    def save(self, *args, **kwargs):
        self.slug = slugify(self.name)
        super(Region, self).save(*args, **kwargs)

    def __str__(self):
        return self.name

    class Meta:
        abstract = True


class Kraj(Region):
    subunit_str = 'Województwo'

    def subunit(self):
        return Wojewodztwo

    def buildQ(self):
        return Q()

    def path(self):
        format_str = '/'
        if settings.GEN_MODE:
            format_str = './index.html'
        return format_str

    def css_dir(self):
        return './'


class Wojewodztwo(Region):
    subunit_str = 'Powiat'

    def subunit(self):
        return Powiat

    def buildQ(self):
        return Q(wojewodztwo=self)

    def path(self):
        format_str = '/{0}/'
        if settings.GEN_MODE:
            format_str = './{0}/index.html'
        return format_str.format(self.slug)

    def file(self):
        return './gen/{0}/index.html'.format(self.slug)
    
    def css_dir(self):
        return './../'


class Powiat(Region):
    wojewodztwo = models.ForeignKey(Wojewodztwo)

    subunit_str = 'Gmina'

    def subunit(self):
        return Gmina

    def buildQ(self):
        return Q(powiat=self)

    def path(self):
        if settings.GEN_MODE:
            return './{0}/index.html'.format(self.slug)
        else:
            return '/{0}/{1}/'.format(self.wojewodztwo.slug, self.slug)

    def file(self):
        return './gen/{0}/{1}/index.html'.format(self.wojewodztwo.slug, self.slug)

    def css_dir(self):
        return './../../'


class Gmina(Region):
    powiat = models.ForeignKey(Powiat)
    code = models.IntegerField()

    def path(self):
        if settings.GEN_MODE:
            return './{0}/index.html'.format(self.slug)
        else:
            return '/{0}/{1}/{2}/'.format(self.powiat.wojewodztwo.slug, self.powiat.slug, self.slug)

    def buildQ(self):
        return Q(gmina=self)

    def __str__(self):
        return self.name

    def file(self):
        return './gen/{0}/{1}/{2}/index.html'.format(self.powiat.wojewodztwo.slug, self.powiat.slug, self.slug)

    def css_dir(self):
        return './../../../'


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
