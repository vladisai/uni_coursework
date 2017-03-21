import os
import jinja2
import json
import random

from elections.models import *
from django.db.models import *
from functools import reduce
from django.http import HttpResponse


def render(tpl_path, context):
    path, filename = os.path.split(tpl_path)
    return jinja2.Environment(
        loader=jinja2.FileSystemLoader(path or './')
    ).get_template(filename).render(context)


def renderHTML(q, model, unitName, title, json = False):
    vals = {'candidates_stats': buildCandidatesStats(q),
            'regions': model.objects.filter(q),
            'title': title,
            'unit': unitName,
            }
    if json:
        vals['data'] = buildJSONData()
        
    return render('elections/templates/test.html', vals)


def buildJSONData():
    data = {}
    cols = [{'label': 'County', 'type': 'string'},
            {'label': 'Frekwencja', 'type': 'number'}]

    rows = []

    for w in Wojewodztwo.objects.all():
        turnout = w.ballots / w.voters * 100
        vals = []
        turnout_str = '{0}%'.format(round(turnout, 2))
        vals.append({'v': w.name, 'f': None})
        vals.append({'v': turnout, 'f': turnout_str})
        rows.append({'c': vals})

    data['cols'] = cols
    data['rows'] = rows

    json_data = json.dumps(data, sort_keys=True, indent=4)
    return json_data


def getCandidatesVotes(candidates, q=Q()):
    votes = list(map(lambda c: Vote.objects.filter(q).filter(
        candidate=c).aggregate(Sum('votes'))['votes__sum'], candidates))
    total = reduce(lambda x, y: x + y, votes)
    votes = list(map(lambda c: c / total * 100, votes))
    return votes


def getCandidates():
    return Candidate.objects.all().order_by('name')


def buildRow(candidates, q=Q()):
    votes = getCandidatesVotes(candidates, q)
    total = list(zip(candidates, votes))
    total.sort(key=lambda tup: tup[1])
    total = total[::-1]
    mx = total[0][1]
    total = list(map(lambda x: (x[0], x[1], x[1] / mx * 100), total))
    return total


def buildCandidatesStats(q=Q()):
    candidates = getCandidates()
    return buildRow(candidates, q)


def buildPieChartData(q=Q()):
    candidates = getCandidates()
    votes = getCandidatesVotes(candidates, q)
    total = list(zip(candidates, votes))
    total.sort(key=lambda tup: tup[1])

    data = {}
    cols = [{'label': 'Kandydat', 'type': 'string'},
            {'label': 'Procent głosów', 'type': 'number'}]

    rows = []

    for w in total:
        vals = []
        turnout_str = '{0}%'.format(round(w[1], 2))
        vals.append({'v': str(w[0]), 'f': None})
        vals.append({'v': w[1], 'f': turnout_str})
        rows.append({'c': vals})

    data['cols'] = cols
    data['rows'] = rows

    json_data = json.dumps(data, sort_keys=True, indent=4)
    return json_data
