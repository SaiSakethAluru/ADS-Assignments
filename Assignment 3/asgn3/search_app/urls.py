from django.urls import path
from . import views

app_name = 'search_app'

urlpatterns = [
    path("",views.homepage, name="homepage"),
    path("search/",views.homepage, name="search"),
]