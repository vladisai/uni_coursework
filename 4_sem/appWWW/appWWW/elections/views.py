from django.shortcuts import render
from django.http import HttpResponse

from .utility import *
# Create your views here.

def index(request):
    return HttpResponse(renderHTML(Q(), Wojewodztwo, 'Wojewodztwo', 'Cała Polska', True))

def wojewodztwo(request, wojewodztwo_slug):
    w = Wojewodztwo.objects.get(slug = wojewodztwo_slug)
    q = Q(wojewodztwo = w)
    return HttpResponse(renderHTML(q, Powiat, 'Powiat', 'Województwo ' + w.name))

def powiat(request, wojewodztwo_slug, powiat_slug):
    w = Wojewodztwo.objects.get(slug = wojewodztwo_slug)
    p = Powiat.objects.get(slug = powiat_slug, wojewodztwo = w)
    q = Q(powiat = p)
    return HttpResponse(renderHTML(q, Gmina, 'Gmina', 'Powiat ' + p.name))

def gmina(request, wojewodztwo_slug, powiat_slug, gmina_slug):
    w = Wojewodztwo.objects.get(slug = wojewodztwo_slug)
    p = Powiat.objects.get(slug = powiat_slug, wojewodztwo = w)
    w = Gmina.objects.get(powiat = p, slug = gmina_slug)
    q = Q(gmina = w)
    return HttpResponse(renderSimplifiedHTML(q, 'Gmina ' + w.name))
