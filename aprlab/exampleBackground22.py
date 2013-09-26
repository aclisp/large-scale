#! /usr/bin/env python
import popen2
import os

def explainStatus(statusCode):
    signal = statusCode & 0xFF
    exitCode = statusCode >> 8
    if signal == 0:
        return 'exit %d' % exitCode
    else:
        return 'signal %d' % signal

repos = ['/etc', '/proc', '/var']
# launch 3 async calls:
procs = [popen2.Popen3(r'ls -l %s > %s.log 2>&1'%(repo,repo.lstrip('/'))) for repo in repos]
# wait.
stats = [proc.wait() for proc in procs]
# check for results:
for k, v in zip(repos, [explainStatus(stat) for stat in stats]):
    print k, v
# THIS IS NOT BACKGROUND! THERE IS NO WAY IN PYTHON 2.2!
