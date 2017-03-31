#!/usr/bin/env python

from django.db import transaction

import os
os.environ.setdefault("DJANGO_SETTINGS_MODULE", "appWWW.settings")
import django
django.setup()

from elections.models import *
from elections.html_render import *

from django.conf import settings

def generateCountry():
    html = index()
    with open('./gen/index.html', 'w') as f:
        f.write(html)


def generateWojewodztwo():
    for w in Wojewodztwo.objects.all():
        html = renderWojewodztwo(w)
        file_path = w.file()
        directory = os.path.dirname(file_path)
        os.makedirs(directory)
        with open(file_path, 'w') as f:
            f.write(html)
        print(w.name)


def generatePowiat():
    for p in Powiat.objects.all():
        html = renderPowiat(p)
        file_path = p.file()
        directory = os.path.dirname(file_path)
        os.makedirs(directory)
        with open(file_path, 'w') as f:
            f.write(html)
        print(p.name)


def generateGmina():
    for g in Gmina.objects.all():
        html = renderGmina(g)
        file_path = g.file()
        directory = os.path.dirname(file_path)
        os.makedirs(directory)
        with open(file_path, 'w') as f:
            f.write(html)
        print(g.name)


def generateAll():
    generateCountry()
    generateWojewodztwo()
    generatePowiat()
    generateGmina()

if __name__ == "__main__":
    generateAll()
