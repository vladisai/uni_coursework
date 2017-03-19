import os
os.environ.setdefault("DJANGO_SETTINGS_MODULE", "appWWW.settings")

import django
django.setup()

import os
import jinja2
import json
import random

from elections.models import *
from django.db.models import *
from functools import reduce


def render(tpl_path, context):
    path, filename = os.path.split(tpl_path)
    return jinja2.Environment(
        loader=jinja2.FileSystemLoader(path or './')
    ).get_template(filename).render(context)

data = {}
cols = [{'label': 'County', 'type': 'string'},
        {'label': 'Głosów', 'type': 'number'}]

rows = []

for w in Wojewodztwo.objects.all():
    votes = Vote.objects.filter(wojewodztwo=w).aggregate(
        Sum('votes'))['votes__sum']
    vals = []
    vals.append({'v': w.name, 'f': None})
    vals.append({'v': votes, 'f': None})
    rows.append({'c': vals})

data['cols'] = cols
data['rows'] = rows

json_data = json.dumps(data, sort_keys=True, indent=4)
# print(json_data)


def getCandidatesVotes(candidates, q=Q()):
    votes = list(map(lambda c: Vote.objects.filter(q).filter(
        candidate=c).aggregate(Sum('votes'))['votes__sum'], candidates))
    total = reduce(lambda x, y: x + y, votes)
    votes = list(map(lambda c: c / total * 100, votes))
    return votes


def getCandidates():
    return Candidate.objects.all().order_by('name')


def buildRow(candidates, title, q=Q()):
    votes = getCandidatesVotes(candidates, q)
    row = {'title': title, 'votes': votes}
    return row

candidates = getCandidates()

rows = []
rows.append(buildRow(candidates, 'Cały kraj'))

for w in Wojewodztwo.objects.all():
    q = Q(wojewodztwo=w)
    rows.append(buildRow(candidates, str(w), q))


def buildColours():
    h_s = 200
    s_s = 10
    l_s = 10

    h_e = 250
    s_e = 90
    l_e = 90

    h, l, s = h_s, l_s, s_s
    colours = 12

    h_step = (h_e - h_s) / colours
    l_step = (l_e - l_s) / colours
    s_step = (s_e - s_s) / colours

    res = []

    for i in range(colours):
        colour_str = 'hsl({0}, {1}%, {2}%)'.format(h, s, l)
        res.append(colour_str)
        h += h_step
        l += l_step
        s += s_step

    # random.shuffle(res)

    return res

print(render('templates/test.html', {'data': json_data,
                                     'candidates': getCandidates(),
                                     'rows': rows,
                                     'colours': buildColours(),
                                     }))
