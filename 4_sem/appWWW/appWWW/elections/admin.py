from django.contrib import admin
from elections import models

admin.site.register(models.Kraj)

region_list_display = ['name', 'getBallots', 'getVoters', 'getTurnout']

@admin.register(models.Wojewodztwo)
class WojewodztwoAdmin(admin.ModelAdmin):
    search_fields = ['name']
    list_display = region_list_display

@admin.register(models.Powiat)
class PowiatAdmin(admin.ModelAdmin):
    search_fields = ['name']
    list_display = region_list_display + ['wojewodztwo']

@admin.register(models.Gmina)
class GminaAdmin(admin.ModelAdmin):
    search_fields = ['name']
    list_display = region_list_display + ['powiat']

@admin.register(models.Candidate)
class CandidateAdmin(admin.ModelAdmin):
    search_fields = ['name']

@admin.register(models.Vote)
class VoteAdmin(admin.ModelAdmin):
    list_filter = ['candidate', 'wojewodztwo']
    search_fields = ['candidate', 'wojewodztwo', 'powiat', 'gmina']
    list_display = ['candidate', 'votes', 'wojewodztwo', 'powiat', 'gmina']

