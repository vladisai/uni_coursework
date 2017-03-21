import os
os.environ.setdefault("DJANGO_SETTINGS_MODULE", "appWWW.settings")

import django
django.setup()

from utility import *

if __name__ == '__main__':
    w = Wojewodztwo.objects.get(name = 'LUBUSKIE')
    q = Q(wojewodztwo = w)
    print(renderHTML(q, Powiat, 'Powiat', 'Województwo Lubuskie'))
