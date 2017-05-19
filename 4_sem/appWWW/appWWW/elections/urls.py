from django.conf.urls import url, include
from django.contrib.auth import views as auth_views

from rest_framework import routers
from rest_framework.authtoken import views as rest_views

from . import views

app_name = 'elections'

router = routers.DefaultRouter()
router.register(r'gmina', views.GminaViewSet)
router.register(r'powiat', views.PowiatViewSet)
router.register(r'wojewodztwo', views.WojewodztwoViewSet)
router.register(r'vote', views.VoteViewSet)
router.register(r'candidate', views.CandidateViewSet)
router.register(r'kraj', views.KrajViewSet)

rest_api = [
    url(r'^', include(router.urls)),
    url(r'^authtoken/$', rest_views.ObtainAuthToken.as_view()),
    url(r'^login/$', views.rest_login), 
    url(r'^gmina/search/(?P<pattern>[^/]+)/', views.GminaSearchView.as_view()),
]

urlpatterns = [
    url(r'^$', views.index, name='index'),

    url(r'^search/gmina/$', views.search_gmina, name='search_gmina'),

    url(r'^results/$', views.index, name='kraj'),

    url(r'^results/(?P<wojewodztwo_slug>[^/]+)/$',
        views.wojewodztwo, name='wojewodztwo'),

    url(r'^results/(?P<wojewodztwo_slug>[^/]+)/(?P<powiat_slug>[^/]+)/$',
        views.powiat, name='powiat'),

    url(r'^results/(?P<wojewodztwo_slug>[^/]+)/(?P<powiat_slug>[^/]+)/(?P<gmina_slug>[^/]+)/$',
        views.gmina, name='gmina'),

    url(r'^results/(?P<wojewodztwo_slug>[^/]+)/(?P<powiat_slug>[^/]+)/(?P<gmina_slug>[^/]+)/edit/$',
        views.edit_gmina, name='edit_gmina'),

    url(r'^api/', include(rest_api)),
]
