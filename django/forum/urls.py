from django.conf.urls.defaults import patterns, include, url
from django.views.generic import DetailView, ListView
from board.models import Category

# Uncomment the next two lines to enable the admin:
from django.contrib import admin
admin.autodiscover()


urlpatterns = patterns('',
    # Examples:
    # url(r'^$', 'forum.views.home', name='home'),
    # url(r'^forum/', include('forum.foo.urls')),

    (r'^board/$', 'board.views.index'),
    (r'^board/category/(?P<cat_id>\d+)/$', 'board.views.cat_index'),
    (r'^board/users/(?P<user_id>\d+)/$', 'board.views.users'),
    (r'^board/topic/(?P<topic_id>\d+)/$', 'board.views.topic'),
    (r'^board/new_topic/(?P<cat_id>\d+)/$', 'board.views.new_topic'),
    (r'^board/new_post/(?P<topic_id>\d+)/$', 'board.views.new_post'),
    (r'^accounts/logout/$', 'django.contrib.auth.views.logout', {'next_page' : '/board/'}),
    (r'^accounts/login/$', 'django.contrib.auth.views.login', {'template_name': 'board/login.html'}),
    

    # Uncomment the admin/doc line below to enable admin documentation:
    url(r'^admin/doc/', include('django.contrib.admindocs.urls')),

    # Uncomment the next line to enable the admin:
    url(r'^admin/', include(admin.site.urls)),
)

from django.contrib.staticfiles.urls import staticfiles_urlpatterns
urlpatterns += staticfiles_urlpatterns()
