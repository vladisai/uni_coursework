import os
os.environ.setdefault("DJANGO_SETTINGS_MODULE", "appWWW.settings")

import django
django.setup()

from utility import *

if __name__ == '__main__':

    print(renderHTML(Q(), Wojewodztwo, 'Wojewodztwo', 'Cały kraj'))
