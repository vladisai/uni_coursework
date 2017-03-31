from django.conf.urls import url

from django.conf import settings

from . import views

app_name = 'elections'

if settings.GEN_MODE:
    urlpatterns = [
                url(r'^index.html$', views.index, name='index'),
                url(r'^(?P<wojewodztwo_slug>[^/]+)/index.html/$', views.wojewodztwo, name='wojewodztwo'),
                url(r'^(?P<wojewodztwo_slug>[^/]+)/(?P<powiat_slug>[^/]+)/index.html/$', views.powiat, name='powiat'),
                url(r'^(?P<wojewodztwo_slug>[^/]+)/(?P<powiat_slug>[^/]+)/(?P<gmina_slug>[^/]+)/index.html/$', views.gmina, name='gmina'),
                ]

else:
    urlpatterns = [
                url(r'^$', views.index, name='index'),
                url(r'^(?P<wojewodztwo_slug>[^/]+)/$', views.wojewodztwo, name='wojewodztwo'),
                url(r'^(?P<wojewodztwo_slug>[^/]+)/(?P<powiat_slug>[^/]+)/$', views.powiat, name='powiat'),
                url(r'^(?P<wojewodztwo_slug>[^/]+)/(?P<powiat_slug>[^/]+)/(?P<gmina_slug>[^/]+)/$', views.gmina, name='gmina'),
                ]
