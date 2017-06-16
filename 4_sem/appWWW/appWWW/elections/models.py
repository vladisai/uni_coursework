from django.db import models
from django.db.models import *
from django.template.defaultfilters import slugify

from django.conf import settings
from django.urls import reverse

from django.db.models.signals import post_save
from django.dispatch import receiver

from rest_framework.authtoken.models import Token

from channels.binding import Binding
from channels.channel import Group


import elections.utility as utility


class Region(models.Model):
    name = models.CharField(max_length=200)
    slug = models.CharField(max_length=200)

    def save(self, *args, **kwargs):
        self.slug = slugify(self.name)
        super(Region, self).save(*args, **kwargs)

    def __str__(self):
        return self.name

    def getBallots(self):
        pass

    def getVoters(self):
        pass

    def getTurnout(self):
        return round(self.getBallots() / self.getVoters() * 100, 2)

    def getDistribution(self):
        return utility.buildVotesDistributionDict(self.buildQ())

    class Meta:
        abstract = True


class Kraj(Region):
    subunit_str = 'Województwo'

    def subunit(self):
        return Wojewodztwo

    def getBallots(self):
        return Gmina.objects.all().aggregate(Sum('ballots'))['ballots__sum']

    def getVoters(self):
        return Gmina.objects.all().aggregate(Sum('voters'))['voters__sum']

    def buildQ(self):
        return Q()

    def path(self):
        return reverse('elections:kraj')


class Wojewodztwo(Region):
    subunit_str = 'Powiat'
    kraj = models.ForeignKey(Kraj)

    def subunit(self):
        return Powiat

    def getBallots(self):
        p = Powiat.objects.filter(wojewodztwo=self)
        res = 0
        for e in p:
            res += Gmina.objects.filter(powiat=e).aggregate(
                Sum('ballots'))['ballots__sum']
        return res

    def getVoters(self):
        p = Powiat.objects.filter(wojewodztwo=self)
        res = 0
        for e in p:
            res += Gmina.objects.filter(powiat=e).aggregate(
                Sum('voters'))['voters__sum']
        return res

    def buildQ(self):
        return Q(wojewodztwo=self)

    def path(self):
        return reverse('elections:wojewodztwo', args=[self.slug])

class Powiat(Region):
    wojewodztwo = models.ForeignKey(Wojewodztwo)

    subunit_str = 'Gmina'

    def subunit(self):
        return Gmina

    def getBallots(self):
        return Gmina.objects.filter(powiat=self).aggregate(Sum('ballots'))['ballots__sum']

    def getVoters(self):
        return Gmina.objects.filter(powiat=self).aggregate(Sum('voters'))['voters__sum']

    def buildQ(self):
        return Q(powiat=self)

    def path(self):
        return reverse('elections:powiat', args=[self.wojewodztwo.slug, self.slug])


class Gmina(Region):
    powiat = models.ForeignKey(Powiat)
    code = models.IntegerField()
    ballots = models.IntegerField(default=0)
    voters = models.IntegerField(default=0)

    def buildQ(self):
        return Q(gmina=self)

    def getBallots(self):
        return self.ballots

    def getVoters(self):
        return self.voters

    def votes_dict(self):
        return utility.buildVotesDict(self)

    def __str__(self):
        return self.name

    def path(self):
        return reverse('elections:gmina', args=[self.powiat.wojewodztwo.slug,
                                                self.powiat.slug,
                                                self.slug])

    def edit_path(self):
        return reverse('elections:edit_gmina', args=[self.powiat.wojewodztwo.slug,
                                                     self.powiat.slug,
                                                     self.slug])


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

@receiver(post_save, sender=settings.AUTH_USER_MODEL)
def create_auth_token(sender, instance=None, created=False, **kwargs):
    if created:
        Token.objects.create(user=instance)
