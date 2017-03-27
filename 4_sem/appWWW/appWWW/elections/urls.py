from django.conf.urls import url

from . import views

app_name = 'elections'

urlpatterns = [
            url(r'^$', views.index, name='index'),
            url(r'^(?P<wojewodztwo_slug>[^/]+)/$', views.wojewodztwo, name='wojewodztwo'),
            url(r'^(?P<wojewodztwo_slug>[^/]+)/(?P<powiat_slug>[^/]+)/$', views.powiat, name='powiat'),
            url(r'^(?P<wojewodztwo_slug>[^/]+)/(?P<powiat_slug>[^/]+)/(?P<gmina_slug>[^/]+)/$', views.gmina, name='gmina'),
            ]
