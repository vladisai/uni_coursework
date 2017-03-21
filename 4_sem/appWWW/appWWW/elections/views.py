from django.shortcuts import render
from django.http import HttpResponse

from .utility import *
# Create your views here.

def index(request):
    return HttpResponse(renderHTML(Q(), Wojewodztwo, 'Wojewodztwo', 'Cała Polska', True))
