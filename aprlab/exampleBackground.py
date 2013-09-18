#! /usr/bin/env python
import subprocess

repos = ['/repo1', '/repo2', '/repo3']
# launch 3 async calls:
procs = [subprocess.Popen(['svn', 'update', repo]) for repo in repos]
# wait.
for proc in procs:
    proc.wait()
# check for results:
if any(proc.returncode != 0 for proc in procs):
    print 'Something failed'

# http://stackoverflow.com/questions/2496772/asynchronous-background-processes-in-python
