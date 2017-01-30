from django.shortcuts import render, redirect
from django.http import HttpResponse
from django.urls import reverse
from django.db.models import Q

from .models import DriverEntry 
from .parser import *

import cx_Oracle

def index(request):
    return render(request, 'home/index.html')

def list_drivers(request):
    drivers_list = DriverEntry.objects.all()
    return render(request, 'drivers/list_all.html', {'drivers_list' : drivers_list})

def list_log_sessions(request):
    sessions_list = LogSession.objects.order_by('date')
    return render(request, 'log_sessions/list_all.html', {'sessions_list' : sessions_list})

def list_log_entries(request, session_id):
    entries_list = LogEntry.objects.filter(logSession = session_id).order_by('time')
    session = LogSession.objects.get(id = session_id)
    session_info = str(session)
    return render(request, 'log_entries/list_all.html', {'entries_list' : entries_list, 
                                                         'session_info' : session_info })

def compare_choose(request):
    sessions_list = LogSession.objects.order_by('date')
    return render(request, 'compare/choose.html', {'sessions_list' : sessions_list})

def compare_validate(request):
    selected = request.POST.getlist('checks')
    if (len(selected) != 2):
        return HttpResponse('You must select exactly two sessions')
    else:
        return redirect(reverse('dblabapp:compare_show', args = (selected[0], selected[1])))

def compare_show(request, session_1, session_2):
    session_1_obj = LogSession.objects.get(id = session_1)
    session_1_info = str(session_1_obj)
    entries_list_1 = LogEntry.objects.filter(logSession = session_1).order_by('time')
    
    session_2_obj = LogSession.objects.get(id = session_2)
    session_2_info = str(session_2_obj)

    entries = LogEntry.objects.filter(Q(logSession = session_1) | Q(logSession = session_2)).order_by('time')

    divided_entries = []
    for e in entries:
        if e.logSession == session_1:
            divided_entries.append([1, e])
        else:
            divided_entries.append([2, e])

    return render(request, 'compare/show.html', {'entries_list' : entries,
                                                'session_1_info' : session_1_info,
                                                'session_2_info' : session_2_info,
                                                'session_1' : session_1_obj, 
                                                'session_2' : session_2_obj})

def upload_text(request):
    return render(request, 'upload/upload_text.html')

def process_upload_text(request):
    log_text = request.POST['log_text']
    comment = request.POST['comment']
    try:
        parseText(log_text, comment)
        return redirect(reverse('dblabapp:index'))
    except:
        return HttpResponse('Could not parse log, check your file')

def upload_file(request):
    return render(request, 'upload/upload_file.html')

def process_upload_file(request):
    log_text = request.FILES['log_text'].read()
    comment = request.POST['comment']
    try:
        parseText(log_text, comment)
        return redirect(reverse('dblabapp:index'))
    except Exception as e:
        return HttpResponse('Error in line ' + str(e))

def delete_session(request, session_id):
    LogSession.objects.get(id = session_id).delete() # trigger takes care of the entries
    return redirect(reverse('dblabapp:list_sessions'))

# Create your views here.
