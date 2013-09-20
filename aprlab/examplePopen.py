#! /usr/bin/env python
import os
def callShellCmd(cmd):
    p = os.popen(cmd)
    out = p.read()
    rc = p.close()
    print '#### The command "%s" output is:'%cmd
    print '----------------------'
    print out.split('\n')
    print '----------------------'
    if rc:
        print 'rc = "%X", exit status = "%d"'%(rc, rc>>8)
    else:
        print 'The command terminate without error'

callShellCmd(r'ls -1 /etc | grep "\.d$"')
callShellCmd('ls -1 /etc | grep xxxx')

# http://docs.python.org/2.2/lib/os-newstreams.html
