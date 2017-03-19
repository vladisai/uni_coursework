from subprocess import Popen, PIPE, check_output
import sys
import dateutil.parser
import re
from model import RawLogEntry

entryRegex = '^(\S*) (.*?): (.*)$'
generalEntryRegex = '^(\S*) (.*)$'

class UnknownFormat(Exception):
    pass

def runCommand(command):
    return check_output(command, shell = True).decode(encoding = 'utf-8')

def ParseRawEntry(data):
    entryMatch = re.match(entryRegex, data)
    if entryMatch is not None:
        dateStr, driver, message = entryMatch.groups()
        date = dateutil.parser.parse(dateStr)
        return RawLogEntry(date, driver, message)
    else:
        entryMatch = re.match(generalEntryRegex, data)
        if entryMatch is not None:
            dateStr, message = entryMatch.groups()
            date = dateutil.parser.parse(dateStr)
            driver = ''
            return RawLogEntry(date, driver, message)
        else:
            raise UnknownFormat()

class Parser:

    def __init__(self):
        commandToRun = 'dmesg --time-format iso'
        ret =  [s.strip() for s in runCommand(commandToRun).splitlines()]
        print(ret[0])
        print(ParseRawEntry(ret[0]))

Parser()
