from elections import models, utility
from django.shortcuts import render 

def index(request):
    return render(request, 'client_render/kraj.html')

def wojewodztwo(request, wojewodztwo_slug):
    w = utility.getWojewodztwo(wojewodztwo_slug)
    return render(request, 'client_render/wojewodztwo.html', {'title' : w.name, 'pk' : w.pk})

def powiat(request, wojewodztwo_slug, powiat_slug):
    p = utility.getPowiat(wojewodztwo_slug, powiat_slug)
    return render(request, 'client_render/powiat.html', {'title' : p.name, 'pk' : p.pk})

def gmina(request, wojewodztwo_slug, powiat_slug, gmina_slug):
    g = utility.getGmina(wojewodztwo_slug, powiat_slug, gmina_slug)
    return render(request, 'client_render/gmina.html', {'title' : g.name, 'pk' : g.pk})
