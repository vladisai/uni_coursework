from django.db import transaction

import os
os.environ.setdefault("DJANGO_SETTINGS_MODULE", "appWWW.settings")
import django
django.setup()

from elections.models import *
import parse

def clean():
    Vote.objects.all().delete()
    Gmina.objects.all().delete()
    Powiat.objects.all().delete()
    Wojewodztwo.objects.all().delete()
    Candidate.objects.all().delete()

def constructVote(wojewodztwo, powiat, gmina, candidate, votes):
    vote = Vote()
    vote.wojewodztwo = wojewodztwo
    vote.gmina = gmina
    vote.powiat = powiat
    vote.candidate = candidate
    vote.votes = votes 
    return vote

def populate():
    cols, rows = parse.read()

    with transaction.atomic():

        all_votes = []
        for row in rows:
            kraj, _ = Kraj.objects.get_or_create(name = 'Polska')
            wojewodztwo, _ = Wojewodztwo.objects.get_or_create(name = row[0])
            powiat, _ = Powiat.objects.get_or_create(name = row[4], wojewodztwo = wojewodztwo)
            gmina, _ = Gmina.objects.get_or_create(name = row[3], code = row[2], powiat = powiat)

            for i in range(11, 23):
                candidate, _ = Candidate.objects.get_or_create(name = cols[i])
                vote = constructVote(wojewodztwo, powiat, gmina, candidate, row[i])
                all_votes.append(vote)

            voters = int(row[6])
            ballots = int(row[7])

            gmina.voters += voters
            gmina.ballots += ballots

            powiat.voters += voters
            powiat.ballots += ballots

            wojewodztwo.voters += voters
            wojewodztwo.ballots += ballots

            kraj.voters += voters
            kraj.ballots += ballots

            gmina.save()
            powiat.save()
            wojewodztwo.save()
            kraj.save()

            print(len(all_votes))

        Vote.objects.bulk_create(all_votes)

    print(Vote.objects.all())

clean()
populate()
