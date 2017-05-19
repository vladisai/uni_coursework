import os
import json
import random

from elections import models
from django.db import models as django_models
from functools import reduce
from django.shortcuts import render

from django.conf import settings

def renderHTML(request, region, title, json=False):
    q = region.buildQ()
    region.turnout = region.getBallots() / region.getVoters() * 100
    vals = {'candidates_stats': buildCandidatesStats(q),
            'regions_data': buildRegionsData(region.subunit().objects.filter(q)),
            'region_info': region,
            'title': title,
            'unit': region.subunit_str,
            'search_form': forms.SearchForm()
            }
    if json:
        vals['data'] = buildJSONData()

    return render(request, 'stats.html', vals)


def renderGminaHTML(request, region, title):
    q = region.buildQ()
    region.turnout = region.getBallots() / region.getVoters() * 100
    vals = {'candidates_stats': buildCandidatesStats(q),
            'region_info': region,
            'title': title,
            'search_form': forms.SearchForm()
            }

    return render(request, 'gmina.html', vals)


def buildJSONData():
    data = {}
    cols = [{'label': 'County', 'type': 'string'},
            {'label': 'Frekwencja', 'type': 'number'}]

    rows = []

    for w in models.Wojewodztwo.objects.all():
        turnout = w.getBallots() / w.getVoters() * 100
        vals = []
        turnout_str = '{0}%'.format(round(turnout, 2))
        vals.append({'v': w.name, 'f': None})
        vals.append({'v': turnout, 'f': turnout_str})
        rows.append({'c': vals})

    data['cols'] = cols
    data['rows'] = rows

    json_data = json.dumps(data, sort_keys=True, indent=4)
    return json_data


def getCandidatesVotes(candidates, q=django_models.Q()):
    votes = list(map(lambda c: models.Vote.objects.filter(q).filter(
        candidate=c).aggregate(django_models.Sum('votes'))['votes__sum'], candidates))
    return votes


def getCandidatesVotesPercentage(candidates, q=django_models.Q()):
    votes = getCandidatesVotes(candidates, q)
    total = reduce(lambda x, y: x + y, votes)
    votes = list(map(lambda c: c / total * 100, votes))
    return votes


def getCandidates():
    return models.Candidate.objects.all().order_by('name')


def buildRow(candidates, q=django_models.Q()):
    votes = getCandidatesVotesPercentage(candidates, q)
    total = list(zip(candidates, votes))
    total.sort(key=lambda tup: tup[1])
    total = total[::-1]
    mx = total[0][1]
    total = list(map(lambda x: (x[0], x[1], x[1] / mx * 100), total))
    return total


def buildCandidatesStats(q=django_models.Q()):
    candidates = getCandidates()
    return buildRow(candidates, q)


def buildVotesDistributionDict(q=django_models.Q()):
    votes = getCandidatesAndVotesPercentage(q)
    votes = list(map(lambda x: {'name': str(x[0]), 'votes': round(x[1], 2) }, votes))
    return votes


def buildColors():

    h = 320
    l = 20
    s = 80

    h_step = -10
    l_step = 3
    s_step = -3

    cols = []

    for i in range(12):
        col = 'hsl({0}, {1}%, {2}%)'.format(h, s, l)
        if i % 2 == 0:
            cols.append(col)
        cols.insert(0, col)

        h += h_step
        l += l_step
        s += s_step

    return cols


def getCandidatesAndVotesPercentage(q=django_models.Q()):
    candidates = getCandidates()
    votes = getCandidatesVotesPercentage(candidates, q)
    total = list(zip(candidates, votes))
    total.sort(key=lambda tup: tup[1])
    total.reverse()
    return total


def getCandidatesAndVotes(q=django_models.Q()):
    candidates = getCandidates()
    votes = getCandidatesVotes(candidates, q)
    total = list(zip(candidates, votes))
    total.sort(key=lambda tup: tup[0].name)
    total.reverse()
    return total


def buildHorizontalBar(q=django_models.Q()):
    cols = buildColors()
    votes = getCandidatesAndVotesPercentage(q)
    res = zip(votes, cols)
    res = map(lambda x: (str(x[0][0]), x[0][1], x[1]), res)
    res = list(res)
    return res


def buildRegionsData(objects):
    for o in objects:
        o.bar = buildHorizontalBar(o.buildQ())
        o.turnout = o.getBallots() / o.getVoters() * 100
    return objects


def getGmina(wojewodztwo_slug, powiat_slug, gmina_slug):
    w = models.Wojewodztwo.objects.get(slug=wojewodztwo_slug)
    p = models.Powiat.objects.get(slug=powiat_slug, wojewodztwo=w)
    g = models.Gmina.objects.get(powiat=p, slug=gmina_slug)
    return g


def getPowiat(wojewodztwo_slug, powiat_slug):
    w = models.Wojewodztwo.objects.get(slug=wojewodztwo_slug)
    p = models.Powiat.objects.get(slug=powiat_slug, wojewodztwo=w)
    return p


def getWojewodztwo(wojewodztwo_slug):
    return models.Wojewodztwo.objects.get(slug=wojewodztwo_slug)


def buildVotesDict(gmina):
    q = gmina.buildQ()
    votes_list = getCandidatesAndVotes(q)
    votes_list = map(lambda x: (x[0].name, x[1]), votes_list)
    return dict(votes_list)

def updateVotes(gmina, votes_dict):
    for key, value in votes_dict.items():
        candidate = models.Candidate.objects.get(name=key)
        print(candidate)
        vote = models.Vote.objects.get(gmina=gmina, candidate=candidate)
        vote.votes = value
        vote.save()
