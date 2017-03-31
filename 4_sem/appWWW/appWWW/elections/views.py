from django.shortcuts import render
from django.http import HttpResponse

from . import html_render as html


def index(request):
    return HttpResponse(html.index())


def wojewodztwo(request, wojewodztwo_slug):
    return HttpResponse(html.wojewodztwo(wojewodztwo_slug))


def powiat(request, wojewodztwo_slug, powiat_slug):
    return HttpResponse(html.powiat(wojewodztwo_slug, powiat_slug))


def gmina(request, wojewodztwo_slug, powiat_slug, gmina_slug):
    return HttpResponse(html.gmina(wojewodztwo_slug, powiat_slug, gmina_slug))
