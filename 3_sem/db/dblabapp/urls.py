from django.conf.urls import url

from . import views

app_name = 'dblabapp'

urlpatterns = [
            url(r'^$', views.index, name='index'),
            url(r'^drivers$', views.list_drivers, name='list_drivers'),
            url(r'^logs/upload_text$', views.upload_text, name = 'upload_text'),
            url(r'^logs/process_upload_text$', views.process_upload_text, name = 'process_upload_text'),
            url(r'^logs/upload_file$', views.upload_file, name = 'upload_file'),
            url(r'^logs/process_upload_file$', views.process_upload_file, name = 'process_upload_file'),
            url(r'^log_sessions/list_all$', views.list_log_sessions, name = 'list_sessions'),
            url(r'^log_sessions/(?P<session_id>[0-9]+)', views.list_log_entries, name = 'list_log_entries'),
            url(r'^log_sessions/compare/choose$', views.compare_choose, name = 'compare_choose'),
            url(r'^log_sessions/compare/validate$', views.compare_validate, name = 'compare_validate'),
            url(r'^log_sessions/compare/(?P<session_1>[0-9]+)/(?P<session_2>[0-9]+)$', views.compare_show, name = 'compare_show'),
            url(r'^log_sessions/delete/(?P<session_id>[0-9]+)$', views.delete_session, name = 'delete_session'),
            ]
