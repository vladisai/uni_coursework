from django.shortcuts import render, redirect
from django.http import HttpResponse, HttpResponseForbidden
from rest_framework import viewsets
from django.contrib import auth

from rest_framework.response import Response
from rest_framework.renderers import JSONRenderer
from rest_framework.decorators import detail_route
from rest_framework import status
from rest_framework.parsers import JSONParser
from rest_framework.decorators import api_view, permission_classes
from rest_framework.permissions import IsAuthenticated
from rest_framework import generics

from . import utility, serializers
from .models import *
from . import html_render as html


def index(request):
    return HttpResponse(html.index(request))


def wojewodztwo(request, wojewodztwo_slug):
    return HttpResponse(html.wojewodztwo(request, wojewodztwo_slug))


def powiat(request, wojewodztwo_slug, powiat_slug):
    return HttpResponse(html.powiat(request, wojewodztwo_slug, powiat_slug))


def gmina(request, wojewodztwo_slug, powiat_slug, gmina_slug):
    return HttpResponse(html.gmina(request, wojewodztwo_slug, powiat_slug, gmina_slug))


def edit_gmina(request, wojewodztwo_slug, powiat_slug, gmina_slug):
    g = utility.getGmina(wojewodztwo_slug, powiat_slug, gmina_slug)
    return render(request, 'client_render/edit_gmina.html', {'pk': g.pk})


def search_gmina(request):
    pattern = request.GET.get('pattern', '')
    return render(request, 'client_render/search.html', {'pattern': pattern})


class GminaViewSet(viewsets.ReadOnlyModelViewSet):
    queryset = Gmina.objects.all()
    serializer_class = serializers.GminaSerializer

    @detail_route(methods=['get', 'put'], permission_classes=[IsAuthenticated])
    def vote_stats(self, request, pk=None):
        if request.method == 'GET':
            g = self.get_object()
            serializer = serializers.GminaStatsSerializer(g)
            return Response(serializer.data)
        else:
            data = JSONParser().parse(request)
            serializer = serializers.GminaStatsSerializer(data=data)
            if serializer.is_valid():
                g = self.get_object()
                serializer.update(g, serializer.validated_data)
                return Response('all\'s good', status.HTTP_200_OK)
            return Response(serializer.errors, status.HTTP_400_BAD_REQUEST)


class GminaSearchView(generics.ListAPIView):
    serializer_class = serializers.GminaSerializer

    def get_queryset(self):
        pattern = self.kwargs['pattern']
        return Gmina.objects.filter(name__contains=pattern)


class PowiatViewSet(viewsets.ReadOnlyModelViewSet):
    queryset = Powiat.objects.all()
    serializer_class = serializers.PowiatSerializer

    @detail_route(methods=['get'])
    def gmina_set(self, request, pk=None):
        powiat = self.get_object()
        result = powiat.gmina_set
        serializer = serializers.GminaSerializer(
            result, context={'request': request}, many=True)
        return Response(serializer.data)


class WojewodztwoViewSet(viewsets.ReadOnlyModelViewSet):
    queryset = Wojewodztwo.objects.all()
    serializer_class = serializers.WojewodztwoSerializer

    @detail_route(methods=['get'])
    def powiat_set(self, request, pk=None):
        wojewodztwo = self.get_object()
        result = wojewodztwo.powiat_set
        serializer = serializers.PowiatSerializer(
            result, context={'request': request}, many=True)
        return Response(serializer.data)


class VoteViewSet(viewsets.ReadOnlyModelViewSet):
    queryset = Vote.objects.all()
    serializer_class = serializers.VoteSerializer


class CandidateViewSet(viewsets.ReadOnlyModelViewSet):
    queryset = Candidate.objects.all()
    serializer_class = serializers.CandidateSerializer


class KrajViewSet(viewsets.ReadOnlyModelViewSet):
    queryset = Kraj.objects.all()
    serializer_class = serializers.KrajSerializer


def rest_login(request):
    return render(request, 'client_render/login.html')
