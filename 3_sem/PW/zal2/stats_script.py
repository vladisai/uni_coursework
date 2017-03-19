from subprocess import Popen, PIPE, check_output
import re

def runCommand(command):
    return check_output(command, shell = True).decode(encoding = 'utf-8')

def runForThreads(threads):
    command = "(time ./brandes " + str(threads) + " bigin /dev/null) 2>&1 | awk '/real/{print $2}'"
    ret = runCommand(command)
    ex = '^([0-9]*)m(.*)s'
    matchRet = re.match(ex, ret)
    minutes = float(matchRet.group(1))
    seconds = float(matchRet.group(2))
    return round(minutes * 60 + seconds, 3)

timeForOne = runForThreads(1)
print(1, timeForOne, 1, sep = ' & ', end = '\\\\\n')

for i in range(2, 9):
    time = runForThreads(i)
    improvement = round(timeForOne / time, 3)
    print(i, time, improvement, sep = ' & ', end = '\\\\\n')

