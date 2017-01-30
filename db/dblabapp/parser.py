from subprocess import Popen, PIPE, check_output
from .models import *
import sys
import re

entryRegex = '^\[(.*?)\] (.*?): (.*)$'
generalEntryRegex = '^\[(.*?)\] (.*)$'

class UnknownFormat(Exception):
    pass

def runCommand(command):
    return check_output(command, shell = True).decode(encoding = 'utf-8')

class RawLogEntry:

    def __init__(self, time, driver_name, message):
        self.time = time
        self.driver_name = driver_name
        self.message = message

    def __str__(self):
        return 'time(' + str(self.time) + ') driver(' + self.driver_name + ') message(' + self.message + ')'

def parseRawEntry(data):
    print(data)
    entryMatch = re.match(entryRegex, data)
    if entryMatch is not None:
        timeStr, driver, message = entryMatch.groups()
        time = float(timeStr)
        return RawLogEntry(time, driver, message)
    else:
        entryMatch = re.match(generalEntryRegex, data)
        if entryMatch is not None:
            timeStr, message = entryMatch.groups()
            time = float(timeStr)
            driver = ''
            return RawLogEntry(time, driver, message)
        else:
            raise UnknownFormat(data)

def parseText(text, comment):
    raw = [parseRawEntry(s.strip()) for s in text.splitlines()] # an exception may be thrown here

    logSession = LogSession(comment = comment)
    logSession.save()
    logEntries = []
    for e in raw:
        driver, _ = DriverEntry.objects.get_or_create(driver_name = e.driver_name)
        newEntry = LogEntry(time = e.time, message = e.message, logSession = logSession, driver = driver)
        newEntry.save()
        logEntries.append(newEntry)
    return logSession, logEntries

# class Parser:

#     def __init__(self):
#         commandToRun = 'dmesg'
#         text = runCommand(commandToRun)
#         raw = [parseRawEntry(s.strip()) for s in text.splitlines()]
#         for e in raw:
#             print(str(e))

# Parser()
