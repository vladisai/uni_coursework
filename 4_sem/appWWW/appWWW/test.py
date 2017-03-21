import os
os.environ.setdefault("DJANGO_SETTINGS_MODULE", "appWWW.settings")
import django
django.setup()

from elections.models import *

print(Wojewodztwo.objects.all()[0].ballots)

